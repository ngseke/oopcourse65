#ifndef CBOSSA
#define CBOSSA
namespace game_framework {

class CBossA : public CEnemy {
    public:
        CBossA();
        CBossA(int x, int y, int delay, bool alive, CDict* d, int minVL, int maxVL, vector<CEnemy*>* enemyQueue, vector<CBomb*>* bombList);// (X軸, Y軸, 移動速度, 生死, 字典檔, 最小長度, 最大長度, 敵人隊伍)

        void CallEnemy(int, int);		// BossA的技能: 召喚小怪(3~4字)
        void kill();
        void OnMove();
        void LoadBitmap();
    protected:
        vector<CEnemy*>* enemyQueue;
        int callEnemyCounter, maxCallEnemyCounter;
};
}

#endif