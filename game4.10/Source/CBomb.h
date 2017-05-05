#pragma once

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// 這個class提供會一直彈跳的球
// 看懂就可以改寫成自己的程式了
/////////////////////////////////////////////////////////////////////////////

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
        CAnimation animation;	// 利用動畫作圖形
        ///
        bool is_alive;				// 是否活著

};
}
