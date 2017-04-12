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

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// CEnemy: Ball class
/////////////////////////////////////////////////////////////////////////////

CEnemy::CEnemy() {
    is_alive = false;
    x = y = dx = dy = index = delay_counter = 0;
    currWordLeng = 0;	  // 當前的游標在0 還未輸入的意思 (ex: ""apple), 若currWord = 1 (ex: "a"pple)
}

CEnemy::CEnemy(int x, int y, int delay, bool alive, CDict* d, int minVL, int maxVL) {	//	初始值都在此處設定
    is_alive = false;
    dx = dy = index = delay_counter = 0;
    currWordLeng = 0;
    targetX = -2;
    targetY = -2;
    ////
    SetXY(x, y);
    SetDelay(delay);
    SetIsAlive(alive);
    dict = d;
    minVocabLeng = minVL;
    maxVocabLeng = maxVL;
    SetVocab();
}


bool CEnemy::HitRectangle(int tx1, int ty1, int tx2, int ty2) {
    int x1 = x + dx;				// 怪物face的左上角x座標
    int y1 = y + dy;				// 怪物face的左上角y座標
    int x2 = x1 + bmp.Width();		// 怪物face的右下角x座標
    int y2 = y1 + bmp.Height();		// 怪物face的右下角y座標
    //
    // 檢測怪物face的矩形與參數矩形是否有交集
    //
    return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
}

bool CEnemy::IsAlive() {
    return is_alive;
}

void CEnemy::LoadBitmap() {
    char str[30];
    const unsigned int bitmapNum = 7;		// 圖檔數量
    sprintf(str, "Bitmaps/face/face%d.bmp", rand() % bitmapNum + 1);
    bmp.LoadBitmap(str, RGB(0, 255, 0)); // 載入 怪物SKIN
    textCursor.LoadBitmap("Bitmaps/text_cursor.bmp", RGB(0, 255, 0));  //載入 光標
    /////
    talkBoxL.LoadBitmap("Bitmaps/talk_box_blur/talk_box_left.bmp", RGB(0, 255, 0));
    talkBoxC.LoadBitmap("Bitmaps/talk_box_blur/talk_box_center.bmp", RGB(0, 255, 0));
    talkBoxR.LoadBitmap("Bitmaps/talk_box_blur/talk_box_right.bmp", RGB(0, 255, 0));
    /////
    char* filename[2] = { "Bitmaps/target_s1.bmp", "Bitmaps/target_s2.bmp" };

    for (int i = 0; i < 26; i++) {
        letter.push_back(new CMovingBitmap);
        char str[20];
        sprintf(str, "Bitmaps/char1/%c.bmp", i + 97);
        letter.back()->LoadBitmap(str, RGB(255, 255, 255));
        //letter[i]->LoadBitmap(str, RGB(255, 255, 255));
    }

    for (int i = 0; i < 2; i++)		// 載入動畫
        target.AddBitmap(filename[i], RGB(0, 255, 0));
}

void CEnemy::OnMove() {
    const int STEPS = 250;	// 切成幾分dx

    if (!is_alive) return;

    delay_counter--;
    target.OnMove();

    if (delay_counter < 0) {
        delay_counter = delay;
        index++;

        if (index >= STEPS)
            index = 0;

        // dx = xMoveDistance / STEPS * index;
        double dxTemp = (double(SIZE_X / 2) - x) / STEPS * index;
        dx  = int(dxTemp);  // dx為 (Enemy<->Me之x總距離) / STEPS * index;
        dy = ((SIZE_Y - y) / STEPS) * index ;
    }
}

void CEnemy::SetDelay(int d) {
    delay = d;
}
void CEnemy::SetIsAlive(bool alive) {
    is_alive = alive;
}
void CEnemy::SetXY(int nx, int ny) {
    x = nx;
    y = ny;
}
void CEnemy::OnShow() {
    if (is_alive) {
        bmp.SetTopLeft(x + dx, y + dy);
        bmp.ShowBitmap();
        // 改良後顯示talkbox方式, 完全依照單字長度
        talkBoxL.SetTopLeft(x + dx + bmp.Width(), y + dy);
        talkBoxL.ShowBitmap();

        for (int i = 0; i < length; i++) {
            talkBoxC.SetTopLeft(x + dx + bmp.Width() + talkBoxL.Width() + i * talkBoxC.Width(), y + dy);
            talkBoxC.ShowBitmap();
        }

        talkBoxR.SetTopLeft(x + dx + bmp.Width() + talkBoxL.Width() + length * talkBoxC.Width(), y + dy);
        talkBoxR.ShowBitmap();
        ////
        //// show target bmp
        target.SetTopLeft(x + dx + targetX, y + dy + targetY);

        if (currWordLeng != 0) target.OnShow();

        //
        textCursor.SetTopLeft(x + dx + bmp.Width() + talkBoxL.Width() + ((currWordLeng ) * 10) - 1, y + dy);	// 顯示光標

        if (currWordLeng != 0) textCursor.ShowBitmap();

        for (int i = 0; i < length; i++) {
            letter[vocab[i] - 97]->SetTopLeft(x + dx + bmp.Width() + talkBoxL.Width() + letter[0]->Width() * i, y + dy + 3);
            letter[vocab[i] - 97]->ShowBitmap();
        }

        for (int i = 0; i < currWordLeng; i++) {	// 讓打過的單字蓋掉 消失不見
            talkBoxC.SetTopLeft(x + dx + bmp.Width() + talkBoxL.Width() + i * talkBoxC.Width(), y + dy);
            talkBoxC.ShowBitmap();
        }
    }
}

////////////
void  CEnemy::SetVocab() {			//隨機從dict中抓取一個單字到vocab裡面
    //CDict* dict = new CDict;
    while (1) {
        vocab = dict->GetText();	// 給vocab一個單字
        length = vocab.length();

        if (length >= minVocabLeng && length <= maxVocabLeng)			// 條件成立,使用break跳出迴圈 確定生成此單字
            break;
    }
}
string CEnemy::GetVocab() {		  // 回傳整組單字(ex: "apple")
    return vocab;
}
char CEnemy::GetFirstWord()		{ // 以char回傳一個字 (ex: 'a')
    return vocab[0];
}
void CEnemy::AddCurrWordLeng() {
    currWordLeng++;
}
int CEnemy::GetCurrWordLeng() {
    return currWordLeng;
}
int CEnemy::GetVocabLeng() {
    return length;
}
int CEnemy::GetX() {
    return x + dx;
}
int CEnemy::GetY() {
    return y + dy;
}
void CEnemy::MinusIndex(int num) {
    index = index - num;
}
bool CEnemy::HitMe(CMe* me) {
    return HitRectangle(me->GetX1(), me->GetY1(),
                        me->GetX2(), me->GetY2());
}
}