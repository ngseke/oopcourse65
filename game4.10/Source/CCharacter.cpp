#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CCharacter.h"

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// CCharacter: 主角
/////////////////////////////////////////////////////////////////////////////

CCharacter::CCharacter(string name, string subName, string fn, int bmpNum, int x, int y) {
    this->name = name;
    this->subName = subName;
    this->bmpNum = bmpNum;
    this->x = x;
    this->y = y;
    this->fileName = fn;
    LoadBitmap();
}

void CCharacter::LoadBitmap() {
    char str[50];

    if (bmpNum == 1) {
        sprintf(str, "Bitmaps/me/%s.bmp", fileName.c_str());
        animation.AddBitmap(str, RGB(0, 255, 0));
    }
    else {
        for (int i = 0; i < bmpNum; i++) {		// 載入動畫
            sprintf(str, "Bitmaps/me/%s%d.bmp", fileName.c_str(), i + 1);
            animation.AddBitmap(str, RGB(0, 255, 0));
        }
    }

    if (name == "Tsai Ing-wen")animation.SetDelayCount(5);

    if (name == "Kirby")animation.SetDelayCount(3);
}

void CCharacter::OnMove() {
    animation.OnMove();
}

void CCharacter::OnShow() {
    animation.SetTopLeft(x, y);
    animation.OnShow();
}

void CCharacter::SetXY(int x, int y) {
    this->x = x;
    this->y = y;
}
int CCharacter::GetX1() {
    return x;
}
int CCharacter::GetY1() {
    return y;
}
int CCharacter::GetX2() {
    return x + animation.Width();
}
int CCharacter::GetY2() {
    return y + animation.Height();
}
int CCharacter::GetWidth() {
    return animation.Width();
}
int CCharacter::GetHeight() {
    return animation.Height();
}
string CCharacter::GetName() {
    return name;
}
string CCharacter::GetSubName() {
    return subName;
}
}