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
#include "CBossB.h"

namespace game_framework {
CBossB::CBossB() {}
CBossB::CBossB(int x, int y, int delay, bool alive, CDict* d, int minVL, int maxVL, vector<CEnemy*>* enemyQueue, vector<CBomb*>* bombList) {	//	初始值都在此處設定
    this->enemyQueue = enemyQueue;
    this->bombList = bombList;
    is_alive = is_bombed = false;
    dx = dy = index = delay_counter = 0;
    currWordLeng = 0;
    targetX = -2;
    targetY = -2;
    ////
    SetXY(x, y);
    SetDelay(delay);
    SetIsAlive(alive);
    dict = d;
    this->bombList = bombList;
    minVocabLeng = minVL;
    maxVocabLeng = maxVL;
    callEnemyCounter = maxCallEnemyCounter = 300;		// 發動召喚小怪技能的間隔
    endX = SIZE_X / 2;
    endY = SIZE_Y;
    //
    SetVocab();
}
CBossB::CBossB(int x, int y, int delay, bool alive, CDict* d, int minVL, int maxVL, vector<CEnemy*>* enemyQueue, vector<CBomb*>* bombList,int endx,int endy) {	//	初始值都在此處設定
	this->enemyQueue = enemyQueue;
	this->bombList = bombList;
	is_alive = is_bombed = false;
	dx = dy = index = delay_counter = 0;
	currWordLeng = 0;
	targetX = -2;
	targetY = -2;
	////
	SetXY(x, y);
	SetDelay(delay);
	SetIsAlive(alive);
	dict = d;
	this->bombList = bombList;
	minVocabLeng = minVL;
	maxVocabLeng = maxVL;
	callEnemyCounter = maxCallEnemyCounter = 300;		// 發動召喚小怪技能的間隔
	this->endX = endx;
	this->endY = endy;
	//
	SetVocab();
}
void CBossB::CallEnemy(int x, int y) {
    enemyQueue->push_back(new CEnemy(x, y, 3, 1, dict, 3, 4, bombList,-30,30));
    enemyQueue->back()->LoadBitmap();
	enemyQueue->push_back(new CEnemy(x, y, 3, 1, dict, 3, 4, bombList, -20, 20));
	enemyQueue->back()->LoadBitmap();
	enemyQueue->push_back(new CEnemy(x, y, 3, 1, dict, 3, 4, bombList, 0, -20));
	enemyQueue->back()->LoadBitmap();
	enemyQueue->push_back(new CEnemy(x, y, 3, 1, dict, 3, 4, bombList, 20, 20));
	enemyQueue->back()->LoadBitmap();
	enemyQueue->push_back(new CEnemy(x, y, 3, 1, dict, 3, 4, bombList, -20, 20));
	enemyQueue->back()->LoadBitmap();
}
void CBossB::OnMove() {
    const int STEPS = 300;	// 切成幾分dx

    if (!is_alive) return;

    delay_counter--;
    callEnemyCounter--;
    target.OnMove();

    if (delay_counter < 0) {
        delay_counter = delay;
        index++;

        if (index >= STEPS)
            index = 0;

        // dx = xMoveDistance / STEPS * index;
        double dxTemp = (double(SIZE_X / 2) - x) / STEPS * index;
        dx = int(dxTemp);  // dx為 (Enemy<->Me之x總距離) / STEPS * index;
        dy = ((SIZE_Y - y) / STEPS) * index;
    }

    if (callEnemyCounter < 0) {		// BossA技能:召喚小怪
        callEnemyCounter = maxCallEnemyCounter;
        CallEnemy((this->x + dx / 2 + rand() % bmp.Width()), (this->y + dy + 5 + bmp.Height()));
    }
}
void CBossB::LoadBitmap() {
    char str[30];
    const unsigned int bitmapNum = 7;		// 圖檔數量
    sprintf(str, "Bitmaps/face/face_boss%d.bmp", rand() % bitmapNum + 1);
    bmp.LoadBitmap(str, RGB(0, 255, 0)); // 載入 怪物SKIN
    //
    textCursor.LoadBitmap("Bitmaps/text_cursor.bmp", RGB(0, 255, 0));  //載入 光標
    /////
    talkBoxL.LoadBitmap("Bitmaps/talk_box_left.bmp", RGB(0, 255, 0));
    talkBoxC.LoadBitmap("Bitmaps/talk_box_center.bmp", RGB(0, 255, 0));
    talkBoxR.LoadBitmap("Bitmaps/talk_box_right.bmp", RGB(0, 255, 0));
    /////
    char* filename[2] = { "Bitmaps/target_m1.bmp", "Bitmaps/target_m2.bmp" };

    for (int i = 0; i < 26; i++) {
        letter.push_back(new CMovingBitmap);
        char str[20];
        sprintf(str, "Bitmaps/char1/%c.bmp", i + 97);
        letter.back()->LoadBitmap(str, RGB(255, 255, 255));
    }

    for (int i = 0; i < 2; i++)		// 載入動畫
        target.AddBitmap(filename[i], RGB(0, 255, 0));
}
}