#include "config.h"
#include "globals.h"
#include "movement.h"
#include "vision.h"

void findBlock() //Looking for a block
{
  if(targetBlock == BlockType::Blue)
    Lamp.move_velocity(LAMP_ON_SPEED);
  else
    Lamp.move(0);

  bool finished = false;
  std::vector<std::tuple<MovementType, double, double>> trackedMovements;
  MovementType lastMvmt = MovementType::None;
  trackedMovements.push_back(std::make_tuple(MovementType::None, LeftMotor.get_position(), RightMotor.get_position()));

  //Calibrate the vision sensor for the given block colour
  //calibrateVisionExposure(BlockVision, blockTypeToBlockSig(targetBlock), blockVisionObjects);
  setBlockVisionExposure(targetBlock);

  int numSights = 0; //Number of times the block has been seen in a row as being ready to pick up
  while(!finished)
  {
    //Brain.Screen.clearScreen();

    blockVisionObjectCount = takeSnapshot(BlockVision, blockTypeToBlockSig(targetBlock), blockVisionObjects);
    clearScreenObjects();
    drawAllSnapshotObjects(blockVisionObjectCount, blockVisionObjects, blockTypeToColour(targetBlock));
    if(blockVisionObjectCount > 0)
    {
      pros::vision_object_s_t largestObj = getLargestObject(blockVisionObjectCount, blockVisionObjects);
      //For some reason, x_middle_coord and y_middle_coord are not populated
      printf("left_coord: %d\n", largestObj.left_coord);
      printf("left_coord + width / 2.0 < VISION_CENTER - VISION_CENTER_SENSITIVITY | > VISION_CENTER + VISION_CENTER_SENSITIVITY: %d + %d / 2.0 < %d - %d | > %d + %d: %f < %d | %f > %d\n", blockVisionObjects[0].left_coord, blockVisionObjects[0].width, VISION_CENTER, VISION_CENTER_SENSITIVITY, VISION_CENTER, VISION_CENTER_SENSITIVITY, blockVisionObjects[0].left_coord + blockVisionObjects[0].width / 2.0, VISION_CENTER - VISION_CENTER_SENSITIVITY, blockVisionObjects[0].left_coord + largestObj.width / 2.0, VISION_CENTER + VISION_CENTER_SENSITIVITY);
      printf("turn: %s\n", (largestObj.left_coord + largestObj.width / 2.0 < VISION_CENTER - VISION_CENTER_SENSITIVITY ? "RIGHT" : (largestObj.left_coord + largestObj.width / 2.0 > VISION_CENTER + VISION_CENTER_SENSITIVITY ? "LEFT" : "FWD/BWD")));
      printf("top_coord: %d\n", largestObj.top_coord);
      printf("x_middle_coord: %d\n", largestObj.x_middle_coord);
      printf("y_middle_coord: %d\n", largestObj.y_middle_coord);
      printf("width: %d\n", largestObj.width);
      printf("height: %d\n", largestObj.height);
      //Turn left
      if(largestObj.left_coord + largestObj.width / 2.0 < VISION_CENTER - VISION_CENTER_SENSITIVITY)
      {
        numSights = 0;
        lastMvmt = MovementType::Left;
        LeftMotor.move_velocity(-CAMERA_MOVEMENT_SPEED);
        RightMotor.move_velocity(CAMERA_MOVEMENT_SPEED);
      }
      //Turn right
      else if(largestObj.left_coord + largestObj.width / 2.0 > VISION_CENTER + VISION_CENTER_SENSITIVITY)
      {
        numSights = 0;
        lastMvmt = MovementType::Right;
        LeftMotor.move_velocity(CAMERA_MOVEMENT_SPEED);
        RightMotor.move_velocity(-CAMERA_MOVEMENT_SPEED);
      }
      //Go straight
      else
      {
        float blockDist = getDistFromObjWidth(largestObj.width);
        //Too close - go backwards
        if(blockDist < PICKUP_DIST - PICKUP_SENSITIVITY)
        {
          numSights = 0;
          lastMvmt = MovementType::Backward;
          LeftMotor.move_velocity(-CAMERA_MOVEMENT_SPEED);
          RightMotor.move_velocity(-CAMERA_MOVEMENT_SPEED);
        }
        //Too far - go forwards
        else if(blockDist > PICKUP_DIST + PICKUP_SENSITIVITY)
        {
          numSights = 0;
          lastMvmt = MovementType::Forward;
          LeftMotor.move_velocity(CAMERA_MOVEMENT_SPEED);
          RightMotor.move_velocity(CAMERA_MOVEMENT_SPEED);
        }
        //Optimal distance to pick up with claw
        else
        {
          resetMotors();
          if(numSights >= VISION_NUM_SIGHTS_PICKUP)
          {
            lastMvmt = MovementType::None;
            pickupBlock();
            finished = true;
          }
          else
          {
            numSights++;
          }
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
      LeftMotor.move_absolute(leftTarget, CAMERA_MOVEMENT_SPEED);
      RightMotor.move_absolute(rightTarget, CAMERA_MOVEMENT_SPEED);
      while(fabs(fabs(LeftMotor.get_position()) - fabs(leftTarget)) > MOVE_SENSITIVITY || fabs(fabs(RightMotor.get_position()) - fabs(rightTarget)) > MOVE_SENSITIVITY)
        pros::delay(50);
  }
}

void findPad() //Have a block, looking for the floor tile to deposit it at
{
  int *relevantDeliverCount;
  switch(targetBlock)
  {
    case BlockType::Blue:
      relevantDeliverCount = &deliveredBlues;
      break;
    case BlockType::Red:
      relevantDeliverCount = &deliveredReds;
      break;
    case BlockType::Yellow:
      relevantDeliverCount = &deliveredYellows;
      break;
    default:
      return;
  }

  /*if(targetBlock == BlockType::Blue)
    Lamp.move_velocity(LAMP_ON_SPEED);//spin(directionType::fwd, lampOnSpeed, velocityUnits::rpm);
  else
    Lamp.move(0);*/

  bool finished = false;
  std::vector<std::tuple<MovementType, double, double>> trackedMovements;
  MovementType lastMvmt = MovementType::None;
  trackedMovements.push_back(std::make_tuple(MovementType::None, LeftMotor.get_position(), RightMotor.get_position()));

  setFloorVisionExposure(targetBlock);

  while(!finished)
  {
    //Brain.Screen.clearScreen();

    floorVisionObjectCount = takeSnapshot(FloorVision, blockTypeToFloorSig(targetBlock), floorVisionObjects);
    clearScreenObjects();
    drawAllSnapshotObjects(floorVisionObjectCount, floorVisionObjects, blockTypeToColour(targetBlock));
    if(floorVisionObjectCount > 0)
    {
      pros::vision_object_s_t largestObj = getLargestObject(floorVisionObjectCount, floorVisionObjects);
      //Turn left
      if(largestObj.left_coord + largestObj.width / 2.0 < VISION_CENTER - VISION_CENTER_SENSITIVITY)
      {
        lastMvmt = MovementType::Left;
        LeftMotor.move_velocity(-CAMERA_MOVEMENT_SPEED);
        RightMotor.move_velocity(CAMERA_MOVEMENT_SPEED);
      }
      //Turn right
      else if(largestObj.left_coord + largestObj.width / 2.0 > VISION_CENTER + VISION_CENTER_SENSITIVITY)
      {
        lastMvmt = MovementType::Right;
        LeftMotor.move_velocity(CAMERA_MOVEMENT_SPEED);
        RightMotor.move_velocity(-CAMERA_MOVEMENT_SPEED);
      }
      //Go straight
      else
      {
        //Not close enough - go forwards
        if(largestObj.top_coord + largestObj.height / 2.0 < DROPOFF_MIN)
        {
          lastMvmt = MovementType::Forward;
          LeftMotor.move_velocity(CAMERA_MOVEMENT_SPEED);
          RightMotor.move_velocity(CAMERA_MOVEMENT_SPEED);
        }
        //Too far - go backwards
        else if(largestObj.top_coord + largestObj.height / 2.0 > DROPOFF_MAX)
        {
          lastMvmt = MovementType::Backward;
          LeftMotor.move_velocity(-CAMERA_MOVEMENT_SPEED);
          RightMotor.move_velocity(-CAMERA_MOVEMENT_SPEED);
        }
        else
        {
          lastMvmt = MovementType::None;
          resetMotors();
          if(*relevantDeliverCount > 0) //Second block to this pad
          {
            move(-BLOCK_DROPOFF_INITIAL_BACKWARD);
            lowerBlock();
            move(BLOCK_DROPOFF_INITIAL_BACKWARD + BLOCK_DROPOFF_FORWARD_BONUS);
            openClawRelaxed();
            raiseArms();
            move(-BLOCK_DROPOFF_FORWARD_BONUS);
          }
          else
          {
            move(BLOCK_DROPOFF_FORWARD_BONUS); //THIS NEEDS TO BE CHANGED FOR IF HAVING 0 | 1 OF BLOCK ALREADY
            dropoffBlock();
            move(-BLOCK_DROPOFF_FORWARD_BONUS);
          }
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
    LeftMotor.move_absolute(leftTarget, CAMERA_MOVEMENT_SPEED);
    RightMotor.move_absolute(rightTarget, CAMERA_MOVEMENT_SPEED);
    while(fabs(fabs(LeftMotor.get_position()) - fabs(leftTarget)) > MOVE_SENSITIVITY || fabs(fabs(RightMotor.get_position()) - fabs(rightTarget)) > MOVE_SENSITIVITY)
      pros::delay(50);
  }

  *relevantDeliverCount += 1;
}
