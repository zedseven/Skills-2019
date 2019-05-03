#include "../config.h"
#include "../globals.h"
#include "../movement.h"
#include "../operations.h"

void layout3()
{
  //Red Block A
  targetBlock = BlockType::Red;
  move(52);
  rotate(90);
  //realign();
  //move(24);
  //realign();
  moveUntilDist(90, 50);
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
  rotate(134);
  //realign();
  move(33);
  realign();
  //moveUntilDist(145, 50);
  rotate(90);
  move(10);
  //rotate(-90);
  //rotate(75);
  rotate(-15);
  move(10);
  findBlock();
  move(-10);
  rotate(-75);
  realign();
  move(-29);
  realign();
  rotate(-135);
  findPad();
  //Yellow Block A
  targetBlock = BlockType::Yellow;
  move(-5);
  rotate(45);
  move(-40);
  rotate(180);
  moveUntilDist(119, 50);
  rotate(90);
  moveUntilDist(65, 50);
  rotate(-180);
  //move(10);
  findBlock();
  //move(-10);
  rotate(-90);
  //moveUntilDist(60, 50);
  move(55);
  rotate(90);
  move(70);
  moveUntilDist(119, 50);
  rotate(-90);
  moveUntilDist(60, 50);
  findPad();
  rotate(90);
  //Yellow Block B
  targetBlock = BlockType::Yellow;
  moveUntilDist(60, 50);
  rotate(90);
  moveUntilDist(119, 50);
  rotate(90);
  move(10);
  findBlock();
  move(-10);
  rotate(90);
  moveUntilDist(60, 50);
  rotate(90);
  moveUntilDist(119, 50);
  rotate(-90);
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
  moveUntilDist(70, 50);
  rotate(-45);
  findPad();
  rotate(135);
  //Blue Block B
  targetBlock = BlockType::Blue;
  moveUntilDist(60, 50);
  rotate(90);
  move(70);
  moveUntilDist(86, 50);
  rotate(105);
  move(10);
  findBlock();
  move(-10);
  rotate(75);
  moveUntilDist(60, 50);
  rotate(90);
  moveUntilDist(170, 50);
  rotate(-135);
  findPad();
}
