#include "main.h"
#include "globals.h"
#include "config.h"
#include "api.h"
#include "pros/api_legacy.h"

//Motors and Sensors
pros::Motor LeftMotor(LEFT_MOTOR_PORT, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);
pros::Motor RightMotor(RIGHT_MOTOR_PORT, MOTOR_GEARSET_18, true, MOTOR_ENCODER_DEGREES);
pros::Motor ClawMotor(CLAW_MOTOR_PORT, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);
pros::Motor ArmMotorL(ARM_LEFT_PORT, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);
pros::Motor ArmMotorR(ARM_RIGHT_PORT, MOTOR_GEARSET_18, true, MOTOR_ENCODER_DEGREES);
pros::Motor Lamp(LAMP_PORT, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);
pros::Vision BlockVision(BLOCK_VISION_PORT);
pros::Vision FloorVision(FLOOR_VISION_PORT);
Ultrasonic SonarL = pros::c::ultrasonicInit(SONAR_L_PORT_IN, SONAR_L_PORT_OUT);
Ultrasonic SonarR = pros::c::ultrasonicInit(SONAR_R_PORT_IN, SONAR_R_PORT_OUT);

//Utility Variables
okapi::Rate rate;
okapi::Timer timer;

//Runtime Variables
BlockType targetBlock = BlockType::None;
bool holdingBlock = false;
