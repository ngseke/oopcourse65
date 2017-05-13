#pragma once

namespace game_framework {

class CLevel {
    public:
        CLevel();

        void LoadBitmap();
        void OnMove();
        void OnShow();
        //
        void SetXY(int, int);
        int  GetX1(), GetY1(), GetX2(), GetY2();
        int  GetWidth(), GetHeight();
        void Play(int level, int score);

    protected:
        CAnimation			animation;							// 主角動畫
        CMovingBitmap		numBmp[10], numBmpSmall[10];		// 數字圖檔
        int					x, y;
        int					level, score;
        int					delay_counter;
        double				easeC;
        int					topPosY, btmPosY;
        int					centerPosY, centerPosX;
};
}
