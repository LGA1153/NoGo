#pragma GCC optimize(3)
#include <algorithm>
#include <cmath>
#include <ctime>
#include <iostream>

#include "jsoncpp/json.h"
using namespace std;

const int INF = 1 << 30;
clock_t startTime;

struct board {
  int board[9][9] = {0};
  int qi[9][9] = {0};
  bool invalid[2][9][9] = {false};
  int turn;
  bool player;
  int invalidNum[2] = {0, 0};
  int preX;
  int preY;
};

struct treeNode {
  treeNode* branch[81] = {nullptr};
  board nodeBoard;
  int branchNum = 0;
  double visitedTimes = 0;
  double value;
};

void dfsQi(int x, int y, int& qi, int& cnt, bool searched[9][9], int color,
           int boardBoard[9][9], int chessPos[41][2]) {
  if ((x == -1 || x == 9 || y == -1 || y == 9) || searched[x][y] ||
      boardBoard[x][y] == -color) {
    return;
  } else if (boardBoard[x][y] == 0) {
    searched[x][y] = true;
    qi++;
    return;
  }
  chessPos[cnt][0] = x;
  chessPos[cnt][1] = y;
  cnt++;
  searched[x][y] = true;
  dfsQi(x + 1, y, qi, cnt, searched, color, boardBoard, chessPos);
  dfsQi(x - 1, y, qi, cnt, searched, color, boardBoard, chessPos);
  dfsQi(x, y + 1, qi, cnt, searched, color, boardBoard, chessPos);
  dfsQi(x, y - 1, qi, cnt, searched, color, boardBoard, chessPos);
  return;
}

void dfsQi_init(int x, int y, board& b) {
  int color = b.board[x][y];
  int qi = 0, cnt = 0;
  bool searched[9][9] = {false};
  int chessPos[81][2] = {0};
  dfsQi(x, y, qi, cnt, searched, color, b.board, chessPos);
  qi *= color;
  for (int i = 0; i < cnt; i++) {
    b.qi[chessPos[i][0]][chessPos[i][1]] = qi;
  }
  return;
}

void dfsQi_update(int x, int y, board& b, bool searched[9][9], int newQi,
                  int color) {
  if ((x == -1 || x == 9 || y == -1 || y == 9) || searched[x][y] ||
      b.board[x][y] != color) {
    return;
  }
  searched[x][y] = true;
  b.qi[x][y] = newQi;
  dfsQi_update(x + 1, y, b, searched, newQi, color);
  dfsQi_update(x - 1, y, b, searched, newQi, color);
  dfsQi_update(x, y + 1, b, searched, newQi, color);
  dfsQi_update(x, y - 1, b, searched, newQi, color);
  return;
}

void invalidCheck(int x, int y, board& b, int color, bool player) {
  if (b.invalid[!player][x][y]) return;
  bool notEdge[4] = {x > 0, x<8, y> 0, y < 8};
  if ((notEdge[0] && b.qi[x - 1][y] == -color) ||
      (notEdge[1] && b.qi[x + 1][y] == -color) ||
      (notEdge[2] && b.qi[x][y - 1] == -color) ||
      (notEdge[3] && b.qi[x][y + 1] == -color)) {
    b.invalid[!player][x][y] = true;
    b.invalidNum[!player]++;
    return;
  }
  if ((!notEdge[0] || b.qi[x - 1][y] == color || b.qi[x - 1][y] * color < 0) &&
      (!notEdge[1] || b.qi[x + 1][y] == color || b.qi[x + 1][y] * color < 0) &&
      (!notEdge[2] || b.qi[x][y - 1] == color || b.qi[x][y - 1] * color < 0) &&
      (!notEdge[3] || b.qi[x][y + 1] == color || b.qi[x][y + 1] * color < 0)) {
    b.invalid[!player][x][y] = true;
    b.invalidNum[!player]++;
    return;
  }
  return;
}

void dfsValid(int x, int y, board& b, bool searched[9][9], int color) {
  if (x == -1 || x == 9 || y == -1 || y == 9 || searched[x][y] ||
      b.board[x][y] == -color)
    return;
  if (b.board[x][y] == 0) {
    searched[x][y] = true;
    invalidCheck(x, y, b, 1, 1);
    invalidCheck(x, y, b, -1, 0);
    return;
  }
  searched[x][y] = true;
  dfsValid(x - 1, y, b, searched, color);
  dfsValid(x + 1, y, b, searched, color);
  dfsValid(x, y - 1, b, searched, color);
  dfsValid(x, y + 1, b, searched, color);
  return;
}

void boardUpdate(board& b) {  //更新preX,preY后立即更新盘面
  int color = b.turn % 2 ? 1 : -1;
  int x = b.preX, y = b.preY;
  b.board[x][y] = color;
  b.invalid[0][x][y] = true;
  b.invalid[1][x][y] = true;
  bool notEdge[4] = {x > 0, x<8, y> 0, y < 8};
  bool qiSearched[9][9] = {false};
  if (notEdge[0] && b.board[x - 1][y] == -color)
    dfsQi_update(x - 1, y, b, qiSearched, b.qi[x - 1][y] + color, -color);
  if (notEdge[1] && b.board[x + 1][y] == -color)
    dfsQi_update(x + 1, y, b, qiSearched, b.qi[x + 1][y] + color, -color);
  if (notEdge[2] && b.board[x][y - 1] == -color)
    dfsQi_update(x, y - 1, b, qiSearched, b.qi[x][y - 1] + color, -color);
  if (notEdge[3] && b.board[x][y + 1] == -color)
    dfsQi_update(x, y + 1, b, qiSearched, b.qi[x][y + 1] + color, -color);
  dfsQi_init(x, y, b);
  bool validSearched[9][9] = {false};
  if (notEdge[0] && b.board[b.preX - 1][b.preY] == -color)
    dfsValid(b.preX - 1, b.preY, b, validSearched, -color);
  if (notEdge[1] && b.board[b.preX + 1][b.preY] == -color)
    dfsValid(b.preX + 1, b.preY, b, validSearched, -color);
  if (notEdge[2] && b.board[b.preX][b.preY - 1] == -color)
    dfsValid(b.preX, b.preY - 1, b, validSearched, -color);
  if (notEdge[3] && b.board[b.preX][b.preY + 1] == -color)
    dfsValid(b.preX, b.preY + 1, b, validSearched, -color);
  dfsValid(b.preX, b.preY, b, validSearched, color);
  b.turn++;
  b.player = !b.player;
  return;
}

