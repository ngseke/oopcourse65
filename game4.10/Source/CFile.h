#pragma once
#include "CRecord.h"
namespace game_framework {

class CFile {
    public:
        CFile();
        void WriteHighScore(int score, int level, double accuracy, string meName, int totalCorrectKeyCount);
        void ReadHighScoreFile();
        int ReadHighScore_Score();							 //�^�ǳ̰����̭�����
        int ReadHighScore_Level();  						 //�^�ǳ̰����̭�����
        double ReadHighScore_Accuracy();					 //�^�ǳ̰����̭������T�v
        string  ReadHighScore_Character();					 //�^�ǳ̰����̭�������W
        string  ReadHighScore_Date();						 //�^�ǳ̰����̭������
        int		ReadHighScore_TotalCorrectKeyCount();		 //�^�ǳ̰����̭������T����ƶq
        bool isHighScoreExist();							 //�^�ǬO�_�s�b�̰�����
        //================================
        void WriteRecord(int score, int level, double accuracy, string meName, int totalCorrectKeyCount);			//�g�J:���� ���d ���T�v �D���W��
        void ReadRecordFile();
        int ReadRecordScore_Score();						 //�^�ǲ�x���̭�����
        int ReadRecordScore_Level();						 //�^�ǲ�x���̭�����
        double ReadRecordScore_Accuracy();					 //�^�ǲ�x���̭������T�v
        string  ReadRecordScore_Character();				 //�^�ǲ�x���̭�������W
        string  ReadRecordScore_Date();						 //�^�ǲ�x���̭������
        int		ReadRecordScore_TotalCorrectKeyCount();		 //�^�ǲ�x���̭������T����ƶq

    private:
        int HighScore_Score;				//�̰��������o��
        int HighScore_Level;				//�̰����������d
        double HighScore_Accuracy;			//�̰����������T�v
        string HighScore_Date;				//�̰����������
        string HighScore_MeName;			//�̰������D���W��(�^��)
        int HighScore_TotalCorrectKeyCount;	//�̰����������T����ƶq
        //===================
        vector<CRecord*> record;
        int record_Score;					//�o��
        int record_Level;					//���d
        double record_Accuracy;				//���T�v
        string record_Date;					//���
        string record_MeName;				//�D���W��(�^��)
        int	   record_SelectedChar;			//�D���s��
        int	   record_TotalCorrectKeyCount; //���T�ץ��
        char ChDate[80];					//char���A�����
};
}
