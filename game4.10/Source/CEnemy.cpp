#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <time.h>
#include "audio.h"
#include "gamelib.h"
#include "CEraser.h"
#include "CEnemy.h"
#include "CDict.h"

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// CEnemy: Ball class
/////////////////////////////////////////////////////////////////////////////

CEnemy::CEnemy() {
    is_alive = false;
    x = y = dx = dy = index = delay_counter = 0;
    currWordLeng = 0;	  // ��e����Цb0 �٥���J���N�� (ex: ""apple), �YcurrWord = 1 (ex: "a"pple)
    SetVocab();
}
CEnemy::CEnemy(int x, int y, int delay, bool alive) {
    dx = dy = index = delay_counter = 0;
    currWordLeng = 0;
    ////
    SetXY(x, y);
    SetDelay(delay);
    SetIsAlive(alive);
    //useDict =  dict;
    SetVocab();
}
bool CEnemy::HitEraser(CEraser* eraser) {
    // �˴�(���l�Һc�����x��)�O�_�I��y
    // �ˬd�Ǫ��O�_�I��D��
    return HitRectangle(eraser->GetX1(), eraser->GetY1(),
                        eraser->GetX2(), eraser->GetY2());
}

bool CEnemy::HitRectangle(int tx1, int ty1, int tx2, int ty2) {
    int x1 = x + dx;				// �y�����W��x�y��
    int y1 = y + dy;				// �y�����W��y�y��
    int x2 = x1 + bmp.Width();		// �y���k�U��x�y��
    int y2 = y1 + bmp.Height();		// �y���k�U��y�y��
    //
    // �˴��y���x�λP�ѼƯx�άO�_���涰
    //
    return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);
}

bool CEnemy::IsAlive() {
    return is_alive;
}

void CEnemy::LoadBitmap() {
    char* faceFile[] = { "Bitmaps/face1.bmp", "Bitmaps/face2.bmp", "Bitmaps/face3.bmp", "Bitmaps/face4.bmp", "Bitmaps/face5.bmp", "Bitmaps/face6.bmp", "Bitmaps/face7.bmp" };		// �x�s�Ǫ��ɮ׸��|���}�C
    bmp.LoadBitmap(faceFile[(rand() % 7)], RGB(0, 255, 0)); // ���J �Ǫ�SKIN
    textCursor.LoadBitmap("Bitmaps/text_cursor.bmp", RGB(0, 255, 0));  //���J ����
    /////
}
void CEnemy::LoadTextbox() {
    // �ھڳ�r���� ���J���P�e�׹�ܮ�
    if (length <= 3)
        talkBox.LoadBitmap("Bitmaps/talk_box_3words.bmp", RGB(0, 255, 0));
    else if (length <= 5)
        talkBox.LoadBitmap("Bitmaps/talk_box_5words.bmp", RGB(0, 255, 0));
    else if (length <= 10)
        talkBox.LoadBitmap("Bitmaps/talk_box_10words.bmp", RGB(0, 255, 0));
    else
        talkBox.LoadBitmap("Bitmaps/talk_box_15words.bmp", RGB(0, 255, 0));
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
        const int STEPS = SIZE_Y / 3 ; // �C������5��
        index++;

        if (index >= STEPS)
            index = 0;

        dx = - xMoveDistance / STEPS * index;
        dy = index * 3;  // index(0~STEPS) * ���ʮ��
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
        talkBox.SetTopLeft(x + dx + bmp.Width(), y + dy);
        talkBox.ShowBitmap();
        textCursor.SetTopLeft(x + dx + bmp.Width() + 5 + ((currWordLeng + 1) * 9), y + dy);	// ��ܥ���

        if (currWordLeng != 0) textCursor.ShowBitmap();

        //////////// Display FONT
        //
        CDC* pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC (�¦�I��)
        CFont f, *fp;
        f.CreatePointFont(120, "Consolas");			// ���� font f; �r���ε��e�r Consolas,�� Courier
        fp = pDC->SelectObject(&f);					// ��� font f
        pDC->SetBkMode(TRANSPARENT);				// TEXT�I���]�w���z��
        pDC->SetTextColor(RGB(20, 20, 20));
        char str[10];
        sprintf(str, "curr %d", currWordLeng);
        pDC->TextOut(x + dx + bmp.Width() + 13, y + dy, vocab.c_str());	// ��ܳ�r
        //pDC->TextOut(x + dx + 40, y + dy + 20, str);			// �Ȯ�DEBUG��:���CURR
        pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
        CDDraw::ReleaseBackCDC();					// �� Back Plaisn �� CDC
    }
}

////////////
void  CEnemy::SetVocab() {			//�H���qdict������@�ӳ�r��vocab�̭�
    CDict* dict = new CDict;

    while (1) {
        vocab = dict->GetText();	// ��vocab�@�ӳ�r
        length = vocab.length();

        if (length <= 100)			// ���󦨥�,�ϥ�break���X�j�� �T�w�ͦ�����r
            break;
    }

    free(dict);		// ����dict�O����
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
} int CEnemy::GetY() {
    return y + dy;
}
}