#pragma once
#include "CMe.h"
namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// �o��class���ѷ|�@���u�����y
// �����N�i�H��g���ۤv���{���F
/////////////////////////////////////////////////////////////////////////////

class CRecord {
    public:
        CRecord(int score, int level, double accuracy, string meName);
        void WriteRecord(int score, int level, double accuracy, string meName);
        int ReadRecordScore_Score();  //�^�ǲ�x���̭�����
        int ReadRecordScore_Level();  //�^�ǲ�x���̭�����
        double ReadRecordScore_Accuracy();  //�^�ǲ�x���̭������T�v
        string  ReadRecordScore_Character();  //�^�ǲ�x���̭�������W
        string  ReadRecordScore_Date();  //�^�ǲ�x���̭������
    private:
        int score;
        int level;
        double accuracy;
        string date;
        string meName;
};
}
