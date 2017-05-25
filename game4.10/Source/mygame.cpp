/*
3/24
�w��BUG :		[�ѨM�F] 1. ���s�򪺳�r(bee), ����ĤG��e�N�|��������. �i��Okeydown���F��.
				[�ѨM�F] 2. ��p���W�Ⱖ��r���u(appl"e" "e"gg), �b����apple�̫᪺"e"��, �L�|��������egg�}�Y��"e"
				[�ѨM�F] 3. ���F����r���M���W����, �|���w�gnew���Ǫ���, ���o�ݭn���s��emeny SetVocab,
	    					�o�y���Feneny�bLoadBitmap, ��ܮت��׬O���d�b�����length, �ӫD�sSetVocab
							�᪺���ӳ�r����. ���]���୫�sLoadBitmap, �|�X��, �Ȯ�����ܮت��׳��@��.
				[�ѨM�F] 4. �Ǫ��V�h���ɭԷ|�}�l�� -> �O�]���ϥΦѮv������ܤ�r�����Y,
					���өҦ���r������bitmap���ܴN�i�H�ѨM
				[�ѨM�F] 5. float������, �b�p��dx���ɭԵo�ͪ�
				[�ѨM�F] 6. #pragma once

[Q&A]			1. ERROR: "�Nĵ�i�������~�B�z - �S������ 'object' �ɮ�",
					�i��O�ϥ�for�j��M.size()��, �S����]�j��Ϊ�i�]�w�� unsigned int.

*/
#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <time.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
/////////////////////////////////////////////////////////////////////////////

int PublicData::score = 0;
int PublicData::level = 0;
double PublicData::accuracy = 0.0;
CMe	PublicData::me;

CGameStateInit::CGameStateInit(CGame* g)
    : CGameState(g), NOTE_TEXT_X(60), NOTE_TEXT_Y(280), MENU_POS_Y(320),
      MENU_ITEM_NUM(5), CHARACTER_POS_Y(320) {
}

CGameStateInit::~CGameStateInit() {
    for (CMovingBitmap* mt : menuText) delete mt;

    for (CMovingBitmap* nt : note) delete nt;
}
void CGameStateInit::OnInit() {
    ShowInitProgress(0);	// �@�}�l��loading�i�׬�0%
    const unsigned int exkeyNum = 6;										// �����ظ̭�������ʵe �ƶq
    currSelectItem = displayState = 0;										// ��l�ƿ��������
    noteDisplayState = 0;
    PublicData::me.LoadBitmap();											// �D��
    map.LoadBitmap();														// �I�������ʵe
    typing_logo.LoadBitmap("Bitmaps/start_logo1.bmp", RGB(0, 255, 0));		// logo
    taipin.LoadBitmap("Bitmaps/taipin.bmp", RGB(0, 255, 0));
    text1.LoadBitmap("Bitmaps/text1_start.bmp", RGB(0, 255, 0));			// �ާ@�覡����bitmap
    highScoreBorder.LoadBitmap("Bitmaps/menu/highscore_border.bmp", RGB(0, 255, 0));
    /////
    //
    // ���J��椸��
    ShowInitProgress(5);

    for (int i = 0; i < 5; i++) {		// 5�ӿ��
        char str[50];
        sprintf(str, "Bitmaps/menu/menu_t_%de.bmp", i + 1);
        menuText.push_back(new CMovingBitmap);
        menuText.back()->LoadBitmap(str, RGB(0, 255, 0));
    }

    ShowInitProgress(10);
    menuText.push_back(new CMovingBitmap);
    menuText.back()->LoadBitmap("Bitmaps/menu/menu_t_be.bmp", RGB(0, 255, 0));	// ��^���s
    menuBorder.LoadBitmap("Bitmaps/menu/menu_border.bmp", RGB(0, 255, 0));
    menuBorder_ckecked.LoadBitmap("Bitmaps/menu/menu_border_checked.bmp", RGB(0, 255, 0));
    // ���J�C����������
    noteBorder.LoadBitmap("Bitmaps/menu/note/note_text_border.bmp", RGB(0, 255, 0)); // �����ؽu
    noteArrow.LoadBitmap ("Bitmaps/menu/note/note_text_arraw.bmp", RGB(0, 255, 0));	// �����b�Y
    noteUnselected.LoadBitmap("Bitmaps/menu/note/note_unselected.bmp", RGB(0, 255, 0));
    noteSelected.LoadBitmap("Bitmaps/menu/note/note_selected.bmp", RGB(0, 255, 0));
    ShowInitProgress(15);

    for (int i = 0; i < exkeyNum; i++) {	// �����ظ̭�������ʵe
        char str[50];
        sprintf(str, "Bitmaps/menu/note/note1_exkey_%d.bmp", i + 1);
        noteExkey.AddBitmap(str, RGB(0, 255, 0));
    }

    for (int i = 0; i < 8; i++) {		// �h����������r
        char str[50];
        sprintf(str, "Bitmaps/menu/note/note_text_p%d.bmp", i + 1);
        note.push_back(new CMovingBitmap);
        note.back()->LoadBitmap(str, RGB(0, 255, 0));
    }

    for (int i = 0; i < 10; i++) {		// ���J�Ʀr��
        char str[50];
        sprintf(str, "Bitmaps/level/num/%d.bmp", i);
        numBmp[i].LoadBitmap(str, RGB(0, 255, 0));
        sprintf(str, "Bitmaps/level/num_s/%d.bmp", i);
        numBmpSmall[i].LoadBitmap(str, RGB(0, 255, 0));
    }

    ShowInitProgress(20);
    // ���J������ ����
    characterBorder.LoadBitmap("Bitmaps/menu/character/character_border.bmp", RGB(0, 255, 0));
    characterArrow.LoadBitmap("Bitmaps/menu/character/character_arraw.bmp", RGB(0, 255, 0));
    // ���J���󤸯�
    aboutBorder.LoadBitmap("Bitmaps/menu/about/about_border.bmp", RGB(0, 255, 0)); // ���Юؽu
    about.LoadBitmap("Bitmaps/menu/about/about_text_p2.bmp", RGB(0, 255, 0)); // ���Ф�r
    //
    // ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
    //
}

