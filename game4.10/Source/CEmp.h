#pragma once

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// �o��class���ѥi�H����L�ηƹ�������l
// �����N�i�H��g���ۤv���{���F
/////////////////////////////////////////////////////////////////////////////
class CEmp {
    public:
        CEmp();
        int  GetX1();					// ���l���W�� x �y��
        int  GetY1();					// ���l���W�� y �y��
        int  GetX2();					// ���l�k�U�� x �y��
        int  GetY2();					// ���l�k�U�� y �y��
        void Initialize();				// �]�w���l����l��
        void LoadBitmap();				// ���J�ϧ�
        void OnMove();					// �������l
        void OnShow();					// �N���l�ϧζK��e��
        void SetXY(int nx, int ny);		// �]�w���l���W���y��
        void SetEQ(vector<CEnemy*>* enemyQueue, CInteger* score, bool* lock, CEnemy* targetEnemy);
        void CallEmp();
        //bool HitMe(vector<CEnemy*>* enemyQueue);
    protected:
        CAnimation emp;				// �����i
        vector<CEnemy*>* enemyQueue;
        bool state;
        int x, y;					// ���l���W���y��
        //
        CInteger* score;
        bool* lock;
        CEnemy* targetEnemy;
    private:
        bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
};
}
