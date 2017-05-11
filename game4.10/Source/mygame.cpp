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

CGameStateInit::CGameStateInit(CGame* g)
    : CGameState(g), NOTE_TEXT_X(60), NOTE_TEXT_Y(300), MENU_POS_Y(350),
      MENU_ITEM_NUM(4) {
}

void CGameStateInit::OnInit() {
    ShowInitProgress(0);	// �@�}�l��loading�i�׬�0%
    const unsigned int exkeyNum = 6;										// �����ظ̭�������ʵe �ƶq
    currSelectItem = displayState = 1;
    noteDisplayState = 0;
    map.LoadBitmap();														// �I�������ʵe
    typing_logo.LoadBitmap("Bitmaps/start_logo1.bmp", RGB(0, 255, 0));		// logo
    text1.LoadBitmap("Bitmaps/text1_start.bmp", RGB(0, 255, 0));			// �� �ƹ�����}�l�C��
    /////
    //
    // ���J��椸��

    for (int i = 0; i < 4; i++) {		// 4�ӿ��
        char str[40];
        sprintf(str, "Bitmaps/menu/menu_t_%d.bmp", i + 1);
        menuText.push_back(new CMovingBitmap);
        menuText.back()->LoadBitmap(str, RGB(0, 255, 0));
    }

    menuBorder.LoadBitmap("Bitmaps/menu/menu_border.bmp", RGB(0, 255, 0));
    menuBorder_ckecked.LoadBitmap("Bitmaps/menu/menu_border_checked.bmp", RGB(0, 255, 0));
    menuText.push_back(new CMovingBitmap);
    menuText.back()->LoadBitmap("Bitmaps/menu/menu_t_b.bmp", RGB(0, 255, 0));
    // ���J�C����������
    noteBorder.LoadBitmap("Bitmaps/menu/note/note_text_border.bmp", RGB(0, 255, 0)); // �����ؽu
    noteArrow.LoadBitmap ("Bitmaps/menu/note/note_text_arraw.bmp", RGB(0, 255, 0));	// �����b�Y
    noteUnselected.LoadBitmap("Bitmaps/menu/note/note_unselected.bmp", RGB(0, 255, 0));
    noteSelected.LoadBitmap("Bitmaps/menu/note/note_selected.bmp", RGB(0, 255, 0));

    for (int i = 0; i < exkeyNum; i++) {	// �����ظ̭�������ʵe
        char str[50];
        sprintf(str, "Bitmaps/menu/note/note1_exkey_%d.bmp", i + 1);
        noteExkey.AddBitmap(str, RGB(0, 255, 0));
    }

    for (int i = 0; i < 3; i++) {		// �h����������r
        char str[50];
        sprintf(str, "Bitmaps/menu/note/note_text_p%d.bmp", i + 1);
        note.push_back(new CMovingBitmap);
        note.back()->LoadBitmap(str, RGB(0, 255, 0));
    }

    //
    // ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
    //
}

void CGameStateInit::OnBeginState() {
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
    const char KEY_ESC = 27;
    const char KEY_SPACE = ' ';
    const char KEY_ENTER = 0xD;
    const char KEY_LEFT = 0x25;	// keyboard���b�Y
    const char KEY_UP = 0x26;	// keyboard�W�b�Y
    const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
    const char KEY_DOWN = 0x28; // keyboard�U�b�Y

    if (displayState == 0 && (nChar == KEY_UP || nChar == KEY_DOWN)) { // ���ʥD���
        if (nChar == KEY_UP) currSelectItem--;
        else if (nChar == KEY_DOWN) currSelectItem++;

        if (currSelectItem < 0)currSelectItem = MENU_ITEM_NUM - 1;
        else if (currSelectItem > MENU_ITEM_NUM - 1)currSelectItem = 0;
    }

    if (displayState == 0 && nChar == KEY_ENTER ) { // ���UENTER��ܫ�...
        if ( currSelectItem == 0 ) // �}�l�C��
            GotoGameState(GAME_STATE_RUN);

        if ( currSelectItem == 1 ) {
            displayState = 1;	// ��ܹC��������state
            noteDisplayState = 0;
        }
    }
    else if (displayState == 1 ) { // [�C������]
        if (nChar == KEY_ENTER)displayState = 0;	// ->��^�D���
        else if (nChar == KEY_LEFT || nChar == KEY_RIGHT) { // [�C������] ���k½���C������
            if		(nChar == KEY_LEFT)   noteDisplayState--;
            else if (nChar == KEY_RIGHT) noteDisplayState++;

            if (noteDisplayState < 0) noteDisplayState = int(note.size()) - 1;
            else if (noteDisplayState >  int(note.size()) - 1) noteDisplayState = 0;
        }
    }
    else if (1) {
    }

    //PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// �����C��
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point) {
    //GotoGameState(GAME_STATE_RUN);		// ������GAME_STATE_RUN
}
void CGameStateInit::OnMove() {
    noteExkey.OnMove();
    map.OnMove();
}