void CGameStateInit::OnBeginState() {
    text1_y = 550;
    text1_count = 0;
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
    const char KEY_ESC = 27;
    const char KEY_SPACE = ' ';
    const char KEY_ENTER = 0xD;
    const char KEY_LEFT = 0x25;	// keyboard���b�Y
    const char KEY_UP = 0x26;	// keyboard�W�b�Y
    const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
    const char KEY_DOWN = 0x28; // keyboard�U�b�Y

    if (nChar == KEY_ESC)   displayState = 0; // ESC���^�D���

    if (displayState == 0 ) { // �b�D���...
        if (nChar == KEY_UP || nChar == KEY_DOWN) {		// ���ʥ���
            if (nChar == KEY_UP) currSelectItem--;
            else if (nChar == KEY_DOWN) currSelectItem++;

            if (currSelectItem < 0)currSelectItem = MENU_ITEM_NUM - 1;
            else if (currSelectItem > MENU_ITEM_NUM - 1)currSelectItem = 0;
        }
        else if (nChar == KEY_ENTER) {					// ���UENTER����...
            if (currSelectItem == 0) {
                GotoGameState(GAME_STATE_RUN);			// �}�l�C��
            }
            else if ( currSelectItem == 1 ) {
                displayState = 1;						// ��ܹC��������state
                noteDisplayState = 0;
            }
            else if (currSelectItem == 2) {
                displayState = 2;						// �����ܪ�state
            }
            else if (currSelectItem == 3) {
                // �έp��state
            }
            else if (currSelectItem == 4) {
                displayState = 4;	// ����state
            }
        }
    }
    else if (displayState == 1 ) {							// [�C������]
        if (nChar == KEY_ENTER) displayState = 0;			//  ->��^�D���
        else if (nChar == KEY_LEFT || nChar == KEY_RIGHT) { // [�C������] ���k½���C������
            if		(nChar == KEY_LEFT)   noteDisplayState--;
            else if (nChar == KEY_RIGHT) noteDisplayState++;

            if (noteDisplayState < 0) noteDisplayState = int(note.size()) - 1;
            else if (noteDisplayState >  int(note.size()) - 1) noteDisplayState = 0;
        }
    }
    else if (displayState == 2) { // [������]
        if (nChar == KEY_ENTER) displayState = 0;	// ->��^�D���
        else if (nChar == KEY_LEFT || nChar == KEY_RIGHT) {
            if		(nChar == KEY_LEFT)   PublicData::me.AddSelectedChar(-1);
            else if (nChar == KEY_RIGHT)  PublicData::me.AddSelectedChar(1);
        }
    }
    else if (displayState == 3 && nChar == KEY_ENTER) { // [�έp]
    }
    else if (displayState == 4 && nChar == KEY_ENTER) { // [����]
        if (nChar == KEY_ENTER) displayState = 0;	// ->��^�D���
    }
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point) {}
void CGameStateInit::OnMouseMove(UINT nFlags, CPoint point) {}
void CGameStateInit::OnMove() {
    noteExkey.OnMove();
    map.OnMove();
    PublicData::me.OnMove();

    if (text1_count < 400)text1_count++;

    if (text1_count > 5 * 30)  text1_y += int((text1_count - 5 * 30) * 1.1);
}

