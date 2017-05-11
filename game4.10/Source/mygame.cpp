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
				[解決了] 6. #pragma once

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
    : CGameState(g), NOTE_TEXT_X(60), NOTE_TEXT_Y(300), MENU_POS_Y(350),
      MENU_ITEM_NUM(4) {
}

void CGameStateInit::OnInit() {
    ShowInitProgress(0);	// 一開始的loading進度為0%
    const unsigned int exkeyNum = 6;										// 說明框裡面的按鍵動畫 數量
    currSelectItem = displayState = 1;
    noteDisplayState = 0;
    map.LoadBitmap();														// 背景網狀動畫
    typing_logo.LoadBitmap("Bitmaps/start_logo1.bmp", RGB(0, 255, 0));		// logo
    text1.LoadBitmap("Bitmaps/text1_start.bmp", RGB(0, 255, 0));			// 按 滑鼠左鍵開始遊戲
    /////
    //
    // 載入選單元素

    for (int i = 0; i < 4; i++) {		// 4個選單
        char str[40];
        sprintf(str, "Bitmaps/menu/menu_t_%d.bmp", i + 1);
        menuText.push_back(new CMovingBitmap);
        menuText.back()->LoadBitmap(str, RGB(0, 255, 0));
    }

    menuBorder.LoadBitmap("Bitmaps/menu/menu_border.bmp", RGB(0, 255, 0));
    menuBorder_ckecked.LoadBitmap("Bitmaps/menu/menu_border_checked.bmp", RGB(0, 255, 0));
    menuText.push_back(new CMovingBitmap);
    menuText.back()->LoadBitmap("Bitmaps/menu/menu_t_b.bmp", RGB(0, 255, 0));
    // 載入遊戲說明元素
    noteBorder.LoadBitmap("Bitmaps/menu/note/note_text_border.bmp", RGB(0, 255, 0)); // 說明框線
    noteArrow.LoadBitmap ("Bitmaps/menu/note/note_text_arraw.bmp", RGB(0, 255, 0));	// 說明箭頭
    noteUnselected.LoadBitmap("Bitmaps/menu/note/note_unselected.bmp", RGB(0, 255, 0));
    noteSelected.LoadBitmap("Bitmaps/menu/note/note_selected.bmp", RGB(0, 255, 0));

    for (int i = 0; i < exkeyNum; i++) {	// 說明框裡面的按鍵動畫
        char str[50];
        sprintf(str, "Bitmaps/menu/note/note1_exkey_%d.bmp", i + 1);
        noteExkey.AddBitmap(str, RGB(0, 255, 0));
    }

    for (int i = 0; i < 3; i++) {		// 多頁的說明文字
        char str[50];
        sprintf(str, "Bitmaps/menu/note/note_text_p%d.bmp", i + 1);
        note.push_back(new CMovingBitmap);
        note.back()->LoadBitmap(str, RGB(0, 255, 0));
    }

    //
    // 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
    //
}

