/*
3/24
已知BUG :		[解決了] 1. 當有連續的單字(bee), 打到第二個e就會直接消失. 可能是keydown太靈敏.
				[解決了] 2. 比如場上兩隻單字頂真(appl"e" "e"gg), 在打完apple最後的"e"時, 他會直接接到egg開頭的"e"
				[解決了] 3. 為了讓單字不和場上重複, 會有已經new完怪物後, 但卻需要重新讓emeny SetVocab,
	    					這造成了eneny在LoadBitmap, 對話框長度是停留在原先的length, 而非新SetVocab
							後的那個單字長度. 但也不能重新LoadBitmap, 會出錯, 暫時讓對話框長度都一樣.
				[解決了] 4. 怪物越多的時候會開始當掉 -> 是因為使用老師內建顯示文字的關係,
					應該所有單字都做成bitmap的話就可以解決
				[解決了] 5. float的陷阱, 在計算dx的時候發生的

[Q&A]			1. ERROR: "將警告視為錯誤處理 - 沒有產生 'object' 檔案",
					可能是使用for迴圈和.size()時, 沒有把跑迴圈用的i設定成 unsigned int.

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
// 這個class為遊戲的遊戲開頭畫面物件
/////////////////////////////////////////////////////////////////////////////

CGameStateInit::CGameStateInit(CGame* g)
    : CGameState(g), NOTE_TEXT_X(60), NOTE_TEXT_Y(300) {
}

void CGameStateInit::OnInit() {
    ShowInitProgress(0);	// 一開始的loading進度為0%
    //
    // 開始載入資料
    //
    //logo.LoadBitmap(IDB_BACKGROUND);
    typing_logo.LoadBitmap("Bitmaps/start_logo1.bmp", RGB(0, 255, 0));		// logo
    text1.LoadBitmap("Bitmaps/text1_start.bmp", RGB(0, 255, 0));			// 按 滑鼠左鍵開始遊戲
    noteText.LoadBitmap("Bitmaps/note/note_text_zh.bmp", RGB(0, 255, 0));	// 說明框
    const unsigned int exkeyNum = 6;										// 說明框裡面的按鍵動畫 數量

    for (int i = 0; i < exkeyNum; i++) {											// 說明框裡面的按鍵動畫
        char str[30];
        sprintf(str, "Bitmaps/note/note1_exkey_%d.bmp", i + 1);
        noteExkey.AddBitmap(str, RGB(0, 255, 0));
    }

    map.LoadBitmap();														// 背景網狀動畫
    //
    // 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
    //
}

void CGameStateInit::OnBeginState() {
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
    const char KEY_ESC = 27;
    const char KEY_SPACE = ' ';

    if (nChar == KEY_SPACE)
        GotoGameState(GAME_STATE_RUN);						// 切換至GAME_STATE_RUN
    else if (nChar == KEY_ESC)								// Demo 關閉遊戲的方法
        PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point) {
    GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
}
void CGameStateInit::OnMove() {
    noteExkey.OnMove();
    map.OnMove();
}
void CGameStateInit::OnShow() {
    //
    // 貼上logo
    //
    //logo.SetTopLeft((SIZE_X - logo.Width()) / 2, SIZE_Y / 8);
    //logo.ShowBitmap();
    map.OnShow();
    typing_logo.SetTopLeft((SIZE_X - typing_logo.Width()) / 2, SIZE_Y / 5);
    typing_logo.ShowBitmap();
    text1.SetTopLeft((SIZE_X - text1.Width()) / 2, SIZE_Y / 5 + typing_logo.Height() + 180);
    text1.ShowBitmap();
    noteText.SetTopLeft(NOTE_TEXT_X, NOTE_TEXT_Y );
    noteText.ShowBitmap();
    noteExkey.SetTopLeft(NOTE_TEXT_X + 60, NOTE_TEXT_Y + 125 );
    noteExkey.OnShow();
    //
    // Demo螢幕字型的使用，不過開發時請盡量避免直接使用字型，改用CMovingBitmap比較好
    //
    /*
    CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
    CFont f, *fp;
    f.CreatePointFont(160, "Consolas");	// 產生 font f; 160表示16 point的字
    fp = pDC->SelectObject(&f);					// 選用 font f
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(41, 171, 226));
    pDC->TextOut(180, 350, "Huang Xingqiao / Yu kaici");  //test text

    if (ENABLE_GAME_PAUSE)
        pDC->TextOut(5, 425, "Press Ctrl-Q to pause the Game.");

    pDC->TextOut(5, 455, "Press Alt-F4 or ESC to Quit.");
    pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
    CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
    */
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
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
    // 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
    //     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
    //
    ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
    //
    // 開始載入資料
    //
    Sleep(300);				// 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
    //
    // 最終進度為100%
    //
    ShowInitProgress(100);
}

