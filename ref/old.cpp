#include <cmath>
#include <vector>
#include <tuple>
#include "robot-config.h"
/*
Robotics Club Skills Canada Autonomous 2019
Zacchary Dempsey-Plante
For future reference:
-Brain screen is 480*272px
-Vision sensor picture is 304*208px
*/

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

//Methods
//Utility Methods
double getDistFromObjWidth(int objWidth)
{
    return widthToDistConst / (((double) objWidth) / visionWidth);
}
vision::signature& blockTypeToBlockSig(BlockType blockType)
{
    switch(blockType)
    {
        case BlockType::Blue:
            return SIG_BLUE_BLOCK;
        case BlockType::Yellow:
            return SIG_YELLOW_BLOCK;
        case BlockType::Red:
            return SIG_RED_BLOCK;
        default:
            return SIG_NULL_7A; //Cannot convert BlockType::None to a signature
    }
}
vision::signature& blockTypeToFloorSig(BlockType blockType)
{
    switch(blockType)
    {
        case BlockType::Blue:
            return SIG_BLUE_FLOOR;
        case BlockType::Yellow:
            return SIG_YELLOW_FLOOR;
        case BlockType::Red:
            return SIG_RED_FLOOR;
        default:
            return SIG_NULL_7B; //Cannot convert BlockType::None to a signature
    }
}
color blockTypeToColour(BlockType blockType)
{
    switch(blockType)
    {
        case BlockType::Blue:
            return color(0, 0, 255);
        case BlockType::Yellow:
            return color(255, 255, 0);
        case BlockType::Red:
            return color(255, 0, 0);
        default:
            return color(255, 0, 255); //Cannot convert BlockType::None to a colour
    }
}

//Display/Debug Methods
void drawAllSnapshotObjects(vex::color colour, bool blockVision)
{
    Brain.Screen.setFillColor(blockTypeToColour(targetBlock));
    Brain.Screen.drawRectangle(blockTypeMinX, blockTypeMinY, blockTypeMaxX - blockTypeMinX, blockTypeMaxY - blockTypeMinY);
    Brain.Screen.setFillColor(colour);
    if(blockVision)
    {
        Brain.Screen.drawRectangle(BlockVision.largestObject.centerX - BlockVision.largestObject.width / 2, BlockVision.largestObject.centerY - BlockVision.largestObject.height / 2, BlockVision.largestObject.width, BlockVision.largestObject.height);
        /*for(int i = 0; i < BlockVision.objectCount; i++)
        {
            Brain.Screen.drawRectangle(BlockVision.objects[i].centerX - BlockVision.objects[i].width / 2, BlockVision.objects[i].centerY - BlockVision.objects[i].height / 2, BlockVision.objects[i].width, BlockVision.objects[i].height);
        }*/
    }
    else
    {
        for(int i = 0; i < FloorVision.objectCount; i++)
        {
            Brain.Screen.drawRectangle(FloorVision.objects[i].centerX - FloorVision.objects[i].width / 2, FloorVision.objects[i].centerY - FloorVision.objects[i].height / 2, FloorVision.objects[i].width, FloorVision.objects[i].height);
        }
    }
}

