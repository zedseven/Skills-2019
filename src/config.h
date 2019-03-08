#pragma once

//Motor Ports
#define LEFT_MOTOR_PORT 1
#define RIGHT_MOTOR_PORT 2
#define CLAW_MOTOR_PORT 3
#define ARM_LEFT_PORT 20
#define ARM_RIGHT_PORT 11
#define LAMP_PORT 10
#define BLOCK_VISION_PORT 5
#define FLOOR_VISION_PORT 6
#define SONAR_L_PORT_IN 1
#define SONAR_L_PORT_OUT 2
#define SONAR_R_PORT_IN 3
#define SONAR_R_PORT_OUT 4

//Vision Sensor Signature Constants
#define SIG_BLUE_BLOCK 0
#define SIG_YELLOW_BLOCK 1
#define SIG_RED_BLOCK 2
#define SIG_BLUE_FLOOR 0
#define SIG_YELLOW_FLOOR 1
#define SIG_RED_FLOOR 2
#define SIG_NULL 6

//Constants
#define WIDTH_TO_DIST 7.81705
#define CM_TO_MOTOR_DEG 16.4245
#define ROBOT_TO_MOTOR_DEG 3.5114//3.5097;//3.508457;
#define BRAIN_SCREEN_WIDTH 480
#define BRAIN_SCREEN_HEIGHT 272
#define VISION_WIDTH 304
#define VISION_HEIGHT 208
enum class BlockType { None, Blue, Yellow, Red };
enum class MovementType { None, Left, Right, Forward, Backward };

//Config Variables
#define SNAPSHOT_OBJ_COUNT 10
#define MOVEMENT_SPEED 40
#define REALIGN_SPEED 5
#define VISION_CENTER 150
#define VISION_CENTER_SENSITIVITY 40
#define PICKUP_DIST 23
#define PICKUP_SENSITIVITY 1.0
#define DROPOFF_DIST 11
#define DROPOFF_SENSITIVITY 0.6
#define ARM_PICKUP_DEG 450
#define ARM_DROPOFF_DEG 550
#define ARM_PICKUP_SENSITIVITY 5
#define ARM_PICKUP_SPEED 30
#define MOVE_SENSITIVITY 1
#define DROPOFF_MIN 110
#define DROPOFF_MAX 150
#define CLAW_CLOSE_DEG -190//-170;//-155; //degrees
#define CLAW_MOVE_SPEED 40
#define LAMP_ON_SPEED 100
#define REALIGN_SENSITIVITY 0.1
#define REALIGN_DIST 50.0
#define MOVE_UNTIL_SENSITIVITY 1.0
#define BLOCK_TYPE_MIN_X 300
#define BLOCK_TYPE_MAX_X 350
#define BLOCK_TYPE_MIN_Y 100
#define BLOCK_TYPE_MAX_Y 150
