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
    currWordLeng = 0;	  // ��e����Цb0 �٥���J���N�� (ex: ""apple), �YcurrWord = 1 (ex: "a"pple)
}

CEnemy::CEnemy(int x, int y, int delay, bool alive, CDict* d, int minVL, int maxVL) {	//	��l�ȳ��b���B�]�w
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
    int x1 = x + dx;				// �Ǫ�face�����W��x�y��
    int y1 = y + dy;				// �Ǫ�face�����W��y�y��
    int x2 = x1 + bmp.Width();		// �Ǫ�face���k�U��x�y��
    int y2 = y1 + bmp.Height();		// �Ǫ�face���k�U��y�y��
    //
    // �˴��Ǫ�face���x�λP�ѼƯx�άO�_���涰
    //
    return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
}

bool CEnemy::IsAlive() {
    return is_alive;
}

void CEnemy::LoadBitmap() {
    char str[30];
    const unsigned int bitmapNum = 7;		// ���ɼƶq
    sprintf(str, "Bitmaps/face/face%d.bmp", rand() % bitmapNum + 1);
    bmp.LoadBitmap(str, RGB(0, 255, 0)); // ���J �Ǫ�SKIN
    textCursor.LoadBitmap("Bitmaps/text_cursor.bmp", RGB(0, 255, 0));  //���J ����
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

    for (int i = 0; i < 2; i++)		// ���J�ʵe
        target.AddBitmap(filename[i], RGB(0, 255, 0));
}

void CEnemy::OnMove() {
    const int STEPS = 250;	// �����X��dx

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
        dx  = int(dxTemp);  // dx�� (Enemy<->Me��x�`�Z��) / STEPS * index;
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
        // ��}�����talkbox�覡, �����̷ӳ�r����
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
        textCursor.SetTopLeft(x + dx + bmp.Width() + talkBoxL.Width() + ((currWordLeng ) * 10) - 1, y + dy);	// ��ܥ���

        if (currWordLeng != 0) textCursor.ShowBitmap();

        for (int i = 0; i < length; i++) {
            letter[vocab[i] - 97]->SetTopLeft(x + dx + bmp.Width() + talkBoxL.Width() + letter[0]->Width() * i, y + dy + 3);
            letter[vocab[i] - 97]->ShowBitmap();
        }

        for (int i = 0; i < currWordLeng; i++) {	// �����L����r�\�� ��������
            talkBoxC.SetTopLeft(x + dx + bmp.Width() + talkBoxL.Width() + i * talkBoxC.Width(), y + dy);
            talkBoxC.ShowBitmap();
        }
    }
}

////////////
void  CEnemy::SetVocab() {			//�H���qdict������@�ӳ�r��vocab�̭�
    //CDict* dict = new CDict;
    while (1) {
        vocab = dict->GetText();	// ��vocab�@�ӳ�r
        length = vocab.length();

        if (length >= minVocabLeng && length <= maxVocabLeng)			// ���󦨥�,�ϥ�break���X�j�� �T�w�ͦ�����r
            break;
    }
}
string CEnemy::GetVocab() {		  // �^�Ǿ�ճ�r(ex: "apple")
    return vocab;
}
char CEnemy::GetFirstWord()		{ // �Hchar�^�Ǥ@�Ӧr (ex: 'a')
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