//Mechanical Methods
void resetMotors()
{
    LeftMotor.setStopping(brakeType::brake);
    RightMotor.setStopping(brakeType::brake);
    LeftMotor.stop();
    RightMotor.stop();
    ClawMotor.stop();

}
void resetMotorsAndLamp()
{
    Lamp.stop();
    resetMotors();
}
void initMotors()
{
    resetMotorsAndLamp();
    ClawMotor.startRotateTo(0, rotationUnits::deg, 30, velocityUnits::rpm);
    ArmMotorL.startRotateTo(0, rotationUnits::deg, 30, velocityUnits::rpm);
    ArmMotorR.startRotateTo(0, rotationUnits::deg, 30, velocityUnits::rpm);
}
void moveDeg(double moveDegs, double moveSpeed) //distance in cm
{
    double leftStartDeg = LeftMotor.rotation(rotationUnits::deg);
    double rightStartDeg = RightMotor.rotation(rotationUnits::deg);
    LeftMotor.startRotateFor(moveDegs, rotationUnits::deg, moveSpeed, velocityUnits::rpm);
    RightMotor.startRotateFor(moveDegs, rotationUnits::deg, moveSpeed, velocityUnits::rpm);
    while(abs(abs(LeftMotor.rotation(rotationUnits::deg) - leftStartDeg) - abs(moveDegs)) > moveBand || abs(abs(RightMotor.rotation(rotationUnits::deg) - rightStartDeg) - abs(moveDegs)) > moveBand)
        vex::task::sleep(50);
}
void moveDeg(double moveDegs)
{
    moveDeg(moveDegs, movementSpeed);
}
void move(double distance, double moveSpeed) //distance in cm
{
    moveDeg(distance * cmToMotorDeg, moveSpeed);
}
void move(double distance) //distance in cm
{
    moveDeg(distance * cmToMotorDeg);
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
void switchTargetBlock(BlockType blockType)
{
    targetBlock = blockType;
}
void screenPressed()
{
    if (Brain.Screen.xPosition() >= blockTypeMinX && Brain.Screen.xPosition() <= blockTypeMaxX && Brain.Screen.yPosition() >= blockTypeMinY && Brain.Screen.yPosition() <= blockTypeMaxY)
    {
        char targetBlockInt = ((char) targetBlock) + 1;
        if (targetBlockInt > 3)
            targetBlockInt = 1;
        switchTargetBlock((BlockType) targetBlockInt);
    }
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

//Operation Methods
void findBlock() //Looking for a block
{
    if(targetBlock != BlockType::Yellow)
        Lamp.spin(directionType::fwd, lampOnSpeed, velocityUnits::rpm);
    else
        Lamp.stop();

    bool finished = false;
    std::vector<std::tuple<MovementType, double, double>> trackedMovements;
    MovementType lastMvmt = MovementType::None;
    trackedMovements.push_back(std::make_tuple(MovementType::None, LeftMotor.rotation(rotationUnits::deg), RightMotor.rotation(rotationUnits::deg)));

    while(!finished)
    {
        Brain.Screen.clearScreen();

        BlockVision.takeSnapshot(blockTypeToBlockSig(targetBlock));
        drawAllSnapshotObjects(blockTypeToColour(targetBlock), true);
        if(BlockVision.objectCount > 0)
        {
            //Turn right
            if(BlockVision.largestObject.centerX < visionCenter - visionCenterBand)
            {
                lastMvmt = MovementType::Right;
                LeftMotor.spin(directionType::rev, movementSpeed, velocityUnits::rpm);
                RightMotor.spin(directionType::fwd, movementSpeed, velocityUnits::rpm);
            }
            //Turn left
            else if(BlockVision.largestObject.centerX > visionCenter + visionCenterBand)
            {
                lastMvmt = MovementType::Left;
                LeftMotor.spin(directionType::fwd, movementSpeed, velocityUnits::rpm);
                RightMotor.spin(directionType::rev, movementSpeed, velocityUnits::rpm);
            }
            //Go straight
            else
            {
                float blockDist = getDistFromObjWidth(BlockVision.largestObject.width);
                //Too close - go backwards
                if(blockDist < pickupDist - pickupBand)
                {
                    lastMvmt = MovementType::Backward;
                    LeftMotor.spin(directionType::rev, movementSpeed, velocityUnits::rpm);
                    RightMotor.spin(directionType::rev, movementSpeed, velocityUnits::rpm);
                }
                //Too far - go forwards
                else if(blockDist > pickupDist + pickupBand)
                {
                    lastMvmt = MovementType::Forward;
                    LeftMotor.spin(directionType::fwd, movementSpeed, velocityUnits::rpm);
                    RightMotor.spin(directionType::fwd, movementSpeed, velocityUnits::rpm);
                }
                //Optimal distance to pick up with claw
                else
                {
                    lastMvmt = MovementType::None;
                    resetMotors();
                    pickupBlock();
                    finished = true;
                }
            }
            if(trackedMovements.empty() || std::get<0>(trackedMovements.back()) != lastMvmt)
            {
                MovementType mvmtType = std::get<0>(trackedMovements.back());
                trackedMovements.pop_back();
                trackedMovements.push_back(std::make_tuple(mvmtType, LeftMotor.rotation(rotationUnits::deg), RightMotor.rotation(rotationUnits::deg)));
                if(lastMvmt != MovementType::None)
                    trackedMovements.push_back(std::make_tuple(lastMvmt, 0.0, 0.0));
            }
        }
        else
        {
            resetMotors();
        }
        vex::task::sleep(50);
    }

    Lamp.stop();

    for(int i = trackedMovements.size() - 1; i >= 1; i--)
    {
        if(std::get<0>(trackedMovements[i]) == MovementType::None)
            continue;
        int j = i - 1;
        if(std::get<0>(trackedMovements[i]) == MovementType::Left || std::get<0>(trackedMovements[i]) == MovementType::Right)
            while(j > 0 && std::get<0>(trackedMovements[j]) == MovementType::Left || std::get<0>(trackedMovements[j]) == MovementType::Right)
                j--;
        else if(std::get<0>(trackedMovements[i]) == MovementType::Forward || std::get<0>(trackedMovements[i]) == MovementType::Backward)
            while(j > 0 && std::get<0>(trackedMovements[j]) == MovementType::Forward || std::get<0>(trackedMovements[j]) == MovementType::Backward)
                j--;
        i = j + 1;
        double leftTarget = std::get<1>(trackedMovements[j]);
        double rightTarget = std::get<2>(trackedMovements[j]);
        LeftMotor.startRotateTo(leftTarget, rotationUnits::deg, movementSpeed, velocityUnits::rpm);
        RightMotor.startRotateTo(rightTarget, rotationUnits::deg, movementSpeed, velocityUnits::rpm);
        while(abs(abs(LeftMotor.rotation(rotationUnits::deg)) - abs(leftTarget)) > moveBand || abs(abs(RightMotor.rotation(rotationUnits::deg)) - abs(rightTarget)) > moveBand)
            vex::task::sleep(50);
    }
}

void findPad() //Have a block, looking for the floor tile to deposit it at
{
    if(targetBlock != BlockType::Yellow)
        Lamp.spin(directionType::fwd, lampOnSpeed, velocityUnits::rpm);
    else
        Lamp.stop();

    bool finished = false;
    std::vector<std::tuple<MovementType, double, double>> trackedMovements;
    MovementType lastMvmt = MovementType::None;
    trackedMovements.push_back(std::make_tuple(MovementType::None, LeftMotor.rotation(rotationUnits::deg), RightMotor.rotation(rotationUnits::deg)));

    while(!finished)
    {
        Brain.Screen.clearScreen();

        FloorVision.takeSnapshot(blockTypeToFloorSig(targetBlock));
        drawAllSnapshotObjects(blockTypeToColour(targetBlock), false);
        if(FloorVision.objectCount > 0)
        {
            //Turn right
            if(FloorVision.largestObject.centerX < visionCenter - visionCenterBand)
            {
                lastMvmt = MovementType::Right;
                LeftMotor.spin(directionType::rev, movementSpeed, velocityUnits::rpm);
                RightMotor.spin(directionType::fwd, movementSpeed, velocityUnits::rpm);
            }
            //Turn left
            else if(FloorVision.largestObject.centerX > visionCenter + visionCenterBand)
            {
                lastMvmt = MovementType::Left;
                LeftMotor.spin(directionType::fwd, movementSpeed, velocityUnits::rpm);
                RightMotor.spin(directionType::rev, movementSpeed, velocityUnits::rpm);
            }
            //Go straight
            else
            {
                //Not close enough - go forwards
                if(FloorVision.largestObject.centerY < dropoffMin)
                {
                    lastMvmt = MovementType::Forward;
                    LeftMotor.spin(directionType::fwd, movementSpeed, velocityUnits::rpm);
                    RightMotor.spin(directionType::fwd, movementSpeed, velocityUnits::rpm);
                }
                //Too far - go backwards
                else if(FloorVision.largestObject.centerY > dropoffMax)
                {
                    lastMvmt = MovementType::Backward;
                    LeftMotor.spin(directionType::rev, movementSpeed, velocityUnits::rpm);
                    RightMotor.spin(directionType::rev, movementSpeed, velocityUnits::rpm);
                }
                else
                {
                    lastMvmt = MovementType::None;
                    resetMotors();
                    move(12); //THIS NEEDS TO BE CHANGED FOR IF HAVING 0 | 1 OF BLOCK ALREADY
                    dropoffBlock();
                    move(-12);
                    finished = true;
                }
            }
            if(trackedMovements.empty() || std::get<0>(trackedMovements.back()) != lastMvmt)
            {
                MovementType mvmtType = std::get<0>(trackedMovements.back());
                trackedMovements.pop_back();
                trackedMovements.push_back(std::make_tuple(mvmtType, LeftMotor.rotation(rotationUnits::deg), RightMotor.rotation(rotationUnits::deg)));
                if(lastMvmt != MovementType::None)
                    trackedMovements.push_back(std::make_tuple(lastMvmt, 0.0, 0.0));
            }
        }
        else
        {
            resetMotors();
        }
        vex::task::sleep(50);
    }

    Lamp.stop();

    for(int i = trackedMovements.size() - 1; i >= 1; i--)
    {
        if(std::get<0>(trackedMovements[i]) == MovementType::None)
            continue;
        int j = i - 1;
        if(std::get<0>(trackedMovements[i]) == MovementType::Left || std::get<0>(trackedMovements[i]) == MovementType::Right)
            while(j > 0 && std::get<0>(trackedMovements[j]) == MovementType::Left || std::get<0>(trackedMovements[j]) == MovementType::Right)
                j--;
        else if(std::get<0>(trackedMovements[i]) == MovementType::Forward || std::get<0>(trackedMovements[i]) == MovementType::Backward)
            while(j > 0 && std::get<0>(trackedMovements[j]) == MovementType::Forward || std::get<0>(trackedMovements[j]) == MovementType::Backward)
                j--;
        i = j + 1;
        double leftTarget = std::get<1>(trackedMovements[j]);
        double rightTarget = std::get<2>(trackedMovements[j]);
        LeftMotor.startRotateTo(leftTarget, rotationUnits::deg, movementSpeed, velocityUnits::rpm);
        RightMotor.startRotateTo(rightTarget, rotationUnits::deg, movementSpeed, velocityUnits::rpm);
        while(abs(abs(LeftMotor.rotation(rotationUnits::deg)) - abs(leftTarget)) > moveBand || abs(abs(RightMotor.rotation(rotationUnits::deg)) - abs(rightTarget)) > moveBand)
            vex::task::sleep(50);
    }
}

void layoutB()
{
    //Red Block A
    switchTargetBlock(BlockType::Red);
    moveUntilDist(145, 80);
    rotate(90);
    findBlock();
    rotate(-90);
    moveUntilDist(56, 30);
    rotate(-45);
    findPad();
    //Blue Block A
    switchTargetBlock(BlockType::Blue);
    moveUntilDist(123, 30);
    rotate(90);
    findBlock();
    rotate(-90);
    moveUntilDist(55, 30);
    rotate(90);
    moveUntilDist(55, 30);
    rotate(-45);
    findPad();
    //Yellow Block A
    switchTargetBlock(BlockType::Yellow);
    rotate(135);
    moveUntilDist(88, 30);
    rotate(90);
    findBlock();
    rotate(-90);
    moveUntilDist(56, 30);
    rotate(-45);
    findPad();
    //Blue Block B
    rotate(-135);
    moveUntilDist(88, 30);
    rotate(-90);
    findBlock();
    rotate(90);
    realign();
    move(31.5);
    rotate(45);
    findPad();
    //Red Block B
    switchTargetBlock(BlockType::Red);
    rotate(-45);
    realign();
    //moveUntilDist(123, 30);
    move(-55);
    //realign();
    rotate(-90);
    findBlock();
    rotate(90);
    moveUntilDist(55, 30);
    rotate(-90);
    moveUntilDist(55, 30);
    rotate(45);
    findPad();
}

int main()
{
    Brain.Screen.print("User Program has Started.");
    //Brain.Screen.printAt(0, 50, "", BlockVision.getExposure());

    Brain.Screen.pressed(&screenPressed);

    BlockVision.setWifiMode(vex::vision::wifiMode::off);//on);
    FloorVision.setWifiMode(vex::vision::wifiMode::off);//on);

    //Debug TEMP
    /*switchTargetBlock(BlockType::Red);
    findBlock();
    return 0;*/

    initMotors();
    openClaw();

    //Debug Section
    switchTargetBlock(BlockType::Yellow);
    //realign();
    /*moveUntilDist(60.0, 20.0);
    Brain.Screen.printAt(50, 150, "Done!");
    /*rotate(90);
    rotate(-90);
    rotate(360);
    realign();*-/
    double lDist = -1.0;
    double rDist = -1.0;
    while(true || lDist < 0.0 || rDist < 0.0)
    {
        lDist = SonarL.distance(distanceUnits::cm);
        rDist = SonarR.distance(distanceUnits::cm);
        Brain.Screen.printAt(100, 100, "L: %f", lDist);
        Brain.Screen.printAt(100, 200, "R: %f", rDist);
    }
    return 0;*/

    /*std::vector<std::tuple<MovementType, double, double>> trackedMovements;
    trackedMovements.push_back(std::make_tuple(MovementType::None, LeftMotor.rotation(rotationUnits::deg), RightMotor.rotation(rotationUnits::deg)));
    move(30);
    trackedMovements.push_back(std::make_tuple(MovementType::Forward, LeftMotor.rotation(rotationUnits::deg), RightMotor.rotation(rotationUnits::deg)));
    rotate(-90);
    trackedMovements.push_back(std::make_tuple(MovementType::Left, LeftMotor.rotation(rotationUnits::deg), RightMotor.rotation(rotationUnits::deg)));
    rotate(35);
    trackedMovements.push_back(std::make_tuple(MovementType::Right, LeftMotor.rotation(rotationUnits::deg), RightMotor.rotation(rotationUnits::deg)));
    rotate(-120);
    trackedMovements.push_back(std::make_tuple(MovementType::Left, LeftMotor.rotation(rotationUnits::deg), RightMotor.rotation(rotationUnits::deg)));
    rotate(-90);
    trackedMovements.push_back(std::make_tuple(MovementType::Right, LeftMotor.rotation(rotationUnits::deg), RightMotor.rotation(rotationUnits::deg)));
    move(-10);
    trackedMovements.push_back(std::make_tuple(MovementType::Backward, LeftMotor.rotation(rotationUnits::deg), RightMotor.rotation(rotationUnits::deg)));
    vex::task::sleep(500);

    for(int i = trackedMovements.size() - 1; i >= 1; i--)
    {
        if(std::get<0>(trackedMovements[i]) == MovementType::None)
            continue;
        int j = i - 1;
        if(std::get<0>(trackedMovements[i]) == MovementType::Left || std::get<0>(trackedMovements[i]) == MovementType::Right)
            while(j > 0 && std::get<0>(trackedMovements[j]) == MovementType::Left || std::get<0>(trackedMovements[j]) == MovementType::Right)
                j--;
        else if(std::get<0>(trackedMovements[i]) == MovementType::Forward || std::get<0>(trackedMovements[i]) == MovementType::Backward)
            while(j > 0 && std::get<0>(trackedMovements[j]) == MovementType::Forward || std::get<0>(trackedMovements[j]) == MovementType::Backward)
                j--;
        double leftTarget = std::get<1>(trackedMovements[j]);
        double rightTarget = std::get<2>(trackedMovements[j]);
        LeftMotor.startRotateTo(leftTarget, rotationUnits::deg, movementSpeed, velocityUnits::rpm);
        RightMotor.startRotateTo(rightTarget, rotationUnits::deg, movementSpeed, velocityUnits::rpm);
        while(abs(abs(LeftMotor.rotation(rotationUnits::deg)) - abs(leftTarget)) > moveBand || abs(abs(RightMotor.rotation(rotationUnits::deg)) - abs(rightTarget)) > moveBand)
            vex::task::sleep(50);
    }
    return 0*/

    //Temp config
    //Old testing config
    /*
    //Yellow block A
    move(35);
    rotate(-90);
    realign();
    //move(53);
    moveUntilDist(145, 60);
    //realign();
    rotate(-90);
    switchTargetBlock(BlockType::Yellow);
    findBlock();
    rotate(-90);
    //realign();
    move(31.5);
    rotate(-45);
    findPad();
    //Blue block A
    rotate(-135);
    moveUntilDist(88, 30);
    //move(98);
    rotate(-90);
    switchTargetBlock(BlockType::Blue);
    findBlock();
    rotate(90);
    realign();
    move(31.5);
    rotate(45);
    findPad();
    //Red Block A
    switchTargetBlock(BlockType::Red);
    rotate(-45);
    realign();
    //moveUntilDist(123, 30);
    move(-55);
    //realign();
    rotate(-90);
    findBlock();
    //move(30);
    //switchTargetBlock(BlockType::Red);
    //findBlock();
    /*move(55);
    findBlock();
    move(-30);
    rotate(-90);*/
    //move(5);
    //rotate(45);
    //move(-20);
    //move(20);
    //rotate(-45);
    //move(-30);
    //rotate(-90);
    //findPad();*/

    move(34);
    rotate(-45);
    realign();

    //layoutA();
    layoutB();
    //layoutC();
    //layoutD();
    //layoutE();
    //layoutF();

    /*while(true)
    {
        //Brain.Screen.setFillColor(color(0, 0, 0));
        //Brain.Screen.printAt(1, 80, "The object count is %d", BlockVision.objectCount);
        //Brain.Screen.printAt(1, 160, "largestObject.centerX: %d", BlockVision.largestObject.centerX);
        vex::task::sleep(50);
    }*/
}
