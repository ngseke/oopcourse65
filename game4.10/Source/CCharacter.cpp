#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CCharacter.h"

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// CCharacter: Enemy
/////////////////////////////////////////////////////////////////////////////

CCharacter::CCharacter(string name, string pathName[10], int bmpNum, int x, int y) {
    this->name = name;
    this->bmpNum = bmpNum;
    this->x = x;
    this->y = y;

    for (int i = 0; i < bmpNum; i++)
        this->pathName[i] = pathName[i];

    LoadBitmap();
}

void CCharacter::LoadBitmap() {
    for (int i = 0; i < bmpNum; i++) {		// ¸ü¤J°Êµe
        char str[50];
        sprintf(str, "%s", pathName[i].c_str());
        animation.AddBitmap(str, RGB(0, 255, 0) );
    }
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
}