#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include <string>
#include <fstream>
#include <ctime>
#include "CFile.h"


namespace game_framework {


CFile::CFile() {
}
void CFile::WriteHighScore(int score, int level, double accuracy, string meName) {
    struct tm* T = NULL;
    time_t t;
    time(&t);
    T = localtime(&t);
    char filename[] = "user/bestRecord.txt";
    fstream	fp;
    fp.open(filename, ios::out);
    sprintf(ChDate, "%d%02d%02d%02d%02d", int(T->tm_year + 1900), int(T->tm_mon + 1), int(T->tm_mday), int(T->tm_hour), int(T->tm_min));
    this->HighScore_Date.assign(ChDate);
    fp << "character:" << meName
       << ",score:" << score
       << ",level:" << level
       << ",accuracy:" << accuracy
       << ",date:" << date << endl;
}
void CFile::ReadHighScoreFile() {}

int CFile::ReadHighScore_Score() {
    return this->HighScore_Accuracy;
}
int CFile::ReadHighScore_Level() {
    return this->HighScore_Level;
}
double CFile::ReadHighScore_Accuracy() {
    return this->HighScore_Accuracy;
}
string CFile::ReadHighScore_Character() {
    return this->HighScore_MeName;
}
string CFile::ReadHighScore_Date() {
    return this->HighScore_Date;
}
bool CFile::isHighScoreExist() {
    return 1;
}
//======================================
void CFile::WriteRecord(int score, int level, double accuracy, string meName) {
}
int CFile::ReadRecordScore_Score() {
    return this->record_Score;
}
int CFile::ReadRecordScore_Level() {
    return this->record_Level;
}
double CFile::ReadRecordScore_Accuracy() {
    return this->record_Accuracy;
}
string CFile::ReadHighScore_Character() {
    return this->record_MeName;
}
string CFile::ReadRecordScore_Date() {
    return this->record_Date;
}
}

