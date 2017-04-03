#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CMe.h"

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// CMe: еDид
/////////////////////////////////////////////////////////////////////////////

CMe::CMe() {
    Initialize();
}

int CMe::GetX1() {
    return x;
}

int CMe::GetY1() {
    return y;
}

int CMe::GetX2() {
    return x + animation.Width();
}

int CMe::GetY2() {
    return y + animation.Height();
}

void CMe::Initialize() {
    const int X_POS = ( SIZE_X - 16) / 2;
    const int Y_POS = SIZE_Y - 50;
    x = X_POS;
    y = Y_POS;
    isMovingLeft = isMovingRight = isMovingUp = isMovingDown = false;
}

void CMe::LoadBitmap() {
    animation.AddBitmap("Bitmaps/me_cow.bmp", RGB(0, 255, 0));
}

void CMe::OnMove() {
}


void CMe::SetXY(int nx, int ny) {
    x = nx;
    y = ny;
}

void CMe::OnShow() {
    animation.SetTopLeft(x, y);
    animation.OnShow();
}
}