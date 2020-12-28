#include <iomanip>
#include <iostream>
using namespace std;

extern int board[9][9];
extern int rnd;
extern int x, y;

string chess(int c) {
  switch (c) {
    case 1:
      return "�T��";
    case -1:
      return "�T��";
    case 0:
      return "�T�p�T";
  }
  return 0;
}

void printStartMenu() {
  cout << 
"����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������\n"
"��                   :Qr                                                                         BBB7                  ��\n"
"��                   BBQ7ir:.               bv                   :BBB:                           iBBBi                 ��\n"
"��            S7   rEBQBMZqBBBQ           2BB     iL            7BBBBM                Xr          qBBB                 ��\n"
"��            PQBBBB7iBP   BBBB          7BBBsEBBBBQBX        :BBBR1i                 RBBr        .BBB                 ��\n"
"��             gBBBB.JQBBBBBBg           BBBQBBivBBBBBQ       sBQBq                   .BBBD        BBB                 ��\n"
"��              2BBQDPBP.i5X:  ..       rBBBs   BBU  Id          .BBi                  qBBBB.      BBB                 ��\n"
"��               .UQi5QgsssuXBBBQQ       BBr  :BBi               .RBBBB7     ir        :BBBBQ      BBB                 ��\n"
"��          :XdBBBBBBLLJPUJ.     .          sEBBBRBb           rBBBQBB7 : rBBBBB5       BBB:      .BBB                 ��\n"
"��          iBBP .BBB2si.ZQBB              BBBQ.7BBr          DBB PBQQ :BBQg.:BBBL      BBB vBBq  iBBj                 ��\n"
"��                RBBsXBB  BQB             P gBBPBBBr         7BD  BQBBBBBBE  1BQB       QBB.IBBBg EBBR                ��\n"
"��                BBB .Bg  BBQ            5 PBq:..EBs         BB  BBBQBv.BBBuBBBBP       BBQBBBBBQ QBBBQ.              ��\n"
"��              rQBB.PBI  BBB           r.JBB:r5BBQ71.r.     PB.:BBRBB  dBBBQBBB       QBQBBs RBBQBBigBBR.             ��\n"
"��              :BBBYrsg  BBQ           s7Bdrrr:.. v  BBB     sd   SBB   jY   r      JBBQBBB    gBBJ rBBBBE            ��\n"
"��               BBBQBBB  BB:          7BBBquPQBBu PBQBBE        jBBBB               uBBBB5    bBQ7  EBBBBB:           ��\n"
"��                iKQB1  BQD           BBB .Di   :ZjBBBB          SBBY                .BB:   rBBB:    .ZBBBB           ��\n"
"��                ZBD    :BBR          UB:       .dBBBQ            :                        MBBR        :BBB           ��\n"
"��             rBQ.       L.                                                               .r.           v             ��\n"
"��                                                       �� Χ ��                                                      ��\n"
"��                                                         NoGo                                                        ��\n"
"��ver 2.0                                                                                                        by zsl��\n"
"����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������\n"
          "������ѡ��: "
          "0.�˳���Ϸ "
          "1.��ʼ����Ϸ "
          "2.���ش浵 "
          "3.�鿴��Ϸ����\n"
       << endl;
  return;
}

void printBoard() {
  cout << "      0  1  2  3  4  5  6  7  8  \n"
          "  �X�T�T�j�T�T�j�T�T�j�T�T�j�T�T�j�T�T�j�T�T�j�T�T�j�T�T�j�T�T�[\n";
  for (int i = 0; i < 9; i++) {
    cout << i << " �d�T" << chess(board[0][i]) << chess(board[1][i])
         << chess(board[2][i]) << chess(board[3][i]) << chess(board[4][i])
         << chess(board[5][i]) << chess(board[6][i]) << chess(board[7][i])
         << chess(board[8][i]) << "�T�g" << '\n';
  }
  cout << "  �^�T�T�m�T�T�m�T�T�m�T�T�m�T�T�m�T�T�m�T�T�m�T�T�m�T�T�m�T�T�a\n";
  cout << "��ǰ�ǵ� " << rnd << " ��" << endl;
  if(rnd > 1) cout << "��һ����λ��Ϊ " << x << " �� " << y << " ��\n" << endl;
  return;
}

void printInput() {
  cout << "������������������λ�õ���ָ�����ָ��,��������\"-1 0\"�Իص�\n"
          "��ʼ�˵�,����\"-1 1\"���浱ǰ��Ϸ,����\"-1 2\"��ȡ�������Ϸ,\n"
          "����-1 3�鿴��Ϸ����."
       << endl;
  return;
}

void printLoad(Json::Value sav[10]) {
  cout << "������������ش浵�ı��:\n" << endl;
  cout << setw(6) << "���" << setw(22) << "����" << setw(21) << "����ʱ��"
       << setw(6) << "�غ�" << endl;
  for (int i = 0; i < 10; i++) {
    if (!sav[i].empty()) {
      cout << setw(6) << i << setw(22) << sav[i]["name"].asString() << setw(21)
           << sav[i]["time"].asCString() << setw(6) << sav[i]["rnd"].asInt()
           << endl;
    }
  }
  cout << "\n����-1�˻���һ����." << endl;
  return;
}

void printSave(Json::Value sav[10]) {
  cout << "���������������Ϸ�Ĵ浵���:\n" << endl;
  cout << setw(6) << "���" << setw(22) << "����" << setw(21) << "����ʱ��"
       << setw(6) << "�غ�" << endl;
  for (int i = 0; i < 10; i++) {
    if (sav[i].empty()) {
      cout << setw(6) << i << setw(22) << ' ' << setw(21) << "�մ浵" << setw(6)
           << ' ' << endl;
    } else {
      cout << setw(6) << i << setw(22) << sav[i]["name"].asString() << setw(21)
           << sav[i]["time"].asString() << setw(6) << sav[i]["rnd"].asInt()
           << endl;
    }
  }
  cout << "\n����-1�ص���һ����." << endl;
  return;
}

void printHelp() {
  cout << "NoGo(��Χ��),"
          "��Χ������������������������걸��Ϣ���ģ�������Χ���෴��\n"
          "���̣���9*9Χ��������ͬ(�����0��ʼ����x��y, ԭ�������Ͻ�)\n"
          "1. ����ͬ��·Χ�����̣�9��9��\n"
          "2. �������֣�˫���������ӣ����Ӻ����Ӳ����ƶ���\n"
          "3. ���ĵ�Ŀ�겻�ǳԵ��Է�������\n"
          "4. ǡǡ�෴�����һ�����Ӻ�Ե��˶Է������ӣ�������һ���и���\n"
          "5. ���Ľ�ֹ��ɱ��������ɱһ���и���\n"
          "6. ���Ľ�ֹ����(pass)������һ���и���\n"
          "7. ���Ľ��ֻ��ʤ����û�к��塣\n"
          "8�����Ӷ��壺һ�������������ϣ�����ֱ�߽��ڵĿյ���������ӵġ������� "
          "����ֱ�߽��ڵĵ��ϣ������ͬɫ���Ӵ��ڣ������Ǳ��໥���ӳ�һ�����ɷ�"
          "������塣���ǵ���ҲӦһ�����㡣 "
          "����ֱ�߽��ڵĵ��ϣ��������ɫ���Ӵ��ڣ�������Ͳ������ڡ������е���"
          "��Ϊ�Է���ռ�ݣ��������״̬������״̬�����Ӳ����������ϴ��ڣ�Ҳ����"
          "���ӡ�������֮�����������ֶνС����ӡ�\n"
       << endl;
  return;
}