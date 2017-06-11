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
// CDict: 字典

CDict::CDict() {
    fstream file;
    bool repeated;							// 重複
    file.open("dict/text.txt", ios::in);	// 讀取字典檔
    string temp;

    if (file) {
        while (file >> temp) {				// 判斷讀入的單字 在字典裡是否已重複
            repeated = false;

            for (unsigned int i = 0; i < dictionary.size(); i++) {
                if (temp == dictionary[i])
                    repeated = true;
            }

            if (!repeated)
                dictionary.push_back(temp); // 將字典檔讀入
        }
    }

    file.close();
}
string CDict::GetText() {							 // 每次GetText()時 都隨機return一個單字
    unsigned int rnd = rand()  % dictionary.size();  // rnd 從 0~字典字數 隨機抓數字
    return dictionary[rnd];
}

}
