#pragma warning( disable : 4996 )
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CRecord.h"
#include <iomanip>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>

namespace game_framework {

CRecord::CRecord(int score, int level, double accuracy, string meName, int correctKeyCount, string date) {
    this->score = score;
    this->level = level;
    this->accuracy = accuracy;
    this->meName = meName;
    this->correctKeyCount = correctKeyCount;
    this->date = date;
    //this->totalKeyCount = totalKeyCount;
}

int CRecord::ReadRecordScore_Score() {
    return score;
}
int CRecord::ReadRecordScore_Level() {
    return level;
}
double CRecord::ReadRecordScore_Accuracy() {
    return accuracy;
}
string CRecord::ReadRecordScore_Character() {
    return meName;
}
string CRecord::ReadRecordScore_Date() {
    return date;
}
int CRecord::ReadRecordScore_selectedChar() {
    return selectedChar;
}
int CRecord::ReadRecordScore_CorrectKeyCount() {
    return correctKeyCount;
}
/*
int	CRecord::ReadRecordScore_TotalKeyCount() {
    return this->totalKeyCount;
}
*/
}

