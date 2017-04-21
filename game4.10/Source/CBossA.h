#ifndef CBOSSA
#define CBOSSA
namespace game_framework {

class CBossA : public CEnemy {
    public:
        CBossA();
        CBossA(int x, int y, int delay, bool alive, CDict* d, int minVL, int maxVL, vector<CEnemy*>* enemyQueue, vector<CBomb*>* bombList);// (X�b, Y�b, ���ʳt��, �ͦ�, �r����, �̤p����, �̤j����, �ĤH����)

        void CallEnemy(int, int);		// BossA���ޯ�: �l��p��(3~4�r)
        void kill();
        void OnMove();
        void LoadBitmap();
    protected:
        vector<CEnemy*>* enemyQueue;
        int callEnemyCounter, maxCallEnemyCounter;
};
}

#endif