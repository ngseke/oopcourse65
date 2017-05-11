#include "CMe.h"
#include "CDict.h"
#include "CBomb.h"
#include "CEnemy.h"
#include "CBullet.h"
#include "CMap.h"
#include "CBossA.h"
#include "CBossB.h"
#include "CEmp.h"


namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// Constants
/////////////////////////////////////////////////////////////////////////////

enum AUDIO_ID {				// �w�q�U�ح��Ī��s��
    AUDIO_DING,				// 0
    AUDIO_LAKE,				// 1
    AUDIO_NTUT,				// 2
    //===========================================
    AUDIO_ROCK,             // 3
    AUDIO_SHOT              // 4
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
        const int NOTE_TEXT_X, NOTE_TEXT_Y;				// �w�q �C������ �\�񪺦�m
        const int MENU_POS_Y;							// �w�q MENU ��Y�b
        const int MENU_ITEM_NUM;						// �w�q MENU ���ت��ƶq
        const int CHARACTER_POS_Y;
        CMovingBitmap typing_logo;						// typing typing �����LOGO
        CMovingBitmap text1;							// ������r
        CMap map;										// �I��
        //�D��椸��
        CMovingBitmap menuBorder, menuBorder_ckecked;
        vector<CMovingBitmap*> menuText;				// ����rVECTOR
        int currSelectItem, displayState, noteDisplayState; // ��e��ܪ�MENU, ��e��ܪ����A, ��e��ܪ��������A

        // �C������ ����
        CAnimation				noteExkey;						// �C�������̭��� ���r�ʵe
        CMovingBitmap			noteBorder, noteArrow;			// �ؽu, �b�Y
        CMovingBitmap			noteSelected, noteUnselected;	// ���ܿO
        vector<CMovingBitmap*>	note;							// �h����������r
        // ������ ����
        CMovingBitmap			characterBorder, characterArrow[2];
        CMovingBitmap			me_ironman;
        // ���Э���
        CMovingBitmap			aboutBorder;
        CMovingBitmap			about;


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
        CMovingBitmap	corner;		// ������
        //////////
        bool			showDebug = false;								// �O�_���debug��T
        CDict			dictionary;										// �Ҧ��Ǫ��@�Ϊ��r��
        CMap			map;											// �I����
        CMe				me;												// �D��
        vector<CEnemy*> enemyQueue;										// �x�s�Ҧ��ĤH��Vector
        vector<CBullet*>bulletList;										// �x�s���椤���l�u��Vector
        vector<CBomb*>  bombList;

        CEnemy* 			targetEnemy;									// ���� �Ω���V�˷Ǫ��ĤH
        CInteger		score;											// ������ܾ�
        CEmp			emp;
        const int		LEVEL;											// ���d�`��
        int				callEnemyCounter, maxCallEnemyCounter;			// �l��Ǫ����j�p�ƾ�, �l��Ǫ����j; maxCallEnemyCounter �M�w�Ǫ��ͦ��t�� �V�p�t�׶V��
        int				callBossACounter, maxCallBossACounter;
        int				callBossBCounter, maxCallBossBCounter;
        int				currEnemyNum;									// ��e�����d �w�l�ꪺ�ĤH�ƶq
        int				currBossANum, currBossBNum;						// ��e�����d �w�l�ꪺBossA & BossB�ƶq
        bool			lock;											// �P�_�O�_���Ĥ@�Ӧr���F
        int				currLevel;										// ��e���d

        int				levelEnemyNum[10] = {  4,  5,  5,  6,  7,  7,  7, 20, 20, 20 };		// �����d�̤j���ĤH��
        int				levelBossANum[10] = {  0,  0,  1,  1,  1,  2,  1,  0,  0,  0 };
        int				levelBossBNum[10] = {  0,  0,  0,  0,  0,  0,  1,  0,  0,  0 };

        int				lives;
        int				totalKeyDownCount, totalCorrectKeyCount;		// �`�����, �`���T�����
        double			accuracy;										// ���T�v

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