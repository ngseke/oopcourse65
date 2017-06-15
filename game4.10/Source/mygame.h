#pragma once
#include "CCharacter.h"
#include "CMe.h"
#include "CDict.h"
#include "CBomb.h"
#include "CEnemy.h"
#include "CBullet.h"
#include "CMap.h"
#include "CBossA.h"
#include "CBossB.h"
#include "CEmp.h"
#include "CLevel.h"
#include "CRecord.h"
#include "CFile.h"

namespace game_framework {
// Constants
enum AUDIO_ID {				// �w�q�U�ح��Ī��s��
    AUDIO_DING,				// 0
    AUDIO_LAKE,				// 1
    AUDIO_NTUT,				// 2
    //===========================================
    AUDIO_ROCK,             // 3
    AUDIO_SHOT,             // 4
    AUDIO_ERROR,			// 5
    AUDIO_CONGRATULATION,	// 6
    AUDIO_CONGRATULATION2,	// 6
    AUDIO_GAMEOVER			// 7

};

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
// �C��Member function��Implementation���n����
/////////////////////////////////////////////////////////////////////////////
class PublicData {
    public:
        static int			score;						// �@���ܼơG�x�s����
        static int			level;						// �@���ܼơG�x�s���d
        static double		accuracy;					// �@���ܼơG���T�v
        static CMe			me;							// �@���ܼơG�D���Ѽ�
        static CFile		file;

        static int			CorrectKeyCount;		// �@���ܼơG���T�����
        static int			totalKeyCount;			// �@���ܼơG���T�����(�|�ֿn��)
        static bool			musicOnOff;					// �@���ܼơG���ֶ}��
        static bool			newUnlock;					// �@���ܼơG�O�_���s�����ꨤ��

