#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "gamelib.h"
#include "CDict.h"
#include <fstream>
#include <string>
#include <vector>
#include <cctype>
#include <ctime>

namespace game_framework {

CDict::CDict() {
    //srand((unsigned)time(NULL));
    fstream file;
    bool chongfu;	//����
    file.open("dict/text.txt", ios::in); //Ū���r����
    string temp;

    if (!file) {}
    else {
        while (file >> temp) {
            chongfu = 0;

            //�H�U�Ω�P�_Ū�J����r �b�r��̬O�_�w����

            for (unsigned int i = 0; i < dictionary.size(); i++) {
                if (temp == dictionary[i])	chongfu = 1;
            }

            if (!chongfu)dictionary.push_back(temp); //�N�r����Ū�J
        }
    }

    file.close();
}
string CDict::GetText() { //�C��GetText()�� ���H��return�@�ӳ�r
    unsigned int rnd = rand()  % dictionary.size();  //rnd �q 0~�r��r�� �H����Ʀr
    return dictionary[rnd];
}

}
