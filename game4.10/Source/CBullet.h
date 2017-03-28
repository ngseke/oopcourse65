namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// �o��class���ѷ|�@���u�����y
// �����N�i�H��g���ۤv���{���F
/////////////////////////////////////////////////////////////////////////////

class CBullet {
    public:
        CBullet();
        void LoadBitmap();			// ���J�ϧ�
        void OnMove();				// ����
        void OnShow();				// �N�ϧζK��e��
        void SetFloor(int);
        void SetXY(int, int);
        void SetVelocity(int);
        void SetEndXY(int, int);
    private:
        int x, y;					// �ϧήy��  where ZHUJIAO is
        int floor;					// �a�O��Y�y��
        bool rising;				// true��W�ɡBfalse��U��
        int initial_velocity;		// ��l�t��
        int velocity;				// �ثe���t��(�I/��)
        CAnimation animation;		// �Q�ΰʵe�@�ϧ�
        int endX, endY;             // where X & Y should end, it's the position of the enemy.
        int dX, dY;
        int delay, delay_counter;   //delay_counter=delay; ;delay--
};
}