void CGameStateOver::OnShow() {
    CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
    CFont f, *fp;
    f.CreatePointFont(160, "Consolas");			// 產生 font f; 160表示16 point的字
    fp = pDC->SelectObject(&f);					// 選用 font f
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(255, 255, 0));
    char str[80];								// Demo 數字對字串的轉換
    sprintf(str, "Game Over(%d)", counter / 30);
    pDC->TextOut(240, 210, str);
    //	顯示分數 (not done)
    // char scoreChr[80];
    // sprintf(str, "SCORE: %d", score);
    //
    pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
    CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame* g)
    : CGameState(g), NUMBALLS(28), LEVEL(10) {
    srand((unsigned)time(NULL));	// 亂數種子
    callEnemyCounter = maxCallEnemyCounter = 30;	// maxCallEnemyCounter 決定怪物生成速度
    callBossACounter = maxCallBossACounter = 100;
    callBossBCounter = maxCallBossBCounter = 100;
}

CGameStateRun::~CGameStateRun() {
    //delete[] ball;
    //delete[] &enemyQueue;
}

void CGameStateRun::OnBeginState() {
    const int BALL_GAP = 90;
    const int BALL_XY_OFFSET = 45;
    const int BALL_PER_ROW = 7;
    const int HITS_LEFT = 10;
    const int HITS_LEFT_X = SIZE_X - 100;
    const int HITS_LEFT_Y = 20;
    const int BACKGROUND_X = 60;
    const int ANIMATION_SPEED = 15;
    const int SCORE_X = 240, SCORE_Y = 240;
    /* 老師的示範球
    for (int i = 0; i < NUMBALLS; i++) {				// 設定球的起始座標
        int x_pos = i % BALL_PER_ROW;
        int y_pos = i / BALL_PER_ROW;
        ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
        ball[i].SetDelay(x_pos);
        ball[i].SetIsAlive(true);
    }
    */
    //background.SetTopLeft(BACKGROUND_X, 0);				// 設定背景的起始座標
    help.SetTopLeft(0, SIZE_Y - help.Height());			// 設定說明圖的起始座標
    //hits_left.SetInteger(HITS_LEFT);					// 指定剩下的撞擊數
    //hits_left.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);		// 指定剩下撞擊數的座標
    //CAudio::Instance()->Play(AUDIO_LAKE, true);			// 撥放 WAVE
    //CAudio::Instance()->Play(AUDIO_DING, false);		// 撥放 WAVE
    //CAudio::Instance()->Play(AUDIO_NTUT, true);			// 撥放 MIDI
    //
    score.SetInteger(0);			//設定SCORE為0
    score.SetTopLeft(SCORE_X, SCORE_Y);
    currEnemyNum = currBossANum = currBossBNum = 0;
    lock = false;
    currLevel = 0;
    enemyQueue.clear();
    lives = 3;
}

