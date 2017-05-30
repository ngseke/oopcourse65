#pragma once
#include "CBomb.h"
#include "CDict.h"
#include "CMe.h"
namespace game_framework {

class CEnemy {
    public:
        CEnemy();
        ~CEnemy();
        CEnemy(int x, int y, int delay, bool alive, CDict* dict, int minVL, int maxVL, \
               vector<CEnemy*>* enemyQueue, vector<CBomb*>* bombList, int endX, int endY, \
               vector<CMovingBitmap*>* letter);
        virtual void	LoadBitmap();					// ���J�ϧ�
        virtual void	OnMove();						// ����
        bool		IsAlive();							// �O�_����
        void		OnShow();							// �N�ϧζK��e��
        void		SetXY(int nx, int ny);				// �]�w����
        void		SetIsAlive(bool alive);				// �]�w�O�_����
        void		SetDelay(int d);					// �]�w�����t��

        /////////////

        void		SetVocab();							// �H���qdict������@�ӳ�r��vocab�̭�
        string		GetVocab();							// �^�Ǿ�ճ�r(ex: "apple")
        char		GetFirstWord();						// �Hchar�^�Ǥ@�Ӧr (ex: 'a')
        void		AddCurrWordLeng();					// CurrWord++
        int			GetCurrWordLeng();					// �^��int���A��CurrWord
        int			GetVocabLeng();						// �^�ǳ�r�`����
        int			GetX(), GetY();						// ���oX�b(x+dx), Y�b(y+dy)
        int			GetX2(), GetY2();					// ���oX2 (x+dx+bmp.width), Y2(y+dy+bmp.height)
        void		MinusIndex(int num);				// ���h�Ǫ� num�����h�h��index
        bool		HitMe(CMe* me);						// �O�_�I��D��
        bool		IsBombed();							// �O�_�z���L�F
        virtual void	 kill();
        string		GetBossType();

    protected:
        CMovingBitmap bmp;								// �y����
        CMovingBitmap bmp_center;						// ��ߪ���
        CMovingBitmap textCursor;						// ��r���� ��
        CMovingBitmap talkBoxL, talkBoxC, talkBoxR;		// �u�ƹL����ܮ� ��
        vector<CEnemy*>* enemyQueue;
        vector<CBomb*>* bombList;
        vector<CMovingBitmap*>* letter;					// ��r����
        CDict* dict;				// �r���ɫ���
        string vocab;				// �x�s��r
        string bossType;			// �Ǫ�������
        int x, y;					// ��ߪ��y��
        int dx, dy;					// �y�Z����ߪ��첾�q
        int endX, endY;				// ���檺���I����
        int index;					// �N�Z������index����
        int delay_counter;			// �����t�ת��p�ƾ�
        int delay;					// �������t��
        bool is_alive;				// �O�_����
        bool is_bombed;				// �O�_�z���L�F
        int length;					// ��r�`����
        int currWordLeng;			// ��e��J��r���r��
        int minVocabLeng, maxVocabLeng;		// �W�w�Ǫ��ͦ���r���ת��϶�( minVocabLeng ~ maxVocabLeng)

    private:
        bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��

};
}
