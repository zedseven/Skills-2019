#include "config.h"
#include "globals.h"
#include "main.h"
#include "pros/vision.hpp"
#include "pros/apix.h"

using namespace std;
using namespace okapi;

void initSignatures()
{
  BlockVision.set_wifi_mode(VISION_BLOCK_WIFI_ON);
  FloorVision.set_wifi_mode(VISION_FLOOR_WIFI_ON);
  pros::vision_signature_s_t blueBlockSig = SIG_INIT_BLUE_BLOCK;
  pros::vision_signature_s_t yellowBlockSig = SIG_INIT_YELLOW_BLOCK;
  pros::vision_signature_s_t redBlockSig = SIG_INIT_RED_BLOCK;
  pros::vision_signature_s_t blueFloorSig = SIG_INIT_BLUE_FLOOR;
  pros::vision_signature_s_t yellowFloorSig = SIG_INIT_YELLOW_FLOOR;
  pros::vision_signature_s_t redFloorSig = SIG_INIT_RED_FLOOR;
  BlockVision.set_signature(SIG_BLUE_BLOCK, &blueBlockSig);
  BlockVision.set_signature(SIG_YELLOW_BLOCK, &yellowBlockSig);
  BlockVision.set_signature(SIG_RED_BLOCK, &redBlockSig);
  FloorVision.set_signature(SIG_BLUE_FLOOR, &blueFloorSig);
  FloorVision.set_signature(SIG_YELLOW_FLOOR, &yellowFloorSig);
  FloorVision.set_signature(SIG_RED_FLOOR, &redFloorSig);
  printf("Initialized the vision sensors. Exposure values: BlockVision: %d | FloorVision: %d\n", BlockVision.get_exposure(), FloorVision.get_exposure());
}

double getDistFromObjWidth(int objWidth)
{
    return WIDTH_TO_DIST / (((double) objWidth) / VISION_WIDTH);
}

int blockTypeToBlockSig(BlockType blockType)
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
            return SIG_NULL; //Cannot convert BlockType::None to a signature
    }
}
int blockTypeToFloorSig(BlockType blockType)
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
            return SIG_NULL; //Cannot convert BlockType::None to a signature
    }
}
lv_color_t blockTypeToColour(BlockType blockType)
{
    switch(blockType)
    {
        case BlockType::Blue:
            return LV_COLOR_BLUE;
        case BlockType::Yellow:
            return LV_COLOR_YELLOW;
        case BlockType::Red:
            return LV_COLOR_RED;
        default:
            return LV_COLOR_PURPLE; //Cannot convert BlockType::None to a colour
    }
}

//Display/Debug Methods
void/*lv_obj_t **/drawRectangle(int x, int y, int width, int height, lv_color_t colour)
{
  lv_obj_t *obj1 = lv_obj_create(lv_scr_act(), NULL);

  lv_style_t *style1 = (lv_style_t *) malloc(sizeof(lv_style_t));
  lv_style_copy(style1, &lv_style_plain_color);
  style1->body.empty = 0;
  style1->body.main_color = colour;
  style1->body.grad_color = colour;
  //style1->body.border.color = colour;
  style1->body.border.width = 0;
  style1->body.border.part = LV_BORDER_FULL;

  lv_obj_set_style(obj1, style1);
  lv_obj_set_pos(obj1, x, y);
  lv_obj_set_size(obj1, width, height);

  //lv_obj_del(obj1);
  //return obj1;
  screenObjects.push_back(obj1);
}

void clearScreenObjects()
{
  for(int i = 0; i < screenObjects.size(); i++)
    lv_obj_del(screenObjects[i]);
  screenObjects.clear();
}

void drawAllSnapshotObjects(int objectCount, pros::vision_object_s_t *objects, lv_color_t colour)
{
  printf("objectCount: %d\n", objectCount);
  BlockVision.print_signature(BlockVision.get_signature(SIG_YELLOW_BLOCK));
  for(int i = 0; i < objectCount; i++)
    drawRectangle(DRAWN_VISION_BORDER + round(objects[i].left_coord * VISION_TO_DRAWABLE), round(objects[i].top_coord * VISION_TO_DRAWABLE), round(objects[i].width * VISION_TO_DRAWABLE), round(objects[i].height * VISION_TO_DRAWABLE), colour);
}

