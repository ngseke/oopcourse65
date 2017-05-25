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

namespace game_framework {

CRecord::CRecord() {
}
void CRecord::WriteRecord(int score, int level, double accuracy, int selectedChar) {
    char filename[] = "user.txt";
    fstream	fp;
    struct tm* T = NULL;
    time_t t;
    time(&t);
    T = localtime(&t);
    fp.open(filename, ios::out | ios::app);

    if (!fp) {
    }

    fp << "character:" << selectedChar
       << ",score:" << score
       << ",level:" << level
       << ",accuracy:" << accuracy
       << ",date:" << T->tm_year + 1900 << ","
       << setw(2) << setfill('0') << T->tm_mon + 1
       << setw(2) << setfill('0') << T->tm_mday
       << setw(2) << setfill('0') << T->tm_hour
       << setw(2) << setfill('0') << T->tm_min
       << setw(2) << setfill('0') << T->tm_sec << endl;
}

int CRecord::ReadRecordScore_Score(int x) {
    return score;
}
int CRecord::ReadRecordScore_Level(int x) {
    return level;
}
double CRecord::ReadRecordScore_Accuracy(int x) {
    return accuracy;
}
string CRecord::ReadRecordScore_Character(int x) {
    return character;
}
string CRecord::ReadRecordScore_Date(int x) {
    return date;
}
}

