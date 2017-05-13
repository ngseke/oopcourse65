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
        CAnimation			animation;							// �D���ʵe
        CMovingBitmap		numBmp[10], numBmpSmall[10];		// �Ʀr����
        int					x, y;
        int					level, score;
        int					delay_counter;
        double				easeC;
        int					topPosY, btmPosY;
        int					centerPosY, centerPosX;
};
}
