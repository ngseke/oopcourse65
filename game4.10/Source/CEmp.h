#pragma once

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// 這個class提供可以用鍵盤或滑鼠控制的擦子
// 看懂就可以改寫成自己的程式了
/////////////////////////////////////////////////////////////////////////////
class CEmp {
    public:
        CEmp();
        int  GetX1();					// 擦子左上角 x 座標
        int  GetY1();					// 擦子左上角 y 座標
        int  GetX2();					// 擦子右下角 x 座標
        int  GetY2();					// 擦子右下角 y 座標
        void Initialize();				// 設定擦子為初始值
        void LoadBitmap();				// 載入圖形
        void OnMove();					// 移動擦子
        void OnShow();					// 將擦子圖形貼到畫面
        void SetXY(int nx, int ny);		// 設定擦子左上角座標
        void SetEQ(vector<CEnemy*>* enemyQueue, CInteger* score, bool* lock, CEnemy* targetEnemy);
        void CallEmp();
        //bool HitMe(vector<CEnemy*>* enemyQueue);
    protected:
        CAnimation emp;				// 衝擊波
        vector<CEnemy*>* enemyQueue;
        bool state;
        int x, y;					// 擦子左上角座標
        //
        CInteger* score;
        bool* lock;
        CEnemy* targetEnemy;
    private:
        bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
};
}
