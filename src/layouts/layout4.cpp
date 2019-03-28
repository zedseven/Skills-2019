#include "../config.h"
#include "../globals.h"
#include "../movement.h"
#include "../operations.h"

void layout4()
{
  //Blue Block A
  targetBlock = BlockType::Blue;
  moveUntilDist(40, 50);
  rotate(90);
  move(20);
  rotate(-90);
  move(10);
  findBlock();
  move(-10);
  rotate(-90);
  moveUntilDist(60, 50);
  rotate(-90);
  moveUntilDist(60, 50);
  rotate(45);
  findPad();
  rotate(45);
  //Blue Block B
  targetBlock = BlockType::Blue;
  moveUntilDist(190, 50);
  rotate(-90);
  moveUntilDist(150, 50);
  rotate(90);
  findBlock();
  rotate(-90);
  moveUntilDist(60, 50);
  rotate(-90);
  move(110);
  rotate(45);
  findPad();
  rotate(-45);
  //Yellow Block A
  targetBlock = BlockType::Yellow;
  move(-50);
  rotate(90);
  findBlock();
  move(-60);
  rotate(-90);
  moveUntilDist(90, 50);
  rotate(90);
  moveUntilDist(122, 50);
  rotate(-90);
  move(30);
  findPad();
  move(-30);
  //Yellow Block B
  targetBlock = BlockType::Yellow;
  moveUntilDist(122, 50);
  rotate(90);
  move(60);
  findBlock();
  move(-60);
  rotate(-90);
  moveUntilDist(190, 50);
  rotate(180);
  move(15);
  findPad();
  //Red Block A
  targetBlock = BlockType::Red;
  moveUntilDist(40, 50);
  rotate(-90);
  moveUntilDist(60, 50);
  rotate(90);
  findBlock();
  rotate(90);
  moveUntilDist(60, 50);
  rotate(90);
  moveUntilDist(60, 50);
  rotate(-45);
  findPad();
  rotate(45);
}
