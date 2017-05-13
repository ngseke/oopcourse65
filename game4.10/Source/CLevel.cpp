#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CLevel.h"

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// CLevel: Enemy
/////////////////////////////////////////////////////////////////////////////
CLevel::CLevel() {
    delay_counter = 200000;
    easeC = 0;
    level = 0;
    score = 87;
}
void CLevel::Play(int level, int score) {
    delay_counter = 0;
    this->level = level;
    this->score = score;
}
void CLevel::LoadBitmap() {
    char str[50];

    for (int i = 0; i < 1; i++) {		// 載入動畫
        sprintf(str, "Bitmaps/level/level_border.bmp");
        animation.AddBitmap(str, RGB(0, 255, 0));
    }

    for (int i = 0; i < 10; i++) {		// 載入數字圖
        sprintf(str, "Bitmaps/level/num/%d.bmp", i);
        numBmp[i].LoadBitmap(str, RGB(0, 255, 0));
        sprintf(str, "Bitmaps/level/num_s/%d.bmp", i);
        numBmpSmall[i].LoadBitmap(str, RGB(0, 255, 0));
    }

    topPosY = 0 - animation.Height();
    btmPosY = SIZE_Y;
    centerPosX = (SIZE_X - animation.Width()) / 2;
    centerPosY = (SIZE_Y - animation.Height()) / 2 - 15;
    x = centerPosX;
    y = topPosY;
}

void CLevel::OnMove() {
    animation.OnMove();

    if (delay_counter < 30 * 10) delay_counter++;

    if (delay_counter < 30 * 3) {
        if (y < centerPosY) {
            easeC -= .5;
            y += 18 + int(easeC) ;
        }
        else  easeC = 0;
    }
    else if (delay_counter < 30 * 5) {
        if (y < btmPosY) {
            easeC += .6;
            y += 0 + int(easeC);
        }
        else  easeC = 0;
    }
    else if (delay_counter < 30 * 10) {
        //delay_counter = 0;
        easeC = 0;
        y = topPosY;
    }
}

void CLevel::OnShow() {
    animation.SetTopLeft(x, y);
    animation.OnShow();
    int tempScore = score, tempLevel = level;

    for (int i = 0; i < 5; i++) {
        numBmpSmall[tempScore % 10].SetTopLeft(x + 97 - 10 * i, y + 66);
        numBmpSmall[tempScore % 10].ShowBitmap();
        tempScore /= 10;
    }

    for (int i = 0; i < 2; i++) {
        numBmp[tempLevel % 10].SetTopLeft(x + 108 - 20 * i, y );
        numBmp[tempLevel % 10].ShowBitmap();
        tempLevel /=  10;
    }

    for (int i = 0; i < 10; i++) {
        numBmp[i].SetTopLeft(x + 20 * i, y + animation.Height());
        //numBmp[i].ShowBitmap();
        numBmpSmall[i].SetTopLeft(x + 20 * i, y + animation.Height() + 24);
        //numBmpSmall[i].ShowBitmap();
    }
}

void CLevel::SetXY(int x, int y) {
    this->x = x;
    this->y = y;
}
int CLevel::GetX1() {
    return x;
}
int CLevel::GetY1() {
    return y;
}
int CLevel::GetX2() {
    return x + animation.Width();
}
int CLevel::GetY2() {
    return y + animation.Height();
}
int CLevel::GetWidth() {
    return animation.Width();
}
int CLevel::GetHeight() {
    return animation.Height();
}

}