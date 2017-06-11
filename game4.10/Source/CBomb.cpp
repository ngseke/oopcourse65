#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CBomb.h"

namespace game_framework {
// CBomb: 爆炸動畫

CBomb::CBomb() {
}

CBomb::CBomb(int x, int y) {
    animation.SetDelayCount(1);
    this->x = x;
    this->y = y;
    is_alive = true;
}

void CBomb::LoadBitmap() {
    for (int i = 0; i < 16; i++) {
        char str[30];
        sprintf(str, "Bitmaps/bomb/bomb%d.bmp", i + 1);
        animation.AddBitmap(str, RGB(255, 0, 255));
    }
}

void CBomb::OnMove() {
    if (!is_alive) return;

    animation.OnMove();		// 執行一次animation.OnMove()，animation才會換圖

    if (animation.GetCurrentBitmapNumber() >= 15) is_alive = false;
}

void CBomb::OnShow() {
    if (!is_alive) return;	// 若is_alive為false則不執行OnShow

    animation.SetTopLeft(x - 20, y - 20);
    animation.OnShow();
}

void CBomb::SetXY(int x, int y) {
    this->x = x;
    this->y = y;
}
bool CBomb::IsAlive() {
    return is_alive;
}

}