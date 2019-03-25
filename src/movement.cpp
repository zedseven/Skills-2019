#include "main.h"
#include "config.h"
#include "globals.h"
#include "movement.h"
#include "okapi/api.hpp"
#include "api.h"
#include "pros/adi.hpp"
#include "pros/api_legacy.h"

using namespace std;
using namespace okapi;

//Mechanical Methods
void resetMotors()
{
  LeftMotor.set_brake_mode(MOTOR_BRAKE_BRAKE);
  RightMotor.set_brake_mode(MOTOR_BRAKE_BRAKE);
  LeftMotor.move(0);
  RightMotor.move(0);
  ClawMotor.move(0);
}
void resetMotorsAndLamp()
{
  Lamp.move(0);
  resetMotors();
}
void initMotors()
{
  resetMotorsAndLamp();
  ClawMotor.move_absolute(0, 30);
  ArmMotorL.move_absolute(0, 30);
  ArmMotorR.move_absolute(0, 30);
}
void await1Motor(pros::Motor motor, double target, double sensitivity, double timeout)
{
  timer.placeMark();
  while(fabs(motor.get_position() - target) > sensitivity && (timeout < 0 || timer.getDtFromMark().convert(millisecond) < timeout))
    pros::delay(50);
  timer.clearMark();
}
void await2Motors(pros::Motor motor1, pros::Motor motor2, double start1, double start2, double target1, double target2, double sensitivity1, double sensitivity2, double timeout)
{
  timer.placeMark();
  while((fabs(fabs(motor1.get_position() - start1) - fabs(target1)) > sensitivity1 || fabs(fabs(motor2.get_position() - start2) - fabs(target2)) > sensitivity2) && (timeout < 0 || timer.getDtFromMark().convert(millisecond) < timeout))
    pros::delay(50);
  timer.clearMark();
}
void await2Motors(pros::Motor motor1, pros::Motor motor2, double target1, double target2, double sensitivity1, double sensitivity2, double timeout)
{
  timer.placeMark();
  while((fabs(motor1.get_position() - target1) > sensitivity1 || fabs(motor2.get_position() - target2) > sensitivity2) && (timeout < 0 || timer.getDtFromMark().convert(millisecond) < timeout))
    pros::delay(50);
  timer.clearMark();
}
void await2Motors(pros::Motor motor1, pros::Motor motor2, double target1, double target2, double sensitivity, double timeout)
{
  await2Motors(motor1, motor2, target1, target2, sensitivity, sensitivity, timeout);
}
void await2Motors(pros::Motor motor1, pros::Motor motor2, double target, double sensitivity, double timeout)
{
  await2Motors(motor1, motor2, target, target, sensitivity, timeout);
}
void spinMotorUntilTimeout(pros::Motor motor, double speed, double timeout)
{
  timer.placeMark();
  motor.move_velocity(speed);
  while((motor.get_actual_velocity() != 0.0 || timer.getDtFromMark().convert(millisecond) < SPIN_UNTIL_MIN_MSEC) && (timeout < 0 || timer.getDtFromMark().convert(millisecond) < timeout))
    pros::delay(50);
  timer.clearMark();
}
void spinMotorUntilTimeout(pros::Motor motor, double speed)
{
  spinMotorUntilTimeout(motor, speed, -1);
}
void moveDeg(double moveDegs, double moveSpeed)
{
  double leftStartDeg = LeftMotor.get_position();
  double rightStartDeg = RightMotor.get_position();
  LeftMotor.move_relative(moveDegs, moveSpeed);
  RightMotor.move_relative(moveDegs, moveSpeed);
  //while(abs(abs(LeftMotor.get_position() - leftStartDeg) - abs(moveDegs)) > MOVE_SENSITIVITY || abs(abs(RightMotor.get_position() - rightStartDeg) - abs(moveDegs)) > MOVE_SENSITIVITY)
  //  pros::delay(50);
  await2Motors(LeftMotor, RightMotor, leftStartDeg, rightStartDeg, moveDegs, moveDegs, MOVE_SENSITIVITY, MOVE_SENSITIVITY, -1);
}
void moveDeg(double moveDegs)
{
  moveDeg(moveDegs, MOVEMENT_SPEED);
}
void move(double distance, double moveSpeed) //distance in cm
{
  moveDeg(distance * CM_TO_MOTOR_DEG, moveSpeed);
}
void move(double distance) //distance in cm
{
  moveDeg(distance * CM_TO_MOTOR_DEG);
}
void rotateDeg(double moveDeg)
{
  double leftStartDeg = LeftMotor.get_position();
  double rightStartDeg = RightMotor.get_position();
  LeftMotor.move_relative(moveDeg, MOVEMENT_SPEED);
  RightMotor.move_relative(-moveDeg, MOVEMENT_SPEED);
  //while(abs(abs(LeftMotor.get_position() - leftStartDeg) - abs(moveDeg)) > MOVE_SENSITIVITY || abs(abs(RightMotor.get_position() - rightStartDeg) - abs(moveDeg)) > MOVE_SENSITIVITY)
  //  pros::delay(50);
  await2Motors(LeftMotor, RightMotor, leftStartDeg, rightStartDeg, moveDeg, -moveDeg, MOVE_SENSITIVITY, MOVE_SENSITIVITY, -1);
}
void rotate(double rotationDeg)
{
  rotateDeg(rotationDeg * ROBOT_TO_MOTOR_DEG);
}
void realign()
{
  double rBand;
  int realignNum = 1;
  while(true)
  {
    pros::delay(50/*100*/);
    double lDist = -1.0;
    double rDist = -1.0;
    while(/*true || */lDist < 0.0 || rDist < 0.0)
    {
      lDist = pros::c::ultrasonicGet(SonarL);
      rDist = pros::c::ultrasonicGet(SonarR);
      //Brain.Screen.printAt(100, 100, "L: %f", lDist);
      //Brain.Screen.printAt(100, 200, "R: %f", rDist);
    }
    rBand = fmax(0.2, (((lDist + rDist) / 2.0) / REALIGN_DIST) * 0.125 + REALIGN_SENSITIVITY) * (pow(fmax(1.0, realignNum / 3.0), 0.1));
    //Brain.Screen.printAt(100, 220, "rBand: %f", rBand);
    double distDiff = rDist - lDist;
    if(distDiff > rBand) //Right is further away - turn left
    {
      LeftMotor.move_velocity(-REALIGN_SPEED);
      RightMotor.move_velocity(REALIGN_SPEED);
    }
    else if(distDiff < -rBand) //Left is further away - turn right
    {
      LeftMotor.move_velocity(REALIGN_SPEED);
      RightMotor.move_velocity(-REALIGN_SPEED);
    }
    else
    {
      resetMotors();
      bool done = true;
      for(int j = 0; j < 5; j++)
      {
        if(!done)
          break;
        //Brain.Screen.printAt(100, 150, "Main check #%d", j + 1);
        /*Brain.Screen.printAt(200, 100, "L: %f", SonarL.distance(distanceUnits::cm));
        Brain.Screen.printAt(200, 200, "R: %f", SonarR.distance(distanceUnits::cm));
        vex::task::sleep(1000);
        Brain.Screen.printAt(200, 100, "L: %f", SonarL.distance(distanceUnits::cm));
        Brain.Screen.printAt(200, 200, "R: %f", SonarR.distance(distanceUnits::cm));
        vex::task::sleep(1000);
        Brain.Screen.printAt(200, 100, "L: %f", SonarL.distance(distanceUnits::cm));
        Brain.Screen.printAt(200, 200, "R: %f", SonarR.distance(distanceUnits::cm));*/
        double lDistN = -1.0;
        double rDistN = -1.0;
        for(int i = 0; i < 10; i++)
        {
          lDistN = -1.0;
          rDistN = -1.0;
          while(/*true || */lDistN < 0.0 || rDistN < 0.0)
          {
            lDistN = pros::c::ultrasonicGet(SonarL);
            rDistN = pros::c::ultrasonicGet(SonarR);
            //Brain.Screen.printAt(100, 100, "L: %f", lDistN);
            //Brain.Screen.printAt(100, 200, "R: %f", rDistN);
          }
          if(fabs(rDistN - lDistN) > rBand /*|| fabs(lDist - lDistN) > realignBand || fabs(rDist - rDistN) > realignBand*/)
          {
            done = false;
            break;
          }
          else
          {
            pros::delay(50);
          }
        }
      }
      if(done)
        break;
    }
    realignNum++;
  }
}
void moveUntilDist(double targetDist, double moveIncrement)
{
  double lDist;
  double rDist;
  double distDiff;
  double measureDist;
  while(true)
  {
    lDist = -1.0;
    rDist = -1.0;
    while(/*true || */lDist < 0.0 || rDist < 0.0)
    {
      lDist = pros::c::ultrasonicGet(SonarL) / 10.0;
      rDist = pros::c::ultrasonicGet(SonarR) / 10.0;
      printf("lDist: %f rDist: %f\n", lDist, rDist);
    }
    measureDist = (lDist + rDist) / 2.0;
    distDiff = measureDist - targetDist;
    realign();
    if(distDiff > MOVE_UNTIL_SENSITIVITY)
      move(fmin(measureDist - targetDist, moveIncrement));
    else if(distDiff < -MOVE_UNTIL_SENSITIVITY)
      move(-fmin(targetDist - measureDist, moveIncrement));
    else
      break;
  }
  //Brain.Screen.printAt(50, 120, "Done moving.");
  realign();
}
void closeClawOnBlock()
{
  ClawMotor.set_brake_mode(MOTOR_BRAKE_HOLD);//setTimeout(1500, timeUnits::msec);
  ClawMotor.move_absolute(CLAW_CLOSE_DEG, CLAW_MOVE_SPEED);
  await1Motor(ClawMotor, CLAW_CLOSE_DEG, 1, 1500);
}
void openClawTensed()
{
  spinMotorUntilTimeout(ClawMotor, CLAW_MOVE_SPEED, 2000);
  ClawMotor.tare_position();
  ClawMotor.set_brake_mode(MOTOR_BRAKE_HOLD);
}
void openClawRelaxed()
{
  spinMotorUntilTimeout(ClawMotor, CLAW_MOVE_SPEED, 2000);
  ClawMotor.tare_position();
  ClawMotor.set_brake_mode(MOTOR_BRAKE_COAST);
}
void pickupBlock()
{
  //Open claw
  openClawTensed();
  ArmMotorL.move_absolute(ARM_PICKUP_DEG, ARM_PICKUP_SPEED);
  ArmMotorR.move_absolute(ARM_PICKUP_DEG, ARM_PICKUP_SPEED);
  //Wait until the arms have been lowered
  await2Motors(ArmMotorL, ArmMotorR, ARM_PICKUP_DEG, ARM_PICKUP_SENSITIVITY, 4000);
  closeClawOnBlock();
  ArmMotorL.move_absolute(0, ARM_PICKUP_SPEED);
  ArmMotorR.move_absolute(0, ARM_PICKUP_SPEED);
  //Wait until the arms have been raised
  await2Motors(ArmMotorL, ArmMotorR, 0, ARM_PICKUP_SENSITIVITY, 4000);
  holdingBlock = true;
}
void lowerBlock()
{
  //Assuming claw is closed
  ArmMotorL.move_absolute(ARM_DROPOFF_DEG, ARM_PICKUP_SPEED);
  ArmMotorR.move_absolute(ARM_DROPOFF_DEG, ARM_PICKUP_SPEED);
  //Wait until the arms have been lowered
  await2Motors(ArmMotorL, ArmMotorR, ARM_DROPOFF_DEG, ARM_PICKUP_SENSITIVITY, 4000);
}
void raiseArms()
{
  ArmMotorL.move_absolute(0, ARM_PICKUP_SPEED);
  ArmMotorR.move_absolute(0, ARM_PICKUP_SPEED);
  //Wait until the arms have been raised
  await2Motors(ArmMotorL, ArmMotorR, 0, ARM_PICKUP_SENSITIVITY, 4000);
}
void dropoffBlock()
{
  lowerBlock();
  openClawRelaxed();
  raiseArms();
  holdingBlock = false;
}
