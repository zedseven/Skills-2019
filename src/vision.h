#pragma once

#include "main.h"
#include "config.h"

void initSignatures();
double getDistFromObjWidth(int objWidth);
int blockTypeToBlockSig(BlockType blockType);
int blockTypeToFloorSig(BlockType blockType);
lv_color_t blockTypeToColour(BlockType blockType);
void drawRectangle(int x, int y, int width, int height, lv_color_t colour);
void clearScreenObjects();
void drawAllSnapshotObjects(int objectCount, pros::vision_object_s_t *objects, lv_color_t colour);
int calibrateVisionExposure(pros::Vision vision, int signature, pros::vision_object_s_t *objects);
void setBlockVisionExposure(BlockType blockType);
void setFloorVisionExposure(BlockType blockType);
void setBlockVisionWhiteBalance(BlockType blockType);
void setFloorVisionWhiteBalance(BlockType blockType);
pros::vision_object_s_t getLargestObject(int objectCount, pros::vision_object_s_t *objects);
int takeSnapshot(pros::Vision vision, int signature, pros::vision_object_s_t *objects);
