#include <ctime>
#include <fstream>
#include <iostream>

#include "bot.cpp"
#include "game.cpp"
#include "jsoncpp/json.h"
#include "print.cpp"
#include "saveload.cpp"
using namespace std;

int rnd;
int board[9][9]; // 1:�� -1:�� 0:��
int previous;
int goneTurn = 0;
bool isBot[2];
int invalidPlace[2][9]
                [9]; //[2]0��ʾ����,1��ʾ����,������ 0:���� 1:�������� 2:��ɱ
extern int interface;
extern bot bot1;
extern bot bot2;
int x, y;

void IF_startMenu() {
  int optTmp;
  printStartMenu();
  cin >> optTmp;
  cin.ignore(1024, '\n');
  while (optTmp < 0 || optTmp > 3) {
    cout << "������Ϸ�ѡ��!" << endl;
    cin >> optTmp;
    cin.ignore(1024, '\n');
  }
  switch (optTmp) {
  case 0:
    interface = -1;
    break;
  case 3:
    interface = 5;
    break;
  default:
    interface = optTmp;
    break;
  }
  previous = 0;
  system("cls");
  return;
}

void IF_load() {
  fstream savFile;
  savFile.open("sav.dat", ios::in);
  if (!savFile.is_open()) {
    cout << "����:�޷��򿪴浵!" << endl;
    cout << "�Ƿ񴴽��´浵�ļ�? ��:1 ��:0" << endl;
    bool choice;
    cin >> choice;
    cin.ignore(1024, '\n');
    if (choice) {
      savInit(savFile);
      cout << "�´浵�Ѵ���!" << endl;
      system("pause");
    }
    system("cls");
    interface = previous;
    return;
  }
  Json::Value sav[10];
  bool exist = false;
  load(sav, savFile);
  for (int i = 0; i < 10; i++) {
    exist = exist || !sav[i]["empty"].asBool();
  }
  savFile.close();
  if (!exist) {
    cout << "����:�����ڱ������Ϸ!" << endl;
    system("pause");
    system("cls");
    interface = previous;
    return;
  }
  printLoad(sav);
  int index;
  cin >> index;
  cin.ignore(1024, '\n');
  while (index < -1 || index > 9 || (index > -1 && sav[index].empty())) {
    cout << "��������Ч����!" << endl;
    cin >> index;
    cin.ignore(1024, '\n');
  }
  if (index == -1) {
    system("cls");
    interface = previous;
    return;
  }
  saveToGame(sav[index]);
  goneTurn = 0;
  system("cls");
  interface = 4;
  return;
}

void IF_save() {
  fstream savFile;
  savFile.open("sav.dat", ios::in);
  if (!savFile.is_open()) {
    cout << "����:�޷��򿪴洢����Ϸ!" << endl;
    cout << "���Ƿ���Ҫ����һ���µĴ浵�ļ�?\n��:1 ��:0" << endl;
    bool choice;
    cin >> choice;
    if (choice) {
      savInit(savFile);
      cout << "�´浵�ļ������ɹ�!" << endl;
      system("pause");
    }
    system("cls");
    interface = 4;
    return;
  }
  Json::Value sav[10];
  load(sav, savFile);
  savFile.close();
  printSave(sav);
  int index;
  cin >> index;
  cin.ignore(1024, '\n');
  while (index < -1 || index > 9) {
    cout << "������Ϸ���ָ��!" << endl;
    cin >> index;
    cin.ignore(1024, '\n');
  }
  if (index == -1) {
    system("cls");
    interface = 4;
    return;
  }
  string name;
  cout << "������浵����(����20�ַ�):" << endl;
  getline(cin, name);
  while (name.length() > 20) {
    cout << "������һ���Ϸ�������!" << endl;
    getline(cin, name);
  }
  sav[index]["name"] = name;
  time_t rawt;
  tm *t;
  char savTime[20];
  time(&rawt);
  t = localtime(&rawt);
  strftime(savTime, 20, "%Y-%m-%d %H:%M:%S", t);
  sav[index]["time"] = savTime;
  savFile.open("sav.dat", ios::out);
  gameToSave(sav[index]);
  save(sav, savFile);
  savFile.close();
  cout << "����ɹ�!" << endl;
  system("pause");
  system("cls");
  interface = 4;
  return;
}

void IF_newGame() {
  memset(board, 0, 9 * 9 * sizeof(int));
  memset(invalidPlace, 0, 2 * 9 * 9 * sizeof(int));
  rnd = 1;
  goneTurn = 0;
  cout << "��Ϸ�趨:" << endl;
  cout << "���1(����)�ǵ��Ի�������?\n����:1 ����:0" << endl;
  cin >> isBot[0];
  cin.ignore(1024, '\n');
  if (isBot[0]) {
    int mode = 0;
    cout << "���������ˮƽ ���:0 ��:1 ��ͨ:2 ����:3 ����:4" << endl;
    do {
      if (mode < 0 || mode > 4)
        cout << "��������Чֵ!" << endl;
      cin >> mode;
      cin.ignore(1024, '\n');
    } while (mode < 0 || mode > 4);
    bot1.init(mode);
  }
  cout << "���2(����)�ǵ��Ի�������?\n����:1 ����:0" << endl;
  cin >> isBot[1];
  cin.ignore(1024, '\n');
  if (isBot[1]) {
    int mode = 0;
    cout << "���������ˮƽ ���:0 ��:1 ��ͨ:2 ����:3 ����:4" << endl;
    do {
      if (mode < 0 || mode > 4)
        cout << "��������Чֵ!" << endl;
      cin >> mode;
      cin.ignore(1024, '\n');
    } while (mode < 0 || mode > 4);
    bot2.init(mode);
  }
  system("cls");
  interface = 4;
  return;
}

void IF_game() {
  bool blackWhiteTurn = rnd % 2; // t:�� f:��
  printBoard();
  if (blackWhiteTurn && isBot[0]) {
    cout << "�ȴ����Ծ�����......" << endl;
    bot1.input(x, y);
  } else if (!blackWhiteTurn && isBot[1]) {
    cout << "�ȴ����Ծ�����......" << endl;
    bot2.input(x, y);
  } else {
    playerInput(x, y);
  }
  if (x == -1) {
    switch (y) {
    case 0:
      interface = 0;
      break;
    case 1:
      interface = 3;
      break;
    case 2:
      interface = 2;
      break;
    case 3:
      interface = 5;
      break;
    }
    previous = 4;
    system("cls");
    return;
  }
  board[x][y] = blackWhiteTurn ? 1 : -1;
  validCheck();
  if (judge()) {
    system("cls");
    printBoard();
    cout << x << ' ' << y << endl;
    string winner = (blackWhiteTurn) ? "�ڷ�" : "�׷�";
    cout << winner << "��һ�ʤ!" << endl;
    system("pause");
    system("cls");
    interface = 0;
    return;
  }
  rnd++;
  goneTurn++;
  system("cls");
  return;
}

void IF_help() {
  printHelp();
  system("pause");
  system("cls");
  interface = previous;
  return;
}