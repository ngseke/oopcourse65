
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
    : CGameState(g) {
}

void CGameStateInit::OnInit() {
    ShowInitProgress(0);	// �@�}�l��loading�i�׬�0%
    //
    // �}�l���J���
    //
    //logo.LoadBitmap(IDB_BACKGROUND);
    typing_logo.LoadBitmap("Bitmaps/start_logo.bmp", RGB(0, 255, 0));
    text1.LoadBitmap("Bitmaps/text1_start.bmp", RGB(0, 255, 0));
    //
    // ��OnInit�ʧ@�|����CGameStaterRun::OnInit()�A�ҥH�i���٨S��100%
    //
}

void CGameStateInit::OnBeginState() {
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
    const char KEY_ESC = 27;
    const char KEY_SPACE = ' ';

    if (nChar == KEY_SPACE)
        GotoGameState(GAME_STATE_RUN);						// ������GAME_STATE_RUN
    else if (nChar == KEY_ESC)								// Demo �����C������k
        PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// �����C��
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point) {
    GotoGameState(GAME_STATE_RUN);		// ������GAME_STATE_RUN
}

void CGameStateInit::OnShow() {
    //
    // �K�Wlogo
    //
    //logo.SetTopLeft((SIZE_X - logo.Width()) / 2, SIZE_Y / 8);
    //logo.ShowBitmap();
    typing_logo.SetTopLeft((SIZE_X - typing_logo.Width()) / 2, 40);
    typing_logo.ShowBitmap();
    text1.SetTopLeft((SIZE_X - text1.Width()) / 2, 300);
    text1.ShowBitmap();
    //
    // Demo�ù��r�����ϥΡA���L�}�o�ɽкɶq�קK�����ϥΦr���A���CMovingBitmap����n
    //
    CDC* pDC = CDDraw::GetBackCDC();			// ���o Back Plain �� CDC
    CFont f, *fp;
    f.CreatePointFont(160, "Consolas");	// ���� font f; 160���16 point���r
    fp = pDC->SelectObject(&f);					// ��� font f
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(41, 171, 226));
    //pDC->TextOut(180, 350, "Huang Xingqiao / Yu kaici");  //test text

    if (ENABLE_GAME_PAUSE)
        pDC->TextOut(5, 425, "Press Ctrl-Q to pause the Game.");

    pDC->TextOut(5, 455, "Press Alt-F4 or ESC to Quit.");
    ////
    pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
    CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
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
    f.CreatePointFont(160, "Times New Roman");	// ���� font f; 160���16 point���r
    fp = pDC->SelectObject(&f);					// ��� font f
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetTextColor(RGB(255, 255, 0));
    char str[80];								// Demo �Ʀr��r�ꪺ�ഫ
    sprintf(str, "�A �� �F ! (%d)", counter / 30);
    pDC->TextOut(240, 210, str);
    //	��ܤ��� (not done)
    // char scoreChr[80];
    // sprintf(str, "SCORE: %d", score);
    pDC->TextOut(240, 250, str);
    //
    pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
    CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
}

/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame* g)
    : CGameState(g), NUMBALLS(28), LEVEL(10) {
    srand((unsigned)time(NULL));
    ball = new CBall[NUMBALLS];
    picX = picY = 0;
    enemy1 = new CEnemy[20];
    ///////
    counter = maxCounter = 40;
    currEnemy = 0;
    lock = 0;
    currLevel = 0;
}

CGameStateRun::~CGameStateRun() {
    delete[] ball;
    ////////
    delete[] enemy1;
}

void CGameStateRun::OnBeginState() {
    const int BALL_GAP = 90;
    const int BALL_XY_OFFSET = 45;
    const int BALL_PER_ROW = 7;
    const int HITS_LEFT = 10;
    const int HITS_LEFT_X = 590;
    const int HITS_LEFT_Y = 0;
    const int BACKGROUND_X = 60;
    const int ANIMATION_SPEED = 15;

    for (int i = 0; i < NUMBALLS; i++) {				// �]�w�y���_�l�y��
        int x_pos = i % BALL_PER_ROW;
        int y_pos = i / BALL_PER_ROW;
        ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
        ball[i].SetDelay(x_pos);
        ball[i].SetIsAlive(true);
    }

    eraser.Initialize();
    //background.SetTopLeft(BACKGROUND_X, 0);				// �]�w�I�����_�l�y��
    help.SetTopLeft(0, SIZE_Y - help.Height());			// �]�w�����Ϫ��_�l�y��
    //hits_left.SetInteger(HITS_LEFT);					// ���w�ѤU��������
    //hits_left.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);		// ���w�ѤU�����ƪ��y��
    //CAudio::Instance()->Play(AUDIO_LAKE, true);			// ���� WAVE
    //CAudio::Instance()->Play(AUDIO_DING, false);		// ���� WAVE
    //CAudio::Instance()->Play(AUDIO_NTUT, true);			// ���� MIDI

    /////// SET Eneny's ��l��

    for (int i = 0; i < levelEnemyNum[currLevel]; i++) {
        enemy1[i].SetXY(i * 50, 0);
        enemy1[i].SetDelay(10);
        enemy1[i].SetIsAlive(false);
    }

    score.SetInteger(0);			//�]�wSCORE��0;
    score.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);
}