void CGameStateRun::OnMove() {						// 移動遊戲元素
    //
    // 如果希望修改cursor的樣式，則將下面程式的commment取消即可
    //
    //SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
    //
    ////////
    callEnemyCounter--;	//每隻怪物 生成間隔 之 counter
    callBossACounter--;

    if (callEnemyCounter < 0 && currEnemyNum < levelEnemyNum[currLevel]) {	// counter 數到0後就開始召喚新怪
        callEnemyCounter = maxCallEnemyCounter;				// 把counter 調回max繼續數
        int randX = (rand() % (SIZE_X - 100)) ;				// SIZE_X - 100 為了不讓怪物的單字超出螢幕太多
        enemyQueue.push_back(new CEnemy(randX, 0, 2, false, &dictionary, 2, 6) );
        enemyQueue.back()->LoadBitmap();
        // 注意: 下面enemyQueue.back()指的都是剛新增的那隻怪物

        while (1) {								//	此迴圈 檢查新召喚的怪物 是否跟場上現有的第一個字撞
            bool firstWordBounceFlag = 0;		//	有撞到第一個單字的flag

            for (int i = enemyQueue.size() - 1; i >= 0 ; i--) {
                if (enemyQueue.back()->GetFirstWord() == enemyQueue[i]->GetFirstWord() && enemyQueue[i]->IsAlive())
                    firstWordBounceFlag = 1;
            }

            if (firstWordBounceFlag && !(enemyQueue.size() >= 24)) enemyQueue.back()->SetVocab();
            else break;
        }

        enemyQueue.back()->SetIsAlive(true);
        currEnemyNum++;
    }

    if (callBossACounter < 0 && currBossANum < levelBossANum[currLevel]) {	// counter 數到0後就開始召喚新怪
        callBossACounter = maxCallBossACounter;				// 把counter 調回max繼續數
        int randX = (rand() % (SIZE_X - 100));				// SIZE_X - 100 為了不讓怪物的單字超出螢幕太多
        enemyQueue.push_back(new CBossA((rand() % (SIZE_X - 100)), 0, 5, false, &dictionary, 6, 20, &enemyQueue));
        enemyQueue.back()->LoadBitmap();
        // 注意: 下面enemyQueue.back()指的都是剛新增的那隻怪物

        while (1) {								//	此迴圈 檢查新召喚的怪物 是否跟場上現有的第一個字撞
            bool firstWordBounceFlag = 0;		//	有撞到第一個單字的flag

            for (int i = enemyQueue.size() - 1; i >= 0; i--) {
                if (enemyQueue.back()->GetFirstWord() == enemyQueue[i]->GetFirstWord() && enemyQueue[i]->IsAlive())
                    firstWordBounceFlag = 1;
            }

            if (firstWordBounceFlag && !(enemyQueue.size() >= 24)) enemyQueue.back()->SetVocab();
            else break;
        }

        enemyQueue.back()->SetIsAlive(true);
        currBossANum++;
    }

    //===判斷Me是否碰到Enemy===

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
        //若Enemy IsAlive=0, 則從vector中移除
        if (!enemyQueue[i]->IsAlive() && bulletList.size() == 0) {
            // push_back爆炸效果
            bombList.push_back(new CBomb(enemyQueue[i]->GetX(), enemyQueue[i]->GetY()));
            bombList.back()->LoadBitmap();
            vector<CEnemy*>::iterator iterenemyQueue = enemyQueue.begin();
            enemyQueue.erase(iterenemyQueue + i);
            break;
        }
    }

    ////===bullet===

    for (unsigned int i = 0; i < bulletList.size(); i++)
        bulletList[i]->OnMove();	// 移動bullet

    for (int i = bulletList.size() - 1; i >= 0; i--) {
        //若bullet IsAlive=0, 則從vector中移除
        if (!bulletList[i]->IsAlive())	bulletList.erase(bulletList.begin());
    }

    ////===bomb===

    for (unsigned int i = 0; i < bombList.size(); i++) {
        bombList[i]->OnMove();
    }

    for (int i = bombList.size() - 1; i >= 0; i--) {
        //若 bomb IsAlive=0, 則從vector中移除
        if (!bombList[i]->IsAlive())	bombList.erase(bombList.begin());
    }

    ////===Change Level===
    if (currEnemyNum >= levelEnemyNum[currLevel] && currBossANum >= levelBossANum[currLevel] \
            && currBossBNum >= levelBossBNum[currLevel] && enemyQueue.size() == 0) {
        // 換 關卡
        currLevel++;
        currEnemyNum = currBossANum = currBossBNum = 0;
        callEnemyCounter = maxCallEnemyCounter;
        callBossACounter = maxCallBossACounter;
        callBossBCounter = maxCallBossBCounter;
    }

    map.OnMove();
    me.OnMove();
}

