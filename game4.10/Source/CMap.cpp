#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <time.h>
#include "gamelib.h"
#include "CMap.h"


namespace game_framework {
CMap::CMap(): X(0), Y(0), MW(27), MH(27) {
    delay = delay_counter = index = 0;
    dx = dy = 0;
}
void CMap::LoadBitmap() {
    bgColor.LoadBitmap("Bitmaps/background_color.bmp", RGB(0, 255, 0));
    net.LoadBitmap("Bitmaps/background_net.bmp", RGB(0, 255, 0));
}

void CMap::OnMove() {
    delay_counter--;

    if (delay_counter < 0) {
        delay_counter = delay;
        const int STEPS = 27;
        index++;

        if (index >= STEPS)index = 0;

        dy = index;
    }
}
void CMap::OnShow() {
    /*
    for (int i = 0; i < SIZE_X; i++) {
        bgColor.SetTopLeft(i, 0);
        bgColor.ShowBitmap();
    }
    */
    for (int i = 0; i < (SIZE_X / MW) + 1; i++) {
        for (int j = 0; j < SIZE_Y / MH + 27; j++) {
            net.SetTopLeft(X + i * MW, Y + j * MH + dy - 27);
            net.ShowBitmap();
        }
    }
}

}