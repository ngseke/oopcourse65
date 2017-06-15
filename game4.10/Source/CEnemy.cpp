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
#include <math.h>
#include <stdlib.h>

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// CEnemy: Enemy
/////////////////////////////////////////////////////////////////////////////
CEnemy::CEnemy() {
}
CEnemy::~CEnemy() {
}
CEnemy::CEnemy(int x, int y, int delay, bool alive, CDict* d, int minVL, int maxVL, vector<CEnemy*>* enemyQueue, vector<CBomb*>* bombList, int endX, int endY, vector<CMovingBitmap*>* letter) {	//	��l�ȳ��b���B�]�w
    is_alive = is_bombed = false;
    dx = dy = index = delay_counter = 0;
    currWordLeng = 0;
    bossType = "enemy";
    ////
    SetXY(x, y);
    SetDelay(delay);
    SetIsAlive(alive);
    this->dict = d;
    this->bombList = bombList;
    this->enemyQueue = enemyQueue;
    this->minVocabLeng = minVL;
    this->maxVocabLeng = maxVL;
    this->endX = endX;
    this->endY = endY;
    this->letter = letter;
    SetVocab();
    index = (maxVocabLeng == 1 && minVocabLeng == 1) ? 30 : 0;
}

bool CEnemy::HitRectangle(int tx1, int ty1, int tx2, int ty2) {
    int x1 = x + dx;				// �Ǫ�face�����W��x�y��
    int y1 = y + dy;				// �Ǫ�face�����W��y�y��
    int x2 = x1 + bmp.Width();		// �Ǫ�face���k�U��x�y��
    int y2 = y1 + bmp.Height();		// �Ǫ�face���k�U��y�y��
    return (tx2 >= x1 && tx1 <= x2 && ty2 >= y1 && ty1 <= y2);	// �˴��Ǫ�face���x�λP�ѼƯx�άO�_���涰
}

bool CEnemy::IsAlive() {
    return is_alive;
}

void CEnemy::LoadBitmap() {
    char str[30];
    const unsigned int bitmapNum = 7;										// �����`�ƶq

    if (maxVocabLeng == 1 && minVocabLeng == 1)								// �H���D�� bitmap
        sprintf(str, "Bitmaps/face/face_min%d.bmp", rand() % (4) + 1);		// 1�r�p�Ǫ�bmp
    else {
        sprintf(str, "Bitmaps/face/face%d.bmp", rand() % bitmapNum + 1);	// �@��p�Ǫ�bmp
    }

    bmp.LoadBitmap(str, RGB(0, 255, 0));						// ���J �Ǫ�SKIN
    textCursor.LoadBitmap(IDB_TEXT_CURSOR, RGB(0, 255, 0));		// ���J ����
    talkBoxL.LoadBitmap(IDB_TALK_BOX_LEFT, RGB(0, 255, 0));		// ���J ��ܮإ�
    talkBoxC.LoadBitmap(IDB_TALK_BOX_CENTER, RGB(0, 255, 0));	// ���J ��ܮؤ�
    talkBoxR.LoadBitmap(IDB_TALK_BOX_RIGHT, RGB(0, 255, 0));	// ���J ��ܮإk
}

