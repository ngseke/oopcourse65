#pragma once

namespace game_framework {
// CBomb：敵人死亡後會的爆炸動畫
class CBomb {
    public:
        CBomb();
        CBomb(int x, int y);
        void LoadBitmap();		// 載入圖形
        void OnMove();			// 移動
        void OnShow();			// 將圖形貼到畫面
        void SetXY(int, int);
        bool IsAlive();
    protected:
        int x, y;				// 圖形座標
        CAnimation animation;	// 爆炸動畫
        bool is_alive;			// 是否活著

};
}
