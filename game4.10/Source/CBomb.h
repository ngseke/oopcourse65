#pragma once

namespace game_framework {
// CBomb�G�ĤH���`��|���z���ʵe
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
        CAnimation animation;	// �z���ʵe
        bool is_alive;			// �O�_����

};
}