void CGameStateInit::OnShow() {
    map.OnShow();
    // logo
    typing_logo.SetTopLeft((SIZE_X - typing_logo.Width()) / 2, SIZE_Y / 5);
    typing_logo.ShowBitmap();

    if (displayState == 0) {	// ��ܥD���
        menuBorder_ckecked.SetTopLeft((SIZE_X - menuBorder.Width()) / 2, MENU_POS_Y + 40 * currSelectItem);
        menuBorder_ckecked.ShowBitmap();

        for (int i = 0; i < MENU_ITEM_NUM; i++) {
            menuBorder.SetTopLeft((SIZE_X - menuBorder.Width()) / 2, MENU_POS_Y + 40 * i);
            menuBorder.ShowBitmap();
            menuText[i]->SetTopLeft((SIZE_X - menuText[i]->Width()) / 2, MENU_POS_Y + 7 + 40 * i);
            menuText[i]->ShowBitmap();
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
        const int BACK_BTN_POS = 170;
        menuBorder_ckecked.SetTopLeft((SIZE_X - menuBorder.Width()) / 2, MENU_POS_Y + BACK_BTN_POS);
        menuBorder_ckecked.ShowBitmap();
        menuBorder.SetTopLeft((SIZE_X - menuBorder.Width()) / 2, MENU_POS_Y + BACK_BTN_POS);
        menuBorder.ShowBitmap();
        menuText[4]->SetTopLeft((SIZE_X - menuText[4]->Width()) / 2, MENU_POS_Y + 7 + BACK_BTN_POS);
        menuText[4]->ShowBitmap();
    }
    else if (displayState == 2) {
        // ��ܨ���
    }
    else if (displayState == 3) {
        // ����
    }
}

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(Game Over)
/////////////////////////////////////////////////////////////////////////////

CGameStateOver::CGameStateOver(CGame* g)
    : CGameState(g) {
}

void CGameStateOver::OnMove() {
    counter--;

    if (counter < 0)
        GotoGameState(GAME_STATE_INIT);
}

void CGameStateOver::OnBeginState() {
    counter = 30 * 5; // 5 seconds
}

void CGameStateOver::OnInit() {
    //
    // ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
    //     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
    //
    ShowInitProgress(66);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���66%
    //
    // �}�l���J���
    //
    Sleep(300);				// ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
    //
    // �̲׶i�׬�100%
    //
    ShowInitProgress(100);
}

void CGameStateOver::OnShow() {
    CDC* pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC
    CFont f, *fp;
    f.CreatePointFont(160, "Consolas");			// ���� font f; 160���16 point���r
    fp = pDC->SelectObject(&f);					// ��� font f
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(255, 255, 0));
    char str[80];								// Demo �Ʀr��r�ꪺ�ഫ
    sprintf(str, "Game Over(%d)", counter / 30);
    pDC->TextOut(240, 210, str);
    //	��ܤ��� (not done)
    // char scoreChr[80];
    // sprintf(str, "SCORE: %d", score);
    //
    pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
    CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
}

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame* g)
    : CGameState(g), NUMBALLS(28), LEVEL(10) {
    srand((unsigned)time(NULL));	// �üƺؤl
    callEnemyCounter = maxCallEnemyCounter = 20;	// maxCallEnemyCounter �M�w�Ǫ��ͦ��t��
    callBossACounter = maxCallBossACounter = 80;
    callBossBCounter = maxCallBossBCounter = 80;
}

CGameStateRun::~CGameStateRun() {
    //delete[] ball;
    //delete[] &enemyQueue;
}

