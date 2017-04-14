#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CBomb.h"

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// CBouncingBall: BouncingBall class
/////////////////////////////////////////////////////////////////////////////

CBomb::CBomb() {
    const int INITIAL_VELOCITY = 20;	// 初始上升速度
    const int FLOOR = 400;				// 地板座標
    floor = FLOOR;
    x = 95;
    y = FLOOR - 1;				// y座標比地板高1點(站在地板上)
    rising = true;
    initial_velocity = INITIAL_VELOCITY;
    velocity = initial_velocity;

	animation.SetDelayCount(1);

}

void CBomb::LoadBitmap() {
	for (int i = 0; i< 6;i++) {
		char str[30];
		sprintf(str, "Bitmaps/bomb/bomb%d.bmp", i + 1);
		animation.AddBitmap(str, RGB(255,0,255));
	}
}

void CBomb::OnMove() {
    animation.OnMove();		// 執行一次animation.OnMove()，animation才會換圖
}

void CBomb::OnShow() {
    animation.SetTopLeft(x, y);
    animation.OnShow();
}

void CBomb::SetXY(int x, int y) {
    this->x = x;
    this->y = y;
}

void CBomb::SetFloor(int floor) {
    this->floor = floor;
}
void CBomb::SetVelocity(int velocity) {
    this->velocity = velocity;
    this->initial_velocity = velocity;
}
}