#pragma once
namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// 這個class提供會一直彈跳的球
// 看懂就可以改寫成自己的程式了
/////////////////////////////////////////////////////////////////////////////

class CFile {
    public:
        CFile(int score, int level, double accuracy, string meName, string date);
        void WriteHighScore(int score, int level, double accuracy, string meName, string date);
        int ReadHighScore_Score();  //回傳最高分裡面的分
        int ReadHighScore_Level();  //回傳最高分裡面的關
        double ReadHighScore_Accuracy();  //回傳最高分裡面的正確率
        string  ReadHighScore_Character();  //回傳最高分裡面的角色名
        string  ReadHighScore_Date();  //回傳最高分裡面的日期


    private:
        int score;
        int level;
        double accuracy;
        string date;
        string meName;

};
}
