#include "main.h"
#include "config.h"
#include "okapi/api.hpp"

//Motors and Sensors
extern pros::Motor LeftMotor;
extern pros::Motor RightMotor;
extern pros::Motor ClawMotor;
extern pros::Motor ArmMotorL;
extern pros::Motor ArmMotorR;
extern pros::Motor Lamp;
extern pros::Vision BlockVision;
extern pros::Vision FloorVision;

//Utility Variables
extern okapi::Rate rate;

//Runtime Variables
extern BlockType targetBlock;
extern bool holdingBlock;
