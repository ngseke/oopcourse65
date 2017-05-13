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
}
CLevel::CLevel(string name, string subName, string fn, int bmpNum, int x, int y) {
}

void CLevel::LoadBitmap() {
    char str[50];

    for (int i = 0; i < 1; i++) {		// 更笆礶
        sprintf(str, "Bitmaps/level/level_border.bmp");
        animation.AddBitmap(str, RGB(0, 255, 0));
    }

    for (int i = 0; i < 10; i++) {		// 更计瓜
        sprintf(str, "Bitmaps/level/num/%d.bmp", i);
        numBmp[i].LoadBitmap(str, RGB(0, 255, 0));
    }

    x = (SIZE_X - animation.Width()) / 2;
    y = (SIZE_Y - animation.Height()) / 2;
    /*
    if (bmpNum == 1) {
        sprintf(str, "Bitmaps/me/%s", fileName.c_str());
        animation.AddBitmap(str, RGB(0, 255, 0));
    }
    else {
        for (int i = 0; i < bmpNum; i++) {		// 更笆礶
            sprintf(str, "Bitmaps/me/%s%d.bmp", fileName.c_str(), i + 1);
            animation.AddBitmap(str, RGB(0, 255, 0));
        }
    }
    */
}

void CLevel::OnMove() {
    animation.OnMove();
}

void CLevel::OnShow() {
    animation.SetTopLeft(x, y);
    animation.OnShow();

    for (int i = 0; i < 10; i++) {		// 更计瓜
        numBmp[i].SetTopLeft(x + 20 * i, y + animation.Height());
        numBmp[i].ShowBitmap();
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