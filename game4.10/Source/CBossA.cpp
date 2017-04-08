#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <time.h>
#include "audio.h"
#include "gamelib.h"
#include "CDict.h"
#include "CMe.h"
#include "CEraser.h"
#include "CEnemy.h"
#include "CBossA.h"

namespace game_framework {
CBossA::CBossA(int x, int y, int delay, bool alive, CDict* d, int minVL, int maxVL, vector<CEnemy*>* enemyQueue) {	//	��l�ȳ��b���B�]�w
    this->enemyQueue = enemyQueue;
    is_alive = false;
    dx = dy = index = delay_counter = 0;
    currWordLeng = 0;
    targetX = 15;
    targetY = 15;
    ////
    SetXY(x, y);
    SetDelay(delay);
    SetIsAlive(alive);
    dict = d;
    minVocabLeng = minVL;
    maxVocabLeng = maxVL;
    callEnemyCounter = maxCallEnemyCounter = 300;		// �o�ʥl��p�ǧޯ઺���j
    //
    SetVocab();
}

void CBossA::CallEnemy(int x, int y) {
    enemyQueue->push_back(new CEnemy(x, y, 3, 1, dict, 3, 4));
    enemyQueue->back()->LoadBitmap();
}
void CBossA::OnMove() {
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
        CallEnemy((this->x + dx  / 2 + rand() % bmp.Width()), (this->y + dy + 5 + bmp.Height() ));
    }
}


void CBossA::LoadBitmap() {
    char* faceFile[] = { "Bitmaps/face/face_boss3.bmp", "Bitmaps/face/face_boss4.bmp", "Bitmaps/face/face_boss5.bmp", "Bitmaps/face/face_boss6.bmp", "Bitmaps/face/face_boss7.bmp" };		// �x�s�Ǫ��ɮ׸��|���}�C
    bmp.LoadBitmap(faceFile[(rand() % 5)], RGB(0, 255, 0)); // ���J �Ǫ�SKIN
    textCursor.LoadBitmap("Bitmaps/text_cursor.bmp", RGB(0, 255, 0));  //���J ����
    /////
    talkBoxL.LoadBitmap("Bitmaps/talk_box_left.bmp", RGB(0, 255, 0));
    talkBoxC.LoadBitmap("Bitmaps/talk_box_center.bmp", RGB(0, 255, 0));
    talkBoxR.LoadBitmap("Bitmaps/talk_box_right.bmp", RGB(0, 255, 0));
    /////
    char* filename[2] = { "Bitmaps/target_s1.bmp", "Bitmaps/target_s2.bmp" };

    for (int i = 0; i < 26; i++) {
        letter.push_back(new CMovingBitmap);
        char str[20];
        sprintf(str, "Bitmaps/char/%c.bmp", i + 97);
        letter.back()->LoadBitmap(str, RGB(255, 255, 255));
    }

    for (int i = 0; i < 2; i++)		// ���J�ʵe
        target.AddBitmap(filename[i], RGB(0, 255, 0));
}
}