namespace game_framework {

class CBossA : public CEnemy {
    public:
        CBossA(int x, int y, int delay, bool alive, CDict* d, int minVL, int maxVL, vector<CEnemy*>* enemyQueue);// (X�b, Y�b, ���ʳt��, �ͦ�, �r����, �̤p����, �̤j����, �ĤH����)
        void CallEnemy(int, int );		// BossA���ޯ�: �l��p��(3~4�r)
        void OnMove();
        void LoadBitmap();
    private:
        vector<CEnemy*>* enemyQueue;
        int callEnemyCounter, maxCallEnemyCounter;
};
}