void CGameStateInit::OnBeginState() {
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
    const char KEY_ESC = 27;
    const char KEY_SPACE = ' ';
    const char KEY_ENTER = 0xD;
    const char KEY_LEFT = 0x25;	// keyboard左箭頭
    const char KEY_UP = 0x26;	// keyboard上箭頭
    const char KEY_RIGHT = 0x27; // keyboard右箭頭
    const char KEY_DOWN = 0x28; // keyboard下箭頭

    if (displayState == 0 && (nChar == KEY_UP || nChar == KEY_DOWN)) { // 移動主選單
        if (nChar == KEY_UP) currSelectItem--;
        else if (nChar == KEY_DOWN) currSelectItem++;

        if (currSelectItem < 0)currSelectItem = MENU_ITEM_NUM - 1;
        else if (currSelectItem > MENU_ITEM_NUM - 1)currSelectItem = 0;
    }

    if (displayState == 0 && nChar == KEY_ENTER ) { // 按下ENTER選擇後...
        if ( currSelectItem == 0 ) // 開始遊戲
            GotoGameState(GAME_STATE_RUN);

        if ( currSelectItem == 1 ) {
            displayState = 1;	// 顯示遊戲說明的state
            noteDisplayState = 0;
        }
    }
    else if (displayState == 1 ) { // [遊戲說明]
        if (nChar == KEY_ENTER)displayState = 0;	// ->返回主選單
        else if (nChar == KEY_LEFT || nChar == KEY_RIGHT) { // [遊戲說明] 左右翻頁遊戲說明
            if		(nChar == KEY_LEFT)   noteDisplayState--;
            else if (nChar == KEY_RIGHT) noteDisplayState++;

            if (noteDisplayState < 0) noteDisplayState = int(note.size()) - 1;
            else if (noteDisplayState >  int(note.size()) - 1) noteDisplayState = 0;
        }
    }
    else if (1) {
    }

    //PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);	// 關閉遊戲
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point) {
    //GotoGameState(GAME_STATE_RUN);		// 切換至GAME_STATE_RUN
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

    if (displayState == 0) {	// 顯示主選單
        menuBorder_ckecked.SetTopLeft((SIZE_X - menuBorder.Width()) / 2, MENU_POS_Y + 40 * currSelectItem);
        menuBorder_ckecked.ShowBitmap();

        for (int i = 0; i < MENU_ITEM_NUM; i++) {
            menuBorder.SetTopLeft((SIZE_X - menuBorder.Width()) / 2, MENU_POS_Y + 40 * i);
            menuBorder.ShowBitmap();
            menuText[i]->SetTopLeft((SIZE_X - menuText[i]->Width()) / 2, MENU_POS_Y + 7 + 40 * i);
            menuText[i]->ShowBitmap();
        }
    }
    else if (displayState == 1) {	// 顯示說明文字
        // 說明框線
        noteBorder.SetTopLeft((SIZE_X - noteBorder.Width()) / 2, NOTE_TEXT_Y);
        noteBorder.ShowBitmap();
        // 說明箭頭
        noteArrow.SetTopLeft((SIZE_X - noteArrow.Width()) / 2, NOTE_TEXT_Y + (noteBorder.Height()  - noteArrow.Height()) / 2 + 11);
        noteArrow.ShowBitmap();
        // 說明文字
        note[noteDisplayState]->SetTopLeft((SIZE_X - noteBorder.Width()) / 2, NOTE_TEXT_Y + (noteBorder.Height() - note[noteDisplayState]->Height() ) / 2 + 11);
        note[noteDisplayState]->ShowBitmap();

        // 說明文字 指示燈
        for (unsigned int i = 0; i < note.size(); i++) {
            noteUnselected.SetTopLeft((SIZE_X - noteBorder.Width()) / 2 + noteBorder.Width() - 25 - 8 * i, NOTE_TEXT_Y + noteBorder.Height() - 20);
            noteUnselected.ShowBitmap();
        }

        noteSelected.SetTopLeft((SIZE_X - noteBorder.Width()) / 2 + noteBorder.Width() - 25 - 8 * (note.size() - noteDisplayState - 1), NOTE_TEXT_Y + noteBorder.Height() - 20);
        noteSelected.ShowBitmap();

        // 說明打字動畫
        if (noteDisplayState == 0) {
            noteExkey.SetTopLeft((SIZE_X - noteExkey.Width()) / 2, NOTE_TEXT_Y + 150);
            noteExkey.OnShow();
        }

        // 顯示偽返回按鈕
        const int BACK_BTN_POS = 170;
        menuBorder_ckecked.SetTopLeft((SIZE_X - menuBorder.Width()) / 2, MENU_POS_Y + BACK_BTN_POS);
        menuBorder_ckecked.ShowBitmap();
        menuBorder.SetTopLeft((SIZE_X - menuBorder.Width()) / 2, MENU_POS_Y + BACK_BTN_POS);
        menuBorder.ShowBitmap();
        menuText[4]->SetTopLeft((SIZE_X - menuText[4]->Width()) / 2, MENU_POS_Y + 7 + BACK_BTN_POS);
        menuText[4]->ShowBitmap();
    }
    else if (displayState == 2) {
        // 選擇角色
    }
    else if (displayState == 3) {
        // 關於
    }
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
    callEnemyCounter = maxCallEnemyCounter = 20;	// maxCallEnemyCounter 決定怪物生成速度
    callBossACounter = maxCallBossACounter = 80;
    callBossBCounter = maxCallBossBCounter = 80;
}