void boardInit(board& b) {
  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++)
      if (b.board[i][j] != 0 && b.qi[i][j] == 0) dfsQi_init(i, j, b);
  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++) {
      if (b.board[i][j] != 0) {
        b.invalid[0][i][j] = true;
        b.invalid[1][i][j] = true;
        b.invalidNum[0]++;
        b.invalidNum[1]++;
      } else {
        invalidCheck(i, j, b, 1, 1);
        invalidCheck(i, j, b, -1, 0);
      }
    }
  b.player = b.turn % 2;
  return;
}

inline double getValue(board b, bool player) {
  return b.invalidNum[player] - b.invalidNum[!player];
}

void search(treeNode* node, int& globalVisitedNum, bool player) {
  node->visitedTimes++;
  if (node->branchNum == 0) {  //叶子节点
    globalVisitedNum++;
    boardUpdate(node->nodeBoard);
    board tmpBoard;
    int cnt = 0;
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        if (!node->nodeBoard.invalid[!node->nodeBoard.player][i][j]) {
          tmpBoard = node->nodeBoard;
          tmpBoard.preX = i;
          tmpBoard.preY = j;
          node->branch[cnt] = new treeNode;
          node->branch[cnt]->nodeBoard = tmpBoard;
          cnt++;
        }
      }
    }
    if (cnt == 0) {
      node->value = INF * ((player == node->nodeBoard.player) ? -1 : 1);
    } else {
      node->branchNum = cnt;
      node->value = getValue(node->nodeBoard, player);
    }
    return;
  }
  double UCB[81];
  for (int i = 0; i < node->branchNum; i++) {
    UCB[i] =
        node->branch[i]->value * ((player == node->nodeBoard.player) ? 1 : -1) +
        sqrt(log(globalVisitedNum) / node->branch[i]->visitedTimes);
  }
  treeNode* selected =
      node->branch[int(max_element(UCB, UCB + node->branchNum) - UCB)];
  search(selected, globalVisitedNum, player);
  node->value = (node->value * node->visitedTimes + selected->value) /
                (node->visitedTimes + 1);
  return;
}

void computerInput(int& x, int& y, board b, bool player, Json::Value& output) {
  // clock_t startTime = clock();
  if (b.turn == 1) {
    x = 0;
    y = 0;
    return;
  }
  boardInit(b);
  treeNode* root = new treeNode;
  root->nodeBoard = b;
  int globalVisitedNum = 1;
  //初始化根节点以防出错
  board tmpBoard;
  int cnt = 0;
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (!root->nodeBoard.invalid[!root->nodeBoard.player][i][j]) {
        tmpBoard = root->nodeBoard;
        tmpBoard.preX = i;
        tmpBoard.preY = j;
        root->branch[cnt] = new treeNode;
        root->branch[cnt]->nodeBoard = tmpBoard;
        cnt++;
      }
    }
  }
  root->branchNum = cnt;
  while (clock() - startTime < CLOCKS_PER_SEC * 19 / 20) {
    search(root, globalVisitedNum, player);
  }
  double reward[81] = {0};
  for (int i = 0; i < root->branchNum; i++) {
    reward[i] = root->branch[i]->value;
  }
  int ord = int(max_element(reward, reward + root->branchNum) - reward);
  x = root->branch[ord]->nodeBoard.preX;
  y = root->branch[ord]->nodeBoard.preY;
  output["debug"]["nodes"] = globalVisitedNum;
  return;
}

int main() {
  startTime = clock();
  board board_main;
  string strInput;
  int x, y;
  getline(cin, strInput);
  Json::Reader reader;
  Json::Value input;
  reader.parse(strInput, input);
  x = input["requests"][int(0)]["x"].asInt();
  y = input["requests"][int(0)]["y"].asInt();
  bool player = (x == -1 && y == -1);  // t:黑 f:白
  board_main.turn = 2 * input["requests"].size() - player;
  for (int i = 0; i < int(input["responses"].size()); i++) {
    x = input["responses"][i]["x"].asInt();
    y = input["responses"][i]["y"].asInt();
    board_main.board[x][y] = player ? 1 : -1;
  }
  for (int i = player; i < int(input["requests"].size()); i++) {
    x = input["requests"][i]["x"].asInt();
    y = input["requests"][i]["y"].asInt();
    board_main.board[x][y] = player ? -1 : 1;
  }
  board_main.preX = x;
  board_main.preY = y;
  Json::Value output;
  Json::FastWriter writer;
  computerInput(x, y, board_main, player, output);
  output["response"]["x"] = x;
  output["response"]["y"] = y;
  cout << writer.write(output) << endl;
  return 0;
}