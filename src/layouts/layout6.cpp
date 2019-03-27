#include "../config.h"
#include "../globals.h"
#include "../movement.h"
#include "../operations.h"

void layout6()
{
  //Blue Block A
  targetBlock = BlockType::Blue;
  //moveUntilDist(160, 30);
  move(20);
  realign();
  rotate(-90);
  moveUntilDist(75, 50);
  rotate(-90);
  move(10);
  findBlock();
  move(-10);
  rotate(90);
  move(20);
  realign();
  move(-20);
  rotate(-180);
  //moveUntilDist(70, 30);
  move(96);
  rotate(-90);
  move(30);
  moveUntilDist(50, 40);
  findPad();
  //Blue Block B
  targetBlock = BlockType::Blue;
  move(-30);
  rotate(90);
  move(25);
  findBlock();
  move(-25);
  rotate(-90);
  move(30);
  findPad();
  move(-10);
  rotate(90);
  move(25);
  realign();
  move(-25);
  rotate(-90);
  move(-20);
  //Red Block A
  targetBlock = BlockType::Red;
  move(-30);
  rotate(90);
  move(40);
  realign();
  move(-40);
  rotate(-90);
  move(-44);
  rotate(90);
  move(25);
  findBlock();
  move(-25);
  rotate(-90);
  move(44);
  rotate(90);
  move(20);
  realign();
  move(-20);
  rotate(-90);
  move(62);
  rotate(90);
  move(20);
  realign();
  move(-20);
  rotate(-180);
  move(90);
  realign();
  rotate(90);
  move(10);
  findPad();
  move(-40);
  //Red Block B
  targetBlock = BlockType::Red;
  rotate(-90);
  move(25);
  findBlock();
  move(-25);
  rotate(90);
  move(30);
  findPad();
  move(-60);
  rotate(-90);
  move(25);
  realign();
  move(-25);
  rotate(90);
  //Yellow Block A
  targetBlock = BlockType::Yellow;
  move(-44);
  /*rotate(-90);
  move(25);
  realign();
  move(-25);*/
  rotate(-90);
  move(25);
  findBlock();
  move(-25);
  rotate(90);
  move(44);
  rotate(-90);
  move(25);
  realign();
  move(-25);
  rotate(90);
  move(30);
  rotate(90);
  move(45);
  rotate(-90);
  move(40);
  findPad();
  move(-40);
  //Yellow Block B
  targetBlock = BlockType::Yellow;
  rotate(180);
  move(60);
  moveUntilDist(44, 50);
  rotate(-90);
  moveUntilDist(75, 50);
  rotate(90);
  findBlock();
  rotate(-90);
  moveUntilDist(125, 50);
  rotate(90);
  moveUntilDist(195, 50);
  rotate(180);
  findPad();
}
