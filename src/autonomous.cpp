#include "main.h"
#include "config.h"
#include "globals.h"
#include "movement.h"
#include "vision.h"
#include "operations.h"
#include "layouts/layouts.h"

void autonomous()
{
  printf("Started.\n");
  initMotors();
  //openClaw();
  initSignatures();
  switch(LAYOUT_NUM)
  {
    case 1:
      layout1();
      break;
    case 2:
      layout2();
      break;
    case 3:
      layout3();
      break;
    case 4:
      layout4();
      break;
    case 5:
      layout5();
      break;
    case 6:
      layout6();
      break;
  }
}
