#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include <string>
#include <fstream>
#include "CFile.h"


namespace game_framework {


CFile::CFile(int score, int level, double accuracy, string meName, string date) {
    this->score = score;
    this->level = level;
    this->accuracy = accuracy;
    this->meName = meName;
    this->date = date;
}
void CFile::WriteHighScore(int score, int level, double accuracy, string meName, string date) {
    char filename[] = "user/bestRecord.txt";
    fstream	fp;
    fp.open(filename, ios::out);
    fp << "character:" << meName
       << ",score:" << score
       << ",level:" << level
       << ",accuracy:" << accuracy
       << ",date:" << date << endl;
}

int CFile::ReadHighScore_Score() {
    return score;
}
int CFile::ReadHighScore_Level() {
    return level;
}
double CFile::ReadHighScore_Accuracy() {
    return accuracy;
}
string CFile::ReadHighScore_Character() {
    return meName;
}
string CFile::ReadHighScore_Date() {
    return date;
}


}

