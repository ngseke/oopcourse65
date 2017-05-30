#pragma once
namespace game_framework {

class CBossA : public CEnemy {
    public:
        CBossA();
        CBossA(int x, int y, int delay, bool alive, CDict* d, int minVL, int maxVL, vector<CEnemy*>* enemyQueue, vector<CBomb*>* bombList, vector<CMovingBitmap*>* letter);// (X�b, Y�b, ���ʳt��, �ͦ�, �r����, �̤p����, �̤j����, �ĤH����)
        void CallEnemy(int, int);		// BossA���ޯ�: �l��p��(3~4�r)
        void kill();
        void OnMove();
        void LoadBitmap();
    protected:
        int callEnemyCounter, maxCallEnemyCounter;	// �o�ʥl��p�ǧޯ� �� �p�ƾ�
};
}