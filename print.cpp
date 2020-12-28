#include <iomanip>
#include <iostream>
using namespace std;

extern int board[9][9];
extern int rnd;
extern int x, y;

string chess(int c) {
  switch (c) {
    case 1:
      return "═○";
    case -1:
      return "═●";
    case 0:
      return "═╬═";
  }
  return 0;
}

void printStartMenu() {
  cout << 
"┌─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐\n"
"│                   :Qr                                                                         BBB7                  │\n"
"│                   BBQ7ir:.               bv                   :BBB:                           iBBBi                 │\n"
"│            S7   rEBQBMZqBBBQ           2BB     iL            7BBBBM                Xr          qBBB                 │\n"
"│            PQBBBB7iBP   BBBB          7BBBsEBBBBQBX        :BBBR1i                 RBBr        .BBB                 │\n"
"│             gBBBB.JQBBBBBBg           BBBQBBivBBBBBQ       sBQBq                   .BBBD        BBB                 │\n"
"│              2BBQDPBP.i5X:  ..       rBBBs   BBU  Id          .BBi                  qBBBB.      BBB                 │\n"
"│               .UQi5QgsssuXBBBQQ       BBr  :BBi               .RBBBB7     ir        :BBBBQ      BBB                 │\n"
"│          :XdBBBBBBLLJPUJ.     .          sEBBBRBb           rBBBQBB7 : rBBBBB5       BBB:      .BBB                 │\n"
"│          iBBP .BBB2si.ZQBB              BBBQ.7BBr          DBB PBQQ :BBQg.:BBBL      BBB vBBq  iBBj                 │\n"
"│                RBBsXBB  BQB             P gBBPBBBr         7BD  BQBBBBBBE  1BQB       QBB.IBBBg EBBR                │\n"
"│                BBB .Bg  BBQ            5 PBq:..EBs         BB  BBBQBv.BBBuBBBBP       BBQBBBBBQ QBBBQ.              │\n"
"│              rQBB.PBI  BBB           r.JBB:r5BBQ71.r.     PB.:BBRBB  dBBBQBBB       QBQBBs RBBQBBigBBR.             │\n"
"│              :BBBYrsg  BBQ           s7Bdrrr:.. v  BBB     sd   SBB   jY   r      JBBQBBB    gBBJ rBBBBE            │\n"
"│               BBBQBBB  BB:          7BBBquPQBBu PBQBBE        jBBBB               uBBBB5    bBQ7  EBBBBB:           │\n"
"│                iKQB1  BQD           BBB .Di   :ZjBBBB          SBBY                .BB:   rBBB:    .ZBBBB           │\n"
"│                ZBD    :BBR          UB:       .dBBBQ            :                        MBBR        :BBB           │\n"
"│             rBQ.       L.                                                               .r.           v             │\n"
"│                                                       不 围 棋                                                      │\n"
"│                                                         NoGo                                                        │\n"
"│ver 2.0                                                                                                        by zsl│\n"
"└─────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┘\n"
          "请输入选项: "
          "0.退出游戏 "
          "1.开始新游戏 "
          "2.加载存档 "
          "3.查看游戏规则\n"
       << endl;
  return;
}

void printBoard() {
  cout << "      0  1  2  3  4  5  6  7  8  \n"
          "  ╔══╦══╦══╦══╦══╦══╦══╦══╦══╦══╗\n";
  for (int i = 0; i < 9; i++) {
    cout << i << " ╠═" << chess(board[0][i]) << chess(board[1][i])
         << chess(board[2][i]) << chess(board[3][i]) << chess(board[4][i])
         << chess(board[5][i]) << chess(board[6][i]) << chess(board[7][i])
         << chess(board[8][i]) << "═╣" << '\n';
  }
  cout << "  ╚══╩══╩══╩══╩══╩══╩══╩══╩══╩══╝\n";
  cout << "当前是第 " << rnd << " 手" << endl;
  if(rnd > 1) cout << "上一落子位置为 " << x << " 列 " << y << " 行\n" << endl;
  return;
}

void printInput() {
  cout << "请依次输入你想下子位置的列指标和行指标,或者输入\"-1 0\"以回到\n"
          "开始菜单,输入\"-1 1\"保存当前游戏,输入\"-1 2\"读取保存的游戏,\n"
          "输入-1 3查看游戏规则."
       << endl;
  return;
}

void printLoad(Json::Value sav[10]) {
  cout << "请输入你想加载存档的编号:\n" << endl;
  cout << setw(6) << "编号" << setw(22) << "名称" << setw(21) << "保存时间"
       << setw(6) << "回合" << endl;
  for (int i = 0; i < 10; i++) {
    if (!sav[i].empty()) {
      cout << setw(6) << i << setw(22) << sav[i]["name"].asString() << setw(21)
           << sav[i]["time"].asCString() << setw(6) << sav[i]["rnd"].asInt()
           << endl;
    }
  }
  cout << "\n输入-1退回上一界面." << endl;
  return;
}

void printSave(Json::Value sav[10]) {
  cout << "请输入你想存入游戏的存档编号:\n" << endl;
  cout << setw(6) << "编号" << setw(22) << "名称" << setw(21) << "保存时间"
       << setw(6) << "回合" << endl;
  for (int i = 0; i < 10; i++) {
    if (sav[i].empty()) {
      cout << setw(6) << i << setw(22) << ' ' << setw(21) << "空存档" << setw(6)
           << ' ' << endl;
    } else {
      cout << setw(6) << i << setw(22) << sav[i]["name"].asString() << setw(21)
           << sav[i]["time"].asString() << setw(6) << sav[i]["rnd"].asInt()
           << endl;
    }
  }
  cout << "\n输入-1回到上一界面." << endl;
  return;
}

void printHelp() {
  cout << "NoGo(不围棋),"
          "由围棋衍生而来，属于两人零和完备信息博弈，规则与围棋相反。\n"
          "棋盘：与9*9围棋棋盘相同(坐标从0开始，先x后y, 原点在左上角)\n"
          "1. 棋盘同九路围棋棋盘，9×9；\n"
          "2. 黑子先手，双方轮流落子，落子后棋子不可移动；\n"
          "3. 对弈的目标不是吃掉对方的棋子\n"
          "4. 恰恰相反，如果一方落子后吃掉了对方的棋子，则落子一方判负；\n"
          "5. 对弈禁止自杀，落子自杀一方判负；\n"
          "6. 对弈禁止空手(pass)，空手一方判负；\n"
          "7. 对弈结果只有胜负，没有和棋。\n"
          "8．吃子定义：一个棋子在棋盘上，与它直线紧邻的空点是这个棋子的“气”。 "
          "棋子直线紧邻的点上，如果有同色棋子存在，则它们便相互连接成一个不可分"
          "割的整体。它们的气也应一并计算。 "
          "棋子直线紧邻的点上，如果有异色棋子存在，这口气就不复存在。如所有的气"
          "均为对方所占据，便呈无气状态。无气状态的棋子不能在棋盘上存在，也就是"
          "提子。把无气之子提出盘外的手段叫“提子”\n"
       << endl;
  return;
}