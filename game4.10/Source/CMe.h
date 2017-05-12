#pragma once
#include "CCharacter.h"

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// ����Ҧ����D��CHARACTER
/////////////////////////////////////////////////////////////////////////////
class CMe {
    public:
        CMe();
        void Initialize();				// �]�w��l��
        void LoadBitmap();				// ���J�ϧ�
        void OnMove();					// ���ʹϧ�
        void OnShow();					// �N�ϧιϧζK��e��
        int  GetX1();					// ME���W�� x �y��
        int  GetY1();					// ME���W�� y �y��
        int  GetX2();					// ME�k�U�� x �y��
        int  GetY2();					// ME�k�U�� y �y��
        void SetXY(int nx, int ny);		// �]�w���l���W���y��
        void LoadCharacter();

    protected:
        CAnimation animation;			// �ϧΪ��ʵe
        int x, y;						// �ϧΥ��W���y��
        vector<CCharacter*> character;
};
}