void CEnemy::OnMove() {
    const int STEPS = 180;	// �����X��dx

    if (!is_alive) return;

    delay_counter--;

    if (delay_counter < 0) {
        delay_counter = delay;
        index++;
        double dxTemp = (double(endX) - x) / STEPS * index;
        double dyTemp = (double(endY) - y) / STEPS * index;
        dx  = int(dxTemp);  // dx�� (Enemy<->Me��x�`�Z��) / STEPS * index;
        dy  = int(dyTemp);
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

        if (currWordLeng < length) {	// �Y��e��J�쪺�r < �`���� �h��ܹ�ܮ�
            talkBoxL.SetTopLeft(x + dx + bmp.Width(), y + dy);
            talkBoxL.ShowBitmap();		// ��� ��ܮإ�

            for (int i = 0; i < length; i++) {		// ��� �ƭӹ�ܮؤ�(�ھڳ�r����)
                talkBoxC.SetTopLeft(x + dx + bmp.Width() + talkBoxL.Width() + i * talkBoxC.Width(), y + dy);
                talkBoxC.ShowBitmap();
            }

            talkBoxR.SetTopLeft(x + dx + bmp.Width() + talkBoxL.Width() + length * talkBoxC.Width(), y + dy);
            talkBoxR.ShowBitmap();		// ��� ��ܮإk
            textCursor.SetTopLeft(x + dx + bmp.Width() + talkBoxL.Width() + ((currWordLeng ) * 10) - 1, \
                                  y + dy);
            (currWordLeng != 0) ? textCursor.ShowBitmap() : 0;	// �Y��e��J�쪺�r!=0 �h��ܥ���

            for (int i = 0; i < length; i++) {	// ��ܳ�rbmp
                letter->at(vocab[i] - 97)->SetTopLeft(x + dx + bmp.Width() + talkBoxL.Width() + letter->at(0)->Width() * i, y + dy + 4);
                letter->at(vocab[i] - 97)->ShowBitmap();
            }

            for (int i = 0; i < currWordLeng; i++) {	// �A�����talkBoxC, �����L����r �Q�\����������
                talkBoxC.SetTopLeft(x + dx + bmp.Width() + talkBoxL.Width() + i * talkBoxC.Width(), y + dy);
                talkBoxC.ShowBitmap();
            }
        }
    }
}
void CEnemy::kill() {
    is_alive = false;
    bombList->push_back(new CBomb(GetX(), GetY()));		// new�z���S�Ķivecor
    bombList->back()->LoadBitmap();
    is_bombed = true;
}


void  CEnemy::SetVocab() {								// �H���qdict������@�ӳ�r��vocab�̭�
    while (1) {
        if (maxVocabLeng == 1 && minVocabLeng == 1) {	// �Y��1�r�p��
            vocab = "a";								// ���H�N����1�rstring
            vocab[0] = 97 + rand() % 26;				// �H���D���r
            length = 1;
        }
        else {											// �@����שǪ�
            vocab = dict->GetText();					// �q�r�夤�H���D���r��vocab
            length = vocab.length();					// ����= ��r����
        }

        if (length >= minVocabLeng && length <= maxVocabLeng) {	 // ���ײŦX ,�ϥ�break���X�j�� �T�w�ͦ�����r
            bool firstWordBounceFlag = 0;						 // ������Ĥ@�ӳ�r��flag

            for (int i = enemyQueue->size() - 1; i >= 0; i--) {	 // �P���W�Ҧ��Ǫ����, �T�w�O�_���r��r����
                if (vocab[0] == enemyQueue->at(i)->GetFirstWord() && enemyQueue->at(i)->IsAlive())
                    firstWordBounceFlag = 1;
            }

            if (!(firstWordBounceFlag && enemyQueue->size() < 25))
                break;	// �Y�S����B ���W�Ǫ��ƶq�p��26
        }
    }
}

string CEnemy::GetVocab() {			// �^�Ǿ�ճ�r(ex: "apple")
    return vocab;
}
char CEnemy::GetFirstWord() {		// �Hchar�^�Ǥ@�Ӧr (ex: 'a')
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
int CEnemy::GetX2() {
    return x + dx + bmp.Width();
}
int CEnemy::GetY2() {
    return y + dy + bmp.Height();
}
void CEnemy::MinusIndex(int num) {
    index = index - num;
}
bool CEnemy::HitMe(CMe* me) {
    return HitRectangle(me->GetX1(), me->GetY1(),
                        me->GetX2(), me->GetY2());
}
bool CEnemy::IsBombed() {
    return is_bombed;
}
string CEnemy::GetBossType() {
    return bossType;
}
}