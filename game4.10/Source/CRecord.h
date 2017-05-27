#pragma once
#include "CMe.h"
namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// 這個class提供會一直彈跳的球
// 看懂就可以改寫成自己的程式了
/////////////////////////////////////////////////////////////////////////////

class CRecord {
    public:
        CRecord(int score, int level, double accuracy, string meName, int selectedChar);//分數 關卡 正確率 主角名稱 主角編號
        void WriteRecord(int score, int level, double accuracy, string meName);			//寫入:分數 關卡 正確率 主角名稱
        int ReadRecordScore_Score();  //回傳第x筆裡面的分
        int ReadRecordScore_Level();  //回傳第x筆裡面的關
        double ReadRecordScore_Accuracy();  //回傳第x筆裡面的正確率
        string  ReadRecordScore_Character();  //回傳第x筆裡面的角色名
        string  ReadRecordScore_Date();  //回傳第x筆裡面的日期
        int		ReadRecordScore_selectedChar();	//回傳第x筆裡面的主角編號
        //---------
        //void WriteCharacter(string);	//上次選的角色
        //string ReadCharacter();			//回傳上次選的角色
    private:
        int score;				//得分
        int level;				//關卡
        double accuracy;		//正確率
        string date;			//日期
        string meName;			//主角名稱(英文)
        int	   selectedChar;	//主角編號
        char chDate[80];		//char型態的日期
        //string preCharac;		//上一隻主角的名稱

};
}
