#include "../config.h"
#include "../globals.h"
#include "../movement.h"
#include "../operations.h"

void layout5()
{
  //Blue Block A
  targetBlock = BlockType::Blue;
  move(30);
  rotate(-90);
  moveUntilDist(50, 50);
  rotate(-45);
  move(20);
  findBlock();
  move(-20);
  rotate(-135);
  moveUntilDist(50, 50);
  rotate(90);
  realign();
  move(-33);
  realign();
  rotate(135);
  move(20);
  findPad();
  move(-20);
  rotate(-135);
  realign();
  //Blue Block B
  targetBlock = BlockType::Blue;
  //move(50);
  moveUntilDist(50, 50);
  //realign();
  rotate(-45);
  move(20);
  findBlock();
  move(-20);
  rotate(45);
  realign();
  move(-33);
  realign();
  rotate(135);
  move(20);
  findPad();
  move(-20);
  rotate(-135);
  realign();
  //Red Block A
  targetBlock = BlockType::Red;
  move(-27);
  realign();
  rotate(-90);
  move(20);
  findBlock();
  move(-20);
  rotate(90);
  realign();
  move(50);
  realign();
  rotate(90);
  move(70);
  moveUntilDist(50, 50);
  rotate(-90);
  move(-15);
  rotate(-135);
  move(25);
  findPad();
  move(-25);
  rotate(135);
  //Red Block B
  targetBlock = BlockType::Red;
  move(-29);
  realign();
  rotate(90);
  move(20);
  findBlock();
  move(-20);
  rotate(-90);
  realign();
  move(22);
  realign();
  rotate(-135);
  move(20);
  findPad();
  move(-20);
  rotate(-45);
  //Yellow Block A
  targetBlock = BlockType::Yellow;
  move(90);
  moveUntilDist(51, 50);
  rotate(-45);
  move(20);
  findBlock();
  move(-20);
  rotate(135);
  move(70);
  moveUntilDist(118, 50);
  rotate(90);
  move(-30);
  findPad();
  //Yellow Block B
  targetBlock = BlockType::Yellow;
  rotate(-90);
  move(50);
  rotate(-90);
  moveUntilDist(50, 50);
  rotate(90);
  moveUntilDist(50, 50);
  rotate(-45);
  move(20);
  findBlock();
  move(-20);
  rotate(45);
  moveUntilDist(60, 50);
  rotate(-90);
  moveUntilDist(30, 50);
  rotate(-90);
  move(62);
  rotate(-90);
  findPad(10);
}
