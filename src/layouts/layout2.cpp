#include "../config.h"
#include "../globals.h"
#include "../movement.h"
#include "../operations.h"

void layout2()
{
  //Red Block A
  targetBlock = BlockType::Red;
  moveUntilDist(155, 80);//145, 80);
  rotate(90);
  findBlock();
  rotate(-90);
  moveUntilDist(56, 30);
  rotate(-45);
  findPad();
  //Blue Block A
  targetBlock = BlockType::Blue;
  moveUntilDist(123, 30);
  rotate(90);
  findBlock();
  rotate(-90);
  moveUntilDist(55, 30);
  rotate(90);
  moveUntilDist(55, 30);
  rotate(-45);
  findPad();
  //Yellow Block A
  targetBlock = BlockType::Yellow;
  rotate(135);
  moveUntilDist(88, 30);
  rotate(90);
  findBlock();
  rotate(-90);
  moveUntilDist(56, 30);
  rotate(-45);
  findPad();
  //Blue Block B
  rotate(-135);
  moveUntilDist(88, 30);
  rotate(-90);
  findBlock();
  rotate(90);
  realign();
  move(31.5);
  rotate(45);
  findPad();
  //Red Block B
  targetBlock = BlockType::Red;
  rotate(-45);
  realign();
  //moveUntilDist(123, 30);
  move(-55);
  //realign();
  rotate(-90);
  findBlock();
  rotate(90);
  moveUntilDist(55, 30);
  rotate(-90);
  moveUntilDist(55, 30);
  rotate(45);
  findPad();
}
