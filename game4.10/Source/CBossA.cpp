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
	CBossA::CBossA(int x, int y, int delay, bool alive, CDict* d, vector<CEnemy*>* enemyQueue) {	//	﹍常矪砞﹚
		this->enemyQueue = enemyQueue;
		is_alive = false;
		dx = dy = index = delay_counter = 0;
		currWordLeng = 0;
		////
		SetXY(x, y);
		SetDelay(delay);
		SetIsAlive(alive);
		dict = d;
		SetVocab();
		CallEnemy();
	}

	void CBossA::CallEnemy() {
		enemyQueue->push_back(new CEnemy(x+dx, 0, 3, 1, dict));
		enemyQueue->back()->LoadBitmap();
	}

	void CBossA::LoadBitmap() {
		char* faceFile[] = { "Bitmaps/face_mid1.bmp" };		// 纗┣郎隔畖ぇ皚
		bmp.LoadBitmap(faceFile[(rand() % 1)], RGB(0, 255, 0)); // 更 ┣SKIN
		textCursor.LoadBitmap("Bitmaps/text_cursor.bmp", RGB(0, 255, 0));  //更 夹
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
			//letter[i]->LoadBitmap(str, RGB(255, 255, 255));
		}

		for (int i = 0; i < 2; i++)		// 更笆礶
			target.AddBitmap(filename[i], RGB(0, 255, 0));
	}
}