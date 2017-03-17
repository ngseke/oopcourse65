#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CEraser.h"
#include "CEnemy.h"
#include "CDict.h"
#

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// CEnemy: Ball class
/////////////////////////////////////////////////////////////////////////////

CEnemy::CEnemy() {
    is_alive = true;
    x = y = dx = dy = index = delay_counter = 0;
    currWord = 0;  // 當前的游標在0 還未輸入的意思 (ex: ""apple)
    //若currWord = 1 (ex: "a"pple)
    TRACE("\nvocab length: %d\n", length);  //輸出字數
    SetVocab();
}

bool CEnemy::HitEraser(CEraser* eraser) {
    // 檢測擦子所構成的矩形是否碰到球
    return HitRectangle(eraser->GetX1(), eraser->GetY1(),
                        eraser->GetX2(), eraser->GetY2());
}

bool CEnemy::HitRectangle(int tx1, int ty1, int tx2, int ty2) {
    int x1 = x + dx;				// 球的左上角x座標
    int y1 = y + dy;				// 球的左上角y座標
    int x2 = x1 + bmp.Width();	// 球的右下角x座標
    int y2 = y1 + bmp.Height();	// 球的右下角y座標
    //
    // 檢測球的矩形與參數矩形是否有交集
    //
    return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
}

bool CEnemy::IsAlive() {
    return is_alive;
}

void CEnemy::LoadBitmap() {
    bmp.LoadBitmap("Bitmaps/shake.bmp", RGB(0, 255, 0));			// 載入球的圖形
    // bmp_center.LoadBitmap(IDB_CENTER, RGB(0, 0, 0));	// 載入球圓心的圖形
}

void CEnemy::OnMove() {
    if (!is_alive)
        return;

    delay_counter--;
    xMoveDistance = x - (SIZE_X / 2);

    if (delay_counter < 0) {
        delay_counter = delay;
        //
        // 計算球向對於圓心的位移量dx, dy
        //
        /*
        const int STEPS = 18;
        static const int DIFFX[] = { 35, 32, 26, 17, 6, -6, -17, -26, -32, -34, -32, -26, -17, -6, 6, 17, 26, 32, };
        static const int DIFFY[] = { 0, 11, 22, 30, 34, 34, 30, 22, 11, 0, -11, -22, -30, -34, -34, -30, -22, -11, };
        index++;

        if (index >= STEPS)
            index = 0;

        dx = DIFFX[index];
        dy = DIFFY[index];
        */
        const int STEPS = SIZE_Y / 10 ;
        index++;

        if (index >= STEPS)
            index = 0;

        dx = - xMoveDistance / STEPS * index;
        dy = index * 10;   // index(0~STEPS) * 移動格數
        //if (background.Top() > SIZE_Y)
        //   background.SetTopLeft(60, -background.Height());
        //background.SetTopLeft(background.Left(), background.Top() + 1);
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
        //bmp_center.SetTopLeft(x + dx, y + dy);
        //bmp_center.ShowBitmap();
        /////////
        //////////// Display FONT
        CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC (黑色背景)
        CFont f, *fp;
        f.CreatePointFont(120, "Arial");	// 產生 font f;
        fp = pDC->SelectObject(&f);					// 選用 font f
        pDC->SetBkColor(RGB(0, 0, 0));
        pDC->SetTextColor(RGB(255, 255, 255));
        pDC->TextOut(x + dx + 40, y + dy + 5, vocab.c_str()); // 顯示vocab  ( 轉換: vocab.c_str() )
        pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
        CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
    }
}
////////////
void  CEnemy::SetVocab() {  //隨機從dict中抓取一個單字到vocab裡面
    CDict* dict = new CDict;
    vocab = dict->GetText();// 給vocab一個單字
    length = vocab.length();
    free(dict); // 釋放掉dict記憶體
}

string CEnemy::GetVocab() {  //回傳整組單字(ex: "apple")
    return vocab;
}
char CEnemy::GetFirstWord() { //以char回傳一個字 (ex: 'a')
    return vocab[0];
}

void CEnemy::AddCurrWord() {
    currWord++;
}
int CEnemy::GetCurrWord() {
    return currWord;
}

}