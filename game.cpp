#include <ctime>
#include <iostream>
#include <string>

using namespace std;

extern int board[9][9];
extern int invalidPlace[2][9][9];
extern int rnd;
void printInput();

void playerInput(int &x, int &y) {
  printInput();
  // cin.ignore(1024, '\n');
  cin >> x >> y;
  cin.ignore(1024, '\n');
  if (x == -1 && (y >= 0 && y <= 3)) {
    return;
  }
  while (x < 0 || x > 8 || y < 0 || y > 8 || invalidPlace[!(rnd % 2)][x][y]) {
    if (x < 0 || x > 8 || y < 0 || y > 8) {
      cout << "请输入合法位置!" << endl;
    } else if (board[x][y] != 0) {
      cout << "此处已有棋子,请重新输入!" << endl;
    } else {
      cout << "此处为禁手,请重新输入!" << endl;
    }
    cin >> x >> y;
    cin.ignore(1024, '\n');
    if (x == -1 && (y >= 0 && y <= 2)) {
      return;
    }
  }
  return;
}

bool dfs(int r, int c, int pl, bool searched[9][9]) //深搜判断是否有气
{
  if (r == -1 || r == 9 || c == -1 || c == 9 || searched[r][c] ||
      board[r][c] == -pl) {
    return false;
  } else if (board[r][c] == 0) {
    return true;
  }
  searched[r][c] = true;
  return dfs(r - 1, c, pl, searched) || dfs(r + 1, c, pl, searched) ||
         dfs(r, c - 1, pl, searched) || dfs(r, c + 1, pl, searched);
}

void validCheck() //检查对手的可走位置
{
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (invalidPlace[rnd % 2][i][j] == 0) {
        if (board[i][j] != 0) {
          invalidPlace[rnd % 2][i][j] = 1;
          continue;
        }
        bool searched[9][9] = {false};
        int c = (rnd % 2) ? -1 : 1;
        board[i][j] = c;
        if (!dfs(i, j, c, searched)) {
          invalidPlace[rnd % 2][i][j] = 2;
        } else {
          bool u = true, d = true, l = true, r = true;
          if (i > 0 && board[i - 1][j] == -c) {
            memset(searched, 0, 9 * 9 * sizeof(bool));
            u = dfs(i - 1, j, -c, searched);
          }
          if (i < 8 && board[i + 1][j] == -c) {
            memset(searched, 0, 9 * 9 * sizeof(bool));
            d = dfs(i + 1, j, -c, searched);
          }
          if (j > 0 && board[i][j - 1] == -c) {
            memset(searched, 0, 9 * 9 * sizeof(bool));
            l = dfs(i, j - 1, -c, searched);
          }
          if (j < 8 && board[i][j + 1] == -c) {
            memset(searched, 0, 9 * 9 * sizeof(bool));
            r = dfs(i, j + 1, -c, searched);
          }
          if (!(u && d && l && r)) {
            invalidPlace[rnd % 2][i][j] = 2;
          }
        }
        board[i][j] = 0;
      }
    }
  }
  return;
}

bool judge() {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (invalidPlace[rnd % 2][i][j] == 0) {
        return false;
      }
    }
  }
  return true;
}