void CGameStateRun::OnMove() {						// ���ʹC������
    //
    // �p�G�Ʊ�ק�cursor���˦��A�h�N�U���{����commment�����Y�i
    //
    //SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
    //
    // ���ʭI���Ϫ��y��
    //
    /*
    if (background.Top() > SIZE_Y)  // ���T�u�{�t�����쩳���� �^��W��
    	background.SetTopLeft(60, -background.Height());

    background.SetTopLeft(background.Left(), background.Top() + 1);
    */
    //
    // ���ʲy
    //
    int i;

    for (i = 0; i < NUMBALLS; i++)
        ball[i].OnMove();

    ////////////

    if (picX <= SIZE_Y) {
        picX += 5;
        picY += 5;
    }
    else {
        picX = picY = 0;
    }

    ////////
    counter--;	//�C���Ǫ� �ͦ����j �� counter (50~0)

    if (counter < 0 && currEnemy < levelEnemyNum[currLevel]) {	// counter �ƨ�0��N�}�l�l��s��
        counter = maxCounter;									// ��counter �զ^max�~���
        int randX = (rand() % (SIZE_X - 100)) ;					// SIZE_X - 100 ���F�����Ǫ�����r�W�X�ù��Ӧh
        enemy1[currEnemy].SetXY(randX, 0);
        enemy1[currEnemy].SetDelay(10);							// �q0�ƨ�o�ӼƦr�~�|��
        ////
        bool firstWordBounceFlag = 0;

        while (1) {
            for (int i = currEnemy - 1; i >= 0 ; i--) {
                if (enemy1[currEnemy].GetFirstWord() == enemy1[i].GetFirstWord())
                    firstWordBounceFlag = 1;
            }

            break;
            //if (firstWordBounceFlag == 1) enemy1[currEnemy].SetVocab();
            //else break;
        }

        enemy1[currEnemy].SetIsAlive(true);
        currEnemy++;
    }

    for (int i = 0; i < levelEnemyNum[currLevel]; i++)
        enemy1[i].OnMove();

    /////////////
    //
    // �������l
    //
    eraser.OnMove();

    //
    // �P�_���l�O�_�I��y
    //
    for (i = 0; i < NUMBALLS; i++)
        if (ball[i].IsAlive() && ball[i].HitEraser(&eraser)) {
            ball[i].SetIsAlive(false);
            CAudio::Instance()->Play(AUDIO_DING);
            hits_left.Add(-1);

            //
            // �Y�Ѿl�I�����Ƭ�0�A�h����Game Over���A
            //
            if (hits_left.GetInteger() <= 0) {
                CAudio::Instance()->Stop(AUDIO_LAKE);	// ���� WAVE
                CAudio::Instance()->Stop(AUDIO_NTUT);	// ���� MIDI
                GotoGameState(GAME_STATE_OVER);
            }
        }

    //
    // ���ʼu�����y
    //
    bball.OnMove();
}

