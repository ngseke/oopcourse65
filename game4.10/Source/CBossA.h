namespace game_framework {

class CBossA : public CEnemy {
    public:
        CBossA(int x, int y, int delay, bool alive, CDict* d, int minVL, int maxVL, vector<CEnemy*>* enemyQueue);
        void CallEnemy(int, int );		// BossA的技能: 召喚小怪(3~4字)
        void OnMove();
        void LoadBitmap();
    private:
        vector<CEnemy*>* enemyQueue;
        int callEnemyCounter, maxCallEnemyCounter;
};
}