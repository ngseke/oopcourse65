#pragma once
namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// �o��class���ѷ|�@���u�����y
// �����N�i�H��g���ۤv���{���F
/////////////////////////////////////////////////////////////////////////////

class CFile {
    public:
        CFile(int score, int level, double accuracy, string meName, string date);
        void WriteHighScore(int score, int level, double accuracy, string meName, string date);
        int ReadHighScore_Score();  //�^�ǳ̰����̭�����
        int ReadHighScore_Level();  //�^�ǳ̰����̭�����
        double ReadHighScore_Accuracy();  //�^�ǳ̰����̭������T�v
        string  ReadHighScore_Character();  //�^�ǳ̰����̭�������W
        string  ReadHighScore_Date();  //�^�ǳ̰����̭������


    private:
        int score;
        int level;
        double accuracy;
        string date;
        string meName;

};
}
