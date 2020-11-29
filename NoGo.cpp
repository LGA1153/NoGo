#include <iostream>
#include <cstring>
#include "global.h"
#include "interface.cpp"
using namespace std;

int board[9][9]; //1:黑 -1:白 0:空
int rnd;
int interface; //-1:退出 0:开始菜单 1:新游戏 2:载入游戏 3.储存游戏 4:游戏中
int previous;
bool player;
bool computerOpponent;
int invalidPlace[2][9][9]; //[2]0表示黑子,1表示白子,储存数 0:可走 1:已有棋子 2:自杀

int main()
{
    //system("cls");
    interface = 0;
    memset(board, 0, 9 * 9 * sizeof(int));
    while (true)
    {
        switch (interface)
        {
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
        }
    }
    return 0;
}