#pragma once
#include "CMe.h"
namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// �o��class���ѷ|�@���u�����y
// �����N�i�H��g���ۤv���{���F
/////////////////////////////////////////////////////////////////////////////

class CRecord {
    public:
        CRecord(int score, int level, double accuracy, string meName, int selectedChar);//���� ���d ���T�v �D���W�� �D���s��
        void WriteRecord(int score, int level, double accuracy, string meName);			//�g�J:���� ���d ���T�v �D���W��
        int ReadRecordScore_Score();  //�^�ǲ�x���̭�����
        int ReadRecordScore_Level();  //�^�ǲ�x���̭�����
        double ReadRecordScore_Accuracy();  //�^�ǲ�x���̭������T�v
        string  ReadRecordScore_Character();  //�^�ǲ�x���̭�������W
        string  ReadRecordScore_Date();  //�^�ǲ�x���̭������
        int		ReadRecordScore_selectedChar();	//�^�ǲ�x���̭����D���s��
        //---------
        //void WriteCharacter(string);	//�W���諸����
        //string ReadCharacter();			//�^�ǤW���諸����
    private:
        int score;				//�o��
        int level;				//���d
        double accuracy;		//���T�v
        string date;			//���
        string meName;			//�D���W��(�^��)
        int	   selectedChar;	//�D���s��
        char chDate[80];		//char���A�����
        //string preCharac;		//�W�@���D�����W��

};
}
