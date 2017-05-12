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
// CMe: 主角
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
    empTimes = 3;
}

void CEmp::LoadBitmap() {
    for (int i = 0; i < 7; i++) {
        char str[40];
        sprintf(str, "Bitmaps/big_wave/big_wave%d.bmp", i);
        emp.AddBitmap(str, RGB(0, 255, 0));
    }

    displayBG.LoadBitmap("Bitmaps/emp_text/bg.bmp", RGB(0, 255, 0));
    displayNumber[0].LoadBitmap("Bitmaps/emp_text/3.bmp", RGB(0, 255, 0));
    displayNumber[1].LoadBitmap("Bitmaps/emp_text/2.bmp", RGB(0, 255, 0));
    displayNumber[2].LoadBitmap("Bitmaps/emp_text/1.bmp", RGB(0, 255, 0));
    displayNumber[3].LoadBitmap("Bitmaps/emp_text/0.bmp", RGB(0, 255, 0));
    displayBG.SetTopLeft(SIZE_X - 80, SIZE_Y - 80);

    for (int i = 0; i < 4; i++) {
        displayNumber[i].SetTopLeft(SIZE_X - 80 + 14, SIZE_Y - 80 + 8);
    }
}

void CEmp::OnMove() {
    if (state) {
        emp.OnMove();

        for (unsigned int i = 0; i < enemyQueue->size(); i++) {
            if (HitRectangle(enemyQueue->at(i)->GetX(), enemyQueue->at(i)->GetY(), enemyQueue->at(i)->GetX2(), enemyQueue->at(i)->GetY2())) {
                // score->Add(targetEnemy->GetCurrWordLeng());				// 分數+= 怪物長度
                if (*lock && enemyQueue->at(i) == *targetEnemy) {
                    *lock = 0;
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
    displayBG.ShowBitmap();
    displayNumber[4 - empTimes - 1].ShowBitmap();

    if (0) {		// 顯示debug資訊
        CDC* pDC = CDDraw::GetBackCDC();
        CFont f, *fp;
        f.CreatePointFont(100, "Fixedsys");
        fp = pDC->SelectObject(&f);
        pDC->SetBkColor(RGB(0, 0, 0));
        //
        char temp[100];
        //sprintf(temp, "DEBUG-- 1.%s  ", (*targetEnemy)->GetVocab().c_str());
        sprintf(temp, "DEBUG-- empTimes:%d  ", empTimes);
        pDC->SetTextColor(RGB(200, 0, 0));
        pDC->TextOut(10, 10, temp);
        ////
        pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
        CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
    }
}

void CEmp::SetEQ(vector<CEnemy*>* enemyQueue, CInteger* score, bool* lock, CEnemy** targetEnemy) {
    this->enemyQueue = enemyQueue;
    this->score = score;
    this->lock = lock;
    this->targetEnemy = targetEnemy;
}

void CEmp::CallEmp() {
    if (!state && empTimes != 0) {
        empTimes--;
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
        // 檢測怪物face的矩形與參數矩形是否有交集
        //
        return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
    }
    else return 0;
}

}