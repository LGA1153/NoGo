#include <iostream>
#include <fstream>

const string EMPTY =
    "{\n"
    "    1\n"
    "    NULL\n"
    "    NULL\n"
    "    0\n"
    "    0\n"
    "    0\n"
    "    0 0 0 0 0 0 0 0 0\n"
    "    0 0 0 0 0 0 0 0 0\n"
    "    0 0 0 0 0 0 0 0 0\n"
    "    0 0 0 0 0 0 0 0 0\n"
    "    0 0 0 0 0 0 0 0 0\n"
    "    0 0 0 0 0 0 0 0 0\n"
    "    0 0 0 0 0 0 0 0 0\n"
    "    0 0 0 0 0 0 0 0 0\n"
    "    0 0 0 0 0 0 0 0 0\n"
    "}\n\n";

void boardCopy(int sor[9][9], int tar[9][9])
{
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            tar[i][j] = sor[i][j];
        }
    }
    return;
}

void savInit(fstream &savFile)
{
    savFile.open("sav.dat", ios::out);
    for (int i = 0; i < 10; i++)
    {
        savFile << EMPTY;
    }
    return;
}

void load(savedGame sav[10], fstream &savFile)
{
    char tmp = 0;
    for (int i = 0; i < 10; i++)
    {
        while (tmp != '{')
        {
            tmp = savFile.get();
        }
        savFile >> sav[i].empty;
        savFile.ignore();
        savFile.ignore(4);
        getline(savFile, sav[i].name);
        savFile.ignore(4);
        savFile.getline(sav[i].time, 20);
        savFile >> sav[i].computer;
        savFile >> sav[i].player;
        savFile >> sav[i].rnd;
        for (int j = 0; j < 9; j++)
        {
            for (int k = 0; k < 9; k++)
            {
                savFile >> sav[i].savBoard[j][k];
            }
        }
        while (tmp != '}')
        {
            tmp = savFile.get();
        }
    }
    return;
}

void save(savedGame sav[10], fstream &savFile)
{
    for (int i = 0; i < 10; i++)
    {
        savFile << "{\n"
                << "    " << (sav[i].empty) << '\n'
                << "    " + sav[i].name + "\n"
                << "    " << sav[i].time << '\n'
                << "    " << sav[i].computer << '\n'
                << "    " << sav[i].player << '\n'
                << "    " << sav[i].rnd << endl;
        for (int j = 0; j < 9; j++)
        {
            savFile << "   ";
            for (int k = 0; k < 9; k++)
            {
                savFile << ' ' << sav[i].savBoard[j][k];
            }
            savFile << endl;
        }
        savFile << "}\n"
                << endl;
    }
    return;
}