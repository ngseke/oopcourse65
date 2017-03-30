#include "CEraser.h"
#include "CBall.h"
#include "CBouncingBall.h"
#include "CEnemy.h"
#include "CBullet.h"
#include "CDict.h"

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////

enum AUDIO_ID {				// �w�q�U�ح��Ī��s��
    AUDIO_DING,				// 0
    AUDIO_LAKE,				// 1
    AUDIO_NTUT				// 2
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////


class CGameStateInit : public CGameState {
    public:
        CGameStateInit(CGame* g);
        void OnInit();  								// �C������Ȥιϧγ]�w
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnKeyUp(UINT, UINT, UINT); 				// �B�z��LUp���ʧ@
        void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
        void OnMove();
    protected:
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
    private:
        CMovingBitmap logo;								// csie��logo
        CMovingBitmap typing_logo;						// typing-typing �����LOGO
        CMovingBitmap text1;							//	������r1
        CAnimation		note1;
};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateRun : public CGameState {
    public:
        CGameStateRun(CGame* g);
        ~CGameStateRun();
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnInit();  								// �C������Ȥιϧγ]�w
        void OnKeyDown(UINT, UINT, UINT);
        void OnKeyUp(UINT, UINT, UINT);
        void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
        void OnLButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
        void OnMouseMove(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
        void OnRButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
        void OnRButtonUp(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
    protected:
        void OnMove();									// ���ʹC������
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
    private:
        const int		NUMBALLS;	// �y���`��

        CMovingBitmap	background;	// �I����
        CMovingBitmap	help;		// ������
        CBall*			ball;		// �y���}�C
        CMovingBitmap	corner;		// ������
        CEraser			eraser;		// ��l
        CInteger		hits_left;	// �ѤU��������
        CBouncingBall   bball;		// ���мu�����y
        //////////
        const bool		SHOW_DEBUG = 0;	// �O�_���debug��T
        int				picX, picY;
        CEnemy*			enemy;
        vector<CEnemy*>	enemy1;
        CEnemy*			targetEnemy;
        CInteger		score;
        const int		LEVEL;											// ���d�`��
        int				callEnemyCounter, maxCallEnemyCounter;			// �l��Ǫ����j�p�ƾ�, �l��Ǫ����j; maxCallEnemyCounter �M�w�Ǫ��ͦ��t�� �V�p�t�׶V��
        int				currEnemyNum;									// ��e�����d �w�l�ꪺ�ĤH�ƶq
        bool			lock;											// �P�_�O�_���Ĥ@�Ӧr���F
        int				currLevel;										// ��e���d
        int				levelEnemyNum[5] = { 20, 25, 30, 35, 40 };		// �����d�̤j���ĤH��
        vector<CBullet*>	bulletList;
        CDict			dictionary;


};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(Game Over)
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState {
    public:
        CGameStateOver(CGame* g);
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnInit();
    protected:
        void OnMove();									// ���ʹC������
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
    private:
        int counter;	// �˼Ƥ��p�ƾ�
};

}