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
    bool chongfu;	//重複
    file.open("Source/text.txt", ios::in); //讀取字典檔

    if (!file) {}
    else {
        while (file >> temp) {
            chongfu = 0;

            //以下用於判斷讀入的單字 在字典裡是否已重複
            for (unsigned int i = 0; i < dictionary.size(); i++) {
                if (temp == dictionary[i])chongfu = 1;
            }

            if (chongfu != 1)dictionary.push_back(temp); //將字典檔讀入
        }
    }

    file.close();
}
string CDict::GetText() { //每次GetText()時 都隨機return一個單字
    int rnd = rand() % dictionary.size();  //rnd 從 0~字典字數 隨機抓數字
    return dictionary[rnd];
}

}
