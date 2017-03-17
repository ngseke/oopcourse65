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
    currWord = 0;  // ��e����Цb0 �٥���J���N�� (ex: ""apple)
    //�YcurrWord = 1 (ex: "a"pple)
    TRACE("\nvocab length: %d\n", length);  //��X�r��
    SetVocab();
}

bool CEnemy::HitEraser(CEraser* eraser) {
    // �˴����l�Һc�����x�άO�_�I��y
    return HitRectangle(eraser->GetX1(), eraser->GetY1(),
                        eraser->GetX2(), eraser->GetY2());
}

bool CEnemy::HitRectangle(int tx1, int ty1, int tx2, int ty2) {
    int x1 = x + dx;				// �y�����W��x�y��
    int y1 = y + dy;				// �y�����W��y�y��
    int x2 = x1 + bmp.Width();	// �y���k�U��x�y��
    int y2 = y1 + bmp.Height();	// �y���k�U��y�y��
    //
    // �˴��y���x�λP�ѼƯx�άO�_���涰
    //
    return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
}

bool CEnemy::IsAlive() {
    return is_alive;
}

void CEnemy::LoadBitmap() {
    bmp.LoadBitmap("Bitmaps/shake.bmp", RGB(0, 255, 0));			// ���J�y���ϧ�
    // bmp_center.LoadBitmap(IDB_CENTER, RGB(0, 0, 0));	// ���J�y��ߪ��ϧ�
}

void CEnemy::OnMove() {
    if (!is_alive)
        return;

    delay_counter--;
    xMoveDistance = x - (SIZE_X / 2);

    if (delay_counter < 0) {
        delay_counter = delay;
        //
        // �p��y�V����ߪ��첾�qdx, dy
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
        dy = index * 10;   // index(0~STEPS) * ���ʮ��
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
        CDC* pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC (�¦�I��)
        CFont f, *fp;
        f.CreatePointFont(120, "Arial");	// ���� font f;
        fp = pDC->SelectObject(&f);					// ��� font f
        pDC->SetBkColor(RGB(0, 0, 0));
        pDC->SetTextColor(RGB(255, 255, 255));
        pDC->TextOut(x + dx + 40, y + dy + 5, vocab.c_str()); // ���vocab  ( �ഫ: vocab.c_str() )
        pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
        CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
    }
}
////////////
void  CEnemy::SetVocab() {  //�H���qdict������@�ӳ�r��vocab�̭�
    CDict* dict = new CDict;
    vocab = dict->GetText();// ��vocab�@�ӳ�r
    length = vocab.length();
    free(dict); // ����dict�O����
}

string CEnemy::GetVocab() {  //�^�Ǿ�ճ�r(ex: "apple")
    return vocab;
}
char CEnemy::GetFirstWord() { //�Hchar�^�Ǥ@�Ӧr (ex: 'a')
    return vocab[0];
}

void CEnemy::AddCurrWord() {
    currWord++;
}
int CEnemy::GetCurrWord() {
    return currWord;
}

}