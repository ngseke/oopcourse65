#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CLevel.h"

namespace game_framework {
// CLevel: ���d�����ʵe

CLevel::CLevel() {
    delay_counter = 20000;	// �]�w���Ʀr���ʵe�����
    //delay_counter = 0;
    easeC = 0;
    level = 0;
    score = 87;
}
void CLevel::Play(int level, int score) {
    delay_counter = 0;
    easeC = 0;
    y = topPosY;
    this->level = level;
    this->score = score;
}
void CLevel::LoadBitmap() {
    char str[50];
    border.LoadBitmap("Bitmaps/level/level_border.bmp", RGB(0, 255, 0));
    bg.LoadBitmap("Bitmaps/level/level_bg.bmp", RGB(0, 255, 0));

    for (int i = 0; i < 10; i++) {		// ���J�Ʀr��
        sprintf(str, "Bitmaps/level/num/%d.bmp", i);
        numBmp[i].LoadBitmap(str, RGB(0, 255, 0));
        sprintf(str, "Bitmaps/level/num_s/%d.bmp", i);
        numBmpSmall[i].LoadBitmap(str, RGB(0, 255, 0));
    }

    topPosY = 0 - border.Height();
    btmPosY = SIZE_Y;
    centerPosX = (SIZE_X - border.Width()) / 2;
    centerPosY = (SIZE_Y - border.Height()) / 2 - 15;
    x = centerPosX;
    y = topPosY;
}

void CLevel::OnMove() {
    if (delay_counter < 30 * 10) delay_counter++;	// �ʵe�������delay_counter�|����W�[

    if (delay_counter < 30 * 3) {					// �ƤJ����
        if (y < centerPosY) {
            easeC += .60;
            y += 20 - int(easeC)  ;
        }
        else  easeC = 0;
    }
    else if (delay_counter < 30 * 5) {				// �ƥX����
        if (y < btmPosY) {
            easeC += .7;
            y += 0 + int(easeC);
        }
        else  easeC = 0;
    }
    else if (delay_counter < 30 * 10) {				// �����
        //delay_counter = 0;
        easeC = 0;
        y = topPosY;
    }
}

void CLevel::OnShow() {
    bg.SetTopLeft(x, y);
    bg.ShowBitmap();
    border.SetTopLeft(x, y);
    border.ShowBitmap();
    int tempScore = score, tempLevel = level;

    for (int i = 0; i < 5; i++) {		// ��ܤ��ƼƦrbmp
        numBmpSmall[tempScore % 10].SetTopLeft(x + 127 - 10 * i, y + 76);
        numBmpSmall[tempScore % 10].ShowBitmap();
        tempScore /= 10;
    }

    for (int i = 0; i < 2; i++) {		// ������d�Ʀrbmp
        numBmp[tempLevel % 10].SetTopLeft(x + 138 - 20 * i, y + 10 );
        numBmp[tempLevel % 10].ShowBitmap();
        tempLevel /=  10;
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
    return x + border.Width();
}
int CLevel::GetY2() {
    return y + border.Height();
}
int CLevel::GetWidth() {
    return border.Width();
}
int CLevel::GetHeight() {
    return border.Height();
}

}