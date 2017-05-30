#pragma once
#include "CMe.h"
namespace game_framework {

class CRecord {
    public:
        CRecord(int score, int level, double accuracy, string meName, int totalCorrectKeyCount, string date);
        int ReadRecordScore_Score();						 //�^�ǲ�x���̭�����
        int ReadRecordScore_Level();						 //�^�ǲ�x���̭�����
        double ReadRecordScore_Accuracy();					 //�^�ǲ�x���̭������T�v
        string  ReadRecordScore_Character();				 //�^�ǲ�x���̭�������W
        string  ReadRecordScore_Date();						 //�^�ǲ�x���̭������
        int		ReadRecordScore_selectedChar();				 //�^�ǲ�x���̭����D���s��
        int		ReadRecordScore_TotalCorrectKeyCount();		 //�^�ǲ�x���̭������T����ƶq
        //---------
        //void WriteCharacter(string);	//�W���諸����
        //string ReadCharacter();			//�^�ǤW���諸����
    private:
        int score;					//�o��
        int level;					//���d
        double accuracy;			//���T�v
        string date;				//���
        string meName;				//�D���W��(�^��)
        int	   selectedChar;		//�D���s��
        char chDate[80];			//char���A�����
        int totalCorrectKeyCount;	//���T����ƶq
        //string preCharac;			//�W�@���D�����W��

};
}