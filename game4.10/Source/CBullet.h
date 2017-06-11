#pragma once
namespace game_framework {
// CBullet:	�D�������ɵo�g���l�u

class CBullet {
    public:
        CBullet(int endX, int endY);
        void LoadBitmap();			// ���J�ϧ�
        void OnMove();				// ����
        void OnShow();				// �N�ϧζK��e��
        void SetXY(int, int);
        void SetEndXY(int, int);
        bool IsAlive();
    private:
        int x, y;					// �ϧήy��  where ZHUJIAO is
        CAnimation animation;		// �Q�ΰʵe�@�ϧ�
        int endX, endY;             // where X & Y should end, it's the position of the enemy.
        int dx, dy;					// �첾�q
        int delay, delay_counter, index;   //delay_counter=delay; ;delay--
        bool is_alive;
};
}
