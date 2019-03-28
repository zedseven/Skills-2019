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
  realign();
  move(20);
  realign();
  rotate(-105);
  move(10);
  findBlock();
  move(-10);
  rotate(-75);
  moveUntilDist(60, 50);
  rotate(90);
  moveUntilDist(60, 50);
  rotate(-45);
  findPad();
  //Red Block B
  targetBlock = BlockType::Red;
  rotate(135);
  realign();
  move(37);
  realign();
  rotate(75);
  move(10);
  findBlock();
  move(-10);
  rotate(-75);
  realign();
  move(-37);
  realign();
  rotate(-135);
  findPad();
  //Yellow Block A
  targetBlock = BlockType::Yellow;
  rotate(45);
  move(-40);
  rotate(180);
  moveUntilDist(122, 50);
  rotate(-90);
  move(10);
  findBlock();
  //move(-10);
  rotate(-90);
  moveUntilDist(60, 50);
  rotate(90);
  moveUntilDist(122, 50);
  rotate(-90);
  findPad();
  rotate(90);
  //Yellow Block B
  targetBlock = BlockType::Yellow;
  moveUntilDist(60, 50);
  rotate(90);
  moveUntilDist(122, 50);
  rotate(90);
  move(10);
  findBlock();
  move(-10);
  rotate(90);
  moveUntilDist(60, 50);
  rotate(-90);
  moveUntilDist(122, 50);
  rotate(90);
  findPad();
  rotate(90);
  //Blue Block A
  targetBlock = BlockType::Blue;
  move(35);
  rotate(105);
  move(10);
  findBlock();
  move(-10);
  rotate(-105);
  moveUntilDist(60, 50);
  rotate(-45);
  findPad();
  rotate(45);
}
