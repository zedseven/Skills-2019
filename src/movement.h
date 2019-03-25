#pragma once

#include "main.h"

void resetMotors();
void resetMotorsAndLamp();
void initMotors();
void await1Motor(pros::Motor motor, double target, double sensitivity, double timeout);
void await2Motors(pros::Motor motor1, pros::Motor motor2, double target1, double target2, double sensitivity1, double sensitivity2, double timeout);
void await2Motors(pros::Motor motor1, pros::Motor motor2, double target1, double target2, double sensitivity, double timeout);
void await2Motors(pros::Motor motor1, pros::Motor motor2, double target, double sensitivity, double timeout);
void spinMotorUntilTimeout(pros::Motor motor, double speed, double timeout);
void spinMotorUntilTimeout(pros::Motor motor, double speed);
void moveDeg(double moveDegs, double moveSpeed); //distance in cm
void moveDeg(double moveDegs);
void move(double distance, double moveSpeed); //distance in cm
void move(double distance); //distance in cm
void rotateDeg(double moveDeg);
void rotate(double rotationDeg);
void realign();
void moveUntilDist(double targetDist, double moveIncrement);
void closeClawOnBlock();
void openClaw();
void openClawRelaxed();
void pickupBlock();
void lowerBlock();
void raiseArms();
void dropoffBlock();