void CGameStateInit::OnShow() {
    map.OnShow();
    typing_logo.SetTopLeft((SIZE_X - typing_logo.Width()) / 2, 100);
    typing_logo.ShowBitmap();
    taipin.SetTopLeft((SIZE_X + typing_logo.Width()) / 2 - 60, 100 + 31);
    taipin.ShowBitmap();

    if (displayState == 0) {	// ��ܥD���
        const int MENU_MARGIN_BTM = 40;
        menuBorder_ckecked.SetTopLeft((SIZE_X - menuBorder.Width()) / 2, MENU_POS_Y + MENU_MARGIN_BTM * currSelectItem);
        menuBorder_ckecked.ShowBitmap();

        for (int i = 0; i < MENU_ITEM_NUM; i++) {
            menuBorder.SetTopLeft((SIZE_X - menuBorder.Width()) / 2, MENU_POS_Y + MENU_MARGIN_BTM * i);
            menuBorder.ShowBitmap();
            menuText[i]->SetTopLeft((SIZE_X - menuText[i]->Width()) / 2, MENU_POS_Y + 7 + MENU_MARGIN_BTM * i);
            menuText[i]->ShowBitmap();
        }

        int HIGHSCORE_POS_X = (SIZE_X + menuBorder.Width()) / 2 + 8;
        int HIGHSCORE_POS_Y = MENU_POS_Y + 10;

        if (1) {					// ��̰ܳ���(bitmap)
            int tempScore = 9487;
            highScoreBorder.SetTopLeft(HIGHSCORE_POS_X, HIGHSCORE_POS_Y);
            highScoreBorder.ShowBitmap();

            for (int i = 0; i < 5; i++) {					// ��ܤ��ƼƦrbmp
                numBmpSmall[tempScore % 10].SetTopLeft( HIGHSCORE_POS_X - 10 * i + 130, HIGHSCORE_POS_Y + 5);
                numBmpSmall[tempScore % 10].ShowBitmap();
                tempScore /= 10;
            }
        }
    }
    else if (displayState == 1) {	// ��ܻ�����r
        // �����ؽu
        noteBorder.SetTopLeft((SIZE_X - noteBorder.Width()) / 2, NOTE_TEXT_Y);
        noteBorder.ShowBitmap();
        // �����b�Y
        noteArrow.SetTopLeft((SIZE_X - noteArrow.Width()) / 2, NOTE_TEXT_Y + (noteBorder.Height()  - noteArrow.Height()) / 2 + 11);
        noteArrow.ShowBitmap();
        // ������r
        note[noteDisplayState]->SetTopLeft((SIZE_X - noteBorder.Width()) / 2, NOTE_TEXT_Y + (noteBorder.Height() - note[noteDisplayState]->Height() ) / 2 + 11);
        note[noteDisplayState]->ShowBitmap();

        // ������r ���ܿO
        for (unsigned int i = 0; i < note.size(); i++) {
            noteUnselected.SetTopLeft((SIZE_X - noteBorder.Width()) / 2 + noteBorder.Width() - 25 - 8 * i, NOTE_TEXT_Y + noteBorder.Height() - 20);
            noteUnselected.ShowBitmap();
        }

        noteSelected.SetTopLeft((SIZE_X - noteBorder.Width()) / 2 + noteBorder.Width() - 25 - 8 * (note.size() - noteDisplayState - 1), NOTE_TEXT_Y + noteBorder.Height() - 20);
        noteSelected.ShowBitmap();

        // �������r�ʵe
        if (noteDisplayState == 0) {
            noteExkey.SetTopLeft((SIZE_X - noteExkey.Width()) / 2, NOTE_TEXT_Y + 150);
            noteExkey.OnShow();
        }

        // ��ܰ���^���s
        const int BACK_BTN_POS = NOTE_TEXT_Y + noteBorder.Height() + 0;
        menuBorder_ckecked.SetTopLeft((SIZE_X - menuBorder.Width()) / 2, BACK_BTN_POS);
        menuBorder_ckecked.ShowBitmap();
        menuBorder.SetTopLeft((SIZE_X - menuBorder.Width()) / 2, BACK_BTN_POS);
        menuBorder.ShowBitmap();
        menuText[5]->SetTopLeft((SIZE_X - menuText[5]->Width()) / 2,  7 + BACK_BTN_POS);
        menuText[5]->ShowBitmap();
    }
    else if (displayState == 2) {      // ��� ��ܨ��� ����
        characterBorder.SetTopLeft((SIZE_X - characterBorder.Width()) / 2, CHARACTER_POS_Y);
        characterBorder.ShowBitmap();
        characterArrow.SetTopLeft((SIZE_X - characterArrow.Width()) / 2, CHARACTER_POS_Y + characterBorder.Height() / 2);
        characterArrow.ShowBitmap();
        PublicData::me.SetState(1);
        PublicData::me.OnShow();
    }
    else if (displayState == 3) {}
    else if (displayState == 4) {      // ������󭶭�
        // �����
        aboutBorder.SetTopLeft((SIZE_X - aboutBorder.Width()) / 2, NOTE_TEXT_Y);
        aboutBorder.ShowBitmap();
        // �����r
        about.SetTopLeft((SIZE_X - aboutBorder.Width()) / 2, NOTE_TEXT_Y  + 11);
        about.ShowBitmap();
        // ��ܰ���^���s
        const int BACK_BTN_POS = NOTE_TEXT_Y + aboutBorder.Height() - 23;
        menuBorder_ckecked.SetTopLeft((SIZE_X - menuBorder.Width()) / 2, BACK_BTN_POS);
        menuBorder_ckecked.ShowBitmap();
        menuBorder.SetTopLeft((SIZE_X - menuBorder.Width()) / 2, BACK_BTN_POS);
        menuBorder.ShowBitmap();
        menuText[5]->SetTopLeft((SIZE_X - menuText[5]->Width()) / 2, 7 + BACK_BTN_POS);
        menuText[5]->ShowBitmap();
    }

    text1.SetTopLeft((SIZE_X - text1.Width()) / 2, text1_y);
    text1.ShowBitmap();

    if (1) {		// ��ܼƦr�Φr��
        for (int i = 0; i < 10; i++) {
            numBmpSmall[i].SetTopLeft(10 + i * 10, 300);
            numBmpSmall[i].ShowBitmap();
            numBmp[i].SetTopLeft(10 + i * 20, 320);
            numBmp[i].ShowBitmap();
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame* g)
    : CGameState(g) {
}

void CGameStateOver::OnMove() {
    if (counter < 0)	GotoGameState(GAME_STATE_INIT);

    if (barCounter < 200 && barCounter < accuracy) {
        barCounter +=  2;
    }
}
void CGameStateOver::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
    const char KEY_ENTER = 0xD;
    (nChar == KEY_ENTER) ? GotoGameState(GAME_STATE_INIT) : 0;
}
void CGameStateOver::OnBeginState() {
    counter = 1000 * 5;	 // 5 seconds
    barCounter = 0;
    //
    score = PublicData::score;
    level = PublicData::level;
    accuracy = PublicData::accuracy;
    //
    PublicData::me.SetState(2);
}

void CGameStateOver::OnInit() {
    ShowInitProgress(66);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���66%
    border.LoadBitmap("Bitmaps/gameover/gameover_border.bmp", RGB(0, 255, 0));
    char str[80];
    ShowInitProgress(80);

    for (int i = 0; i < 10; i++) {		// ���J�Ʀr��
        sprintf(str, "Bitmaps/level/num/%d.bmp", i);
        numBmp[i].LoadBitmap(str, RGB(0, 255, 0));
        sprintf(str, "Bitmaps/level/num_s/%d.bmp", i);
        numBmpSmall[i].LoadBitmap(str, RGB(0, 255, 0));
    }

    bar[0].LoadBitmap("Bitmaps/gameover/bar_0.bmp", RGB(0, 255, 0));
    bar[1].LoadBitmap("Bitmaps/gameover/bar_1.bmp", RGB(0, 255, 0));
    numBmpSmall[10].LoadBitmap("Bitmaps/level/num_s/per.bmp", RGB(0, 255, 0));
    numBmpSmall[11].LoadBitmap("Bitmaps/level/num_s/dot.bmp", RGB(0, 255, 0));
    x = (SIZE_X - border.Width()) / 2;
    y = (SIZE_Y - border.Height()) / 2;
    ShowInitProgress(100);
}

void CGameStateOver::OnShow() {		// GAMEOVER �e�����
    border.SetTopLeft(x, y);
    border.ShowBitmap();
    //
    int tempScore = score, tempLevel = level, tempAccuracy = int (accuracy * 100.0);
    int dotPos = 0;
    int scorePosX = 230, levelPosX = 130, accPosX = 155, // �w�q�U�Ʀr��m�����q
        scorePosY =  45, levelPosY =  90, accPosY = 108,
        barPosX = 120, barPosY = 122;

    for (int i = 0; i < 5; i++) {					// ��ܤ��ƼƦrbmp
        numBmp[tempScore % 10].SetTopLeft(x + scorePosX - 20 * i, y + scorePosY);
        numBmp[tempScore % 10].ShowBitmap();
        tempScore /= 10;
    }

    for (int i = 0; i < 2; i++) {					// ������d�Ʀrbmp
        numBmpSmall[tempLevel % 10].SetTopLeft(x + levelPosX - 10 * i, y + levelPosY);
        numBmpSmall[tempLevel % 10].ShowBitmap();
        tempLevel /= 10;
    }

    if (accuracy != 100) {								//�Y���T�v�D100%
        for (int i = 0, dotPos = 0; i < 4; i++) {		// ��ܥ��T�vbmp
            numBmpSmall[tempAccuracy % 10].SetTopLeft(x + accPosX - 10 * i - dotPos, y + accPosY);
            numBmpSmall[tempAccuracy % 10].ShowBitmap();
            tempAccuracy /= 10;

            if (i == 1) {								// ��ܤp���I
                dotPos = 5;
                numBmpSmall[11].SetTopLeft(x + accPosX - 10 * i - dotPos, y + accPosY);
                numBmpSmall[11].ShowBitmap();
            }

            tempLevel /= 10;
        }

        numBmpSmall[10].SetTopLeft(x + accPosX + 14, y + accPosY);
        numBmpSmall[10].ShowBitmap();					// ��ܦʤ���Ÿ�
    }
    else {												// �w��100%���
        numBmpSmall[1].SetTopLeft(x + accPosX - 35, y + accPosY);
        numBmpSmall[1].ShowBitmap();
        numBmpSmall[0].SetTopLeft(x + accPosX - 35 + 10, y + accPosY);
        numBmpSmall[0].ShowBitmap();
        numBmpSmall[0].SetTopLeft(x + accPosX - 35 + 10 * 2, y + accPosY);
        numBmpSmall[0].ShowBitmap();
        numBmpSmall[10].SetTopLeft(x + accPosX - 35 + 10 * 4, y + accPosY);
        numBmpSmall[10].ShowBitmap();					// ��ܦʤ���Ÿ�
    }

    for (int i = 0; i < 100; i++) {						// ��ܦʤ��񪺶i�ױ�
        bar[0].SetTopLeft(x + barPosX + i, y + barPosY);
        bar[0].ShowBitmap();

        if (i < barCounter) {
            bar[1].SetTopLeft(x + barPosX + i, y + barPosY);
            bar[1].ShowBitmap();
        }
    }

    PublicData::me.OnShow();
}

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
/////////////////////////////////////////////////////////////////////////////
CGameStateRun::CGameStateRun(CGame* g)
    : CGameState(g), LEVEL(30) {
    srand((unsigned)time(NULL));	// �üƺؤl
    callEnemyCounter = maxCallEnemyCounter = 30;	// maxCallEnemyCounter �M�w�Ǫ��ͦ��t��
    callBossACounter = maxCallBossACounter = 100;
    callBossBCounter = maxCallBossBCounter = 100;
}
CGameStateRun::~CGameStateRun() {
    for (CEnemy* eq : enemyQueue) delete eq;

    for (CBullet* bl : bulletList) delete bl;

    for (CBomb* cb : bombList) delete cb;
}
void CGameStateRun::OnBeginState() {
    const int SCORE_X = 240, SCORE_Y = 240;
    CAudio::Instance()->Play(AUDIO_ROCK, true);			// ���� MIDI
    score.SetInteger(0);								// �]�wSCORE��0
    score.SetTopLeft(SCORE_X, SCORE_Y);
    currEnemyNum = currBossANum = currBossBNum = 0;		// ��l�ƦU���ĤH�w�l��ƶq
    lock = false;										// ������w
    targetEnemy = NULL;
    currLevel = 0;										// ��l�����d
    enemyQueue.clear();									// �M��EQ
    lives = 3;
    totalKeyDownCount = totalCorrectKeyCount = 0;
    accuracy = 0;
    emp.SetEQ(&enemyQueue, &score, &lock, &targetEnemy);
    emp.SetEmpTimes(3);
    PublicData::me.SetState(0);
    totalEnemyNum = 0;
    levelChangeFlag = 0;
    levelChangeDelay = -1;
    levelChangeDelayMax = int( 3.5 * 30 );						// �]�w���d��delay 3��

    //
    if (1) {	//�iDEBUG�ϡj �N��0���]�w�ͦ�200�u�Ǫ��A�B�l��delay��0��
        levelEnemyNum[0] = 200;
        levelChangeDelayMax = 0;
        callEnemyCounter = maxCallEnemyCounter = callBossACounter = maxCallBossACounter = callBossBCounter = maxCallBossBCounter = 0;
    }
}
void CGameStateRun::OnInit() {								// �C������Ȥιϧγ]�w
    srand((unsigned)time(NULL));
    ShowInitProgress(33);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���33%
    //
    // �~����J��L���
    //
    score.LoadBitmap();
    map.LoadBitmap();
    emp.LoadBitmap();
    levelAni.LoadBitmap();
    CAudio::Instance()->Load(AUDIO_ROCK, "sounds\\The_Coming_Storm.mp3");	// ���J�s��3���n��The_Coming_Storm.mp3
    CAudio::Instance()->Load(AUDIO_SHOT, "sounds\\shot.mp3");
    ShowInitProgress(40);

    //
    // ��OnInit�ʧ@�|����CGameStaterOver::OnInit()�A�ҥH�i���٨S��100%
    //
    for (int i = 0; i < 26; i++) {
        char str[50];
        sprintf(str, "Bitmaps/char4/%c.bmp", i + 97);
        letter.push_back(new CMovingBitmap);
        letter.back()->LoadBitmap(str, RGB(255, 255, 255));
    }

    //

    for (int i = 0; i < 22; i++) {	// ���J�ʵe
        char str[50];
        sprintf(str, "Bitmaps/target/target_s%d.bmp", i + 1);
        target.AddBitmap(str, RGB(0, 255, 0));
    }

    target.SetDelayCount(2);
    ShowInitProgress(50);
}
void CGameStateRun::OnMove() {						// ���ʹC������
    //
    // �p�G�Ʊ�ק�cursor���˦��A�h�N�U���{����commment�����Y�i
    //
    //SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
    //
    ////////
    if (levelChangeDelay >= 0)	levelChangeDelay--;

    accuracy = (totalKeyDownCount != 0) ? \
               100 * double(totalCorrectKeyCount) / double(totalKeyDownCount) : \
               100;     // �p�⥿�T�v
    PublicData::score = score.GetInteger();
    PublicData::level = currLevel;
    PublicData::accuracy = accuracy;
    callEnemyCounter--;	//�C���Ǫ� �ͦ����j �� counter
    callBossACounter--;
    callBossBCounter--;

    //==�p��==================================
    if (callEnemyCounter < 0 && currEnemyNum < levelEnemyNum[currLevel]) {	// counter �ƨ�0��N�}�l�l��s��
        callEnemyCounter = maxCallEnemyCounter;				// ��counter �զ^max�~���
        int randX = (rand() % (SIZE_X - 100)) ;				// SIZE_X - 100 ���F�����Ǫ�����r�W�X�ù��Ӧh
        enemyQueue.push_back(new CEnemy(randX, 0, 3, true, &dictionary, 2, 7, &enemyQueue, &bombList, PublicData::me.GetX1(), PublicData::me.GetY1(), &letter) );
        enemyQueue.back()->LoadBitmap();
        currEnemyNum++;
        totalEnemyNum++;
    }

    //==BossA==================================
    if (callBossACounter < 0 && currBossANum < levelBossANum[currLevel]) {	// counter �ƨ�0��N�}�l�l��s��
        callBossACounter = maxCallBossACounter;				// ��counter �զ^max�~���
        int randX = (rand() % (SIZE_X - 150));
        enemyQueue.push_back(new CBossA(randX, 0, 5, true, &dictionary, 7, 20, &enemyQueue, &bombList, &letter));
        enemyQueue.back()->LoadBitmap();
        currBossANum++;
        totalEnemyNum++;
    }

    //==BossB==================================
    if (callBossBCounter < 0 && currBossBNum < levelBossBNum[currLevel]) {	// counter �ƨ�0��N�}�l�l��s��
        callBossBCounter = maxCallBossBCounter;				// ��counter �զ^max�~���
        int randX = (rand() % (SIZE_X - 150));
        enemyQueue.push_back(new CBossB(randX, 0, 5, true, &dictionary, 7, 20, &enemyQueue, &bombList, &letter));
        enemyQueue.back()->LoadBitmap();
        currBossBNum++;
        totalEnemyNum++;
    }

    //===�P�_Me�O�_�I��Enemy===

    for (CEnemy* eq : enemyQueue) {
        if (eq->IsAlive() && eq->HitMe(&PublicData::me)) {
            lives--;
            (lives <= 0) ? GotoGameState(GAME_STATE_OVER) : 0;
        }

        // ��enemy���X�e���~��kill()

        if (eq->GetX() > SIZE_X + 40 || eq->GetX() < -40 || eq->GetY() > SIZE_Y + 40) {
            eq->kill();
        }
    }

    // ===Enemy===
    bool enemyAllDead = true;

    for (unsigned int i = 0; i < enemyQueue.size(); i++) {
        enemyQueue[i]->OnMove();
        enemyQueue[i]->IsAlive() ? enemyAllDead = false : 0;
    }

    for (unsigned int i = 0; i < enemyQueue.size(); i++) {
        //�YEnemy IsAlive=0, �h�qvector������
        if (!enemyQueue[i]->IsAlive() && enemyQueue[i]->IsBombed()) {
            vector<CEnemy*>::iterator iterenemyQueue = enemyQueue.begin();
            delete enemyQueue[i];
            enemyQueue[i] = NULL;
            enemyQueue.erase(iterenemyQueue + i);
            i = 0;
        }
    }

    ////===bullet===
    bool bulletAllDead = true;

    for (CBullet* bl : bulletList) {
        bl->OnMove();	// ����bullet

        if (bl->IsAlive()) bulletAllDead = false;
    }

    if (bulletAllDead) {
        for (CBullet* bl : bulletList) delete bl;

        bulletList.clear();
    }

    ////===bomb===
    bool bombAllDead = true;

    for (CBomb* cb : bombList) {
        cb->OnMove();

        if (cb->IsAlive()) bombAllDead = false;
    }

    if (bombAllDead) {
        for (CBomb* cb : bombList) {
            delete cb;
            cb = NULL;
        }

        bombList.clear();
    }

    ////===Change Level===
    if (currEnemyNum >= levelEnemyNum[currLevel] && currBossANum >= levelBossANum[currLevel] \
            && currBossBNum >= levelBossBNum[currLevel] && enemyQueue.size() == 0) {
        // �� ���d
        if (!levelChangeFlag) {								// ������������d�ʵe �õ���delay�⧹
            levelAni.Play(currLevel, score.GetInteger());
            levelChangeFlag = true;
            levelChangeDelay = levelChangeDelayMax;
        }

        if (levelChangeDelay < 0 && levelChangeFlag) {		// ��delay�⧹�� �A��ڤ������d
            currLevel++;

            if (currLevel >= LEVEL)GotoGameState(GAME_STATE_INIT);

            currEnemyNum = currBossANum = currBossBNum = 0;
            callEnemyCounter = maxCallEnemyCounter;
            callBossACounter = maxCallBossACounter;
            callBossBCounter = maxCallBossBCounter;
            levelChangeFlag = false;
        }
    }

    //
    map.OnMove();
    emp.OnMove();
    PublicData::me.OnMove();
    levelAni.OnMove();

    if (lock && targetEnemy != NULL) {
        target.OnMove();
    }
}

void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
    const char KEY_LEFT = 0x25;	// keyboard���b�Y
    const char KEY_UP = 0x26;	// keyboard�W�b�Y
    const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
    const char KEY_DOWN = 0x28; // keyboard�U�b�Y

    if (key != nChar) {
        key = nChar;

        for (int unsigned i = 0; i < enemyQueue.size(); i++) {			// �]�ثe���d�Ǫ����ƶq
            if (enemyQueue[i]->IsAlive()) {								// �^�Ƿ�e�Ǫ��O�_�s�b
                if (!lock) {										// �|����w�F
                    if (nChar + 32 == enemyQueue[i]->GetFirstWord()) {	// �Y����Ĥ@�Ӧr��:��� and �ثe�r����m+1
                        totalCorrectKeyCount++;							// ���T�����+1
                        map.PlayFlash();
                        CAudio::Instance()->Play(AUDIO_SHOT, false);			// ���� �g������

                        if (enemyQueue[i]->GetVocabLeng() == 1) {
                            targetEnemy = enemyQueue[i];					// targetEnemy������->���b�������ĤH
                            bulletList.push_back(new CBullet(targetEnemy->GetX() + 10, targetEnemy->GetY() + 10));	// �g�l�u
                            targetEnemy->kill();									// ���\���`�Ǫ�
                            score.Add(targetEnemy->GetVocabLeng());				// ����+= �Ǫ�����
                            targetEnemy = NULL;
                            break;
                        }
                        else {
                            lock = true;
                            targetEnemy = enemyQueue[i];					// targetEnemy������->���b�������ĤH
                            targetEnemy->AddCurrWordLeng();
                            bulletList.push_back(new CBullet(targetEnemy->GetX() + 10, targetEnemy->GetY() + 10));	// �g�l�u
                            targetEnemy->MinusIndex(2);					// ���h�Ǫ�
                            break;
                        }
                    }
                }
                else {												// �Y�w��w
                    if (nChar + 32 == targetEnemy->GetVocab()[targetEnemy->GetCurrWordLeng()]) { 	// �Y�����e�r��
                        targetEnemy->AddCurrWordLeng();
                        bulletList.push_back(new CBullet(targetEnemy->GetX(), targetEnemy->GetY()));
                        targetEnemy->MinusIndex(rand() % 2 + 1);		// ���h�Ǫ�
                        totalCorrectKeyCount++;							// ���T�����+1
                        map.PlayFlash();
                        CAudio::Instance()->Play(AUDIO_SHOT, false);	// ���� �g������

                        if (targetEnemy->GetCurrWordLeng() == targetEnemy->GetVocabLeng()) {	 // �Y��e���� ���� �r��������
                            targetEnemy->kill();									// ���\���`�Ǫ�
                            score.Add(targetEnemy->GetVocabLeng());					// ����+= �Ǫ�����
                            targetEnemy = NULL;
                            lock = false;
                        }

                        break;
                    }
                }
            }
        }

        if (nChar >= 65 && nChar <= 90) totalKeyDownCount++;			// �`�����++

        if (nChar == 13)
            emp.CallEmp();
    }
}
void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
    const char KEY_LEFT = 0x25; // keyboard���b�Y
    const char KEY_UP = 0x26; // keyboard�W�b�Y
    const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
    const char KEY_DOWN = 0x28; // keyboard�U�b�Y
    key = NULL;

    // �Ʀr��@��debug���s
    if (nChar == '1')showDebug = showDebug ? false : true;  // ��1 �}��debug

    if (nChar == '2' && enemyQueue.size() > 0) {  // ��2 �M��EQ�̫�@���ĤH
        enemyQueue.back()->kill();
        lock = 0;
    }

    if (nChar == '3' && enemyQueue.size() > 0) {  // ��3 �M��EQ���Ҧ��ĤH
        for (CEnemy* ce : enemyQueue) ce->kill();

        lock = 0;
    }

    if (nChar == '4')GotoGameState(GAME_STATE_INIT);	// ��4 GOTO �_�l�e��

    if (nChar == '5')GotoGameState(GAME_STATE_OVER);    // ��5 GOTO �C�������e��
}
void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point) { // �B�z�ƹ����ʧ@
}
void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point) {	// �B�z�ƹ����ʧ@
    //eraser.SetMovingLeft(false);
}
void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point) {	// �B�z�ƹ����ʧ@
    // �S�ơC�p�G�ݭn�B�z�ƹ����ʪ��ܡA�gcode�b�o��
}
void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point) { // �B�z�ƹ����ʧ@
    //eraser.SetMovingRight(true);
}
void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point) {	// �B�z�ƹ����ʧ@
    //eraser.SetMovingRight(false);
}
void CGameStateRun::OnShow() {
    map.OnShow();						// �K�W�I�����l
    score.ShowBitmap();					// �K�W����
    emp.OnShow();						// �K�WEMP�]�����ޡ^
    PublicData::me.OnShow();			// �K�W�D��
    levelAni.OnShow();					// �K�W���d�����ʵe

    for (CBomb* cb : bombList)	cb->OnShow();

    for (CEnemy* eq : enemyQueue)	eq->OnShow();

    for (CBullet* bl : bulletList)	bl->OnShow();

    if (lock && targetEnemy != NULL) {
        targetEnemy->OnShow();	// �[�W�o�@�� ���Q��w���Ǫ��A��show, �H���Q��L�Ǫ��\��

        if (targetEnemy->GetBossType() == "enemy")
            target.SetTopLeft(targetEnemy->GetX() - 2, targetEnemy->GetY() - 2);
        else
            target.SetTopLeft(targetEnemy->GetX() + 8, targetEnemy->GetY() + 8);

        target.OnShow();
    }

    if (showDebug) {		// ���debug��T
        CDC* pDC = CDDraw::GetBackCDC();
        CFont f, *fp;
        f.CreatePointFont(100, "Fixedsys");
        fp = pDC->SelectObject(&f);
        pDC->SetBkColor(RGB(0, 0, 0));
        //
        char temp[100];
        sprintf(temp, "TotalKeyNum: %d TotalCorrectKeyNum: %d , Accuracy: %.2lf%%", \
                totalKeyDownCount, totalCorrectKeyCount, accuracy);
        pDC->SetTextColor(RGB(50, 200, 200));
        pDC->TextOut(20, 20, temp);
        //
        char temp1[100];
        sprintf(temp1, "EQ size: %d,  Live: %d,  Level: %d,  LOCK: %d,  TotalEnemyNum: %d", \
                enemyQueue.size(), lives, currLevel, bool(lock), totalEnemyNum);
        pDC->SetTextColor(RGB(255, 255, 255));
        pDC->TextOut(20, 40, temp1);

        //

        if (1) {
            for (unsigned int i = 0; i < enemyQueue.size(); i++) {	// ��ܳ��W�Ǫ��� ��r,curr/length
                char temp[50];
                sprintf(temp, "%s %d/%d (x:%d,y:%d)", enemyQueue[i]->GetVocab().c_str(), enemyQueue[i]->GetCurrWordLeng(), enemyQueue[i]->GetVocabLeng(), enemyQueue[i]->GetX(), enemyQueue[i]->GetY());
                pDC->SetTextColor(RGB(180 + i, 180 + i, 180 + i));
                pDC->TextOut(20, i * 14 + 60, temp);
            }
        }

        ////
        pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
        CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
    }
}
}
