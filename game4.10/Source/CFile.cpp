#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include "CFile.h"


namespace game_framework {


CFile::CFile() {
}
void CFile::WriteHighScore(int score, int level, double accuracy, string meName, int correctKeyCount) {
    struct tm* T = NULL;
    time_t t;
    time(&t);
    T = localtime(&t);
    fstream	fp, test;
    fp.open("user/bestRecord.txt", ios::out);
    sprintf(ChDate, "%d%02d%02d%02d%02d", int(T->tm_year + 1900), int(T->tm_mon + 1), int(T->tm_mday), int(T->tm_hour), int(T->tm_min));
    this->HighScore_Date.assign(ChDate);
    fp << "character:" << meName
       << ",score:" << score
       << ",level:" << level
       << ",accuracy:" << accuracy
       << ",date:" << HighScore_Date
       << ",correctKeyCount:" << correctKeyCount << endl;
    fp.close();
}
void CFile::WriteTotalKeyCount(int num) {
    fstream	fp;
    fp.open("user/tkc.txt", ios::out);
    fp << num;
    fp.close();
}
int CFile::ReadTotalKeyCount() {
    fstream	fp;
    fp.open("user/tkc.txt", ios::in);
    string temp;

    if (fp) {
        while (fp >> temp) {}
    }

    return stoi(temp);
    fp.close();
}

void CFile::ReadHighScoreFile() {
    string slideOne, slideTwo[14];
    char temp[200];
    int i = 0;
    fstream	fp;
    fp.open("user/bestRecord.txt", ios::in);

    while (fp.getline(temp, sizeof(temp), ',')) {
        slideOne = temp;
        stringstream ss(slideOne);

        while (getline(ss, slideTwo[i], ':')) {
            if (i == 1) this->HighScore_MeName = slideTwo[1];
            else if (i == 3) this->HighScore_Score = stoi(slideTwo[3], nullptr, 10);
            else if (i == 5) this->HighScore_Level = stoi(slideTwo[5], nullptr, 10);
            else if (i == 7) this->HighScore_Accuracy = stod(slideTwo[7], nullptr);
            else if (i == 9) this->HighScore_Date = slideTwo[9];
            else if (i == 11) this->HighScore_CorrectKeyCount = stoi(slideTwo[11], nullptr, 10);

            i++;
        }
    }

    if (!fp.is_open())HighScore_Score = 0;

    fp.close();
    TRACE("%d, %d\n", HighScore_Score, HighScore_Level);
}
int CFile::ReadHighScore_Score() {
    return this->HighScore_Score;
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
int	CFile::ReadHighScore_CorrectKeyCount() {
    return this->HighScore_CorrectKeyCount;
}
bool CFile::isHighScoreExist() {
    if (!(HighScore_Score == 0))return 1;
    else return 0;
}
//======================================
void CFile::WriteRecord(int score, int level, double accuracy, string meName, int correctKeyCount) {
    struct tm* T = NULL;
    time_t t;
    time(&t);
    T = localtime(&t);
    fstream	fp;
    fp.open("user/record.txt", ios::out | ios::app);
    sprintf(ChDate, "%d%02d%02d%02d%02d", int(T->tm_year + 1900), int(T->tm_mon + 1), int(T->tm_mday), int(T->tm_hour), int(T->tm_min));
    this->record_Date.assign(ChDate);
    fp << "character:" << meName
       << ",score:" << score
       << ",level:" << level
       << ",accuracy:" << accuracy
       << ",date:" << record_Date
       << ",correctKeyCount:" << correctKeyCount << endl;
    fp.close();
}
void CFile::ReadRecordFile() {
    fstream	fp;
    fp.open("user/record.txt", ios::in);
    int i = 0;
    char line[200];
    string SlideOne, SlideTwo, SlideThree[100];

    /*
    for (unsigned int i = 0; i < record.size(); i++) {					//清空Record
    	vector<CRecord*>::iterator iterenemyQueue = record.begin();
    	delete record[i];
    	record[i] = NULL;
    	record.erase(iterenemyQueue + i);
    	i = 0;
    }
    */
    for (CRecord* cr : record) delete cr;

    record.clear();

    while (fp.getline(line, sizeof(line), '\n')) {
        SlideOne = line;
        stringstream ss(SlideOne);

        while (getline(ss, SlideTwo, ',')) {
            stringstream ss2(SlideTwo);

            while (getline(ss2, SlideThree[i], ':')) {
                if (i == 1) this->record_MeName = SlideThree[1];
                else if (i == 3) this->record_Score = stoi(SlideThree[3], nullptr, 10);
                else if (i == 5) this->record_Level = stoi(SlideThree[5], nullptr, 10);
                else if (i == 7) this->record_Accuracy = stod(SlideThree[7], nullptr);
                else if (i == 9) this->record_Date = SlideThree[9];
                else if (i == 11) this->record_CorrectKeyCount = stoi(SlideThree[11], nullptr, 10);

                i++;

                if (i == 12) i = 0;
            }
        }

        if (record_MeName != "")
            record.insert(record.begin(), new CRecord(record_Score, record_Level, record_Accuracy, record_MeName, record_CorrectKeyCount, record_Date));	//從頭插進去
    }

    TRACE("size[%d]\n", record.size());
}
int CFile::ReadRecord_Score(int num) {
    return record.at(num)->ReadRecordScore_Score();
}
int CFile::ReadRecord_Level(int num) {
    return record.at(num)->ReadRecordScore_Level();
}
double CFile::ReadRecord_Accuracy(int num) {
    return record.at(num)->ReadRecordScore_Accuracy();
}
string CFile::ReadRecord_Character(int num) {
    return record.at(num)->ReadRecordScore_Character();
}
string CFile::ReadRecord_Date(int num) {
    return record.at(num)->ReadRecordScore_Date();
}
int	CFile::ReadRecord_CorrectKeyCount(int num) {
    return record.at(num)->ReadRecordScore_CorrectKeyCount();
}
int CFile::GetRecordNum() {
    return int(record.size());
}
void CFile::DeleteAllData() {
    fstream	fp, test;
    fp.open("user/bestRecord.txt", ios::out);
    fp << "";
    fp.close();
    fp.open("user/preSelectedChar.txt", ios::out);
    fp << "";
    fp.close();
    fp.open("user/record.txt", ios::out);
    fp << "";
    fp.close();
    fp.open("user/unlock.txt", ios::out);
    fp << "Iron Man\n";
    fp.close();
    fp.open("user/tkc.txt", ios::out);
    fp << "0";
    fp.close();
    DeleteFile("user/bestRecord.txt");
}
int	CFile::ReadHighScore_TotalKeyCount() {
    return this->HighScore_TotalKeyCount;
}
string CFile::ReadSelectedCharacter() {
    fstream	fp;
    fp.open("user/preSelectedChar.txt", ios::in);
    int i = 0;
    char line[200];
    string SlideOne;

    while (fp.getline(line, sizeof(line), ':')) {
        SlideOne = line;

        if (i == 1) this->preSelectedChar = SlideOne;

        i++;
    }

    fp.close();
    return this->preSelectedChar;
}
void CFile::WriteSelectedCharacter(string preSelectedChar) {
    fstream	fp;
    fp.open("user/preSelectedChar.txt", ios::out);
    fp << "preSelectedChar:" << preSelectedChar;
    fp.close();
}
void CFile::WriteMusicOnOff(bool musicState) {
    fstream fp;
    fp.open("user/musicState.txt", ios::out);
    fp << "musicState:" << musicState;
    fp.close();
}
bool CFile::ReadMusicOnOff() {
    fstream	fp;
    fp.open("user/musicState.txt", ios::in);
    int i = 0;
    char line[200];
    string SlideOne;

    while (fp.getline(line, sizeof(line), ':')) {
        SlideOne = line;
        stringstream ss(SlideOne);

        if (i == 1) ss >> musicState;

        i++;
    }

    fp.close();
    return this->musicState;
}
}