#include "./mcts/bot.cpp"
#include <algorithm>
#include <cmath>
#include <cstring>
#include <ctime>
#include <iostream>

using namespace std;

struct botBoard {
  int board[9][9] = {0};
  int qi[9][9] = {0};
  bool invalid[2][9][9] = {false};
  int preX;
  int preY;
  int turn;
  int invalidNum[2] = {0, 0};
};

const int INF = 1 << 30;
const int MINMAX_NODE_NUM = 32;
const int branchnum = 81;

struct node {
  int x;
  int y;
  int dist;
};

class bot {
private:
  clock_t inputStart;
  clock_t lastInputEnd;
  const int MINMAX_TIME_LIMIT = 1;
  const int MCTS_TIME_LIMIT = 1;
  bool MCTS_mengxin = false;
  void dfsQi(int x, int y, int &qi, int &cnt, bool searched[9][9], int color,
             int boardBoard[9][9], int chessPos[41][2]);
  void dfsQi_init(int x, int y, botBoard &b);
  void dfsQi_update(int x, int y, botBoard &b, bool searched[9][9], int newQi,
                    int color);
  void invalidCheck(int x, int y, botBoard &b, int color, bool player);
  void dfsValid(int x, int y, botBoard &b, bool searched[9][9], int color);
  void boardUpdate(botBoard &b);
  void boardInit(botBoard &b);
  int selectNodes(botBoard &b, node selected[MINMAX_NODE_NUM]);
  int min_max(botBoard &b, int lev, bool player, int alpha_beta[2]);
  void dynamicLevel(botBoard &b);
  void AlphaBeta(int &x, int &y);
  void greed(int &x, int &y);
  void random(int &x, int &y);
  void MCTS(int &x, int &y);

public:
  int mode = 0; // 0:随机 1:一步贪心 2:minmax 5层 5s 3:minmax 6层60s
  int max_search_level = 0;
  int used_time = INF;
  void input(int &x, int &y);
  void init(int modeInput);
  bot() {
    max_search_level = 0;
    used_time = 0;
    return;
  }
};

