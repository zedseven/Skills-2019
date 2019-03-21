#pragma once

#include "main.h"
#include "config.h"
#include "okapi/api.hpp"
#include "api.h"
#include "pros/adi.hpp"
#include "pros/api_legacy.h"

//Motors and Sensors
extern pros::Motor LeftMotor;
extern pros::Motor RightMotor;
extern pros::Motor ClawMotor;
extern pros::Motor ArmMotorL;
extern pros::Motor ArmMotorR;
extern pros::Motor Lamp;
extern pros::Vision BlockVision;
extern pros::Vision FloorVision;
extern Ultrasonic SonarL;
extern Ultrasonic SonarR;

//Utility Variables
//extern okapi::Rate rate;
extern okapi::Timer timer;

//Runtime Variables
extern BlockType targetBlock;
extern bool holdingBlock;
extern pros::vision_object_s_t blockVisionObjects[];
extern pros::vision_object_s_t floorVisionObjects[];
extern int blockVisionObjectCount;
extern int floorVisionObjectCount;
extern std::vector<lv_obj_t *> screenObjects;
