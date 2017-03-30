#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "gamelib.h"
#include "CDict.h"
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <cctype>

namespace game_framework {

CDict::CDict() {
    fstream file;
    bool chongfu;	//����
    file.open("Source/text.txt", ios::in); //Ū���r����

    if (!file) {}
    else {
        while (file >> temp) {
            chongfu = 0;

            //�H�U�Ω�P�_Ū�J����r �b�r��̬O�_�w����
            for (unsigned int i = 0; i < dictionary.size(); i++) {
                if (temp == dictionary[i])chongfu = 1;
            }

            if (chongfu != 1)dictionary.push_back(temp); //�N�r����Ū�J
        }
    }

    file.close();
}
string CDict::GetText() { //�C��GetText()�� ���H��return�@�ӳ�r
    int rnd = rand() % dictionary.size();  //rnd �q 0~�r��r�� �H����Ʀr
    return dictionary[rnd];
}

}
