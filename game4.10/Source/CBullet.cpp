#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CBullet.h"

namespace game_framework {
// CBullet: �l�u
CBullet::CBullet(int endX, int endY) {
    /////
    delay = delay_counter = 0;
    dx = dy = index = 0;
    is_alive = 1;
    this->endX = endX;
    this->endY = endY;
    /////
    x = SIZE_X / 2;				// �Ȯɳ]����������
    y = SIZE_Y - 10;
    LoadBitmap();
}

void CBullet::LoadBitmap() {
    char* filename[2] = { ".\\bitmaps\\bullet1.bmp", ".\\bitmaps\\bullet2.bmp"};

    for (int i = 0; i < 2; i++)
        animation.AddBitmap(filename[i], RGB(0, 255, 0));
}

void CBullet::OnMove() {
    delay_counter--;
    animation.OnMove();		// ����@��animation.OnMove()�Aanimation�~�|����
    const int STEPS = 10;

    if (delay_counter <= 0 && is_alive) {
        index++;

        if (index  >= STEPS ) {
            is_alive = 0;
        }

        dx = -(x - endX) / STEPS * index;
        dy = -(y - endY) / STEPS * index;
        delay_counter = delay;		// �p�ƾ��k�^���
    }
}

void CBullet::OnShow() {
    if (is_alive) {
        animation.SetTopLeft(x + dx, y + dy);
        animation.OnShow();
    }
}

void CBullet::SetXY(int x, int y) {
    this->x = x;
    this->y = y;
}

void CBullet::SetEndXY(int x, int y) {
    this->endX = x;
    this->endY = y;
}
bool CBullet::IsAlive() {
    return is_alive;
}
}
