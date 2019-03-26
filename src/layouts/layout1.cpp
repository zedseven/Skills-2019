#include "../config.h"
#include "../globals.h"
#include "../movement.h"
#include "../operations.h"

void layout1()
{
  //Yellow Block A
  targetBlock = BlockType::Yellow;
  move(30);
  rotate(45);
  moveUntilDist(122, 30);
  rotate(90);
  move(20);
  findBlock();
  move(-20);
  rotate(-90);
  moveUntilDist(55, 30);
  rotate(45);
  findPad();
  rotate(-135);
  //Blue Block A
  targetBlock = BlockType::Blue;
  moveUntilDist(122, 30);
  rotate(90);
  move(20);
  findBlock();
  move(-20);
  rotate(-90);
  moveUntilDist(55, 30);
  rotate(45);
  findPad();
  rotate(-135);
  //Red Block A
  targetBlock = BlockType::Red;
  moveUntilDist(122, 30);
  rotate(90);
  move(20);
  findBlock();
  move(-20);
  rotate(-90);
  moveUntilDist(55, 30);
  rotate(45);
  findPad();
  rotate(-135);
  //Blue Block B
  targetBlock = BlockType::Blue;
  moveUntilDist(122, 30);
  rotate(-90);
  move(30);
  findBlock();
  rotate(180);
  realign();
  rotate(90);
  //moveUntilDist(55, 30);
  move(100);
  realign();
  move(-20);
  rotate(90);
  move(80);
  rotate(-90);
  move(20);
  realign();
  move(-35);
  rotate(90);
  move(30);
  rotate(-45);
  findPad();
  rotate(45);
  //Yellow Block B
  targetBlock = BlockType::Yellow;
  move(-20);
  rotate(-90);
  move(20);
  realign();
  move(-20);
  rotate(-90);
  move(11);
  rotate(-60);
  move(20);
  findBlock();
  move(-20);
  rotate(150);
  move(20);
  realign();
  move(-20);
  rotate(90);
  move(20);
  rotate(90);
  move(90);
  rotate(-90);
  move(20);
  realign();
  move(-20);
  rotate(90);
  move(20);
  rotate(-45);
  findPad();
  rotate(135);
  //Red Block B
  targetBlock = BlockType::Red;
  rotate(60);
  move(20);
  findBlock();
  move(-20);
  rotate(-60);
  moveUntilDist(55, 30);
  rotate(90);
  moveUntilDist(55, 30);
  rotate(-45);
  findPad();
}
