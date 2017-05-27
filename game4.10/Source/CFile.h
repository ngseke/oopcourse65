#pragma once
#include "CRecord.h"
namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// 這個class提供會一直彈跳的球
// 看懂就可以改寫成自己的程式了
/////////////////////////////////////////////////////////////////////////////

class CFile {
    public:
        CFile();
        void WriteHighScore(int score, int level, double accuracy, string meName);
        void ReadHighScoreFile();
        int ReadHighScore_Score();  //回傳最高分裡面的分
        int ReadHighScore_Level();  //回傳最高分裡面的關
        double ReadHighScore_Accuracy();  //回傳最高分裡面的正確率
        string  ReadHighScore_Character();  //回傳最高分裡面的角色名
        string  ReadHighScore_Date();  //回傳最高分裡面的日期
        bool isHighScoreExist();		//回傳是否存在最高紀錄
        //================================
        void WriteRecord(int score, int level, double accuracy, string meName);			//寫入:分數 關卡 正確率 主角名稱
        int ReadRecordScore_Score();  //回傳第x筆裡面的分
        int ReadRecordScore_Level();  //回傳第x筆裡面的關
        double ReadRecordScore_Accuracy();  //回傳第x筆裡面的正確率
        string  ReadRecordScore_Character();  //回傳第x筆裡面的角色名
        string  ReadRecordScore_Date();  //回傳第x筆裡面的日期

    private:
        int HighScore_Score;
        int HighScore_Level;
        double HighScore_Accuracy;
        string HighScore_Date;
        string HighScore_MeName;
        //===================
        vector<CRecord*> record;
        int record_Score;				//得分
        int record_Level;				//關卡
        double record_Accuracy;		//正確率
        string record_Date;			//日期
        string record_MeName;			//主角名稱(英文)
        int	   record_SelectedChar;	//主角編號
        char ChDate[80];		//char型態的日期
};
}
