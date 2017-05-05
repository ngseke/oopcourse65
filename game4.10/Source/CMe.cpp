#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CEnemy.h"
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
    const int X_POS = ( SIZE_X - 24) / 2;
    const int Y_POS = SIZE_Y - 50;
    x = X_POS;
    y = Y_POS;
}

void CMe::LoadBitmap() {
    animation.AddBitmap("Bitmaps/me_ironman.bmp", RGB(255, 255, 255));

    for (int i = 0; i < 5; i++) {
        char str[40];
        sprintf(str, "Bitmaps/big_wave/big_wave%d.bmp", i + 1);
        emp.AddBitmap(str, RGB(0, 255, 0));
    }
}

void CMe::OnMove() {
    emp.OnMove();
    emp.GetCurrentBitmapNumber();
}


void CMe::SetXY(int nx, int ny) {
    x = nx;
    y = ny;
}

void CMe::OnShow() {
    emp.SetTopLeft((SIZE_X - 640) / 2, (SIZE_Y - 350));
    emp.OnShow();
    animation.SetTopLeft(x, y);
    animation.OnShow();
}




}