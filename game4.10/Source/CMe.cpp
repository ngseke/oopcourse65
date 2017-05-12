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
    //char* file[2] = { "Bitmaps/me_ironman.bmp", "Bitmaps/me_ironman1.bmp" }
}

int CMe::GetX1() {
    return x;
    //return character[0]->GetX1();
}

int CMe::GetY1() {
    return y;
    // return character[0]->GetY1();
}

int CMe::GetX2() {
    return x + animation.Width();
    // return character[0]->GetX2();
}

int CMe::GetY2() {
    return y + animation.Height();
    // return character[0]->GetY2();
}

void CMe::Initialize() {
    const int X_POS = ( SIZE_X - 24) / 2;
    const int Y_POS = SIZE_Y - 50;
    x = X_POS;
    y = Y_POS;
}

void CMe::LoadBitmap() {
    int character = 2;

    if (character == 0) {
        animation.AddBitmap("Bitmaps/me_ironman.bmp", RGB(255, 255, 255));
        animation.AddBitmap("Bitmaps/me_ironman1.bmp", RGB(255, 255, 255));
        animation.SetTopLeft(x, y);
    }
    else if (character == 1) {
        animation.AddBitmap("Bitmaps/me_hulk.bmp", RGB(255, 255, 255));
        animation.SetTopLeft(x - 4, y - 2);
    }
    else if (character == 2) {
        animation.AddBitmap("Bitmaps/me_captian_american.bmp", RGB(0, 255, 0));
        animation.SetTopLeft(x, y);
    }

    LoadCharacter();
}

void CMe::LoadCharacter() {
    string file[2] = { "Bitmaps/me_ironman.bmp", "Bitmaps/me_ironman1.bmp" };
    character.push_back(new CCharacter("Iron Man", file, 2, 100, 100));
    //character.back()->LoadBitmap();
}

void CMe::OnMove() {
    //animation.OnMove();
    character[0]->OnMove();
}

void CMe::OnShow() {
    //animation.OnShow();
    character[0]->OnShow();
}

void CMe::SetXY(int nx, int ny) {
    x = nx;
    y = ny;
}



}