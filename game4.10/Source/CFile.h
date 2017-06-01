#pragma once
#include "CRecord.h"
namespace game_framework {

class CFile {
    public:
        CFile();
        void WriteHighScore(int score, int level, double accuracy, string meName, int correctKeyCount);
        void ReadHighScoreFile();
        int ReadHighScore_Score();							 //�^�ǳ̰����̭�����
        int ReadHighScore_Level();  						 //�^�ǳ̰����̭�����
        double ReadHighScore_Accuracy();					 //�^�ǳ̰����̭������T�v
        string  ReadHighScore_Character();					 //�^�ǳ̰����̭�������W
        string  ReadHighScore_Date();						 //�^�ǳ̰����̭������
        int		ReadHighScore_CorrectKeyCount();			 //�^�ǳ̰����̭������T����ƶq �]����^
        int		ReadHighScore_TotalKeyCount();				 //�^���`����ơ]�|�ֿn�^
        bool isHighScoreExist();							 //�^�ǬO�_�s�b�̰�����
        //================================
        void WriteRecord(int score, int level, double accuracy, string meName, int correctKeyCount);			//�g�J:���� ���d ���T�v �D���W��
        void ReadRecordFile();
        int ReadRecord_Score(int);							 //�^�ǲ�x���̭�����
        int ReadRecord_Level(int);							 //�^�ǲ�x���̭�����
        double ReadRecord_Accuracy(int);					 //�^�ǲ�x���̭������T�v
        string  ReadRecord_Character(int);					 //�^�ǲ�x���̭�������W
        string  ReadRecord_Date(int);						 //�^�ǲ�x���̭������
        int		ReadRecord_CorrectKeyCount(int);			 //�^�ǲ�x���̭������T����ƶq
        int     GetRecordNum();								 //�^��record�j�p
        //================================
        void	WriteSelectedCharacter(string);				//�g�J�W�@���ҿ﨤��
        string  ReadSelectedCharacter();					//Ū���W�@���ҿ﨤��
        //================================
        void	WriteMusicOnOff(bool);
        bool	ReadMusicOnOff();

        //-----------
        void WriteTotalKeyCount(int);
        int ReadTotalKeyCount();
        //================================

        void DeleteAllData();					//�M���Ҧ��O��

    private:
        int HighScore_Score;				//�̰��������o��
        int HighScore_Level;				//�̰����������d
        double HighScore_Accuracy;			//�̰����������T�v
        string HighScore_Date;				//�̰����������
        string HighScore_MeName;			//�̰������D���W��(�^��)
        int HighScore_CorrectKeyCount;		//�̰����������T����ƶq
        int HighScore_TotalKeyCount;		//�`����ơ]�|�ֿn���^
        //===================
        vector<CRecord*> record;
        int record_Score;					//�o��
        int record_Level;					//���d
        double record_Accuracy;				//���T�v
        string record_Date;					//���
        string record_MeName;				//�D���W��(�^��)
        int	   record_SelectedChar;			//�D���s��
        int	   record_CorrectKeyCount;		//���T�����
        char ChDate[80];					//char���A�����
        //=====================
        string	preSelectedChar;			//�W�@������
        //================================
        bool  musicState;

};
}
