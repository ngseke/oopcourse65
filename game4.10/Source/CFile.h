#pragma once
#include "CRecord.h"
namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// �o��class���ѷ|�@���u�����y
// �����N�i�H��g���ۤv���{���F
/////////////////////////////////////////////////////////////////////////////

class CFile {
    public:
        CFile();
        void WriteHighScore(int score, int level, double accuracy, string meName);
        void ReadHighScoreFile();
        int ReadHighScore_Score();  //�^�ǳ̰����̭�����
        int ReadHighScore_Level();  //�^�ǳ̰����̭�����
        double ReadHighScore_Accuracy();  //�^�ǳ̰����̭������T�v
        string  ReadHighScore_Character();  //�^�ǳ̰����̭�������W
        string  ReadHighScore_Date();  //�^�ǳ̰����̭������
        bool isHighScoreExist();		//�^�ǬO�_�s�b�̰�����
        //================================
        void WriteRecord(int score, int level, double accuracy, string meName);			//�g�J:���� ���d ���T�v �D���W��
        int ReadRecordScore_Score();  //�^�ǲ�x���̭�����
        int ReadRecordScore_Level();  //�^�ǲ�x���̭�����
        double ReadRecordScore_Accuracy();  //�^�ǲ�x���̭������T�v
        string  ReadRecordScore_Character();  //�^�ǲ�x���̭�������W
        string  ReadRecordScore_Date();  //�^�ǲ�x���̭������

    private:
        int HighScore_Score;
        int HighScore_Level;
        double HighScore_Accuracy;
        string HighScore_Date;
        string HighScore_MeName;
        //===================
        vector<CRecord*> record;
        int record_Score;				//�o��
        int record_Level;				//���d
        double record_Accuracy;		//���T�v
        string record_Date;			//���
        string record_MeName;			//�D���W��(�^��)
        int	   record_SelectedChar;	//�D���s��
        char ChDate[80];		//char���A�����
};
}
