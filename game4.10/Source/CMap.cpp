#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <time.h>
#include "audio.h"
#include "gamelib.h"
#include "CMap.h"


namespace game_framework {
CMap::CMap(): X(0), Y(0), MW(15), MH(15) {
    delay = delay_counter = 0;
    dx = dy = 0;
}
void CMap::LoadBitmap() {
    net.LoadBitmap("Bitmaps/background_net.bmp", RGB(0, 255, 0));
}

void CMap::OnMove() {
    delay_counter--;

    if (delay_counter < 0) {
        delay_counter = delay;
        const int STEPS = 15;
        index++;

        if (index >= STEPS)index = 0;

        dy = index;
    }
}
void CMap::OnShow() {
    for (int i = 0; i < SIZE_X / MW; i++) {
        for (int j = 0; j < SIZE_Y / MH + 15; j++) {
            net.SetTopLeft(X + i * MW, Y + j * MH + dy - 15);
            net.ShowBitmap();
        }
    }
}

}