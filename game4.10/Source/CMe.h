#pragma once

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// �o��class���ѥi�H����L�ηƹ�������l
// �����N�i�H��g���ۤv���{���F
/////////////////////////////////////////////////////////////////////////////
class CMe {
    public:
        CMe();
        int  GetX1();					// ���l���W�� x �y��
        int  GetY1();					// ���l���W�� y �y��
        int  GetX2();					// ���l�k�U�� x �y��
        int  GetY2();					// ���l�k�U�� y �y��
        void Initialize();				// �]�w���l����l��
        void LoadBitmap();				// ���J�ϧ�
        void OnMove();					// �������l
        void OnShow();					// �N���l�ϧζK��e��
        void SetXY(int nx, int ny);		// �]�w���l���W���y��

    protected:
        CAnimation animation;		// ���l���ʵe
        int x, y;					// ���l���W���y��
};
}
