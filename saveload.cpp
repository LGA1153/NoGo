#include <fstream>
#include <iostream>

#include "jsoncpp/json.h"
struct savedGame {
  bool empty;
  string name;
  char time[20];
  bool isBot[2];
  int rnd;
  int savBoard[9][9];
};

extern bool isBot[2];
extern bot bot1;
extern bot bot2;

const string EMPTY = "{}";

void saveToGame(Json::Value &sav) {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      board[i][j] = sav["board"][i][j].asInt();
    }
  }
  isBot[0] = sav["isBot"][0].asBool();
  bot1.mode = sav["bot1"]["mode"].asInt();
  bot1.max_search_level = sav["bot1"]["max_search_level"].asInt();
  bot1.used_time = sav["bot1"]["used_time"].asInt();
  isBot[1] = sav["isBot"][1].asBool();
  bot2.mode = sav["bot2"]["mode"].asInt();
  bot2.max_search_level = sav["bot2"]["max_search_level"].asInt();
  bot2.used_time = sav["bot2"]["used_time"].asInt();
  rnd = sav["rnd"].asInt();
  memset(invalidPlace, 0, 2 * 9 * 9 * sizeof(int));
  rnd++;
  validCheck();
  rnd--;
  validCheck();
  return;
}

void gameToSave(Json::Value &sav) {
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      sav["board"][i][j] = board[i][j];
    }
  }
  sav["isBot"][0] = isBot[0];
  sav["bot1"]["mode"] = bot1.mode;
  sav["bot1"]["max_search_level"] = bot1.max_search_level;
  sav["bot1"]["used_time"] = bot1.used_time;
  sav["isBot"][1] = isBot[1];
  sav["bot2"]["mode"] = bot2.mode;
  sav["bot2"]["max_search_level"] = bot2.max_search_level;
  sav["bot2"]["used_time"] = bot2.used_time;
  sav["rnd"] = rnd;
  return;
}

void savInit(fstream &savFile) {
  savFile.open("sav.dat", ios::out);
  for (int i = 0; i < 10; i++) {
    savFile << EMPTY << endl;
  }
  return;
}

void load(Json::Value sav[10], fstream &savFile) {
  Json::Reader reader;
  string fstreamInput;
  for (int i = 0; i < 10; i++) {
    getline(savFile, fstreamInput);
    reader.parse(fstreamInput, sav[i]);
  }
  return;
}

void save(Json::Value sav[10], fstream &savFile) {
  Json::FastWriter faseWriter;
  for (int i = 0; i < 10; i++) {
    savFile << faseWriter.write(sav[i]);
  }
  return;
}
