#include <string>
using namespace std;

struct savedGame
{
    bool empty;
    string name;
    char time[20];
    bool computer;
    bool player;
    int rnd;
    int savBoard[9][9];
};
