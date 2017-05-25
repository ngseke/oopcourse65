#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CRecord.h"


namespace game_framework {


CRecord::CRecord() {
}
void CRecord::WriteRecord(int score, int level, double accuracy, string character, string date) {
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

