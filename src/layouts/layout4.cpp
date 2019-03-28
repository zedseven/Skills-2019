#include "../config.h"
#include "../globals.h"
#include "../movement.h"
#include "../operations.h"

void layout4()
{
  //Blue Block A
  targetBlock = BlockType::Blue;
  moveUntilDist(185, 50);
  rotate(-90);
  realign();
  move(25);
  realign();
  rotate(-90);
  move(10);
  findBlock();
  move(-10);
  rotate(-90);
  moveUntilDist(60, 50);
  rotate(-90);
  moveUntilDist(60, 50);
  rotate(45);
  move(15);
  findPad();
  move(-15);
  rotate(-135);
  //Blue Block B
  targetBlock = BlockType::Blue;
  moveUntilDist(121, 50);
  rotate(90);
  moveUntilDist(185, 50);
  rotate(90);
  realign();
  move(20);
  realign();
  rotate(90);
  move(20);
  findBlock();
  move(-20);
  rotate(180);
  moveUntilDist(80, 50);
  rotate(90);
  moveUntilDist(60, 50);
  rotate(-90);
  move(20);
  rotate(45);
  move(20);
  findPad();
  move(-20);
  rotate(-45);
  //Yellow Block A
  targetBlock = BlockType::Yellow;
  move(-60);
  rotate(90);
  move(20);
  findBlock();
  move(-20);
  rotate(-90);
  move(45);
  rotate(-90);
  moveUntilDist(122, 50);
  rotate(90);
  moveUntilDist(50, 50);
  move(10);
  findPad();
  move(-10);
  //Yellow Block B
  targetBlock = BlockType::Yellow;
  move(-40);
  rotate(-90);
  move(50);
  rotate(90);
  moveUntilDist(116, 50);
  rotate(-90);
  move(30);
  findBlock();
  move(-30);
  rotate(90);
  move(45);
  rotate(-90);
  moveUntilDist(122, 50);
  rotate(90);
  move(30);
  findPad();
  move(-30);
  //Red Block A
  targetBlock = BlockType::Red;
  rotate(180);
  move(10);
  //moveUntilDist(40, 50);
  rotate(90);
  moveUntilDist(60, 50);
  rotate(-90);
  move(70);
  rotate(90);
  moveUntilDist(40, 50);
  rotate(-90);
  move(30);
  findBlock();
  move(-30);
  rotate(90);
  moveUntilDist(60, 50);
  rotate(90);
  moveUntilDist(60, 50);
  rotate(-45);
  findPad();
  rotate(45);
  //Red Block B
  targetBlock = BlockType::Red;
  move(-20);
  rotate(90);
  moveUntilDist(60, 50);
  rotate(90);
  move(70);
  rotate(-90);
  moveUntilDist(40, 50);
  rotate(90);
  move(30);
  findBlock();
  move(-30);
  rotate(-90);
  moveUntilDist(60, 50);
  rotate(90);
  move(-70);
  rotate(90);
  moveUntilDist(60, 50);
  rotate(45);
  findPad();
}
