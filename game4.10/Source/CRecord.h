#pragma once
#include "CMe.h"
namespace game_framework {

class CRecord {
    public:
        CRecord(int score, int level, double accuracy, string meName, int correctKeyCount, string date);
        int ReadRecordScore_Score();						 //回傳第x筆裡面的分
        int ReadRecordScore_Level();						 //回傳第x筆裡面的關
        double ReadRecordScore_Accuracy();					 //回傳第x筆裡面的正確率
        string  ReadRecordScore_Character();				 //回傳第x筆裡面的角色名
        string  ReadRecordScore_Date();						 //回傳第x筆裡面的日期
        int		ReadRecordScore_selectedChar();				 //回傳第x筆裡面的主角編號
        int		ReadRecordScore_CorrectKeyCount();			 //回傳第x筆裡面的正確按鍵數量
    private:
        int score;					//得分
        int level;					//關卡
        double accuracy;			//正確率
        string date;				//日期
        string meName;				//主角名稱(英文)
        int	   selectedChar;		//主角編號
        char chDate[80];			//char型態的日期
        int correctKeyCount;	    //正確按鍵數量



};
}
