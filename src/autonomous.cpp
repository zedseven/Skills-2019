#include <cmath>
#include <vector>
#include <tuple>
#include "main.h"
#include "config.h"
#include "globals.h"
#include "movement.h"

void autonomous()
{
  LeftMotor.move_relative(1000, 100);
  RightMotor.move_relative(1000, 100);
}
