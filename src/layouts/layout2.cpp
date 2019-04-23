#include "../config.h"
#include "../globals.h"
#include "../movement.h"
#include "../operations.h"

void layout2()
{
  //Init
  move(50);
  rotate(-45);
  realign();
  //Red Block A
  targetBlock = BlockType::Red;
  moveUntilDist(150, 80);//145, 80);
  rotate(95);
  findBlock();
  rotate(-95);
  moveUntilDist(56, 30);
  rotate(-90);
  realign();
  move(DROPOFF_MOVE_OFFSET);
  rotate(45);
  findPad();
  rotate(-45);
  move(-DROPOFF_MOVE_OFFSET);
  rotate(-90);
  //Blue Block A
  targetBlock = BlockType::Blue;
  moveUntilDist(114, 30);
  rotate(-90);
  findBlock(false);
  rotate(90);
  moveUntilDist(184, 30);
  rotate(-90);
  moveUntilDist(55, 30);//55
  rotate(-45);
  findPad();
  rotate(45);
  move(-DROPOFF_MOVE_OFFSET);
  //Yellow Block A
  targetBlock = BlockType::Yellow;
  rotate(90);
  moveUntilDist(85, 30);
  rotate(90);
  findBlock();
  rotate(-90);
  moveUntilDist(56, 30);
  rotate(-90);
  realign();
  move(DROPOFF_MOVE_OFFSET);
  rotate(45);
  findPad();
  rotate(-45);
  move(-DROPOFF_MOVE_OFFSET);
  //move(DROPOFF_MOVE_OFFSET);
  //Blue Block B
  targetBlock = BlockType::Blue;
  rotate(-90);
  move(40);
  rotate(90);
  realign();
  rotate(-90);
  //moveUntilDist(88, 30);
  move(55);
  rotate(90);
  realign();
  rotate(-180);
  findBlock();
  rotate(90);
  //realign();
  move(31.5);
  rotate(90);
  move(DROPOFF_MOVE_OFFSET);
  rotate(-45);
  findPad();
  rotate(45);
  move(-20);
  rotate(-90);
  //Red Block B
  targetBlock = BlockType::Red;
  move(40);
  realign();
  move(-40);
  //moveUntilDist(123, 30);
  //move(-55);
  move(-60);
  //realign();
  rotate(-90);
  findBlock();
  rotate(90);
  //moveUntilDist(55, 30);
  move(88.5/*58.5*/);
  realign();
  rotate(-90);
  //moveUntilDist(55, 30);
  move(95 + DROPOFF_MOVE_OFFSET);
  rotate(90);
  realign();
  move(-20);
  rotate(-45);
  findPad();
  rotate(45);
  move(-DROPOFF_MOVE_OFFSET);
  //Yellow Block B
  targetBlock = BlockType::Yellow;
  rotate(180);
  //move(20);
  //moveUntilDist(184, 30);
  moveUntilDist(150, 30);
  rotate(-90);
  move(25);
  findBlock();
  rotate(90);
  moveUntilDist(55, 30);
  rotate(90);
  moveUntilDist(174, 30);
  rotate(-180);
  move(DROPOFF_MOVE_OFFSET);
  rotate(45);
  findPad();
}