void CGameStateRun::OnInit() {								// 遊戲的初值及圖形設定
    //
    // 當圖很多時，OnInit載入所有的圖要花很多時間。為避免玩遊戲的人
    //     等的不耐煩，遊戲會出現「Loading ...」，顯示Loading的進度。
    //
    srand((unsigned)time(NULL));
    ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
    //
    // 開始載入資料
    //
    //eraser.LoadBitmap();
    //background.LoadBitmap(IDB_BACKGROUND);					// 載入背景的圖形
    //
    // 完成部分Loading動作，提高進度
    //
    ShowInitProgress(50);
    Sleep(0); // 放慢，以便看清楚進度，實際遊戲請刪除此Sleep
    //
    // 繼續載入其他資料
    //
    help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));			// 載入說明的圖形
    score.LoadBitmap();
    map.LoadBitmap();
    me.LoadBitmap();
    //hits_left.LoadBitmap();
    //corner.ShowBitmap(background);							// 將corner貼到background
    CAudio::Instance()->Load(AUDIO_DING, "sounds\\ding.wav");	// 載入編號0的聲音ding.wav
    CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\lake.mp3");	// 載入編號1的聲音lake.mp3
    CAudio::Instance()->Load(AUDIO_NTUT, "sounds\\ntut.mid");	// 載入編號2的聲音ntut.mid
    //
    // 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
    //
}
void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
    const char KEY_LEFT = 0x25;	// keyboard左箭頭
    const char KEY_UP = 0x26;	// keyboard上箭頭
    const char KEY_RIGHT = 0x27; // keyboard右箭頭
    const char KEY_DOWN = 0x28; // keyboard下箭頭

    if (nChar == '1')showDebug = 1;	// 按1 顯示debug

    if (nChar == '2')showDebug = 0;

    if (nChar == '3' && enemyQueue.size() > 0) enemyQueue.back()->SetIsAlive(false);
}
void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
    const char KEY_LEFT = 0x25; // keyboard左箭頭
    const char KEY_UP = 0x26; // keyboard上箭頭
    const char KEY_RIGHT = 0x27; // keyboard右箭頭
    const char KEY_DOWN = 0x28; // keyboard下箭頭

    for (int unsigned i = 0; i < enemyQueue.size(); i++) {			// 跑目前關卡怪物的數量
        if (enemyQueue[i]->IsAlive()) {								// 回傳當前怪物是否存在
            if (!lock) {										// 尚未鎖定了
                if (nChar + 32 == enemyQueue[i]->GetFirstWord()) {	// 若等於第一個字母:鎖住 and 目前字元位置+1
                    lock = true;
                    targetEnemy = enemyQueue[i];					// targetEnemy為指標->正在攻擊的敵人
                    targetEnemy->AddCurrWordLeng();
                    bulletList.push_back(new CBullet(targetEnemy->GetX() + 10, targetEnemy->GetY() + 10));	// 射子彈
                    targetEnemy->MinusIndex(2);					// 擊退怪物
                }
            }
            else {												// 若已鎖定
                if (nChar + 32 == targetEnemy->GetVocab()[targetEnemy->GetCurrWordLeng()]) { 	// 若等於當前字母
                    targetEnemy->AddCurrWordLeng();
                    bulletList.push_back(new CBullet(targetEnemy->GetX(), targetEnemy->GetY()));
                    targetEnemy->MinusIndex(rand() % 2 + 1);		// 擊退怪物

                    if (targetEnemy->GetCurrWordLeng() == targetEnemy->GetVocabLeng()) {	 // 若當前長度 等於 字母的長度
                        targetEnemy->SetIsAlive(false);									// 成功殺害怪物
                        lock = false;
                        score.Add(targetEnemy->GetCurrWordLeng());						// 分數+= 怪物長度
                    }

                    break;
                }
            }
        }
    }
}
void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point) { // 處理滑鼠的動作
}
void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point) {	// 處理滑鼠的動作
    //eraser.SetMovingLeft(false);
}
void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point) {	// 處理滑鼠的動作
    // 沒事。如果需要處理滑鼠移動的話，寫code在這裡
}
void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point) { // 處理滑鼠的動作
    //eraser.SetMovingRight(true);
}
void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point) {	// 處理滑鼠的動作
    //eraser.SetMovingRight(false);
}
void CGameStateRun::OnShow() {
    //
    //  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，d
    //        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
    //        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
    //
    //
    //
    map.OnShow();						// 貼上背景網子
    help.ShowBitmap();					// 貼上說明圖
    score.ShowBitmap();					// 貼上分數
    me.OnShow();
    /////////

    for (unsigned int i = 0; i < enemyQueue.size(); i++)
        enemyQueue[i]->OnShow();

    for (unsigned int i = 0; i < bulletList.size(); i++)
        bulletList[i]->OnShow();

    for (unsigned int i = 0; i < bombList.size(); i++) {
        bombList[i]->OnShow();
    }

    if (lock && targetEnemy->IsAlive())
        targetEnemy->OnShow();	// 加上這一行 讓被鎖定的怪物再次show, 以防被其他怪物蓋住

    //
    //  貼上左上及右下角落的圖
    //
    /*
    corner.SetTopLeft(0, 0);
    corner.ShowBitmap();
    corner.SetTopLeft(SIZE_X - corner.Width(), SIZE_Y - corner.Height());
    corner.ShowBitmap();
    */
    //
    //

    if (showDebug) {		// 顯示debug資訊
        CDC* pDC = CDDraw::GetBackCDC();
        CFont f, *fp;
        f.CreatePointFont(120, "Fixedsys");
        fp = pDC->SelectObject(&f);
        pDC->SetBkColor(RGB(0, 0, 0));
        pDC->SetBkMode(TRANSPARENT);
        //
        char temp[50];
        sprintf(temp, "enemyQueue.size: %d, Lives:%d, currLevel: %d", enemyQueue.size(), lives, currLevel);
        pDC->SetTextColor(RGB(200, 0, 0));
        pDC->TextOut(20, 20, temp);

        for (unsigned int i = 0; i < enemyQueue.size(); i++) {	// 顯示場上怪物之 單字,curr/length
            char temp[40];
            sprintf(temp, "%s %d/%d (x:%d,y:%d)", enemyQueue[i]->GetVocab().c_str(), enemyQueue[i]->GetCurrWordLeng(), enemyQueue[i]->GetVocabLeng(), enemyQueue[i]->GetX(), enemyQueue[i]->GetY());
            pDC->SetTextColor(RGB(180 + i, 180 + i, 180 + i));
            pDC->TextOut(20, i * 14 + 40, temp);
        }

        sprintf(temp, "Bullet Numbers: %d", bulletList.size());
        pDC->SetTextColor(RGB(200, 200, 200 ));
        pDC->TextOut(20, 2, temp);
        ////
        pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
        CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
    }
}
}
