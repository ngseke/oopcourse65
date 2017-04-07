namespace game_framework {

	class CBossA : public CEnemy {
	public:
		CBossA(int x, int y, int delay, bool alive, CDict* d, vector<CEnemy*>* enemyQueue);
		void CallEnemy();
		void LoadBitmap();
	private:
		vector<CEnemy*>* enemyQueue;
	};
}