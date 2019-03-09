#include "config.h"
#include "globals.h"
#include "main.h"
#include "pros/vision.hpp"
#include "pros/apix.h"

void initSignatures()
{
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
void/*lv_obj_t **/drawRectangle(int x, int y, int width, int height, lv_color_t color)
{
  lv_obj_t *obj1 = lv_obj_create(lv_scr_act(), NULL);

  lv_style_t *style1 = (lv_style_t *) malloc(sizeof(lv_style_t));
  lv_style_copy(style1, &lv_style_plain_color);
  style1->body.empty = 1;
  style1->body.border.color = color;
  style1->body.border.width = 1;
  style1->body.border.part = LV_BORDER_FULL;

  lv_obj_set_style(obj1, style1);
  lv_obj_set_pos(obj1, x, y);
  lv_obj_set_size(obj1, width, height);

  lv_obj_del(obj1);
  //return obj1;
}

void drawAllSnapshotObjects(int objectCount, pros::vision_object_s_t *objects, lv_color_t colour)
{
  printf("objectCount: %d\n", objectCount);
  BlockVision.print_signature(BlockVision.get_signature(SIG_YELLOW_BLOCK));
  //for(int i = 0; i < objectCount; i++)
  //  drawRectangle(objects[i].left_coord, objects[i].top_coord, objects[i].width, objects[i].height, colour);
}

int takeSnapshot(pros::Vision vision, int signature, pros::vision_object_s_t *objects)
{
  return vision.read_by_sig(0, signature, VISION_NUM_OBJECTS, objects);
}
