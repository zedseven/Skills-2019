#include "../config.h"
#include "../globals.h"
#include "../movement.h"
#include "../operations.h"

void layout3()
{
  //Red Block A
  targetBlock = BlockType::Red;
  move(50);
  rotate(90);
  move(25);
  realign();
  rotate(-105);
  move(20);
  findBlock();
  move(-20);
  rotate(180);
  moveUntilDist(60, 50);
  rotate(90);
  moveUntilDist(60, 50);
  rotate(-45);
  findPad();
  //Red Block B
  targetBlock = BlockType::Red;
  rotate(135);
  realign();
  move(20);
  realign();
  rotate(75);
  move(20);
  findBlock();
  move(-20);
  rotate(-75);
  realign();
  move(-20);
  realign();
  rotate(-135);
  findPad();
}
