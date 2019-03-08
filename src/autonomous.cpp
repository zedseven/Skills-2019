#include <cmath>
#include <vector>
#include <tuple>
#include "main.h"
#include "movement.h"

//Constants
const double widthToDistConst = 7.81705;
const double cmToMotorDeg = 16.4245;
const double robotDegToMotorDeg = 3.5114;//3.5097;//3.508457;
const int brainWidth = 480;
const int brainHeight = 272;
const int visionWidth = 304;
const int visionHeight = 208;
enum class BlockType { None, Blue, Yellow, Red };
enum class MovementType { None, Left, Right, Forward, Backward };

//Config Variables
const int snapshotObjCount = 10;
const int movementSpeed = 40; //rpm
const int realignSpeed = 5;//20; //rpm
const int visionCenter = 150; //rpm
const int visionCenterBand = 40;
const double pickupDist = 23;//24;//21.8; //cm
const double pickupBand = 1.0; //cm
const double dropoffDist = 11;//21.8; //cm
const double dropoffBand = 0.6; //cm
const double armPickupDeg = 450; //degrees
const double armDropoffDeg = 550; //degrees
const double armPickupBand = 5; //degrees
const int armPickupSpeed = 30; //rpm
const double moveBand = 1; //degrees
const int dropoffMin = 110; //px value in range 0-visionHeight
const int dropoffMax = 150; //px value in range 0-visionHeight
const double clawCloseDeg = -190;//-170;//-155; //degrees
const int clawMoveSpeed = 40; //rpm
const int lampOnSpeed = 100; //rpm
const double realignBand = 0.1; //cm
const double realignDist = 50.0; //cm
const double moveUntilBand = 1.0; //cm
const int blockTypeMinX = 300;
const int blockTypeMaxX = 350;
const int blockTypeMinY = 100;
const int blockTypeMaxY = 150;

//Runtime Variables
BlockType targetBlock = BlockType::None;
bool holdingBlock = false;

//Vision Sensor Signature Constants
#define SIG_BLUE_BLOCK 0
#define SIG_YELLOW_BLOCK 1
#define SIG_RED_BLOCK 2
#define SIG_BLUE_FLOOR 0
#define SIG_YELLOW_FLOOR 1
#define SIG_RED_FLOOR 2
#define SIG_NULL 6

void autonomous()
{
  pros::Motor LeftMotor(LEFT_MOTOR_PORT, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);
  pros::Motor RightMotor(RIGHT_MOTOR_PORT, MOTOR_GEARSET_18, true, MOTOR_ENCODER_DEGREES);
  pros::Motor ClawMotor(CLAW_MOTOR_PORT, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);
  pros::Motor ArmMotorL(ARM_LEFT_PORT, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);
  pros::Motor ArmMotorR(ARM_RIGHT_PORT, MOTOR_GEARSET_18, true, MOTOR_ENCODER_DEGREES);
  pros::Motor Lamp(LAMP_PORT, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);
  pros::Vision BlockVision(BLOCK_VISION_PORT);
  pros::Vision FloorVision(FLOOR_VISION_PORT);

  LeftMotor.move_relative(1000, 100);
  RightMotor.move_relative(1000, 100);
}
