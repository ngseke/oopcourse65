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
    //SetVocab();
}
CEnemy::CEnemy(int x, int y, int delay, bool alive, CDict* d) {	//	初始值都在此處設定
    is_alive = false;
    dx = dy = index = delay_counter = 0;
    currWordLeng = 0;
    ////
    SetXY(x, y);
    SetDelay(delay);
    SetIsAlive(alive);
    dict = d;
    SetVocab();
}
bool CEnemy::HitEraser(CEraser* eraser) {
    // 檢測(擦子所構成的矩形)是否碰到球
    // 檢查怪物是否碰到主角
    return HitRectangle(eraser->GetX1(), eraser->GetY1(),
                        eraser->GetX2(), eraser->GetY2());
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
    char* faceFile[] = { "Bitmaps/face1.bmp", "Bitmaps/face2.bmp", "Bitmaps/face3.bmp", "Bitmaps/face4.bmp", "Bitmaps/face5.bmp", "Bitmaps/face6.bmp", "Bitmaps/face7.bmp" };		// 儲存怪物檔案路徑之陣列
    bmp.LoadBitmap(faceFile[(rand() % 7)], RGB(0, 255, 0)); // 載入 怪物SKIN
    textCursor.LoadBitmap("Bitmaps/text_cursor.bmp", RGB(0, 255, 0));  //載入 光標
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

    for (int i = 0; i < 2; i++)		// 載入動畫
        target.AddBitmap(filename[i], RGB(0, 255, 0));
}
/*
void CEnemy::LoadTextbox() {		//用不到了
    // 根據單字長度 載入不同寬度對話框
    if (length <= 3)
        talkBox.LoadBitmap("Bitmaps/talk_box_3words.bmp", RGB(0, 255, 0));
    else if (length <= 5)
        talkBox.LoadBitmap("Bitmaps/talk_box_5words.bmp", RGB(0, 255, 0));
    else if (length <= 10)
        talkBox.LoadBitmap("Bitmaps/talk_box_10words.bmp", RGB(0, 255, 0));
    else
        talkBox.LoadBitmap("Bitmaps/talk_box_15words.bmp", RGB(0, 255, 0));
}
*/
void CEnemy::OnMove() {
    const int STEPS = 300;	// 切成幾分dx

    if (!is_alive) return;

    delay_counter--;
    target.OnMove();
    int xMoveDistance = (SIZE_X / 2) - x;

    if (delay_counter < 0) {
        delay_counter = delay;
        index++;

        if (index >= STEPS)
            index = 0;

        // dx = xMoveDistance / STEPS * index;
        double dxTemp = (double(SIZE_X / 2) - x) / STEPS * index;
        dx  = int(dxTemp);  // dx為 (Enemy<->Me之x總距離) / STEPS * index;
        dy = ((SIZE_Y - y) / STEPS) * index ;
        TRACE("%d", xMoveDistance / STEPS * index);
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
        target.SetTopLeft(x + dx - 2, y + dy - 2);

        if (currWordLeng != 0) target.OnShow();

        //
        textCursor.SetTopLeft(x + dx + bmp.Width() + talkBoxL.Width() + ((currWordLeng ) * 10) - 1, y + dy);	// 顯示光標

        if (currWordLeng != 0) textCursor.ShowBitmap();

        //////////// Display FONT
        //
        /*
        CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC (黑色背景)
        CFont f, *fp;
        f.CreatePointFont(120, "Consolas");			// 產生 font f; 字體選用等寬字 Consolas,或 Courier
        fp = pDC->SelectObject(&f);					// 選用 font f
        pDC->SetBkMode(TRANSPARENT);				// TEXT背景設定為透明
        pDC->SetTextColor(RGB(20, 20, 20));			// 設定文字顏色

        if (currWordLeng > 0)
            pDC->SetTextColor(RGB(24, 106, 59));	// 若currWordLeng > 0 則文字顏色選為特定顏色

        char str[10];
        sprintf(str, "curr %d", currWordLeng);
        pDC->TextOut(x + dx + bmp.Width() + talkBoxL.Width(), y + dy, vocab.c_str());	// 顯示單字
        pDC->SelectObject(fp);								// 放掉 font f (千萬不要漏了放掉)
        CDDraw::ReleaseBackCDC();
        */

        for (int i = 0; i < length; i++) {
            letter[vocab[i] - 97]->SetTopLeft(x + dx + bmp.Width() + talkBoxL.Width() + letter[0]->Width() * i, y + dy + 3);
            letter[vocab[i] - 97]->ShowBitmap();
        }

        /*
        for (int i = 0; i < length; i++) {
            letter[i]->SetTopLeft(x + dx + bmp.Width() + talkBoxL.Width() + letter[i]->Width() * i, y + dy + 3);
            letter[i]->ShowBitmap();
        }
        */
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

        if (length <= 6)			// 條件成立,使用break跳出迴圈 確定生成此單字
            break;
    }

    //free(dict);		// 釋放掉dict記憶體
    //
    /*
    for (int i = 0; i < length; i++) {
        letter.push_back(new CMovingBitmap);
        char str[20];
        sprintf(str, "Bitmaps/char/%c.bmp", vocab[i]);
        letter.back()->LoadBitmap(str, RGB(255, 255, 255));
    }
    */
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
    //return x + dx + bmp.Width() / 2;
}
int CEnemy::GetY() {
    return y + dy;
    //return y + dy + bmp.Height() / 2;
}
void CEnemy::MinusIndex(int num) {
    index = index - num;
}
bool CEnemy::HitMe(CMe* me) {
    return HitRectangle(me->GetX1(), me->GetY1(),
                        me->GetX2(), me->GetY2());
}
}