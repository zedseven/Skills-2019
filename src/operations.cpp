#include "config.h"
#include "globals.h"
#include "movement.h"
#include "vision.h"

void findBlock() //Looking for a block
{
  if(targetBlock != BlockType::Yellow)
    Lamp.move_velocity(LAMP_ON_SPEED);
  else
    Lamp.move(0);

  bool finished = false;
  std::vector<std::tuple<MovementType, double, double>> trackedMovements;
  MovementType lastMvmt = MovementType::None;
  trackedMovements.push_back(std::make_tuple(MovementType::None, LeftMotor.get_position(), RightMotor.get_position()));

  while(!finished)
  {
    //Brain.Screen.clearScreen();

    blockVisionObjectCount = takeSnapshot(BlockVision, blockTypeToBlockSig(targetBlock), blockVisionObjects);
    drawAllSnapshotObjects(blockVisionObjectCount, blockVisionObjects, blockTypeToColour(targetBlock));
    if(blockVisionObjectCount > 0)
    {
      //Turn right
      printf("left_coord: %d\n", blockVisionObjects[0].left_coord);
      printf("top_coord: %d\n", blockVisionObjects[0].top_coord);
      printf("x_middle_coord: %d\n", blockVisionObjects[0].x_middle_coord);
      printf("y_middle_coord: %d\n", blockVisionObjects[0].y_middle_coord);
      printf("width: %d\n", blockVisionObjects[0].width);
      printf("height: %d\n", blockVisionObjects[0].height);
      //For some reason, x_middle_coord and y_middle_coord are not populated
      if(blockVisionObjects[0].left_coord + blockVisionObjects[0].width / 2.0 < VISION_CENTER - VISION_CENTER_SENSITIVITY)
      {
        lastMvmt = MovementType::Right;
        RightMotor.move_velocity(-MOVEMENT_SPEED);
        LeftMotor.move_velocity(MOVEMENT_SPEED);
      }
      //Turn left
      else if(blockVisionObjects[0].left_coord + blockVisionObjects[0].width / 2.0 > VISION_CENTER + VISION_CENTER_SENSITIVITY)
      {
        lastMvmt = MovementType::Left;
        LeftMotor.move_velocity(MOVEMENT_SPEED);
        RightMotor.move_velocity(-MOVEMENT_SPEED);
      }
      //Go straight
      else
      {
        float blockDist = getDistFromObjWidth(blockVisionObjects[0].width);
        //Too close - go backwards
        if(blockDist < PICKUP_DIST - PICKUP_SENSITIVITY)
        {
          lastMvmt = MovementType::Backward;
          LeftMotor.move_velocity(-MOVEMENT_SPEED);
          RightMotor.move_velocity(-MOVEMENT_SPEED);
        }
        //Too far - go forwards
        else if(blockDist > PICKUP_DIST + PICKUP_SENSITIVITY)
        {
          lastMvmt = MovementType::Forward;
          LeftMotor.move_velocity(MOVEMENT_SPEED);
          RightMotor.move_velocity(MOVEMENT_SPEED);
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
        trackedMovements.push_back(std::make_tuple(mvmtType, LeftMotor.get_position(), RightMotor.get_position()));
        if(lastMvmt != MovementType::None)
          trackedMovements.push_back(std::make_tuple(lastMvmt, 0.0, 0.0));
      }
    }
    else
    {
      resetMotors();
    }
    pros::delay(50);
  }

  Lamp.move(0);

  for(int i = trackedMovements.size() - 1; i >= 1; i--)
  {
      if(std::get<0>(trackedMovements[i]) == MovementType::None)
        continue;
      int j = i - 1;
      if(std::get<0>(trackedMovements[i]) == MovementType::Left || std::get<0>(trackedMovements[i]) == MovementType::Right)
        while(j > 0 && (std::get<0>(trackedMovements[j]) == MovementType::Left || std::get<0>(trackedMovements[j]) == MovementType::Right))
          j--;
      else if(std::get<0>(trackedMovements[i]) == MovementType::Forward || std::get<0>(trackedMovements[i]) == MovementType::Backward)
        while(j > 0 && (std::get<0>(trackedMovements[j]) == MovementType::Forward || std::get<0>(trackedMovements[j]) == MovementType::Backward))
          j--;
      i = j + 1;
      double leftTarget = std::get<1>(trackedMovements[j]);
      double rightTarget = std::get<2>(trackedMovements[j]);
      LeftMotor.move_absolute(leftTarget, MOVEMENT_SPEED);
      RightMotor.move_absolute(rightTarget, MOVEMENT_SPEED);
      while(fabs(fabs(LeftMotor.get_position()) - fabs(leftTarget)) > MOVE_SENSITIVITY || fabs(fabs(RightMotor.get_position()) - fabs(rightTarget)) > MOVE_SENSITIVITY)
        pros::delay(50);
  }
}

void findPad() //Have a block, looking for the floor tile to deposit it at
{
  if(targetBlock != BlockType::Yellow)
    Lamp.move_velocity(LAMP_ON_SPEED);//spin(directionType::fwd, lampOnSpeed, velocityUnits::rpm);
  else
    Lamp.move(0);

  bool finished = false;
  std::vector<std::tuple<MovementType, double, double>> trackedMovements;
  MovementType lastMvmt = MovementType::None;
  trackedMovements.push_back(std::make_tuple(MovementType::None, LeftMotor.get_position(), RightMotor.get_position()));

  while(!finished)
  {
    //Brain.Screen.clearScreen();

    floorVisionObjectCount = takeSnapshot(FloorVision, blockTypeToFloorSig(targetBlock), floorVisionObjects);
    drawAllSnapshotObjects(floorVisionObjectCount, floorVisionObjects, blockTypeToColour(targetBlock));
    if(floorVisionObjectCount > 0)
    {
      //Turn right
      if(floorVisionObjects[0].left_coord + floorVisionObjects[0].width / 2.0 < VISION_CENTER - VISION_CENTER_SENSITIVITY)
      {
        lastMvmt = MovementType::Right;
        LeftMotor.move_velocity(-MOVEMENT_SPEED);
        RightMotor.move_velocity(MOVEMENT_SPEED);
      }
      //Turn left
      else if(floorVisionObjects[0].left_coord + floorVisionObjects[0].width / 2.0 > VISION_CENTER + VISION_CENTER_SENSITIVITY)
      {
        lastMvmt = MovementType::Left;
        LeftMotor.move_velocity(MOVEMENT_SPEED);
        RightMotor.move_velocity(-MOVEMENT_SPEED);
      }
      //Go straight
      else
      {
        //Not close enough - go forwards
        if(floorVisionObjects[0].top_coord + floorVisionObjects[0].height / 2.0 < DROPOFF_MIN)
        {
          lastMvmt = MovementType::Forward;
          LeftMotor.move_velocity(MOVEMENT_SPEED);
          RightMotor.move_velocity(MOVEMENT_SPEED);
        }
        //Too far - go backwards
        else if(floorVisionObjects[0].top_coord + floorVisionObjects[0].height / 2.0 > DROPOFF_MAX)
        {
          lastMvmt = MovementType::Backward;
          LeftMotor.move_velocity(-MOVEMENT_SPEED);
          RightMotor.move_velocity(-MOVEMENT_SPEED);
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
        trackedMovements.push_back(std::make_tuple(mvmtType, LeftMotor.get_position(), RightMotor.get_position()));
        if(lastMvmt != MovementType::None)
          trackedMovements.push_back(std::make_tuple(lastMvmt, 0.0, 0.0));
      }
    }
    else
    {
      resetMotors();
    }
    pros::delay(50);
  }

  Lamp.move(0);

  for(int i = trackedMovements.size() - 1; i >= 1; i--)
  {
    if(std::get<0>(trackedMovements[i]) == MovementType::None)
      continue;
    int j = i - 1;
    if(std::get<0>(trackedMovements[i]) == MovementType::Left || std::get<0>(trackedMovements[i]) == MovementType::Right)
      while(j > 0 && (std::get<0>(trackedMovements[j]) == MovementType::Left || std::get<0>(trackedMovements[j]) == MovementType::Right))
        j--;
    else if(std::get<0>(trackedMovements[i]) == MovementType::Forward || std::get<0>(trackedMovements[i]) == MovementType::Backward)
      while(j > 0 && (std::get<0>(trackedMovements[j]) == MovementType::Forward || std::get<0>(trackedMovements[j]) == MovementType::Backward))
        j--;
    i = j + 1;
    double leftTarget = std::get<1>(trackedMovements[j]);
    double rightTarget = std::get<2>(trackedMovements[j]);
    LeftMotor.move_absolute(leftTarget, MOVEMENT_SPEED);
    RightMotor.move_absolute(rightTarget, MOVEMENT_SPEED);
    while(fabs(fabs(LeftMotor.get_position()) - fabs(leftTarget)) > MOVE_SENSITIVITY || fabs(fabs(RightMotor.get_position()) - fabs(rightTarget)) > MOVE_SENSITIVITY)
      pros::delay(50);
  }
}