void CGameStateRun::OnBeginState() {
    const int SCORE_X = 240, SCORE_Y = 240;
    help.SetTopLeft(0, SIZE_Y - help.Height());			// �]�w�����Ϫ��_�l�y��
    //CAudio::Instance()->Play(AUDIO_LAKE, true);			// ���� WAVE
    //CAudio::Instance()->Play(AUDIO_DING, false);		// ���� WAVE
    //CAudio::Instance()->Play(AUDIO_NTUT, true);			// ���� MIDI
    CAudio::Instance()->Play(AUDIO_ROCK, true);			// ���� MIDI
    score.SetInteger(0);			//�]�wSCORE��0
    score.SetTopLeft(SCORE_X, SCORE_Y);
    currEnemyNum = currBossANum = currBossBNum = 0;
    lock = false;
    currLevel = 0;
    enemyQueue.clear();
    lives = 3;
    totalKeyDownCount = totalCorrectKeyCount = 0;
    accuracy = 0;
    emp.SetEQ(&enemyQueue, &score, &lock, &targetEnemy);
}

void CGameStateRun::OnMove() {						// ���ʹC������
    //
    // �p�G�Ʊ�ק�cursor���˦��A�h�N�U���{����commment�����Y�i
    //
    //SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
    //
    ////////
    callEnemyCounter--;	//�C���Ǫ� �ͦ����j �� counter
    callBossACounter--;
    callBossBCounter--;

    //==�p��==================================
    if (callEnemyCounter < 0 && currEnemyNum < levelEnemyNum[currLevel]) {	// counter �ƨ�0��N�}�l�l��s��
        callEnemyCounter = maxCallEnemyCounter;				// ��counter �զ^max�~���
        int randX = (rand() % (SIZE_X - 100)) ;				// SIZE_X - 100 ���F�����Ǫ�����r�W�X�ù��Ӧh
        enemyQueue.push_back(new CEnemy(randX, 0, 3, true, &dictionary, 2, 6, &enemyQueue, &bombList, me.GetX1(), me.GetY1()) );
        enemyQueue.back()->LoadBitmap();
        currEnemyNum++;
    }

    //==BossA==================================
    if (callBossACounter < 0 && currBossANum < levelBossANum[currLevel]) {	// counter �ƨ�0��N�}�l�l��s��
        callBossACounter = maxCallBossACounter;				// ��counter �զ^max�~���
        int randX = (rand() % (SIZE_X - 100));
        enemyQueue.push_back(new CBossA(randX, 0, 6, true, &dictionary, 6, 20, &enemyQueue, &bombList));
        enemyQueue.back()->LoadBitmap();
        currBossANum++;
    }

    //==BossB==================================
    if (callBossBCounter < 0 && currBossBNum < levelBossBNum[currLevel]) {	// counter �ƨ�0��N�}�l�l��s��
        callBossBCounter = maxCallBossBCounter;				// ��counter �զ^max�~���
        int randX = (rand() % (SIZE_X - 100));
        enemyQueue.push_back(new CBossB(randX, 0, 6, true, &dictionary, 6, 20, &enemyQueue, &bombList));
        enemyQueue.back()->LoadBitmap();
        currBossBNum++;
    }

    //===�P�_Me�O�_�I��Enemy===

    for (int unsigned i = 0; i < enemyQueue.size(); i++) {
        if (enemyQueue[i]->IsAlive() && enemyQueue[i]->HitMe(&me)) {
            lives--;

            if (lives <= 0) {
                GotoGameState(GAME_STATE_OVER);
            }
        }
    }

    // ===Enemy===

    for (unsigned int i = 0; i < enemyQueue.size(); i++)
        if (enemyQueue[i]->IsAlive())	enemyQueue[i]->OnMove();

    for (unsigned int i = 0; i < enemyQueue.size(); i++) {
        if (enemyQueue[i]->GetX() > SIZE_X + 40 || enemyQueue[i]->GetX() < -40 || enemyQueue[i]->GetY() > SIZE_Y + 40) {
            enemyQueue[i]->kill();
        }
    }

    for (unsigned int i = 0; i < enemyQueue.size(); i++) {
        //�YEnemy IsAlive=0, �h�qvector������
        if (!enemyQueue[i]->IsAlive() && enemyQueue[i]->IsBombed()) {
            vector<CEnemy*>::iterator iterenemyQueue = enemyQueue.begin();
            enemyQueue.erase(iterenemyQueue + i);
            break;
        }
    }

    ////===bullet===

    for (unsigned int i = 0; i < bulletList.size(); i++)
        bulletList[i]->OnMove();	// ����bullet

    for (int i = bulletList.size() - 1; i >= 0; i--) {
        //�Ybullet IsAlive=0, �h�qvector������
        if (!bulletList[i]->IsAlive())	bulletList.erase(bulletList.begin());
    }

    ////===bomb===

    for (unsigned int i = 0; i < bombList.size(); i++) {
        bombList[i]->OnMove();
    }

    for (int i = bombList.size() - 1; i >= 0; i--) {
        //�Y bomb IsAlive=0, �h�qvector������
        if (!bombList[i]->IsAlive())	bombList.erase(bombList.begin());
    }

    ////===Change Level===
    if (currEnemyNum >= levelEnemyNum[currLevel] && currBossANum >= levelBossANum[currLevel] \
            && currBossBNum >= levelBossBNum[currLevel] && enemyQueue.size() == 0) {
        // �� ���d
        currLevel++;
        currEnemyNum = currBossANum = currBossBNum = 0;
        callEnemyCounter = maxCallEnemyCounter;
        callBossACounter = maxCallBossACounter;
        callBossBCounter = maxCallBossBCounter;
    }

    map.OnMove();
    emp.OnMove();
    me.OnMove();
}

