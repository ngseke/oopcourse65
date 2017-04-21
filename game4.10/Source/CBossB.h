#ifndef CBOSSB
#define CBOSSB
namespace game_framework {

class CBossB : public CBossA {
public:
	CBossB();
	CBossB(int x, int y, int delay, bool alive, CDict* d, int minVL, int maxVL, vector<CEnemy*>* enemyQueue, vector<CBomb*>* bombList);// (X�b, Y�b, ���ʳt��, �ͦ�, �r����, �̤p����, �̤j����, �ĤH����)

	void CallEnemy(int, int);		// BossA���ޯ�: �l��p��(3~4�r)
	void OnMove();
	void LoadBitmap();
};
}

#endif