#include "main.h"
#include "config.h"
#include "globals.h"
#include "movement.h"
#include "vision.h"
#include "operations.h"

void autonomous()
{
  printf("Started.\n");
  initMotors();
  openClaw();
  initSignatures();
  move(60);
  move(-60);
  targetBlock = BlockType::Red;
  //findBlock();
}