void CGameStateRun::OnInit() {								// �C������Ȥιϧγ]�w
    //
    // ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
    //     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
    //
    srand((unsigned)time(NULL));
    ShowInitProgress(33);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���33%
    //
    // �}�l���J���
    //
    //background.LoadBitmap(IDB_BACKGROUND);					// ���J�I�����ϧ�
    //
    // ��������Loading�ʧ@�A�����i��
    //
    ShowInitProgress(50);
    Sleep(0); // ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
    //
    // �~����J��L���
    //
    help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));			// ���J�������ϧ�
    score.LoadBitmap();
    map.LoadBitmap();
    emp.LoadBitmap();
    me.LoadBitmap();
    //hits_left.LoadBitmap();
    //corner.ShowBitmap(background);							// �Ncorner�K��background
    //CAudio::Instance()->Load(AUDIO_DING, "sounds\\ding.wav");	// ���J�s��0���n��ding.wav
    //CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\lake.mp3");	// ���J�s��1���n��lake.mp3
    //CAudio::Instance()->Load(AUDIO_NTUT, "sounds\\ntut.mid");	// ���J�s��2���n��ntut.mid
    CAudio::Instance()->Load(AUDIO_ROCK, "sounds\\The_Coming_Storm.mp3");	// ���J�s��3���n��The_Coming_Storm.mp3
    CAudio::Instance()->Load(AUDIO_SHOT, "sounds\\shot.mp3");
    //
    // ��OnInit�ʧ@�|����CGameStaterOver::OnInit()�A�ҥH�i���٨S��100%
    //
}
void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
    const char KEY_LEFT = 0x25;	// keyboard���b�Y
    const char KEY_UP = 0x26;	// keyboard�W�b�Y
    const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
    const char KEY_DOWN = 0x28; // keyboard�U�b�Y

    if (nChar == '1')showDebug = 1;	// ��1 ���debug

    if (nChar == '2')showDebug = 0;

    if (nChar == '3' && enemyQueue.size() > 0) {
        enemyQueue.back()->kill();
        lock = 0;
    }
}
void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
    const char KEY_LEFT = 0x25; // keyboard���b�Y
    const char KEY_UP = 0x26; // keyboard�W�b�Y
    const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
    const char KEY_DOWN = 0x28; // keyboard�U�b�Y

    for (int unsigned i = 0; i < enemyQueue.size(); i++) {			// �]�ثe���d�Ǫ����ƶq
        if (enemyQueue[i]->IsAlive()) {								// �^�Ƿ�e�Ǫ��O�_�s�b
            if (!lock) {										// �|����w�F
                if (nChar + 32 == enemyQueue[i]->GetFirstWord()) {	// �Y����Ĥ@�Ӧr��:��� and �ثe�r����m+1
                    totalCorrectKeyCount++;							// ���T�����+1
                    CAudio::Instance()->Play(AUDIO_SHOT, false);			// ���� �g������

                    if (enemyQueue[i]->GetVocabLeng() == 1) {
                        targetEnemy = enemyQueue[i];					// targetEnemy������->���b�������ĤH
                        bulletList.push_back(new CBullet(targetEnemy->GetX() + 10, targetEnemy->GetY() + 10));	// �g�l�u
                        targetEnemy->kill();									// ���\���`�Ǫ�
                        score.Add(targetEnemy->GetVocabLeng());				// ����+= �Ǫ�����
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
                    CAudio::Instance()->Play(AUDIO_SHOT, false);			// ���� �g������

                    if (targetEnemy->GetCurrWordLeng() == targetEnemy->GetVocabLeng()) {	 // �Y��e���� ���� �r��������
                        targetEnemy->kill();									// ���\���`�Ǫ�
                        lock = false;
                        score.Add(targetEnemy->GetVocabLeng());						// ����+= �Ǫ�����
                    }

                    break;
                }
            }
        }
    }

    if (nChar >= 65 && nChar <= 90) totalKeyDownCount++;			// �`�����++

    if (totalKeyDownCount != 0)
        accuracy = 100 * double(totalCorrectKeyCount) / double(totalKeyDownCount);
    else
        accuracy = 100;

    if (nChar == 13) {
        emp.CallEmp();
    }
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
    //
    //  �`�N�GShow�̭��d�U���n���ʥ��󪫥󪺮y�СA���ʮy�Ъ��u�@����Move���~��Ad
    //        �_�h��������sø�Ϯ�(OnDraw)�A����N�|���ʡA�ݰ_�ӷ|�ܩǡC���ӳN�y
    //        ���AMove�t�dMVC����Model�AShow�t�dView�A��View�������Model�C
    //
    //
    //
    map.OnShow();						// �K�W�I�����l
    //help.ShowBitmap();					// �K�W������
    score.ShowBitmap();					// �K�W����
    emp.OnShow();
    me.OnShow();

    /////////
    for (unsigned int i = 0; i < bombList.size(); i++) {
        bombList[i]->OnShow();
    }

    for (unsigned int i = 0; i < enemyQueue.size(); i++)
        enemyQueue[i]->OnShow();

    for (unsigned int i = 0; i < bulletList.size(); i++)
        bulletList[i]->OnShow();

    if (lock && targetEnemy->IsAlive())
        targetEnemy->OnShow();	// �[�W�o�@�� ���Q��w���Ǫ��A��show, �H���Q��L�Ǫ��\��

    //
    //  �K�W���W�Υk�U��������
    //
    /*
    corner.SetTopLeft(0, 0);
    corner.ShowBitmap();
    corner.SetTopLeft(SIZE_X - corner.Width(), SIZE_Y - corner.Height());
    corner.ShowBitmap();
    */
    //
    //

    if (showDebug) {		// ���debug��T
        CDC* pDC = CDDraw::GetBackCDC();
        CFont f, *fp;
        f.CreatePointFont(100, "Fixedsys");
        fp = pDC->SelectObject(&f);
        pDC->SetBkColor(RGB(0, 0, 0));
        pDC->SetBkMode(TRANSPARENT);
        //
        char temp[100];
        sprintf(temp, "�Ǫ��ƶq: %d, �R: %d, �����d: %d, LOCK: %d", enemyQueue.size(), lives, currLevel, int(lock));
        pDC->SetTextColor(RGB(200, 0, 0));
        pDC->TextOut(20, 40, temp);
        //
        sprintf(temp, "�`�����: %d �`���T�����: %d , accuracy���T�v: %.2lf%%", totalKeyDownCount, totalCorrectKeyCount, accuracy);
        pDC->SetTextColor(RGB(50, 200, 200));
        pDC->TextOut(20, 20, temp);

        //
        for (unsigned int i = 0; i < enemyQueue.size(); i++) {	// ��ܳ��W�Ǫ��� ��r,curr/length
            char temp[40];
            sprintf(temp, "%s %d/%d (x:%d,y:%d)", enemyQueue[i]->GetVocab().c_str(), enemyQueue[i]->GetCurrWordLeng(), enemyQueue[i]->GetVocabLeng(), enemyQueue[i]->GetX(), enemyQueue[i]->GetY());
            pDC->SetTextColor(RGB(180 + i, 180 + i, 180 + i));
            pDC->TextOut(20, i * 14 + 60, temp);
        }

        sprintf(temp, "�l�u��: %d", bulletList.size());
        pDC->SetTextColor(RGB(200, 200, 200 ));
        pDC->TextOut(20, 2, temp);
        ////
        pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
        CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
    }
}
}
