#pragma once
namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// �o��class���ѷ|�@���u�����y
// �����N�i�H��g���ۤv���{���F
/////////////////////////////////////////////////////////////////////////////

class CRecord {
    public:
        CRecord();
        void WriteRecord(int score, int level, double accuracy, string character, string date);
        int ReadRecordScore_Score(int);  //�^�ǲ�x���̭�����
        int ReadRecordScore_Level(int);  //�^�ǲ�x���̭�����
        double ReadRecordScore_Accuracy(int);  //�^�ǲ�x���̭������T�v
        string  ReadRecordScore_Character(int);  //�^�ǲ�x���̭�������W
        string  ReadRecordScore_Date(int);  //�^�ǲ�x���̭������

    private:
        int score;
        int level;
        double accuracy;
        string character;
        string date;

};
}
