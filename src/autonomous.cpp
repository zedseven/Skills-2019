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
  /*double lDist;
  double rDist;
  while(true)
  {
    lDist = pros::c::ultrasonicGet(SonarL);
    rDist = pros::c::ultrasonicGet(SonarR);
    printf("l: %f r: %f\n", lDist, rDist);
    pros::delay(50);
  }*/
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
