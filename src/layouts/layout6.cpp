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
  rotate(-89);
  move(15);
  findBlock();
  move(-15);
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
  rotate(90);
  moveUntilDist(69, 50);
  rotate(-90);
  findPad();
  //Blue Block B
  targetBlock = BlockType::Blue;
  move(-27);
  rotate(90);
  move(45);
  findBlock();
  move(-35);
  rotate(-90);
  move(40);
  findPad();
  move(-15);
  rotate(90);
  //move(25);
  //realign();
  moveUntilDist(75, 50);
  //move(-25);
  rotate(-90);
  move(-25);
  //Red Block A
  targetBlock = BlockType::Red;
  move(-30);
  rotate(90);
  //move(40);
  //realign();
  moveUntilDist(75, 50);
  //move(-40);
  rotate(-90);
  move(-43);
  rotate(90);
  move(25);
  findBlock();
  move(-25);
  rotate(-90);
  move(44);
  rotate(90);
  //move(20);
  //realign();
  moveUntilDist(75, 50);
  //move(-20);
  rotate(-90);
  move(62);
  rotate(90);
  //move(20);
  //realign();
  moveUntilDist(75, 50);
  //move(-20);
  rotate(-180);
  move(90);
  //realign();
  moveUntilDist(70, 50);
  rotate(90);
  //move(10);
  findPad();
  move(-29);
  //Red Block B
  targetBlock = BlockType::Red;
  rotate(-90);
  move(35);
  findBlock();
  move(-31);
  rotate(90);
  move(20);
  findPad();
  move(-55);
  rotate(-90);
  //move(25);
  //realign();
  moveUntilDist(75, 50);
  //move(-25);
  rotate(90);
  //Yellow Block A
  targetBlock = BlockType::Yellow;
  move(-41);
  /*rotate(-90);
  move(25);
  realign();
  move(-25);*/
  rotate(-90);
  move(25);
  findBlock();
  move(-25);
  rotate(90);
  move(42);
  rotate(-90);
  //move(25);
  //realign();
  moveUntilDist(75, 50);
  //move(-25);
  rotate(180);
  move(60);
  moveUntilDist(119, 50);
  rotate(-90);
  move(50);
  findPad();
  move(-110);
  //Yellow Block B
  targetBlock = BlockType::Yellow;
  rotate(180);
  //move(60);
  moveUntilDist(50, 50);
  rotate(-90);
  move(30);
  moveUntilDist(73, 50);
  rotate(90);
  move(10);
  findBlock();
  move(-10);
  rotate(-90);
  //moveUntilDist(122, 50);
  move(-40);
  rotate(90);
  //moveUntilDist(195, 50);
  moveUntilDist(110, 50);
  rotate(-90);
  moveUntilDist(119, 50);
  rotate(-90);
  //rotate(180);
  move(85);
  findPad();
}