void bot::dfsQi(int x, int y, int &qi, int &cnt, bool searched[9][9], int color,
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

void bot::dfsQi_init(int x, int y, botBoard &b) {
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

void bot::dfsQi_update(int x, int y, botBoard &b, bool searched[9][9],
                       int newQi, int color) {
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

void bot::invalidCheck(int x, int y, botBoard &b, int color, bool player) {
  if (b.invalid[!player][x][y])
    return;
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

void bot::dfsValid(int x, int y, botBoard &b, bool searched[9][9], int color) {
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

void bot::boardUpdate(botBoard &b) { //更新preX,preY后立即更新盘面
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
  return;
}

void bot::boardInit(botBoard &b) {
  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++)
      if (b.board[i][j] != 0 && b.qi[i][j] == 0)
        dfsQi_init(i, j, b);
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
  return;
}

int bot::selectNodes(botBoard &b, node selected[MINMAX_NODE_NUM]) {
  bool presentPlayer = (b.turn % 2);
  int nodes = 0;
  node validNode[81];
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (!b.invalid[!presentPlayer][i][j]) {
        validNode[nodes].x = i;
        validNode[nodes].y = j;
        validNode[nodes].dist = abs(i - b.preX) + abs(j - b.preY);
        nodes++;
      }
    }
  }
  if (nodes == 0)
    return 0;
  sort(validNode, validNode + nodes,
       [](node n1, node n2) { return n1.dist < n2.dist; });
  if (nodes > MINMAX_NODE_NUM) {
    copy(validNode, validNode + MINMAX_NODE_NUM, selected);
    return MINMAX_NODE_NUM;
  } else {
    copy(validNode, validNode + nodes, selected);
    return nodes;
  }
}

int bot::min_max(botBoard &b, int lev, bool player, int alpha_beta[2]) {
  if (clock() - inputStart > CLOCKS_PER_SEC * MINMAX_TIME_LIMIT)
    return -INF;
  if (lev == 0)
    return b.invalidNum[player] - b.invalidNum[!player];
  bool presentPlayer = (b.turn % 2);
  bool host = (presentPlayer == player);
  node selected[MINMAX_NODE_NUM];
  int nodes = selectNodes(b, selected);
  if (nodes == 0) {
    if (host)
      return -100;
    else
      return 100;
  }
  int reward[MINMAX_NODE_NUM];
  botBoard tmpBoard;
  int Alpha_Beta[2] = {alpha_beta[0], alpha_beta[1]};
  int m;
  if (host) {
    m = -INF;
    for (int i = 0; i < nodes; i++) {
      tmpBoard = b;
      tmpBoard.preX = selected[i].x;
      tmpBoard.preY = selected[i].y;
      boardUpdate(tmpBoard);
      reward[i] = min_max(tmpBoard, lev - 1, player, Alpha_Beta);
      m = max(m, reward[i]);
      Alpha_Beta[0] = max(m, Alpha_Beta[0]);
      if (Alpha_Beta[0] >= Alpha_Beta[1])
        return 200;
    }
  } else {
    m = INF;
    for (int i = 0; i < nodes; i++) {
      tmpBoard = b;
      tmpBoard.preX = selected[i].x;
      tmpBoard.preY = selected[i].y;
      boardUpdate(tmpBoard);
      reward[i] = min_max(tmpBoard, lev - 1, player, Alpha_Beta);
      m = min(m, reward[i]);
      Alpha_Beta[1] = min(m, Alpha_Beta[1]);
      if (Alpha_Beta[0] >= Alpha_Beta[1])
        return -200;
    }
  }
  return m;
}

void bot::dynamicLevel(botBoard &b) {
  if (mode == 2) {
    if (b.turn > 25) {
      if (used_time < MINMAX_TIME_LIMIT * CLOCKS_PER_SEC / 8)
        max_search_level++;
      if (b.turn > 55 && used_time < MINMAX_TIME_LIMIT * CLOCKS_PER_SEC / 2)
        max_search_level++;
    }
  }
  if (mode == 3) {
    if (used_time < 30 * CLOCKS_PER_SEC)
      max_search_level++;
    if (used_time < 10 * CLOCKS_PER_SEC)
      max_search_level++;
  }
  return;
}

void bot::AlphaBeta(int &x, int &y) {
  extern int board[9][9];
  extern int rnd;
  srand(time(0));
  botBoard b;
  b.turn = rnd;
  if (b.turn > 1) {
    b.preX = x;
    b.preY = y;
  } else {
    b.preX = 0;
    b.preY = 0;
  }
  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++)
      b.board[i][j] = board[i][j];
  bool player = b.turn % 2;
  dynamicLevel(b);
  boardInit(b);
  int Alpha_Beta[2] = {-INF, INF};
  node selected[MINMAX_NODE_NUM];
  int nodes = selectNodes(b, selected);
  int reward[MINMAX_NODE_NUM];
  botBoard tmpBoard;
  for (int i = 0; i < nodes; i++) {
    tmpBoard = b;
    tmpBoard.preX = selected[i].x;
    tmpBoard.preY = selected[i].y;
    boardUpdate(tmpBoard);
    reward[i] = min_max(tmpBoard, max_search_level, player, Alpha_Beta);
    Alpha_Beta[0] = max(Alpha_Beta[0], reward[i]);
  }
  int ord = int(max_element(reward, reward + nodes) - reward);
  x = selected[ord].x;
  y = selected[ord].y;
  clock_t t1 = clock();
  used_time = t1 - inputStart;
  return;
}

void bot::greed(int &x, int &y) {
  extern int board[9][9];
  extern int rnd;
  srand(time(0));
  botBoard b;
  b.turn = rnd;
  for (int i = 0; i < 9; i++)
    for (int j = 0; j < 9; j++)
      b.board[i][j] = board[i][j];
  bool player = b.turn % 2;
  boardInit(b);
  node selected[MINMAX_NODE_NUM];
  int nodes = selectNodes(b, selected);
  int reward[MINMAX_NODE_NUM];
  botBoard tmpBoard;
  for (int i = 0; i < nodes; i++) {
    tmpBoard = b;
    tmpBoard.preX = selected[i].x;
    tmpBoard.preY = selected[i].y;
    boardUpdate(tmpBoard);
    reward[i] = b.invalidNum[player] - b.invalidNum[!player];
  }
  int mNodes = 0, m = *max_element(reward, reward + nodes);
  node mNode[MINMAX_NODE_NUM];
  for (int i = 0; i < nodes; i++)
    if (reward[i] == m) {
      mNode[mNodes] = selected[i];
      mNodes++;
    }
  int ord = rand() % mNodes;
  x = mNode[ord].x;
  y = mNode[ord].y;
  return;
}

void bot::random(int &x, int &y) {
  extern int invalidPlace[2][9][9];
  extern int rnd;
  srand(time(0));
  do {
    x = rand() % 9;
    y = rand() % 9;
  } while (invalidPlace[!(rnd % 2)][x][y] > 0);
  return;
}

void bot::MCTS(int &x, int &y) {
    signed char boardForMCTS[81] = {0};
    extern int board[9][9];
    extern int rnd;
    for (int i = 0; i < 81; i++) {
        boardForMCTS[i] = (rnd % 2 == 1) ? board[i / 9][i % 9] : -board[i / 9][i % 9];
    }
    int bestAction = mcts::GetBestAction(boardForMCTS, (double)MCTS_TIME_LIMIT, nullptr);
    x = bestAction / 9;
    y = bestAction % 9;
}

void bot::input(int &x, int &y) {
  inputStart = clock();
  switch (mode) {
  case 0:
    random(x, y);
    break;
  case 1:
    greed(x, y);
    break;
  case 2:
    AlphaBeta(x, y);
    break;
  default:
    MCTS(x, y);
    break;
  }
  while (clock() - inputStart < CLOCKS_PER_SEC)
    continue;
  lastInputEnd = clock();
  return;
}

void bot::init(int modeInput) {
  mode = modeInput;
  switch (mode) {
  case 2:
    max_search_level = 5;
    break;
  case 3:
    MCTS_mengxin = false;
    break;
  case 4:
    MCTS_mengxin = true;
    break;
  }
  return;
}