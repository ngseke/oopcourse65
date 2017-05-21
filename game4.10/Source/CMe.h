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
        void SetState(int);				// �]�w��e��ܪ���

    protected:
        const int CHARACTER_POS_Y;
        int x, y;						// �ϧΥ��W���y��
        vector<CCharacter*> character;
        int  selectedChar;				// �襤������s��
        int  currState;					// 0: �C����, 1: ��ܨ���e��, 2: GAMEOVER�e��
};
}
