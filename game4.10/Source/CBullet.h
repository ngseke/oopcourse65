#pragma once
namespace game_framework {
// CBullet:	主角攻擊時發射的子彈

class CBullet {
    public:
        CBullet(int endX, int endY);
        void LoadBitmap();			// 載入圖形
        void OnMove();				// 移動
        void OnShow();				// 將圖形貼到畫面
        void SetXY(int, int);
        void SetEndXY(int, int);
        bool IsAlive();
    private:
        int x, y;					// 圖形座標  where ZHUJIAO is
        CAnimation animation;		// 利用動畫作圖形
        int endX, endY;             // where X & Y should end, it's the position of the enemy.
        int dx, dy;					// 位移量
        int delay, delay_counter, index;   //delay_counter=delay; ;delay--
        bool is_alive;
};
}
