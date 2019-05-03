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
  moveUntilDist(118, 30);
  rotate(90);
  move(30);
  findBlock();
  move(-30);
  rotate(-90);
  moveUntilDist(55, 30);
  rotate(45);
  findPad();
  rotate(-135);
  //Blue Block A
  targetBlock = BlockType::Blue;
  moveUntilDist(118, 30);
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
  move(50);
  moveUntilDist(118, 30);
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
  moveUntilDist(118, 30);
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
  move(35);
  realign();
  move(-50);
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
  move(15);
  rotate(-45);
  move(20);
  findPad();
  move(-20);
  rotate(135);
  //Red Block B
  targetBlock = BlockType::Red;
  move(15);
  rotate(60);
  move(20);
  findBlock();
  move(-20);
  rotate(-60);
  moveUntilDist(55, 30);
  rotate(90);
  move(90);
  moveUntilDist(55, 30);
  rotate(-45);
  findPad();
}
