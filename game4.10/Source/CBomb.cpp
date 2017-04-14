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
    const int INITIAL_VELOCITY = 20;	// ��l�W�ɳt��
    const int FLOOR = 400;				// �a�O�y��
    floor = FLOOR;
    x = 95;
    y = FLOOR - 1;				// y�y�Ф�a�O��1�I(���b�a�O�W)
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
    animation.OnMove();		// ����@��animation.OnMove()�Aanimation�~�|����
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