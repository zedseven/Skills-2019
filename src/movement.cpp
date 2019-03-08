#include "main.h"
#include "config.h"
#include "globals.h"
#include "movement.h"
#include "okapi/api.hpp"

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
void moveDeg(double moveDegs, double moveSpeed) //distance in cm
{
  double leftStartDeg = LeftMotor.get_position();
  double rightStartDeg = RightMotor.get_position();
  LeftMotor.move_relative(moveDegs, moveSpeed);
  RightMotor.move_relative(moveDegs, moveSpeed);
  while(abs(abs(LeftMotor.get_position() - leftStartDeg) - abs(moveDegs)) > MOVE_SENSITIVITY || abs(abs(RightMotor.get_position() - rightStartDeg) - abs(moveDegs)) > MOVE_SENSITIVITY)
    rate.delayUntil(50);
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
  double leftStartDeg = LeftMotor.rotation(rotationUnits::deg);
  double rightStartDeg = RightMotor.rotation(rotationUnits::deg);
  LeftMotor.startRotateFor(moveDeg, rotationUnits::deg, movementSpeed, velocityUnits::rpm);
  RightMotor.startRotateFor(-moveDeg, rotationUnits::deg, -movementSpeed, velocityUnits::rpm);
  while(abs(abs(LeftMotor.rotation(rotationUnits::deg) - leftStartDeg) - abs(moveDeg)) > moveBand || abs(abs(RightMotor.rotation(rotationUnits::deg) - rightStartDeg) - abs(moveDeg)) > moveBand)
      vex::task::sleep(50);
}
void rotate(double rotationDeg)
{
  rotateDeg(rotationDeg * robotDegToMotorDeg);
}
void realign()
{
  double rBand;
  while(true)
  {
      vex::task::sleep(100);
      double lDist = -1.0;
      double rDist = -1.0;
      while(/*true || */lDist < 0.0 || rDist < 0.0)
      {
          lDist = SonarL.distance(distanceUnits::cm);
          rDist = SonarR.distance(distanceUnits::cm);
          Brain.Screen.printAt(100, 100, "L: %f", lDist);
          Brain.Screen.printAt(100, 200, "R: %f", rDist);
      }
      rBand = fmax(0.2, (((lDist + rDist) / 2.0) / realignDist) * 0.125 + realignBand);
      Brain.Screen.printAt(100, 220, "rBand: %f", rBand);
      double distDiff = rDist - lDist;
      if(distDiff > rBand) //Right is further away - turn left
      {
          LeftMotor.spin(directionType::rev, realignSpeed, velocityUnits::rpm);
          RightMotor.spin(directionType::fwd, realignSpeed, velocityUnits::rpm);
      }
      else if(distDiff < -rBand) //Left is further away - turn right
      {
          LeftMotor.spin(directionType::fwd, realignSpeed, velocityUnits::rpm);
          RightMotor.spin(directionType::rev, realignSpeed, velocityUnits::rpm);
      }
      else
      {
          resetMotors();
          bool done = true;
          for(int j = 0; j < 5; j++)
          {
              if(!done)
                  break;
              Brain.Screen.printAt(100, 150, "Main check #%d", j + 1);
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
                      lDistN = SonarL.distance(distanceUnits::cm);
                      rDistN = SonarR.distance(distanceUnits::cm);
                      Brain.Screen.printAt(100, 100, "L: %f", lDistN);
                      Brain.Screen.printAt(100, 200, "R: %f", rDistN);
                  }
                  if(fabs(rDistN - lDistN) > rBand /*|| fabs(lDist - lDistN) > realignBand || fabs(rDist - rDistN) > realignBand*/)
                  {
                      done = false;
                      break;
                  }
                  else
                  {
                      vex::task::sleep(50);
                  }
              }
          }
          if(done)
              break;
      }
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
          lDist = SonarL.distance(distanceUnits::cm);
          rDist = SonarR.distance(distanceUnits::cm);
      }
      measureDist = (lDist + rDist) / 2.0;
      distDiff = measureDist - targetDist;
      realign();
      if(distDiff > moveUntilBand)
          move(fmin(measureDist - targetDist, moveIncrement));
      else if(distDiff < -moveUntilBand)
          move(-fmin(targetDist - measureDist, moveIncrement));
      else
          break;
  }
  Brain.Screen.printAt(50, 120, "Done moving.");
  realign();
}
void closeClawOnBlock()
{
  ClawMotor.setTimeout(1500, timeUnits::msec);
  ClawMotor.setStopping(brakeType::hold);
  ClawMotor.rotateTo(clawCloseDeg, rotationUnits::deg, clawMoveSpeed, velocityUnits::rpm);
}
void openClaw()
{
  //ClawMotor.setTimeout(3000, timeUnits::msec);
  ClawMotor.rotateFor(2, timeUnits::sec, clawMoveSpeed, velocityUnits::rpm);
  //ClawMotor.rotateTo(0, rotationUnits::deg, clawMoveSpeed, velocityUnits::rpm);
  ClawMotor.resetRotation();
  ClawMotor.setStopping(brakeType::hold);
}
void pickupBlock()
{
  //Assuming claw is open
  ArmMotorL.startRotateTo(armPickupDeg, rotationUnits::deg, armPickupSpeed, velocityUnits::rpm);
  ArmMotorR.startRotateTo(armPickupDeg, rotationUnits::deg, armPickupSpeed, velocityUnits::rpm);
  //Wait until the arms have been lowered
  while(abs(ArmMotorL.rotation(rotationUnits::deg) - armPickupDeg) > armPickupBand || abs(ArmMotorR.rotation(rotationUnits::deg) - armPickupDeg) > armPickupBand)
    vex::task::sleep(50);
  closeClawOnBlock();
  ArmMotorL.startRotateTo(0, rotationUnits::deg, armPickupSpeed, velocityUnits::rpm);
  ArmMotorR.startRotateTo(0, rotationUnits::deg, armPickupSpeed, velocityUnits::rpm);
  //Wait until the arms have been raised
  while(abs(ArmMotorL.rotation(rotationUnits::deg)) > armPickupBand || abs(ArmMotorR.rotation(rotationUnits::deg)) > armPickupBand)
    vex::task::sleep(50);
  holdingBlock = true;
}
void dropoffBlock()
{
  //Assuming claw is closed
  ArmMotorL.startRotateTo(armDropoffDeg, rotationUnits::deg, armPickupSpeed, velocityUnits::rpm);
  ArmMotorR.startRotateTo(armDropoffDeg, rotationUnits::deg, armPickupSpeed, velocityUnits::rpm);
  //Wait until the arms have been lowered
  while(abs(ArmMotorL.rotation(rotationUnits::deg) - armDropoffDeg) > armPickupBand || abs(ArmMotorR.rotation(rotationUnits::deg) - armDropoffDeg) > armPickupBand)
    vex::task::sleep(50);
  openClaw();
  ArmMotorL.startRotateTo(0, rotationUnits::deg, armPickupSpeed, velocityUnits::rpm);
  ArmMotorR.startRotateTo(0, rotationUnits::deg, armPickupSpeed, velocityUnits::rpm);
  //Wait until the arms have been raised
  while(abs(ArmMotorL.rotation(rotationUnits::deg)) > armPickupBand || abs(ArmMotorR.rotation(rotationUnits::deg)) > armPickupBand)
    vex::task::sleep(50);
  holdingBlock = false;
}
