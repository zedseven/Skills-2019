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
  double lStartDeg = LeftMotor.get_position();
  double rStartDeg = RightMotor.get_position();
  bool resetRot = false;
  while(realignNum <= REALIGN_MAX_MOVES)
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
    printf("lDist: %f rDist: %f\n", lDist, rDist);
    rBand = fmax(0.2, (((lDist + rDist) / 2.0) / REALIGN_DIST) * 0.125 + REALIGN_SENSITIVITY) * (pow(fmax(1.0, realignNum / 3.0), 0.1));
    //Brain.Screen.printAt(100, 220, "rBand: %f", rBand);
    double distDiff = rDist - lDist;
    if(distDiff >= REALIGN_CUTOFF_DIFF)
    {
      resetRot = true;
      break;
    }
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
      for(int j = 0; j < 4; j++)
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
            pros::delay(20);
          }
        }
      }
      if(done)
        break;
    }
    //To avoid being completely thrown off - basically just cutting our losses if the realign can't work
    if(resetRot || (fabs(LeftMotor.get_position() - lStartDeg) + fabs(RightMotor.get_position() - rStartDeg)) / 2.0 > REALIGN_CUTOFF_MOTOR_DEG)
    {
      LeftMotor.move_absolute(lStartDeg, REALIGN_SPEED);
      RightMotor.move_absolute(rStartDeg, REALIGN_SPEED);
      await2Motors(LeftMotor, RightMotor, lStartDeg, rStartDeg, 1.0, 6000);
      break;
    }
    realignNum++;
  }
  resetMotors();
  printf("Started with (%f, %f), and ended with (%f, %f) after %d movement%s.\n", lStartDeg, rStartDeg, LeftMotor.get_position(), RightMotor.get_position(), realignNum, (realignNum != 1 ? "s" : ""));
}
void moveUntilDist(double targetDist, double moveIncrement, bool realignFinal)
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
      //printf("lDist: %f rDist: %f\n", lDist, rDist);
    }
    measureDist = (fabs(lDist - rDist) < REALIGN_CUTOFF_DIFF / 10.0 ? (lDist + rDist) / 2.0 : fmin(lDist, rDist));
    distDiff = measureDist - targetDist;
    realign();
    if(distDiff > MOVE_UNTIL_SENSITIVITY)
      move(fmin(measureDist - targetDist, moveIncrement));
    else if(distDiff < -MOVE_UNTIL_SENSITIVITY)
      move(-fmin(targetDist - measureDist, moveIncrement));
    else
      break;
  }
  resetMotors();
  //Brain.Screen.printAt(50, 120, "Done moving.");
  if(realignFinal)
    realign();
}
void realignLine(bool leftDefault, double dist)
{
  bool lineL = false;
  bool lineM = false;
  bool lineR = false;
  bool lineB = false;
  bool lastArc = false; //false = left, true = right
  //Align horizontally
  while(true)
  {
    lineL = pros::c::analogRead(LINE_L_PORT) >= LINE_VALUE_THRESHOLD;
    lineM = pros::c::analogRead(LINE_M_PORT) >= LINE_VALUE_THRESHOLD;
    lineR = pros::c::analogRead(LINE_R_PORT) >= LINE_VALUE_THRESHOLD;
    lineB = pros::c::analogRead(LINE_B_PORT) >= LINE_VALUE_THRESHOLD;
    //printf("%d %d %d %d\n", (lineL ? 1 : 0), (lineM ? 1 : 0), (lineR ? 1 : 0), (lineB ? 1 : 0));
    //printf("%d\n", pros::c::analogRead(LINE_B_PORT));
    if(lineL && lineR)
    {
      if(!lastArc) //Left
      {
        LeftMotor.move_velocity(LINE_MOVEMENT_SPEED);
        RightMotor.move_velocity(-LINE_MOVEMENT_SPEED * 0.5);
      }
      else //Right
      {
        LeftMotor.move_velocity(-LINE_MOVEMENT_SPEED * 0.5);
        RightMotor.move_velocity(LINE_MOVEMENT_SPEED);
      }
    }
    else if(lineL)
    {
      LeftMotor.move_velocity(0);
      RightMotor.move_velocity(LINE_MOVEMENT_SPEED);
    }
    else if(lineR)
    {
      LeftMotor.move_velocity(LINE_MOVEMENT_SPEED);
      RightMotor.move_velocity(0);
    }
    else if(lineM && !lineB)
    {
      LeftMotor.move_velocity(LINE_MOVEMENT_SPEED);
      RightMotor.move_velocity(LINE_MOVEMENT_SPEED);
    }
    else if(lineM && !lineL && !lineR && lineB)
    {
      break;
    }
    else
    {
      //Recovery operation
      resetMotors();
      double lDeg = LeftMotor.get_position();
      double rDeg = RightMotor.get_position();
      lastArc = false;
      //Turn left until we see something or have turned REALIGN_LINE_TURN_DEG robot degrees
      while(pros::c::analogRead(LINE_L_PORT) < LINE_VALUE_THRESHOLD && pros::c::analogRead(LINE_M_PORT) < LINE_VALUE_THRESHOLD && pros::c::analogRead(LINE_R_PORT) < LINE_VALUE_THRESHOLD && (fabs(LeftMotor.get_position() - lDeg) /*+ fabs(RightMotor.get_position() - rDeg)*/) / (/*2.0 **/ ROBOT_TO_MOTOR_DEG) < REALIGN_LINE_TURN_DEG)
      {
        LeftMotor.move_velocity(-MOVEMENT_SPEED);
        RightMotor.move_velocity(/*MOVEMENT_SPEED*/0);
      }
      if(pros::c::analogRead(LINE_L_PORT) >= LINE_VALUE_THRESHOLD || pros::c::analogRead(LINE_M_PORT) >= LINE_VALUE_THRESHOLD || pros::c::analogRead(LINE_R_PORT) >= LINE_VALUE_THRESHOLD)
      {
        rotate(fabs(LeftMotor.get_position() - lDeg) / (2 * ROBOT_TO_MOTOR_DEG));
        /*printf("mr: %f\n", fabs(LeftMotor.get_position() - lDeg) / (2 * ROBOT_TO_MOTOR_DEG));
        printf("ma: %f\n", (acos(-1) / 180.0) * fabs(LeftMotor.get_position() - lDeg) / (2 * ROBOT_TO_MOTOR_DEG));
        printf("mm: %f\n", SENSOR_TO_WHEEL_DIST * sin((acos(-1) / 180.0) * fabs(LeftMotor.get_position() - lDeg) / (2 * ROBOT_TO_MOTOR_DEG)));*/
        move(SENSOR_TO_WHEEL_DIST * sin((acos(-1) / 180.0) * fabs(LeftMotor.get_position() - lDeg) / (2 * ROBOT_TO_MOTOR_DEG)));
        continue;
      }
      LeftMotor.move_absolute(lDeg, MOVEMENT_SPEED);
      await1Motor(LeftMotor, lDeg, MOVE_SENSITIVITY, 10000);
      lDeg = LeftMotor.get_position();
      rDeg = RightMotor.get_position();
      lastArc = true;
      //Turn right until we see something or have turned REALIGN_LINE_TURN_DEG robot degrees
      while(pros::c::analogRead(LINE_L_PORT) < LINE_VALUE_THRESHOLD && pros::c::analogRead(LINE_M_PORT) < LINE_VALUE_THRESHOLD && pros::c::analogRead(LINE_R_PORT) < LINE_VALUE_THRESHOLD && (/*fabs(LeftMotor.get_position() - lDeg) +*/ fabs(RightMotor.get_position() - rDeg)) / (/*2.0 **/ ROBOT_TO_MOTOR_DEG) < REALIGN_LINE_TURN_DEG)
      {
        LeftMotor.move_velocity(/*MOVEMENT_SPEED*/0);
        RightMotor.move_velocity(-MOVEMENT_SPEED);
      }
      if(pros::c::analogRead(LINE_L_PORT) >= LINE_VALUE_THRESHOLD || pros::c::analogRead(LINE_M_PORT) >= LINE_VALUE_THRESHOLD || pros::c::analogRead(LINE_R_PORT) >= LINE_VALUE_THRESHOLD)
      {
        rotate(-fabs(RightMotor.get_position() - rDeg) / (2 * ROBOT_TO_MOTOR_DEG));
        /*printf("mr: %f\n", fabs(RightMotor.get_position() - rDeg) / (2 * ROBOT_TO_MOTOR_DEG));
        printf("ma: %f\n", (acos(-1) / 180.0) * fabs(RightMotor.get_position() - rDeg) / (2 * ROBOT_TO_MOTOR_DEG));
        printf("mm: %f\n", SENSOR_TO_WHEEL_DIST * sin((acos(-1) / 180.0) * fabs(RightMotor.get_position() - rDeg) / (2 * ROBOT_TO_MOTOR_DEG)));*/
        move(SENSOR_TO_WHEEL_DIST * sin((acos(-1) / 180.0) * fabs(RightMotor.get_position() - rDeg) / (2 * ROBOT_TO_MOTOR_DEG)));
        continue;
      }
      RightMotor.move_absolute(rDeg, MOVEMENT_SPEED);
      await1Motor(RightMotor, rDeg, MOVE_SENSITIVITY, 10000);
      lDeg = LeftMotor.get_position();
      rDeg = RightMotor.get_position();
      //Move forward until we see something or have moved REALIGN_LINE_MOVE_DEG motor degrees
      while(pros::c::analogRead(LINE_L_PORT) < LINE_VALUE_THRESHOLD && pros::c::analogRead(LINE_M_PORT) < LINE_VALUE_THRESHOLD && pros::c::analogRead(LINE_R_PORT) < LINE_VALUE_THRESHOLD && (fabs(LeftMotor.get_position() - lDeg) + fabs(RightMotor.get_position() - rDeg)) / 2.0 < REALIGN_LINE_MOVE_DEG)
      {
        LeftMotor.move_velocity(LINE_MOVEMENT_SPEED);
        RightMotor.move_velocity(LINE_MOVEMENT_SPEED);
      }
      /*if(leftDefault) //If we suspect we'll be to the left of the line (turn right) more frequently, etc.
      {
        LeftMotor.move_velocity(-LINE_MOVEMENT_SPEED);
        RightMotor.move_velocity(LINE_MOVEMENT_SPEED);
      }
      else
      {
        LeftMotor.move_velocity(LINE_MOVEMENT_SPEED);
        RightMotor.move_velocity(-LINE_MOVEMENT_SPEED);
      }*/
    }
  }
  while(true)
  {
    lineL = pros::c::analogRead(LINE_L_PORT) >= LINE_VALUE_THRESHOLD;
    lineM = pros::c::analogRead(LINE_M_PORT) >= LINE_VALUE_THRESHOLD;
    lineR = pros::c::analogRead(LINE_R_PORT) >= LINE_VALUE_THRESHOLD;
    lineB = pros::c::analogRead(LINE_B_PORT) >= LINE_VALUE_THRESHOLD;
    //printf("%f\n", (pros::c::ultrasonicGet(SonarL) + pros::c::ultrasonicGet(SonarR)) / 20.0);
    //printf("%d %d %d %d\n", (lineL ? 1 : 0), (lineM ? 1 : 0), (lineR ? 1 : 0), (lineB ? 1 : 0));
    //printf("%d\n", pros::c::analogRead(LINE_B_PORT));
    if(lineL)
    {
      LeftMotor.move_velocity(-LINE_MOVEMENT_SPEED);
      RightMotor.move_velocity(LINE_MOVEMENT_SPEED);
    }
    else if(lineR)
    {
      LeftMotor.move_velocity(LINE_MOVEMENT_SPEED);
      RightMotor.move_velocity(-LINE_MOVEMENT_SPEED);
    }
    /*else if(lineM && !lineB)
    {
      LeftMotor.move_velocity(LINE_MOVEMENT_SPEED);
      RightMotor.move_velocity(LINE_MOVEMENT_SPEED);
    }*/
    else if(lineM && !lineL && !lineR /*&& lineB*/ && fmin(pros::c::ultrasonicGet(SonarL), pros::c::ultrasonicGet(SonarR)) / 10.0 <= dist / 2.0)
    {
      resetMotors();
      break;
    }
    else
    {
      LeftMotor.move_velocity(LINE_MOVEMENT_SPEED);
      RightMotor.move_velocity(LINE_MOVEMENT_SPEED);
    }
  }
  resetMotors();
  /*while(pros::c::analogRead(LINE_L_PORT) < LINE_VALUE_THRESHOLD)
  {
    LeftMotor.move_velocity(LINE_MOVEMENT_SPEED);
    RightMotor.move_velocity(-LINE_MOVEMENT_SPEED);
  }
  resetMotors();
  double lDegrees = LeftMotor.get_position();
  double rDegrees = RightMotor.get_position();
  while(pros::c::analogRead(LINE_R_PORT) < LINE_VALUE_THRESHOLD)
  {
    LeftMotor.move_velocity(-LINE_MOVEMENT_SPEED);
    RightMotor.move_velocity(LINE_MOVEMENT_SPEED);
  }
  resetMotors();
  LeftMotor.move_absolute((LeftMotor.get_position() + lDegrees) / 2.0, LINE_MOVEMENT_SPEED);
  RightMotor.move_absolute((RightMotor.get_position() + rDegrees) / 2.0, LINE_MOVEMENT_SPEED);*/
  realign();
  /*
  //Move until the back sensor is at the base of the line
  while(true)
  {
    lineL = pros::c::analogRead(LINE_L_PORT) >= LINE_VALUE_THRESHOLD;
    lineM = pros::c::analogRead(LINE_M_PORT) >= LINE_VALUE_THRESHOLD;
    lineR = pros::c::analogRead(LINE_R_PORT) >= LINE_VALUE_THRESHOLD;
    lineB = pros::c::analogRead(LINE_B_PORT) >= LINE_VALUE_THRESHOLD;
    printf("%d %d %d %d\n", (lineL ? 1 : 0), (lineM ? 1 : 0), (lineR ? 1 : 0), (lineB ? 1 : 0));
    printf("%d\n", pros::c::analogRead(LINE_B_PORT));
    if(lineL)
    {
      LeftMotor.move_velocity(-LINE_MOVEMENT_SPEED);
      RightMotor.move_velocity(LINE_MOVEMENT_SPEED);
    }
    else if(lineR)
    {
      LeftMotor.move_velocity(LINE_MOVEMENT_SPEED);
      RightMotor.move_velocity(-LINE_MOVEMENT_SPEED);
    }
    else if(!lineM)
    {
      LeftMotor.move_velocity(-LINE_MOVEMENT_SPEED);
      RightMotor.move_velocity(-LINE_MOVEMENT_SPEED);
    }
    else if(lineM && lineB)
    {
      break;
    }
    else
    {
      LeftMotor.move_velocity(LINE_MOVEMENT_SPEED);
      RightMotor.move_velocity(LINE_MOVEMENT_SPEED);
    }
  }
  resetMotors();
  while(true)
  {
    lineL = pros::c::analogRead(LINE_L_PORT) >= LINE_VALUE_THRESHOLD;
    lineM = pros::c::analogRead(LINE_M_PORT) >= LINE_VALUE_THRESHOLD;
    lineR = pros::c::analogRead(LINE_R_PORT) >= LINE_VALUE_THRESHOLD;
    lineB = pros::c::analogRead(LINE_B_PORT) >= LINE_VALUE_THRESHOLD;
    printf("%d %d %d %d\n", (lineL ? 1 : 0), (lineM ? 1 : 0), (lineR ? 1 : 0), (lineB ? 1 : 0));
    printf("%d\n", pros::c::analogRead(LINE_B_PORT));
    if(lineL)
    {
      LeftMotor.move_velocity(-LINE_MOVEMENT_SPEED);
      RightMotor.move_velocity(LINE_MOVEMENT_SPEED);
    }
    else if(lineR)
    {
      LeftMotor.move_velocity(LINE_MOVEMENT_SPEED);
      RightMotor.move_velocity(-LINE_MOVEMENT_SPEED);
    }
    else if(lineM && lineB)
    {
      LeftMotor.move_velocity(-LINE_MOVEMENT_SPEED);
      RightMotor.move_velocity(-LINE_MOVEMENT_SPEED);
    }
    else if(lineM && !lineB)
    {
      break;
    }
    else
    {
      LeftMotor.move_velocity(LINE_MOVEMENT_SPEED);
      RightMotor.move_velocity(LINE_MOVEMENT_SPEED);
    }
  }
  resetMotors();
  printf("Moving.\n");
  move(REALIGN_LINE_FORWARD, LINE_MOVEMENT_SPEED);*/
  moveUntilDist(dist, 40);
}
void realignLine(double dist)
{
  realignLine(true, dist);
}
void closeClawOnBlock(int timeout)
{
  ClawMotor.set_brake_mode(MOTOR_BRAKE_HOLD);//setTimeout(1500, timeUnits::msec);
  ClawMotor.move_absolute(CLAW_CLOSE_DEG, CLAW_MOVE_SPEED);
  await1Motor(ClawMotor, CLAW_CLOSE_DEG, 1, timeout);
}
void closeClawOnBlock()
{
  closeClawOnBlock(1500);
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
  ClawMotor.move(0);
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
  //Relax the claw while holding the block upright to prevent overheating
  ClawMotor.move(0);
  ClawMotor.set_brake_mode(MOTOR_BRAKE_COAST);
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
  closeClawOnBlock(300);
  lowerBlock();
  openClawTensed();
  raiseArms();
  openClawRelaxed();
  holdingBlock = false;
}
