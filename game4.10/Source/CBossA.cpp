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
CBossA::CBossA(int x, int y, int delay, bool alive, CDict* d, int minVL, int maxVL, vector<CEnemy*>* enemyQueue, vector<CBomb*>* bombList, vector<CMovingBitmap*>* letter) {	//	��l�ȳ��b���B�]�w
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
    callEnemyCounter = maxCallEnemyCounter = 30 * 5;		// �o�ʥl��p�ǧޯ઺���j
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
    const int STEPS = 200;	// �����X��dx

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
        dx = int(dxTemp);  // dx�� (Enemy<->Me��x�`�Z��) / STEPS * index;
        dy = int(dyTemp);
    }

    if (callEnemyCounter < 0) {		// BossA�ޯ�:�l��p��
        callEnemyCounter = maxCallEnemyCounter;
        CallEnemy((this->x + dx  / 2 + rand() % bmp.Width()), (this->y + dy + 5 + bmp.Height() ));
    }
}


void CBossA::LoadBitmap() {
    char str[30];
    const unsigned int bitmapNum = 7;		// ���ɼƶq
    sprintf(str, "Bitmaps/face/face_boss%d.bmp", rand() % bitmapNum + 1);	// �H���D�� bitmap
    bmp.LoadBitmap(str, RGB(0, 255, 0)); // ���J �Ǫ�SKIN
    textCursor.LoadBitmap(IDB_TEXT_CURSOR, RGB(0, 255, 0));		// ���J ����
    talkBoxL.LoadBitmap(IDB_TALK_BOX_LEFT, RGB(0, 255, 0));		// ���J ��ܮإ�
    talkBoxC.LoadBitmap(IDB_TALK_BOX_CENTER, RGB(0, 255, 0));	// ���J ��ܮؤ�
    talkBoxR.LoadBitmap(IDB_TALK_BOX_RIGHT, RGB(0, 255, 0));	// ���J ��ܮإk
}

}