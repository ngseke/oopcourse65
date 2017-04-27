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
#include <math.h>
namespace game_framework {
CBossB::CBossB() {}
CBossB::CBossB(int x, int y, int delay, bool alive, CDict* d, int minVL, int maxVL, vector<CEnemy*>* enemyQueue, vector<CBomb*>* bombList) {	//	��l�ȳ��b���B�]�w
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
    callEnemyCounter = maxCallEnemyCounter =500;		// �o�ʥl��p�ǧޯ઺���j
    endX = SIZE_X / 2;
    endY = SIZE_Y;
    //
    SetVocab();
}
CBossB::CBossB(int x, int y, int delay, bool alive, CDict* d, int minVL, int maxVL, vector<CEnemy*>* enemyQueue, vector<CBomb*>* bombList,int endx,int endy) {	//	��l�ȳ��b���B�]�w
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
	callEnemyCounter = maxCallEnemyCounter = 1000;		// �o�ʥl��p�ǧޯ઺���j
	this->endX = endx;
	this->endY = endy;
	//
	SetVocab();
}
void CBossB::CallEnemy(int x, int y) {
	enemyQueue->push_back(new CEnemy(x + (bmp.Width()/2)-5, y+40, 3, 1, dict, 3, 4, bombList, x, int(y * 5  )));//���������U(90��)
    enemyQueue->back()->LoadBitmap();
	enemyQueue->push_back(new CEnemy(x + (bmp.Width() / 2)-5, y + 40, 3, 1, dict, 3, 4, bombList, int(x * 5 * (cos(0.785))), int(y * 5 * (sin(0.785)))));//45��
	enemyQueue->back()->LoadBitmap();
	enemyQueue->push_back(new CEnemy(x + (bmp.Width() / 2)-5, y + 40, 3, 1, dict, 3, 4, bombList, int(x * 5 * (cos(1.1775))), int(y * 5 * (sin(1.1775)))));//67.5��
	enemyQueue->back()->LoadBitmap();
	enemyQueue->push_back(new CEnemy(x + (bmp.Width() / 2)-5, y + 40, 3, 1, dict, 3, 4, bombList, int(x * 5 * (cos(1.9625))), int(y * 5 * (sin(1.9625)))));//112.5��
	enemyQueue->back()->LoadBitmap();
	enemyQueue->push_back(new CEnemy(x + (bmp.Width() / 2)-5, y + 40, 3, 1, dict, 3, 4, bombList, int(x * 5 * (cos(2.355))), int(y * 5 * (sin(2.355)))));//135��
	enemyQueue->back()->LoadBitmap();
}
void CBossB::OnMove() {
    const int STEPS = 300;	// �����X��dx

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
        dx = int(dxTemp);  // dx�� (Enemy<->Me��x�`�Z��) / STEPS * index;
        dy = ((SIZE_Y - y) / STEPS) * index;
    }

    if (callEnemyCounter < 0) {		// BossA�ޯ�:�l��p��
        callEnemyCounter = maxCallEnemyCounter;
        CallEnemy((this->x + dx ), (this->y + dy));
    }
}
void CBossB::LoadBitmap() {
    char str[30];
    const unsigned int bitmapNum = 7;		// ���ɼƶq
    sprintf(str, "Bitmaps/face/face_boss%d.bmp", rand() % bitmapNum + 1);
    bmp.LoadBitmap(str, RGB(0, 255, 0)); // ���J �Ǫ�SKIN
    //
    textCursor.LoadBitmap("Bitmaps/text_cursor.bmp", RGB(0, 255, 0));  //���J ����
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

    for (int i = 0; i < 2; i++)		// ���J�ʵe
        target.AddBitmap(filename[i], RGB(0, 255, 0));
}
}