        static bool			debugMode;
};

class CGameStateInit : public CGameState {
    public:
        CGameStateInit(CGame* g);
        ~CGameStateInit();
        void OnInit();  								// �C������Ȥιϧγ]�w
        void OnBeginState();							// �]�w�C�������һݪ��ܼ�
        void OnKeyUp(UINT, UINT, UINT); 				// �B�z��LUp���ʧ@
        void OnLButtonDown(UINT nFlags, CPoint point);  // �B�z�ƹ����ʧ@
        void OnMouseMove(UINT nFlags, CPoint point);	// �B�z�ƹ����ʧ@
        void OnMove();
    protected:
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
    private:
        const int NOTE_TEXT_X, NOTE_TEXT_Y;				// �w�q �C������ �\�񪺦�m
        const int MENU_Y;								// �w�q MENU ��Y�b
        const int MENU_ITEM_NUM;						// �w�q MENU ���ت��ƶq
        const int CHARACTER_Y;							// �w�q ������ �� ����Y�b
        CMovingBitmap typing_logo, taipin;				// typing typing �����LOGO
        CMovingBitmap text1;							// ������r
        int text1_y, text1_count;						// ������r���X�ĪG
        CMap map;										// �I��
        int currSelectItem, displayState;				// ��e��ܪ�MENU, ��e��ܪ����A
        int noteDisplayState, statsDisplayState, aboutDisplayState;		// ��e��ܪ��������A
        int statsPRItemNum;								// �C���O�������ؼƦr
        int wrongKeyNum;								// ���~�����
        int exitGameCount;								// �����C���p��
        bool exitState = false;
        //�D��椸��
        vector<CMovingBitmap*>	menuText;						// ����rVECTOR
        CMovingBitmap			menuBorder, menuBorder_ckecked;	// �������ܿO
        CMovingBitmap			userBorder, highScoreBorder;	// �̰�������
        CMovingBitmap			numBmp[10], numBmpSmall[14];	// �Ʀr����
        CMovingBitmap			numBmp_White[10], numBmpSmall_White[14];	// �Ʀr���ɡ]�զ�^
        CMovingBitmap			new_text;
        CMovingBitmap			exit;
        // �C������ ����
        CAnimation				noteExkey;						// �C�������̭��� ���r�ʵe
        CMovingBitmap			noteBorder, noteArrow;			// �ؽu, �b�Y
        CMovingBitmap			noteSelected, noteUnselected;	// ���ܿO
        vector<CMovingBitmap*>	note;							// �h����������r
        // ������ ����
        CMovingBitmap			characterBorder, characterArrow;// �����ܮ� �b�Y
        // �έp	   ����
        CMovingBitmap			statsBorder, statsBg[2];		// �έp������, �έp�������k��
        CMovingBitmap			statsArrow[3], statsArrowV[4];	// ���k�b�Y, �W�U�|�ت��A�b�Y
        CMovingBitmap			statsText [4];					// �������ؤ�r
        CMovingBitmap			statsNoRecord;					// �L�O�� ��r
        // ���Э��� ����
        string					cheatCode;						// �x�s�@���X
        CMovingBitmap			aboutBorder, about;				// �����, �����r
        CMovingBitmap			delText;						// �T�{�R����r
        CMovingBitmap			musicOnOff[2];

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
        bool			showDebug = false;				// �O�_���debug��T
        bool			quickCall = false;				// �O�_�}�ҧֳt�l��
        CMovingBitmap	pauseText;										// �Ȱ�������r
        CMovingBitmap	debugText;										// debug���ܤ�r
        CAnimation		target;											// ��w���ʵe
        vector<CMovingBitmap*>	letter;
        vector<CEnemy*> enemyQueue;										// �x�s�Ҧ��ĤH��Vector
        vector<CBullet*>bulletList;										// �x�s���椤���l�u��Vector
        vector<CBomb*>  bombList;										// �x�s�z���ĪG��vector
        //
        CEnemy* 		targetEnemy;									// ���� �Ω���V�˷Ǫ��ĤH
        CDict			dictionary;										// �Ҧ��Ǫ��@�Ϊ��r��
        CMap			map;											// �I����
        CInteger		score;											// ������ܾ�
        CEmp			emp;											// �q�Ϫi
        CLevel			levelAni;										// �������d�ɪ��ʵe
        //
        const int		LEVEL;											// ���d�`��
        char			key;											// �O���ҫ��U������ �Ω󨾤�u��
        bool			lock;											// �P�_�O�_���Ĥ@�Ӧr���F
        bool			pause;
        int				lives;											// �ͩR��
        int				levelEnemyNum[30] = { 4,  5,  5,  6,  7,  7,  7,  7,  8,  9, 10, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 17 };		// �����d�̤j���ĤH��
        int				levelBossANum[30] = { 0,  0,  1,  1,  2,  1,  1,  1,  1,  2,  1,  2,  2,  2,  3,  3,  3,  3,  3,  3,  4,  4,  4,  5,  5,  5,  5,  5,  5,  7 };
        int				levelBossBNum[30] = { 0,  0,  0,  0,  0,  1,  1,  2,  2,  1,  2,  1,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4 };
        int				callEnemyCounter, maxCallEnemyCounter;			// �l��Ǫ����j�p�ƾ�, �l��Ǫ����j; maxCallEnemyCounter �M�w�Ǫ��ͦ��t�� �V�p�t�׶V��
        int				callBossACounter, maxCallBossACounter;
        int				callBossBCounter, maxCallBossBCounter;
        int				currEnemyNum;									// ��e�����d �w�l�ꪺ�ĤH�ƶq
        int				currBossANum, currBossBNum;						// ��e�����d �w�l�ꪺBossA & BossB�ƶq
        int				currLevel;										// ��e���d
        int				totalEnemyNum;									// �`�l�ꪺ�ĤH�ƶq
        int				levelChangeFlag, levelChangeDelay, levelChangeDelayMax; // ���d�M���d����delay
        int				totalKeyDownCount;								// �`�����, �`���T�����
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
        void OnKeyUp(UINT, UINT, UINT);
    protected:
        void OnMove();									// ���ʹC������
        void OnShow();									// ��ܳo�Ӫ��A���C���e��
    private:
        CMovingBitmap		border;								// �ؽu
        CMovingBitmap		numBmp[10], numBmpSmall[12];		// �Ʀr����
        CMovingBitmap		bar[2];			// �i�ױ�
        CAnimation			newHS_text;		// �}������r
        CAnimation			newChar_text;
        bool				isHighScore;	// �����C�����O�_�}����
        bool				isUnlock;		// �����C�����O�_����s����
        int					counter;		// �˼Ƥ��p�ƾ�
        int					x, y;			// ������ܦ�m
        int					score, level;	// ����, ���d
        double				accuracy;		// ���T�v
        int					barCounter;		// �i�ױ����p�ƾ�

};

}