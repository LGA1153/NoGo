#include <iostream>
#include <fstream>
#include <ctime>
#include "print.cpp"
#include "saveload.cpp"
#include "game.cpp"
using namespace std;

extern int board[9][9];
extern int invalidPlace[2][9][9];
extern int rnd;
extern int interface;
extern int previous;
extern bool player;
extern bool computerOpponent;

void IF_startMenu()
{
    int optTmp;
    printStartMenu();
    cin >> optTmp; //0:exit 1:new game 2:load save
    cin.ignore(1024, '\n');
    while (optTmp < 0 || optTmp > 2)
    {
        cout << "Please enter a valid option!" << endl;
        cin >> optTmp;
        cin.ignore(1024, '\n');
    }

    interface = optTmp;
    if (interface == 0)
    {
        interface = -1;
    }
    previous = 0;
    //system("cls");
    return;
}

void IF_load()
{
    fstream savFile;
    savFile.open("sav.dat", ios::in);
    if (!savFile.is_open())
    {
        cout << "Error:Cannot open saved games!" << endl;
        cout << "Do you want to create a new save file? Yes:1 No:0" << endl;
        bool choice;
        cin >> choice;
        cin.ignore(1024, '\n');
        if (choice)
        {
            savInit(savFile);
            cout << "A new save file has been created!" << endl;
            //system("pause");
        }
        //system("cls");
        interface = previous;
        return;
    }
    savedGame sav[10];
    bool exist = false;
    load(sav, savFile);
    for (int i = 0; i < 10; i++)
    {
        exist = exist || !sav[i].empty;
    }
    savFile.close();
    if (!exist)
    {
        cout << "Error:No saved game exists!" << endl;
        //system("pause");
        //system("cls");
        interface = previous;
        return;
    }
    printLoad(sav);
    int index;
    cin >> index;
    cin.ignore(1024, '\n');
    while (index < -1 || index > 9 || sav[index].empty)
    {
        cout << "Invalid input!" << endl;
        cin >> index;
        cin.ignore(1024, '\n');
    }
    if (index == -1)
    {
        //system("cls");
        interface = previous;
        return;
    }
    boardCopy(sav[index].savBoard, board);
    computerOpponent = sav[index].computer;
    player = sav[index].player;
    rnd = sav[index].rnd;
    memset(invalidPlace, 0, 2 * 9 * 9 * sizeof(int));
    rnd++;
    validCheck();
    rnd--;
    validCheck();
    //system("cls");
    interface = 4;
    return;
}

void IF_save()
{
    fstream savFile;
    savFile.open("sav.dat", ios::in);
    if (!savFile.is_open())
    {
        cout << "错误:无法打开存储的游戏!" << endl;
        cout << "你是否想要创建一个新的存档文件?\n是:1 否:0" << endl;
        bool choice;
        cin >> choice;
        if (choice)
        {
            savInit(savFile);
            cout << "新存档文件创建成功!" << endl;
            //system("pause");
        }
        //system("cls");
        interface = 4;
        return;
    }
    savedGame sav[10];
    load(sav, savFile);
    savFile.close();
    printSave(sav);
    int index;
    cin >> index;
    cin.ignore(1024,'\n');
    while (index < -1 || index > 9)
    {
        cout << "请输入合法的指标!" << endl;
        cin >> index;
        cin.ignore(1024,'\n');
    }
    if (index == -1)
    {
        //system("cls");
        interface = 4;
        return;
    }
    boardCopy(board, sav[index].savBoard);
    sav[index].computer = computerOpponent;
    sav[index].player = player;
    sav[index].rnd = rnd;
    sav[index].empty = false;
    string name;
    cout << "请输入存档名称(上限20字符):" << endl;
    cin.ignore();
    getline(cin, name);
    while (name.length() > 20)
    {
        cout << "请输入一个合法的名字!" << endl;
        getline(cin, name);
    }
    sav[index].name = name;
    time_t rawt;
    tm *t;
    time(&rawt);
    t = localtime(&rawt);
    strftime(sav[index].time, 20, "%Y-%m-%d %H:%M:%S", t);
    savFile.open("sav.dat", ios::out);
    save(sav, savFile);
    savFile.close();
    cout << "保存成功!" << endl;
    //system("pause");
    //system("cls");
    interface = 4;
    return;
}

void IF_newGame()
{
    memset(board, 0, 9 * 9 * sizeof(int));
    memset(invalidPlace, 0, 2 * 9 * 9 * sizeof(int));
    rnd = 1;
    cout << "游戏设定:" << endl;
    cout << "玩家2(对手)是电脑还是人类?\n电脑:1 人类:0" << endl;
    cin >> computerOpponent;
    cin.ignore(1024,'\n');
    cout << "玩家1(你)先手?\n是:1 否:0" << endl;
    cin >> player;
    cin.ignore(1024,'\n');
    //system("cls");
    interface = 4;
    return;
}

void IF_game()
{
    int x, y;
    if (computerOpponent && (rnd + player) % 2)
    {
        computerInput(x, y);
    }
    else
    {
        playerInput(x, y);
    }
    if (x == -1)
    {
        switch (y)
        {
        case 0:
            interface = 0;
            break;
        case 1:
            interface = 3;
            break;
        case 2:
            interface = 2;
            break;
        }
        previous = 4;
        //system("cls");
        return;
    }
    board[x][y] = (rnd % 2) ? 1 : -1;
    validCheck();
    if (judge())
    {
        //system("cls");
        printInput();
        cout << x << ' ' << y << endl;
        string winner = (rnd % 2) ? "黑方" : "白方";
        cout << winner << "玩家获胜!" << endl;
        //system("pause");
        //system("cls");
        interface = 0;
        return;
    }
    rnd++;
    if (!(computerOpponent && (rnd + player) % 2))
    {
        //system("cls");
    }
    return;
}
