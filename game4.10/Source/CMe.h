#ifndef CME
#define CME
namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// 這個class提供可以用鍵盤或滑鼠控制的擦子
// 看懂就可以改寫成自己的程式了
/////////////////////////////////////////////////////////////////////////////

class CMe {
    public:
        CMe();
		//CMe(vector<CEnemy*>* enemyQueue);
        int  GetX1();					// 擦子左上角 x 座標
        int  GetY1();					// 擦子左上角 y 座標
        int  GetX2();					// 擦子右下角 x 座標
        int  GetY2();					// 擦子右下角 y 座標
        void Initialize();				// 設定擦子為初始值
        void LoadBitmap();				// 載入圖形
        void OnMove();					// 移動擦子
        void OnShow();					// 將擦子圖形貼到畫面
        void SetXY(int nx, int ny);		// 設定擦子左上角座標

    protected:
        CAnimation animation;		// 擦子的動畫
		CAnimation emp;				// 衝擊波
        int x, y;					// 擦子左上角座標
};
}
#endif