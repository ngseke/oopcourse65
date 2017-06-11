#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CCharacter.h"

namespace game_framework {
// CCharacter: 主角

CCharacter::CCharacter(string name, string subName, string fn, int bmpNum, int ur0, int ur1, int ur2, int ur3) {
    this->name = name;
    this->subName = subName;
    this->bmpNum = bmpNum;
    this->x = 0;
    this->y = 0;
    this->fileName = fn;
    this->isUnlock = false;
    unlockRequirement[0] = ur0;
    unlockRequirement[1] = ur1;
    unlockRequirement[2] = ur2;
    unlockRequirement[3] = ur3;
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
int CCharacter::GetUnlockRequirement(int num) {
    return num < 4 ? unlockRequirement[num] : 0;
}

void CCharacter::SetIsUnlock(bool isUnlock) {
    this->isUnlock = isUnlock;
}
bool CCharacter::GetIsUnlock() {
    return isUnlock;
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