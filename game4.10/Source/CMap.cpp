#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <time.h>
#include "gamelib.h"
#include "CMap.h"

namespace game_framework {
// CMap: ºôª¬­I´º

CMap::CMap(): X(0), Y(0), MW(27), MH(27) {
    delay = delay_counter = index = 0;
    dx = dy = 0;
    flashCounter = -1;
    flashCounterMax = 3;
}
void CMap::LoadBitmap() {
    bgColor.LoadBitmap("Bitmaps/background_color.bmp", RGB(0, 255, 0));
    net.LoadBitmap("Bitmaps/background_net.bmp", RGB(0, 255, 0));
    net1.LoadBitmap("Bitmaps/background_net1.bmp", RGB(0, 255, 0));
}
void CMap::PlayFlash() {
    flashCounter = flashCounterMax;
}
void CMap::OnMove() {
    delay_counter--;

    if (flashCounter >= 0)flashCounter--;

    if (delay_counter < 0) {
        delay_counter = delay;
        const int STEPS = 27;
        index += 1;

        if (index >= STEPS)index = 0;

        dy = index;
    }
}
void CMap::OnShow() {
    for (int i = 0; i < (SIZE_X / MW) + 1; i++) {
        for (int j = 0; j < SIZE_Y / MH + 27; j++) {
            if (flashCounter >= 0) {
                net1.SetTopLeft(X + i * MW, Y + j * MH + dy - 27);
                net1.ShowBitmap();
            }
            else {
                net.SetTopLeft(X + i * MW, Y + j * MH + dy - 27);
                net.ShowBitmap();
            }
        }
    }
}

}