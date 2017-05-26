#pragma once
#include "CMe.h"
namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// 這個class提供會一直彈跳的球
// 看懂就可以改寫成自己的程式了
/////////////////////////////////////////////////////////////////////////////

class CRecord {
    public:
        CRecord(int score, int level, double accuracy, string meName);
        void WriteRecord(int score, int level, double accuracy, string meName);
        int ReadRecordScore_Score();  //回傳第x筆裡面的分
        int ReadRecordScore_Level();  //回傳第x筆裡面的關
        double ReadRecordScore_Accuracy();  //回傳第x筆裡面的正確率
        string  ReadRecordScore_Character();  //回傳第x筆裡面的角色名
        string  ReadRecordScore_Date();  //回傳第x筆裡面的日期
    private:
        int score;
        int level;
        double accuracy;
        string date;
        string meName;
};
}
