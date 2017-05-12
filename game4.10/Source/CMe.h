#pragma once
#include "CCharacter.h"

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// 控制所有的主角CHARACTER
/////////////////////////////////////////////////////////////////////////////
class CMe {
    public:
        CMe();
        void Initialize();				// 設定初始值
        void LoadBitmap();				// 載入圖形
        void OnMove();					// 移動圖形
        void OnShow();					// 將圖形圖形貼到畫面
        int  GetX1();					// ME左上角 x 座標
        int  GetY1();					// ME左上角 y 座標
        int  GetX2();					// ME右下角 x 座標
        int  GetY2();					// ME右下角 y 座標
        void SetXY(int nx, int ny);		// 設定擦子左上角座標
        void LoadCharacter();

    protected:
        CAnimation animation;			// 圖形的動畫
        int x, y;						// 圖形左上角座標
        vector<CCharacter*> character;
};
}
