#pragma once

namespace game_framework {

enum AUDIO_EMP_ID {
    AUDIO_EMP
};

class CEmp {
    public:
        CEmp();
        int  GetX1();					// ���W�� x �y��
        int  GetY1();					// ���W�� y �y��
        int  GetX2();					// �k�U�� x �y��
        int  GetY2();					// �k�U�� y �y��
        void Initialize();				// �]�w��l��
        void LoadBitmap();				// ���J�ϧ�
        void OnMove();
        void OnShow();
        void SetXY(int nx, int ny);		// �]�w���W���y��
        void SetEQ(vector<CEnemy*>* enemyQueue, CInteger* score, bool* lock, CEnemy** targetEnemy);
        void CallEmp(bool);
        void SetEmpTimes(int);			// �]�w�q�Ϫi�i�ϥΪ�����
        int GetEmpTimes();
    protected:
        CMovingBitmap		displayBG, displayNumber[10];
        CAnimation			emp;				// �����i
        vector<CEnemy*>*	enemyQueue;			// [����] �x�s�Ҧ��ĤH��Vector
        CInteger*			score;				// [����] ����
        bool*				lock;				// [����] �O�_�w��w�ĤH
        CEnemy**			targetEnemy;		// [����] �Q��w���ĤH
        bool				state;				// �O�_�B�� �ޯ༽��
        int					x, y;				// ���W���y��
        int					empTimes;			// �Ѿl�i�l�ꪺEMP����
    private:
        bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��

};
}
