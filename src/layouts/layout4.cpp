#include "../config.h"
#include "../globals.h"
#include "../movement.h"
#include "../operations.h"

void layout4()
{
  //Blue Block A
  targetBlock = BlockType::Blue;
  moveUntilDist(180, 50);
  rotate(-90);
  realign();
  move(26);
  realign();
  rotate(-90);
  move(20);
  findBlock();
  move(-20);
  rotate(-90);
  moveUntilDist(60, 50);
  rotate(-90);
  //moveUntilDist(65, 50);
  moveUntilDist(70, 50);
  rotate(90);
  moveUntilDist(63, 50);
  rotate(-90);
  move(5);
  rotate(44);
  move(15);
  findPad();
  move(-15);
  rotate(-134);
  //Blue Block B
  targetBlock = BlockType::Blue;
  moveUntilDist(117, 50);
  rotate(90);
  moveUntilDist(180, 50);
  rotate(90);
  realign();
  move(30);
  realign();
  rotate(90);
  move(20);
  findBlock();
  move(-20);
  rotate(180);
  moveUntilDist(80, 50);
  rotate(90);
  moveUntilDist(63, 50);
  rotate(-90);
  move(18);
  rotate(44);
  move(20);
  findPad();
  move(-20);
  rotate(-44);
  //Yellow Block A
  targetBlock = BlockType::Yellow;
  move(-25);
  rotate(90);
  realign();
  rotate(-90);
  move(-25);
  rotate(90);
  move(35);
  findBlock();
  move(-35);
  rotate(-90);
  move(45);
  rotate(-90);
  moveUntilDist(117, 50);
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
  moveUntilDist(115, 50);
  rotate(-90);
  move(40);
  findBlock();
  move(-40);
  rotate(90);
  move(45);
  realign();
  rotate(-90);
  moveUntilDist(117, 50);
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
  move(50);
  findBlock();
  move(-50);
  rotate(90);
  moveUntilDist(60, 50);
  rotate(90);
  moveUntilDist(58, 50);
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
  moveUntilDist(37, 50);
  rotate(90);
  move(50);
  findBlock();
  move(-50);
  rotate(-90);
  moveUntilDist(60, 50);
  rotate(90);
  move(-70);
  rotate(90);
  moveUntilDist(60, 50);
  rotate(45);
  findPad();
}