void CGameStateRun::OnInit() {								// �C������Ȥιϧγ]�w
    //
    // ��ϫܦh�ɡAOnInit���J�Ҧ����ϭn��ܦh�ɶ��C���קK���C�����H
    //     �������@�СA�C���|�X�{�uLoading ...�v�A���Loading���i�סC
    //
    ShowInitProgress(33);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���33%
    //
    // �}�l���J���
    //

    for (int i = 0; i < NUMBALLS; i++)
        ball[i].LoadBitmap();								// ���J��i�Ӳy���ϧ�

    ////////
    for (int i = 0; i < levelEnemyNum[currLevel]; i++)
        enemy1[i].LoadBitmap();

    ////////
    eraser.LoadBitmap();
    //background.LoadBitmap(IDB_BACKGROUND);					// ���J�I�����ϧ�
    //
    // ��������Loading�ʧ@�A�����i��
    //
    ShowInitProgress(50);
    Sleep(0); // ��C�A�H�K�ݲM���i�סA��ڹC���ЧR����Sleep
    //
    // �~����J��L���
    //
    help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));				// ���J�������ϧ�
    corner.LoadBitmap(IDB_CORNER);							// ���J�����ϧ�
    //corner.ShowBitmap(background);							// �Ncorner�K��background
    bball.LoadBitmap();										// ���J�ϧ�
    //hits_left.LoadBitmap();
    score.LoadBitmap();
    CAudio::Instance()->Load(AUDIO_DING, "sounds\\ding.wav");	// ���J�s��0���n��ding.wav
    CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\lake.mp3");	// ���J�s��1���n��lake.mp3
    CAudio::Instance()->Load(AUDIO_NTUT, "sounds\\ntut.mid");	// ���J�s��2���n��ntut.mid
    //
    // ��OnInit�ʧ@�|����CGameStaterOver::OnInit()�A�ҥH�i���٨S��100%
    //
}
void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
    const char KEY_LEFT = 0x25; // keyboard���b�Y
    const char KEY_UP = 0x26; // keyboard�W�b�Y
    const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
    const char KEY_DOWN = 0x28; // keyboard�U�b�Y

    for (int i = 0; i < levelEnemyNum[currLevel]; i++) {					//�]�ثe���d�Ǫ����ƶq
        if (enemy1[i].IsAlive()) {					//�^�Ƿ�e�Ǫ��O�_�s�b
            if (lock == false) {							//�|����w�F
                if (nChar + 32 == enemy1[i].GetFirstWord()) {		//�Y����Ĥ@�Ӧr��:��� and �ثe�r����m+1
                    lock = true;
                    targetEnemy = &enemy1[i];      // targetEnemy������->���b�������ĤH
                    targetEnemy->AddCurrWordLeng();
                }
            }
            else {											//�Y�w��w
                if (nChar + 32 == targetEnemy->GetVocab()[targetEnemy->GetCurrWordLeng()]) { 	//�Y�����e�r��
                    targetEnemy->AddCurrWordLeng();

                    if (targetEnemy->GetCurrWordLeng() == targetEnemy->GetVocabLeng()) { //�Y��e���� ���� �r��������
                        targetEnemy->SetIsAlive(false);// ���\���`�Ǫ�
                        lock = false;
                        score.Add(targetEnemy->GetCurrWordLeng());
                    }
                }
            }
        }
        else {
            continue;
        }
    }

    /*
    if (nChar == KEY_LEFT)
        eraser.SetMovingLeft(true);

    if (nChar == KEY_RIGHT)
        eraser.SetMovingRight(true);

    if (nChar == KEY_UP)
        eraser.SetMovingUp(true);

    if (nChar == KEY_DOWN)
        eraser.SetMovingDown(true);
    */
}
void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
    const char KEY_LEFT = 0x25; // keyboard���b�Y
    const char KEY_UP = 0x26; // keyboard�W�b�Y
    const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
    const char KEY_DOWN = 0x28; // keyboard�U�b�Y

    if (nChar == KEY_LEFT)
        eraser.SetMovingLeft(false);

    if (nChar == KEY_RIGHT)
        eraser.SetMovingRight(false);

    if (nChar == KEY_UP)
        eraser.SetMovingUp(false);

    if (nChar == KEY_DOWN)
        eraser.SetMovingDown(false);
}
void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point) { // �B�z�ƹ����ʧ@
    eraser.SetMovingLeft(true);
}
void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point) {	// �B�z�ƹ����ʧ@
    eraser.SetMovingLeft(false);
}
void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point) {	// �B�z�ƹ����ʧ@
    // �S�ơC�p�G�ݭn�B�z�ƹ����ʪ��ܡA�gcode�b�o��
}
void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point) { // �B�z�ƹ����ʧ@
    eraser.SetMovingRight(true);
}
void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point) {	// �B�z�ƹ����ʧ@
    eraser.SetMovingRight(false);
}
void CGameStateRun::OnShow() {
    //
    //  �`�N�GShow�̭��d�U���n���ʥ��󪫥󪺮y�СA���ʮy�Ъ��u�@����Move���~��A
    //        �_�h��������sø�Ϯ�(OnDraw)�A����N�|���ʡA�ݰ_�ӷ|�ܩǡC���ӳN�y
    //        ���AMove�t�dMVC����Model�AShow�t�dView�A��View�������Model�C
    //
    //
    //  �K�W�I���ϡB�����ơB�y�B���l�B�u�����y
    //
    //background.ShowBitmap();			// �K�W�I����
    help.ShowBitmap();					// �K�W������
    //hits_left.ShowBitmap();

    for (int i = 100; i < NUMBALLS; i++) // �Ȯɳ]�w100
        ball[i].OnShow();				// �K�W��i���y(��ۣ)

    /////////

    for (int i = 0; i < levelEnemyNum[currLevel]; i++)
        enemy1[i].OnShow();

    score.ShowBitmap();
    /////////
    //bball.OnShow();						// �K�W�u�����y
    eraser.OnShow();					// �K�W���l
    //
    //  �K�W���W�Υk�U��������
    //
    corner.SetTopLeft(0, 0);
    corner.ShowBitmap();
    corner.SetTopLeft(SIZE_X - corner.Width(), SIZE_Y - corner.Height());
    corner.ShowBitmap();
}
}
/*
�o�{��BUG :		1. ���s�򪺳�r(bee), ����ĤG��e�N�|��������, �Pı�O�]����{���g�bkeydown�����Y
*/