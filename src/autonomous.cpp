#include <cmath>
#include <vector>
#include <tuple>
#include "main.h"
#include "config.h"
#include "globals.h"
#include "movement.h"

void autonomous()
{
  move(30);
  rotate(90);
  move(60);
}
