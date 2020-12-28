#include <cstring>
#include <iostream>

#include "interface.cpp"
using namespace std;

int interface; //-1:退出 0:开始菜单 1:新游戏 2:载入游戏 3.储存游戏 4:游戏中
               // 5:查看规则

bot bot1;
bot bot2;

int main() {
  system("cls");
  interface = 0;
  while (true) {
    switch (interface) {
    case -1:
      return 0;
    case 0:
      IF_startMenu();
      break;
    case 1:
      IF_newGame();
      break;
    case 2:
      IF_load();
      break;
    case 3:
      IF_save();
      break;
    case 4:
      IF_game();
      break;
    case 5:
      IF_help();
      break;
    }
  }
  return 0;
}