CGameStateRun::~CGameStateRun() {
    //delete[] ball;
    //delete[] &enemyQueue;
}

void CGameStateRun::OnBeginState() {
    const int SCORE_X = 240, SCORE_Y = 240;
    help.SetTopLeft(0, SIZE_Y - help.Height());			// 設定說明圖的起始座標
    //CAudio::Instance()->Play(AUDIO_LAKE, true);			// 撥放 WAVE
    //CAudio::Instance()->Play(AUDIO_DING, false);		// 撥放 WAVE
    //CAudio::Instance()->Play(AUDIO_NTUT, true);			// 撥放 MIDI
    CAudio::Instance()->Play(AUDIO_ROCK, true);			// 撥放 MIDI
    score.SetInteger(0);			//設定SCORE為0
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

void CGameStateRun::OnMove() {						// 移動遊戲元素
    //
    // 如果希望修改cursor的樣式，則將下面程式的commment取消即可
    //
    //SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
    //
    ////////
    callEnemyCounter--;	//每隻怪物 生成間隔 之 counter
    callBossACounter--;
    callBossBCounter--;

    //==小怪==================================
    if (callEnemyCounter < 0 && currEnemyNum < levelEnemyNum[currLevel]) {	// counter 數到0後就開始召喚新怪
        callEnemyCounter = maxCallEnemyCounter;				// 把counter 調回max繼續數
        int randX = (rand() % (SIZE_X - 100)) ;				// SIZE_X - 100 為了不讓怪物的單字超出螢幕太多
        enemyQueue.push_back(new CEnemy(randX, 0, 3, true, &dictionary, 2, 6, &enemyQueue, &bombList, me.GetX1(), me.GetY1()) );
        enemyQueue.back()->LoadBitmap();
        currEnemyNum++;
    }

    //==BossA==================================
    if (callBossACounter < 0 && currBossANum < levelBossANum[currLevel]) {	// counter 數到0後就開始召喚新怪
        callBossACounter = maxCallBossACounter;				// 把counter 調回max繼續數
        int randX = (rand() % (SIZE_X - 100));
        enemyQueue.push_back(new CBossA(randX, 0, 6, true, &dictionary, 6, 20, &enemyQueue, &bombList));
        enemyQueue.back()->LoadBitmap();
        currBossANum++;
    }

    //==BossB==================================
    if (callBossBCounter < 0 && currBossBNum < levelBossBNum[currLevel]) {	// counter 數到0後就開始召喚新怪
        callBossBCounter = maxCallBossBCounter;				// 把counter 調回max繼續數
        int randX = (rand() % (SIZE_X - 100));
        enemyQueue.push_back(new CBossB(randX, 0, 6, true, &dictionary, 6, 20, &enemyQueue, &bombList));
        enemyQueue.back()->LoadBitmap();
        currBossBNum++;
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
        if (enemyQueue[i]->GetX() > SIZE_X + 40 || enemyQueue[i]->GetX() < -40 || enemyQueue[i]->GetY() > SIZE_Y + 40) {
            enemyQueue[i]->kill();
        }
    }

    for (unsigned int i = 0; i < enemyQueue.size(); i++) {
        //若Enemy IsAlive=0, 則從vector中移除
        if (!enemyQueue[i]->IsAlive() && enemyQueue[i]->IsBombed()) {
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
    emp.OnMove();
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
    emp.LoadBitmap();
    me.LoadBitmap();
    //hits_left.LoadBitmap();
    //corner.ShowBitmap(background);							// 將corner貼到background
    //CAudio::Instance()->Load(AUDIO_DING, "sounds\\ding.wav");	// 載入編號0的聲音ding.wav
    //CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\lake.mp3");	// 載入編號1的聲音lake.mp3
    //CAudio::Instance()->Load(AUDIO_NTUT, "sounds\\ntut.mid");	// 載入編號2的聲音ntut.mid
    CAudio::Instance()->Load(AUDIO_ROCK, "sounds\\The_Coming_Storm.mp3");	// 載入編號3的聲音The_Coming_Storm.mp3
    CAudio::Instance()->Load(AUDIO_SHOT, "sounds\\shot.mp3");
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

    if (nChar == '3' && enemyQueue.size() > 0) {
        enemyQueue.back()->kill();
        lock = 0;
    }
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
                    totalCorrectKeyCount++;							// 正確按鍵數+1
                    CAudio::Instance()->Play(AUDIO_SHOT, false);			// 撥放 射擊音效

                    if (enemyQueue[i]->GetVocabLeng() == 1) {
                        targetEnemy = enemyQueue[i];					// targetEnemy為指標->正在攻擊的敵人
                        bulletList.push_back(new CBullet(targetEnemy->GetX() + 10, targetEnemy->GetY() + 10));	// 射子彈
                        targetEnemy->kill();									// 成功殺害怪物
                        score.Add(targetEnemy->GetVocabLeng());				// 分數+= 怪物長度
                        break;
                    }
                    else {
                        lock = true;
                        targetEnemy = enemyQueue[i];					// targetEnemy為指標->正在攻擊的敵人
                        targetEnemy->AddCurrWordLeng();
                        bulletList.push_back(new CBullet(targetEnemy->GetX() + 10, targetEnemy->GetY() + 10));	// 射子彈
                        targetEnemy->MinusIndex(2);					// 擊退怪物
                        break;
                    }
                }
            }
            else {												// 若已鎖定
                if (nChar + 32 == targetEnemy->GetVocab()[targetEnemy->GetCurrWordLeng()]) { 	// 若等於當前字母
                    targetEnemy->AddCurrWordLeng();
                    bulletList.push_back(new CBullet(targetEnemy->GetX(), targetEnemy->GetY()));
                    targetEnemy->MinusIndex(rand() % 2 + 1);		// 擊退怪物
                    totalCorrectKeyCount++;							// 正確按鍵數+1
                    CAudio::Instance()->Play(AUDIO_SHOT, false);			// 撥放 射擊音效

                    if (targetEnemy->GetCurrWordLeng() == targetEnemy->GetVocabLeng()) {	 // 若當前長度 等於 字母的長度
                        targetEnemy->kill();									// 成功殺害怪物
                        lock = false;
                        score.Add(targetEnemy->GetVocabLeng());						// 分數+= 怪物長度
                    }

                    break;
                }
            }
        }
    }

    if (nChar >= 65 && nChar <= 90) totalKeyDownCount++;			// 總按鍵數++

    if (totalKeyDownCount != 0)
        accuracy = 100 * double(totalCorrectKeyCount) / double(totalKeyDownCount);
    else
        accuracy = 100;

    if (nChar == 13) {
        emp.CallEmp();
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
    //help.ShowBitmap();					// 貼上說明圖
    score.ShowBitmap();					// 貼上分數
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
        f.CreatePointFont(100, "Fixedsys");
        fp = pDC->SelectObject(&f);
        pDC->SetBkColor(RGB(0, 0, 0));
        pDC->SetBkMode(TRANSPARENT);
        //
        char temp[100];
        sprintf(temp, "怪物數量: %d, 命: %d, 本關卡: %d, LOCK: %d", enemyQueue.size(), lives, currLevel, int(lock));
        pDC->SetTextColor(RGB(200, 0, 0));
        pDC->TextOut(20, 40, temp);
        //
        sprintf(temp, "總按鍵數: %d 總正確按鍵數: %d , accuracy正確率: %.2lf%%", totalKeyDownCount, totalCorrectKeyCount, accuracy);
        pDC->SetTextColor(RGB(50, 200, 200));
        pDC->TextOut(20, 20, temp);

        //
        for (unsigned int i = 0; i < enemyQueue.size(); i++) {	// 顯示場上怪物之 單字,curr/length
            char temp[40];
            sprintf(temp, "%s %d/%d (x:%d,y:%d)", enemyQueue[i]->GetVocab().c_str(), enemyQueue[i]->GetCurrWordLeng(), enemyQueue[i]->GetVocabLeng(), enemyQueue[i]->GetX(), enemyQueue[i]->GetY());
            pDC->SetTextColor(RGB(180 + i, 180 + i, 180 + i));
            pDC->TextOut(20, i * 14 + 60, temp);
        }

        sprintf(temp, "子彈數: %d", bulletList.size());
        pDC->SetTextColor(RGB(200, 200, 200 ));
        pDC->TextOut(20, 2, temp);
        ////
        pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
        CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
    }
}
}
