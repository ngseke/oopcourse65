#pragma once
#include "CRecord.h"
namespace game_framework {

class CFile {
    public:
        CFile();
        void WriteHighScore(int score, int level, double accuracy, string meName, int correctKeyCount);
        void ReadHighScoreFile();
        int ReadHighScore_Score();							 //回傳最高分裡面的分
        int ReadHighScore_Level();  						 //回傳最高分裡面的關
        double ReadHighScore_Accuracy();					 //回傳最高分裡面的正確率
        string  ReadHighScore_Character();					 //回傳最高分裡面的角色名
        string  ReadHighScore_Date();						 //回傳最高分裡面的日期
        int		ReadHighScore_CorrectKeyCount();			 //回傳最高分裡面的正確按鍵數量 （單場）
        int		ReadHighScore_TotalKeyCount();				 //回傳總按鍵數（會累積）
        bool isHighScoreExist();							 //回傳是否存在最高紀錄
        //================================
        void WriteRecord(int score, int level, double accuracy, string meName, int correctKeyCount);			//寫入:分數 關卡 正確率 主角名稱
        void ReadRecordFile();
        int ReadRecord_Score(int);							 //回傳第x筆裡面的分
        int ReadRecord_Level(int);							 //回傳第x筆裡面的關
        double ReadRecord_Accuracy(int);					 //回傳第x筆裡面的正確率
        string  ReadRecord_Character(int);					 //回傳第x筆裡面的角色名
        string  ReadRecord_Date(int);						 //回傳第x筆裡面的日期
        int		ReadRecord_CorrectKeyCount(int);			 //回傳第x筆裡面的正確按鍵數量
        int     GetRecordNum();								 //回傳record大小
        //================================
        void	WriteSelectedCharacter(string);				//寫入上一次所選角色
        string  ReadSelectedCharacter();					//讀取上一次所選角色
        //================================
        void	WriteMusicOnOff(bool);
        bool	ReadMusicOnOff();

        //-----------
        void WriteTotalKeyCount(int);
        int ReadTotalKeyCount();
        //================================

        void DeleteAllData();					//清除所有記錄

    private:
        int HighScore_Score;				//最高紀錄的得分
        int HighScore_Level;				//最高紀錄的關卡
        double HighScore_Accuracy;			//最高紀錄的正確率
        string HighScore_Date;				//最高紀錄的日期
        string HighScore_MeName;			//最高紀錄主角名稱(英文)
        int HighScore_CorrectKeyCount;		//最高紀錄的正確按鍵數量
        int HighScore_TotalKeyCount;		//總按鍵數（會累積的）
        //===================
        vector<CRecord*> record;
        int record_Score;					//得分
        int record_Level;					//關卡
        double record_Accuracy;				//正確率
        string record_Date;					//日期
        string record_MeName;				//主角名稱(英文)
        int	   record_SelectedChar;			//主角編號
        int	   record_CorrectKeyCount;		//正確按鍵數
        char ChDate[80];					//char型態的日期
        //=====================
        string	preSelectedChar;			//上一隻角色
        //================================
        bool  musicState;

};
}
