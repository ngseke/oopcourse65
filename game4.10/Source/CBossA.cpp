#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <time.h>
#include "audio.h"
#include "gamelib.h"
#include "CDict.h"
#include "CMe.h"
#include "CEnemy.h"
#include "CBossA.h"

namespace game_framework {
CBossA::CBossA() {}
CBossA::CBossA(int x, int y, int delay, bool alive, CDict* d, int minVL, int maxVL, vector<CEnemy*>* enemyQueue, vector<CBomb*>* bombList, vector<CMovingBitmap*>* letter) {	//	初始值都在此處設定
    is_alive = is_bombed = false;
    dx = dy = index = delay_counter = 0;
    currWordLeng = 0;
    bossType = "bossA";
    ////
    SetXY(x, y);
    SetDelay(delay);
    SetIsAlive(alive);
    this->dict = d;
    this->bombList = bombList;
    this->enemyQueue = enemyQueue;
    this->minVocabLeng = minVL;
    this->maxVocabLeng = maxVL;
    this->letter = letter;
    //
    index = (maxVocabLeng == 1 && minVocabLeng == 1) ? 10 : 0;
    callEnemyCounter = maxCallEnemyCounter = 30 * 5;		// 發動召喚小怪技能的間隔
    this->endX = SIZE_X / 2 - 30 + rand() % 60;
    this->endY = SIZE_Y;
    this->letter = letter;
    //
    SetVocab();
}

void CBossA::CallEnemy(int x, int y) {
    enemyQueue->push_back(new CEnemy(x, y, 2, false, dict, 3, 4, enemyQueue, bombList, SIZE_X / 2 - 50 + rand() % 100, SIZE_Y, letter));
    enemyQueue->back()->LoadBitmap();
    enemyQueue->back()->SetIsAlive(true);
}

void CBossA::kill() {
    is_alive = false;
    bombList->push_back(new CBomb(GetX() + 10, GetY() + 10));
    bombList->back()->LoadBitmap();
    is_bombed = true;
}

void CBossA::OnMove() {
    const int STEPS = 200;	// 切成幾分dx

    if (!is_alive) return;

    delay_counter--;
    callEnemyCounter--;

    if (delay_counter < 0) {
        delay_counter = delay;
        index++;

        if (index >= STEPS)
            index = 0;

        double dxTemp = (double(endX) - x) / STEPS * index;
        double dyTemp = (double(endY) - y) / STEPS * index;
        dx = int(dxTemp);  // dx為 (Enemy<->Me之x總距離) / STEPS * index;
        dy = int(dyTemp);
    }

    if (callEnemyCounter < 0) {		// BossA技能:召喚小怪
        callEnemyCounter = maxCallEnemyCounter;
        CallEnemy((this->x + dx  / 2 + rand() % bmp.Width()), (this->y + dy + 5 + bmp.Height() ));
    }
}


void CBossA::LoadBitmap() {
    char str[30];
    const unsigned int bitmapNum = 7;		// 圖檔數量
    sprintf(str, "Bitmaps/face/face_boss%d.bmp", rand() % bitmapNum + 1);	// 隨機挑選 bitmap
    bmp.LoadBitmap(str, RGB(0, 255, 0)); // 載入 怪物SKIN
    textCursor.LoadBitmap(IDB_TEXT_CURSOR, RGB(0, 255, 0));		// 載入 光標
    talkBoxL.LoadBitmap(IDB_TALK_BOX_LEFT, RGB(0, 255, 0));		// 載入 對話框左
    talkBoxC.LoadBitmap(IDB_TALK_BOX_CENTER, RGB(0, 255, 0));	// 載入 對話框中
    talkBoxR.LoadBitmap(IDB_TALK_BOX_RIGHT, RGB(0, 255, 0));	// 載入 對話框右
}

}