int calibrateVisionExposure(pros::Vision vision, int signature, pros::vision_object_s_t *objects)
{
  //DON'T NEED TO DO INTERMEDIATE PRECISE RANGE
  int startExp = -1;
  int endExp = -1;
  for(int i = VISION_EXPOSURE_MIN / VISION_EXPOSURE_STEP_PRELIM; i <= VISION_EXPOSURE_MAX / VISION_EXPOSURE_STEP_PRELIM; i++)
  {
    printf("Trying exposure value %d\n", i * VISION_EXPOSURE_STEP_PRELIM);
    vision.set_exposure(i * VISION_EXPOSURE_STEP_PRELIM);
    int numObjects = vision.read_by_sig(0, signature, VISION_NUM_OBJECTS, objects);
    printf("numObjects: %d\n", numObjects);
    //Find the start and end for preliminary exposure values that work
    if(startExp < 0 && numObjects > 0 && numObjects != PROS_ERR)
      startExp = i;
    else if(startExp >= 0 && numObjects > 0 && numObjects != PROS_ERR)
      endExp = i;
    pros::delay(25);
  }
  printf("startExp: %d, endExp: %d\n", startExp, endExp);
  if(startExp < 0 || endExp < 0)
  {
    printf("Unable to calibrate vision sensor.");
    return -1;
  }
  int startExpPrec = -1;
  int endExpPrec = -1;
  int bestArea = -1;
  int bestExp = -1;
  for(int i = max((startExp - 1) * VISION_EXPOSURE_STEP_PRELIM, VISION_EXPOSURE_MIN); i <= min((endExp + 1) * VISION_EXPOSURE_STEP_PRELIM, VISION_EXPOSURE_MAX); i++)
  {
    printf("Trying precise exposure value %d\n", i);
    vision.set_exposure(i);
    int numObjects = vision.read_by_sig(0, signature, VISION_NUM_OBJECTS, objects);
    printf("numObjects: %d\n", numObjects);
    //Find the start and end for precise exposure values that work
    if(numObjects > 0 && numObjects != PROS_ERR && objects[0].width * objects[0].height >= bestArea)
    {
      bestArea = objects[0].width * objects[0].height;
      bestExp = i;
    }
    pros::delay(25);
  }
  //int bestExp = round((endExpPrec - startExpPrec) / 2.0);
  printf("Best exposure value: %d\n", bestExp);
  vision.set_exposure(bestExp);
  return bestExp;
}

void setBlockVisionExposure(BlockType blockType)
{
  switch(blockType)
  {
      case BlockType::Blue:
          BlockVision.set_exposure(VISION_BLUE_BLOCK_EXPOSURE);
          break;
      case BlockType::Yellow:
          BlockVision.set_exposure(VISION_YELLOW_BLOCK_EXPOSURE);
          break;
      case BlockType::Red:
          BlockVision.set_exposure(VISION_RED_BLOCK_EXPOSURE);
          break;
      default:
          return; //BlockType::None has no exposure setting
  }
}

void setFloorVisionExposure(BlockType blockType)
{
  switch(blockType)
  {
      case BlockType::Blue:
          FloorVision.set_exposure(VISION_BLUE_FLOOR_EXPOSURE);
          break;
      case BlockType::Yellow:
          FloorVision.set_exposure(VISION_YELLOW_FLOOR_EXPOSURE);
          break;
      case BlockType::Red:
          FloorVision.set_exposure(VISION_RED_FLOOR_EXPOSURE);
          break;
      default:
          return; //BlockType::None has no exposure setting
  }
}

pros::vision_object_s_t getLargestObject(int objectCount, pros::vision_object_s_t *objects)
{
  pros::vision_object_s_t largestObj;
  int largestArea = -1;
  for(int i = 0; i < objectCount; i++)
  {
    if(objects[i].width * objects[i].height > largestArea)
    {
      largestArea = objects[i].width * objects[i].height;
      largestObj = objects[i];
    }
  }
  return largestObj;
}

int takeSnapshot(pros::Vision vision, int signature, pros::vision_object_s_t *objects)
{
  int numObjects = -1;
  int passNum = 0;
  while((numObjects <= 0 || numObjects == PROS_ERR) && passNum < 15)
  {
    numObjects = vision.read_by_sig(0, signature, VISION_NUM_OBJECTS, objects);
    passNum += 1;
    pros::delay(5);
  }
  if(numObjects == PROS_ERR)
    return -2;
  lastVisionTargetCenterX = objects[0].left_coord + objects[0].width / 2;
  return numObjects;
}
