#pragma once

namespace game_framework {

class CLevel {
    public:
        CLevel();
        CLevel(string name, string subName, string fileName, int bmpNum, int, int);
        void LoadBitmap();
        void OnMove();
        void OnShow();
        //
        void SetXY(int, int);
        int  GetX1(), GetY1(), GetX2(), GetY2();
        int  GetWidth(), GetHeight();

    protected:
        CAnimation			animation;							// 主角動畫
        CMovingBitmap		numBmp[10];
        int					bmpNum;								// 圖檔數量
        int					x, y;
};
}
