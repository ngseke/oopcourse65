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
    file.open("Source/text.txt", ios::in); //讀取字典檔

    if (!file) {}
    else {
        while (file >> temp) {
            dictionary.push_back(temp);  //將字典檔讀入
        }
    }

    file.close();
}
string CDict::GetText() { //每次GetText()時 都隨機return一個單字
    int rnd = rand() % dictionary.size();  //rnd 從 1~字典字數 隨機抓數字
    return dictionary[rnd];
}
}
