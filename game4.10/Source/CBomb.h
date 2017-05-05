#pragma once

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// �o��class���ѷ|�@���u�����y
// �����N�i�H��g���ۤv���{���F
/////////////////////////////////////////////////////////////////////////////

class CBomb {
    public:
        CBomb();
        CBomb(int x, int y);
        void LoadBitmap();		// ���J�ϧ�
        void OnMove();			// ����
        void OnShow();			// �N�ϧζK��e��
        void SetXY(int, int);
        bool IsAlive();
    protected:
        int x, y;				// �ϧήy��
        CAnimation animation;	// �Q�ΰʵe�@�ϧ�
        ///
        bool is_alive;				// �O�_����

};
}
