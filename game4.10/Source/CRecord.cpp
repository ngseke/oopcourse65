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

CRecord::CRecord(int score, int level, double accuracy, string meName) {
    struct tm* T = NULL;
    time_t t;
    time(&t);
    T = localtime(&t);
    this->score = score;
    this->level = level;
    this->accuracy = accuracy;
    this->meName = meName;
    sprintf(chDate, "%d%02d%02d%02d%02d", int(T->tm_year + 1900), int(T->tm_mon + 1), int(T->tm_mday), int(T->tm_hour), int(T->tm_min));
    this->date.assign(chDate);
}
void CRecord::WriteRecord(int score, int level, double accuracy, string meName) {
    char filename[] = "user/user.txt";
    fstream	fp;
    struct tm* T = NULL;
    time_t t;
    time(&t);
    T = localtime(&t);
    fp.open(filename, ios::out | ios::app);
    fp << "character:" << meName
       << ",score:" << score
       << ",level:" << level
       << ",accuracy:" << accuracy
       << ",date:" << date << endl;
    /*T->tm_year + 1900
    << setw(2) << setfill('0') << T->tm_mon + 1
    << setw(2) << setfill('0') << T->tm_mday
    << setw(2) << setfill('0') << T->tm_hour
    << setw(2) << setfill('0') << T->tm_min
    << ",test_date:" << date << endl;*/
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

}

