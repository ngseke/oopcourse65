#pragma once

namespace game_framework {

class CMap {
    public:
        CMap();
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void PlayFlash();

    protected:
        CMovingBitmap net, net1, bgColor;
        const int X, Y;
        const int MW, MH;
        int index;
        int delay_counter;
        int delay;
        int dx, dy;
        int flashCounter, flashCounterMax;
};

}