#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CFile.h"


namespace game_framework {


CFile::CFile() {
}
void CFile::WriteHighScore(int score, int level, double accuracy, string character, string date) {
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
    return character;
}
string CFile::ReadHighScore_Date() {
    return date;
}
void CFile::WriteCharacter(string preCharac) {
}
string CFile::ReadCharacter() {
    return preCharac;
}

}

