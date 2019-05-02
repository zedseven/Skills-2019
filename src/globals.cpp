#include "main.h"
#include "globals.h"
#include "config.h"
#include "api.h"
#include "pros/api_legacy.h"
#include "pros/vision.hpp"

//Motors and Sensors
pros::Motor LeftMotor(LEFT_MOTOR_PORT, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);
pros::Motor RightMotor(RIGHT_MOTOR_PORT, MOTOR_GEARSET_18, true, MOTOR_ENCODER_DEGREES);
pros::Motor ClawMotor(CLAW_MOTOR_PORT, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);
pros::Motor ArmMotorL(ARM_LEFT_PORT, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);
pros::Motor ArmMotorR(ARM_RIGHT_PORT, MOTOR_GEARSET_18, true, MOTOR_ENCODER_DEGREES);
pros::Motor Lamp(LAMP_PORT, MOTOR_GEARSET_18, false, MOTOR_ENCODER_DEGREES);
pros::Vision BlockVision(BLOCK_VISION_PORT, pros::E_VISION_ZERO_CENTER);
pros::Vision FloorVision(FLOOR_VISION_PORT, pros::E_VISION_ZERO_CENTER);
Ultrasonic SonarL = pros::c::ultrasonicInit(SONAR_L_PORT_IN, SONAR_L_PORT_OUT);
Ultrasonic SonarR = pros::c::ultrasonicInit(SONAR_R_PORT_IN, SONAR_R_PORT_OUT);

//Utility Variables
okapi::Timer timer;

//Runtime Variables
BlockType targetBlock = BlockType::None;
bool holdingBlock = false;
pros::vision_object_s_t blockVisionObjects[VISION_NUM_OBJECTS];
pros::vision_object_s_t floorVisionObjects[VISION_NUM_OBJECTS];
int blockVisionObjectCount = 0;
int floorVisionObjectCount = 0;
std::vector<lv_obj_t *> screenObjects;
int deliveredBlues = 0;
int deliveredReds = 0;
int deliveredYellows = 0;

void initSensors()
{
  pros::c::pinMode(LINE_L_PORT, INPUT_ANALOG);
  pros::c::pinMode(LINE_M_PORT, INPUT_ANALOG);
  pros::c::pinMode(LINE_R_PORT, INPUT_ANALOG);
  pros::c::pinMode(LINE_B_PORT, INPUT_ANALOG);
}
