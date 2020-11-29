#include <iostream>
#include <iomanip>
using namespace std;

extern int board[9][9];
extern int rnd;

string chess(int c)
{
    switch (c)
    {
    case 1:
        return "○";
    case -1:
        return "●";
    case 0:
        return "╬";
    }
    return 0;
}

void printStartMenu()
{
    cout << "┌───────────────────────────────────────────┐" << endl;
    cout << "│                                           │" << endl;
    cout << "│                  不 围 棋                 │" << endl;
    cout << "│                    NoGo                   │" << endl;
    cout << "│                                           │" << endl;
    cout << "└───────────────────────────────────────────┘" << endl;
    cout << "请输入选项:\n0.退出游戏\n1.开始新游戏\n2.加载存档\n"
         << endl;
    return;
}

void printInput()
{
    cout << "请依次输入你想下子位置的行指标和列指标,或者输入\"-1 0\"以回到\n"
         << "开始菜单,输入\"-1 1\"保存当前游戏,输入\"-1 2\"读取保存的游戏.\n"
         << endl;
    cout << "     0  1  2  3  4  5  6  7  8   \n"
         << "  ╔══╦══╦══╦══╦══╦══╦══╦══╦══╦══╗\n";
    for (int i = 0; i < 9; i++)
    {
        cout << i << " ╠══" << chess(board[i][0]) << "══" << chess(board[i][1]) << "══"
             << chess(board[i][2]) << "══" << chess(board[i][3]) << "══" << chess(board[i][4]) << "══"
             << chess(board[i][5]) << "══" << chess(board[i][6]) << "══" << chess(board[i][7]) << "══"
             << chess(board[i][8]) << "══╣" << '\n';
    }
    cout << "  ╚══╩══╩══╩══╩══╩══╩══╩══╩══╩══╝\n\n";
    cout << "当前是第 " << rnd << " 手" << endl;
    return;
}

void printLoad(savedGame sav[10])
{
    cout << "请输入你想加载存档的编号:\n"
         << endl;
    cout << setw(6) << "编号" << setw(22) << "名称" << setw(21) << "保存时间" << setw(6) << "手数" << endl;
    for (int i = 0; i < 10; i++)
    {
        if (!sav[i].empty)
        {
            cout << setw(6) << i << setw(22) << sav[i].name << setw(21) << sav[i].time << setw(6) << sav[i].rnd << endl;
        }
    }
    cout << "\n输入-1退回上一界面." << endl;
    return;
}

void printSave(savedGame sav[10])
{
    cout << "请输入你想存入游戏的存档编号:\n"
         << endl;
    cout << setw(6) << "编号" << setw(22) << "名称" << setw(21) << "保存时间" << setw(6) << "手数" << endl;
    for (int i = 0; i < 10; i++)
    {
        cout << setw(6) << i << setw(22) << sav[i].name << setw(21) << sav[i].time << setw(6) << sav[i].rnd << endl;
    }
    cout << "\n输入-1回到上一界面." << endl;
    return;
}