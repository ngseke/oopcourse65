#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CEraser.h"
#include "CBall.h"
#include "CDict.h"
#include <fstream>
#include <string>
#include <vector>
namespace game_framework {

CDict::CDict() {
    fstream file;
    file.open("Source/text.txt", ios::in); //Ū���r����

    if (!file) {}
    else {
        while (file >> temp) {
            dictionary.push_back(temp);  //�N�r����Ū�J
        }
    }

    file.close();
}
string CDict::GetText() { //�C��GetText()�� ���H��return�@�ӳ�r
    int rnd = rand() % dictionary.size();  //rnd �q 1~�r��r�� �H����Ʀr
    return dictionary[rnd];
}
}
