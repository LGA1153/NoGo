#include <iostream>
#include <cstring>
#include <ctime>
#include "jsoncpp/json.h"
using namespace std;

int board[9][9] = {0};
int invalidPlace[9][9] = {0};

char itoa(int n)
{
    return '0' + n;
}

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

void printInput()
{
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
    return;
}

void computerInput(int &x, int &y, int rnd)
{
    srand(rnd);
    do
    {
        x = rand() % 9;
        y = rand() % 9;
    } while (invalidPlace[y][x] > 0);
    return;
}

bool dfs(int r, int c, int pl, bool searched[9][9]) //深搜判断是否有气
{
    if (r == -1 || r == 9 || c == -1 || c == 9 || searched[r][c] || board[r][c] == -pl)
    {
        return false;
    }
    else if (board[r][c] == 0)
    {
        return true;
    }
    searched[r][c] = true;
    return dfs(r - 1, c, pl, searched) || dfs(r + 1, c, pl, searched) || dfs(r, c - 1, pl, searched) || dfs(r, c + 1, pl, searched);
}

void validCheck()
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (invalidPlace[i][j] == 0)
            {
                if (board[i][j] != 0)
                {
                    invalidPlace[i][j] = 1;
                    continue;
                }
                bool searched[9][9] = {false};
                int c = 1;
                board[i][j] = c;
                if (!dfs(i, j, c, searched))
                {
                    invalidPlace[i][j] = 2;
                }
                else
                {
                    bool u = true, d = true, l = true, r = true;
                    if (i > 0 && board[i - 1][j] == -c)
                    {
                        memset(searched, 0, 9 * 9 * sizeof(bool));
                        u = dfs(i - 1, j, -c, searched);
                    }
                    if (i < 8 && board[i + 1][j] == -c)
                    {
                        memset(searched, 0, 9 * 9 * sizeof(bool));
                        d = dfs(i + 1, j, -c, searched);
                    }
                    if (j > 0 && board[i][j - 1] == -c)
                    {
                        memset(searched, 0, 9 * 9 * sizeof(bool));
                        l = dfs(i, j - 1, -c, searched);
                    }
                    if (j < 8 && board[i][j + 1] == -c)
                    {
                        memset(searched, 0, 9 * 9 * sizeof(bool));
                        r = dfs(i, j + 1, -c, searched);
                    }
                    if (!(u && d && l && r))
                    {
                        invalidPlace[i][j] = 2;
                    }
                }
                board[i][j] = 0;
            }
        }
    }
    return;
}

int main()
{
    memset(board, 0, 9 * 9 * sizeof(int));
    memset(invalidPlace, 0, 9 * 9 * sizeof(int));
    string strInput;
    int x, y;
    getline(cin, strInput);
    Json::Reader reader;
    Json::Value input;
    reader.parse(strInput, input);
    cout << input["debug"] << endl;
    int turn = input["responses"].size(); //(等效)回合数
    x = input["requests"][int(0)]["x"].asInt();
    y = input["requests"][int(0)]["y"].asInt();
    if (turn == 0 && x == -1)
        invalidPlace[4][4] = 2;
    else
        board[y][x] = -1;
    if (turn == 1)
        invalidPlace[4][4] = 0;
    for (int i = 1; i <= turn; i++)
    {
        x = input["requests"][i]["x"].asInt();
        y = input["requests"][i]["y"].asInt();
        board[y][x] = -1;
        x = input["responses"][i - 1]["x"].asInt();
        y = input["responses"][i - 1]["y"].asInt();
        board[y][x] = 1;
    }
    validCheck();
    computerInput(x, y, x * y);
    Json::Value output;
    Json::FastWriter writer;
    string data = "";
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            data = data + itoa(invalidPlace[i][j]) + " ";
        }
        data = data + "\n";
    }
    output["debug"] = data;
    output["response"]["x"] = x;
    output["response"]["y"] = y;
    cout << writer.write(output) << endl;
    return 0;
}