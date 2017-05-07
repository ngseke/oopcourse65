#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CEnemy.h"
#include "CEmp.h"


namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// CMe: à
/////////////////////////////////////////////////////////////////////////////

CEmp::CEmp() {
    Initialize();
}


int CEmp::GetX1() {
    return x;
}

int CEmp::GetY1() {
    return y;
}

int CEmp::GetX2() {
    return x + emp.Width();
}

int CEmp::GetY2() {
    return y + emp.Height();
}

void CEmp::Initialize() {
    const int X_POS = ( SIZE_X - 640) / 2;
    const int Y_POS = SIZE_Y - 350;
    x = X_POS;
    y = Y_POS;
    state = false;
    emp.SetDelayCount(2);
}

void CEmp::LoadBitmap() {
    for (int i = 0; i < 7; i++) {
        char str[40];
        sprintf(str, "Bitmaps/big_wave/big_wave%d.bmp", i);
        emp.AddBitmap(str, RGB(0, 255, 0));
    }
}

void CEmp::OnMove() {
    if (state) {
        emp.OnMove();

        for (unsigned int i = 0; i < enemyQueue->size(); i++) {
            if (HitRectangle(enemyQueue->at(i)->GetX(), enemyQueue->at(i)->GetY(), enemyQueue->at(i)->GetX2(), enemyQueue->at(i)->GetY2())) {
                //score->Add(targetEnemy->GetCurrWordLeng());				// だ计+= ┣
                if (lock && &enemyQueue->at(i) == &targetEnemy) {
                    lock = false;
                    score->Add(500);
                }

                score->Add(enemyQueue->at(i)->GetVocabLeng());
                enemyQueue->at(i)->kill();
            }
        }

        if (emp.IsFinalBitmap()) {
            state = false;
            emp.Reset();
        }
    }
}


void CEmp::SetXY(int nx, int ny) {
    x = nx;
    y = ny;
}

void CEmp::OnShow() {
    emp.SetTopLeft((SIZE_X - 640) / 2, (SIZE_Y - 350));
    emp.OnShow();
}

void CEmp::SetEQ(vector<CEnemy*>* enemyQueue, CInteger* score, bool* lock, CEnemy* targetEnemy) {
    this->enemyQueue = enemyQueue;
    this->score = score;
    this->lock = lock;
    (this->targetEnemy) = targetEnemy;
}
void CEmp::CallEmp() {
    if (!state) {
        state = true;
    }
}
bool CEmp::HitRectangle(int tx1, int ty1, int tx2, int ty2) {// 80 160 320 480 480
    if (emp.GetCurrentBitmapNumber() != 0 && !emp.IsFinalBitmap()) {
        int arr[10] = { 1, 2, 4, 6, 6, 6, 6, 6, 6 };
        int x1, y1, x2, y2;
        int  i = emp.GetCurrentBitmapNumber() - 1;
        x1 = x + (320 - (80 * arr[i]) / 2);
        y1 = y + (320 - (80 * arr[i]) / 2);
        x2 = x + (320 + (80 * arr[i]) / 2);
        y2 = y + (320 + (80 * arr[i]) / 2);
        //
        // 浪代┣face痻籔把计痻琌Τユ栋
        //
        return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
    }
    else return 0;
}

}