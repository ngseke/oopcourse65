#pragma once
#include "CCharacter.h"


namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// ����Ҧ����D��CHARACTER
/////////////////////////////////////////////////////////////////////////////


class CMe {
    public:
        CMe();
        ~CMe();
        void LoadBitmap();				// ���J�ϧ�
        void OnMove();					// ���ʹϧ�
        void OnShow();					// �N�ϧιϧζK��e��
        int  GetX1();					// ME���W�� x �y��
        int  GetY1();					// ME���W�� y �y��
        int  GetX2();					// ME�k�U�� x �y��
        int  GetY2();					// ME�k�U�� y �y��
        void SetXY(int nx, int ny);		// �]�w���W���y��
        void LoadCharacter();			// ���J����
        void AddSelectedChar(int);		// ��ܨ���ɲ��ʴ��
        void SetState(int);				// �]�w��e��ܪ����A

        int GetselectedChar();			// ���o�襤������s��
        void SetselectedChar(int);		// �]�w�襤������s��
        string GetMeName();				// ���o�襤������W
        void SetSelectedChar(string);	// �]�w�C������ܪ�����
        //
        void SetHighScoreDisplay(string);						// �]�w �i�̰��O���j�������������
        void SetPlayingRecordDisplay(string, string, string);	// �]�w �i�C���O���j������������ܡ�3
        //
        void WriteUnlockCharacter(string);		// �g�J�w���ꪺ����
        void ReadUnlockCharacter();				// Ū�w���ꪺ����
        bool JudgeUnlock(int, int, int, int);	// �a�J�����C�������Z, �d�ݬO�_�ŦX�������, �æ^�Ǥ@��bool
        bool GetSelectedCharIsUnlock();			// �^�ǿ�w������O�_�w����

    protected:
        vector<CCharacter*> character;
        CFile*				file;
        string highScoreName, playingRecordName[3];
        CMovingBitmap		unlockSign, unlock_border;

        const int CHARACTER_POS_Y;
        int x, y;						// �ϧΥ��W���y��
        int  selectedChar;				// �襤������s��
        int  currState;					// 0: �C����, 1: ��ܨ���e��, 2: GAMEOVER�e��, 3: �̰��O���e��, 4: �C���O���e��
        int highScoreCharNum, playingRecordCharNum[3];
};
}
