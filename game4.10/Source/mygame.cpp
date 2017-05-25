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
    ShowInitProgress(0);	// 一開始的loading進度為0%
    const unsigned int exkeyNum = 6;										// 說明框裡面的按鍵動畫 數量
    currSelectItem = displayState = 0;										// 初始化選單選取項目
    noteDisplayState = 0;
    PublicData::me.LoadBitmap();											// 主角
    map.LoadBitmap();														// 背景網狀動畫
    typing_logo.LoadBitmap("Bitmaps/start_logo1.bmp", RGB(0, 255, 0));		// logo
    taipin.LoadBitmap("Bitmaps/taipin.bmp", RGB(0, 255, 0));
    text1.LoadBitmap("Bitmaps/text1_start.bmp", RGB(0, 255, 0));			// 操作方式提示bitmap
    highScoreBorder.LoadBitmap("Bitmaps/menu/highscore_border.bmp", RGB(0, 255, 0));
    /////
    //
    // 載入選單元素
    ShowInitProgress(5);

    for (int i = 0; i < 5; i++) {		// 5個選單
        char str[50];
        sprintf(str, "Bitmaps/menu/menu_t_%de.bmp", i + 1);
        menuText.push_back(new CMovingBitmap);
        menuText.back()->LoadBitmap(str, RGB(0, 255, 0));
    }

    ShowInitProgress(10);
    menuText.push_back(new CMovingBitmap);
    menuText.back()->LoadBitmap("Bitmaps/menu/menu_t_be.bmp", RGB(0, 255, 0));	// 返回按鈕
    menuBorder.LoadBitmap("Bitmaps/menu/menu_border.bmp", RGB(0, 255, 0));
    menuBorder_ckecked.LoadBitmap("Bitmaps/menu/menu_border_checked.bmp", RGB(0, 255, 0));
    // 載入遊戲說明元素
    noteBorder.LoadBitmap("Bitmaps/menu/note/note_text_border.bmp", RGB(0, 255, 0)); // 說明框線
    noteArrow.LoadBitmap ("Bitmaps/menu/note/note_text_arraw.bmp", RGB(0, 255, 0));	// 說明箭頭
    noteUnselected.LoadBitmap("Bitmaps/menu/note/note_unselected.bmp", RGB(0, 255, 0));
    noteSelected.LoadBitmap("Bitmaps/menu/note/note_selected.bmp", RGB(0, 255, 0));
    ShowInitProgress(15);

    for (int i = 0; i < exkeyNum; i++) {	// 說明框裡面的按鍵動畫
        char str[50];
        sprintf(str, "Bitmaps/menu/note/note1_exkey_%d.bmp", i + 1);
        noteExkey.AddBitmap(str, RGB(0, 255, 0));
    }

    for (int i = 0; i < 8; i++) {		// 多頁的說明文字
        char str[50];
        sprintf(str, "Bitmaps/menu/note/note_text_p%d.bmp", i + 1);
        note.push_back(new CMovingBitmap);
        note.back()->LoadBitmap(str, RGB(0, 255, 0));
    }

    for (int i = 0; i < 10; i++) {		// 載入數字圖
        char str[50];
        sprintf(str, "Bitmaps/level/num/%d.bmp", i);
        numBmp[i].LoadBitmap(str, RGB(0, 255, 0));
        sprintf(str, "Bitmaps/level/num_s/%d.bmp", i);
        numBmpSmall[i].LoadBitmap(str, RGB(0, 255, 0));
    }

    ShowInitProgress(20);
    // 載入角色選擇 元素
    characterBorder.LoadBitmap("Bitmaps/menu/character/character_border.bmp", RGB(0, 255, 0));
    characterArrow.LoadBitmap("Bitmaps/menu/character/character_arraw.bmp", RGB(0, 255, 0));
    // 載入關於元素
    aboutBorder.LoadBitmap("Bitmaps/menu/about/about_border.bmp", RGB(0, 255, 0)); // 介紹框線
    about.LoadBitmap("Bitmaps/menu/about/about_text_p2.bmp", RGB(0, 255, 0)); // 介紹文字
    //
    // 此OnInit動作會接到CGameStaterRun::OnInit()，所以進度還沒到100%
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
    const char KEY_LEFT = 0x25;	// keyboard左箭頭
    const char KEY_UP = 0x26;	// keyboard上箭頭
    const char KEY_RIGHT = 0x27; // keyboard右箭頭
    const char KEY_DOWN = 0x28; // keyboard下箭頭

    if (nChar == KEY_ESC)   displayState = 0; // ESC鍵返回主選單

    if (displayState == 0 ) { // 在主選單...
        if (nChar == KEY_UP || nChar == KEY_DOWN) {		// 移動光標
            if (nChar == KEY_UP) currSelectItem--;
            else if (nChar == KEY_DOWN) currSelectItem++;

            if (currSelectItem < 0)currSelectItem = MENU_ITEM_NUM - 1;
            else if (currSelectItem > MENU_ITEM_NUM - 1)currSelectItem = 0;
        }
        else if (nChar == KEY_ENTER) {					// 按下ENTER鍵選取...
            if (currSelectItem == 0) {
                GotoGameState(GAME_STATE_RUN);			// 開始遊戲
            }
            else if ( currSelectItem == 1 ) {
                displayState = 1;						// 顯示遊戲說明的state
                noteDisplayState = 0;
            }
            else if (currSelectItem == 2) {
                displayState = 2;						// 角色選擇的state
            }
            else if (currSelectItem == 3) {
                // 統計的state
            }
            else if (currSelectItem == 4) {
                displayState = 4;	// 關於的state
            }
        }
    }
    else if (displayState == 1 ) {							// [遊戲說明]
        if (nChar == KEY_ENTER) displayState = 0;			//  ->返回主選單
        else if (nChar == KEY_LEFT || nChar == KEY_RIGHT) { // [遊戲說明] 左右翻頁遊戲說明
            if		(nChar == KEY_LEFT)   noteDisplayState--;
            else if (nChar == KEY_RIGHT) noteDisplayState++;

            if (noteDisplayState < 0) noteDisplayState = int(note.size()) - 1;
            else if (noteDisplayState >  int(note.size()) - 1) noteDisplayState = 0;
        }
    }
    else if (displayState == 2) { // [角色選擇]
        if (nChar == KEY_ENTER) displayState = 0;	// ->返回主選單
        else if (nChar == KEY_LEFT || nChar == KEY_RIGHT) {
            if		(nChar == KEY_LEFT)   PublicData::me.AddSelectedChar(-1);
            else if (nChar == KEY_RIGHT)  PublicData::me.AddSelectedChar(1);
        }
    }
    else if (displayState == 3 && nChar == KEY_ENTER) { // [統計]
    }
    else if (displayState == 4 && nChar == KEY_ENTER) { // [關於]
        if (nChar == KEY_ENTER) displayState = 0;	// ->返回主選單
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

    if (displayState == 0) {	// 顯示主選單
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

        if (1) {					// 顯示最高分(bitmap)
            int tempScore = 9487;
            highScoreBorder.SetTopLeft(HIGHSCORE_POS_X, HIGHSCORE_POS_Y);
            highScoreBorder.ShowBitmap();

            for (int i = 0; i < 5; i++) {					// 顯示分數數字bmp
                numBmpSmall[tempScore % 10].SetTopLeft( HIGHSCORE_POS_X - 10 * i + 130, HIGHSCORE_POS_Y + 5);
                numBmpSmall[tempScore % 10].ShowBitmap();
                tempScore /= 10;
            }
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
        const int BACK_BTN_POS = NOTE_TEXT_Y + noteBorder.Height() + 0;
        menuBorder_ckecked.SetTopLeft((SIZE_X - menuBorder.Width()) / 2, BACK_BTN_POS);
        menuBorder_ckecked.ShowBitmap();
        menuBorder.SetTopLeft((SIZE_X - menuBorder.Width()) / 2, BACK_BTN_POS);
        menuBorder.ShowBitmap();
        menuText[5]->SetTopLeft((SIZE_X - menuText[5]->Width()) / 2,  7 + BACK_BTN_POS);
        menuText[5]->ShowBitmap();
    }
    else if (displayState == 2) {      // 顯示 選擇角色 頁面
        characterBorder.SetTopLeft((SIZE_X - characterBorder.Width()) / 2, CHARACTER_POS_Y);
        characterBorder.ShowBitmap();
        characterArrow.SetTopLeft((SIZE_X - characterArrow.Width()) / 2, CHARACTER_POS_Y + characterBorder.Height() / 2);
        characterArrow.ShowBitmap();
        PublicData::me.SetState(1);
        PublicData::me.OnShow();
    }
    else if (displayState == 3) {}
    else if (displayState == 4) {      // 顯示關於頁面
        // 關於框
        aboutBorder.SetTopLeft((SIZE_X - aboutBorder.Width()) / 2, NOTE_TEXT_Y);
        aboutBorder.ShowBitmap();
        // 關於文字
        about.SetTopLeft((SIZE_X - aboutBorder.Width()) / 2, NOTE_TEXT_Y  + 11);
        about.ShowBitmap();
        // 顯示偽返回按鈕
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

    if (1) {		// 顯示數字及字體
        for (int i = 0; i < 10; i++) {
            numBmpSmall[i].SetTopLeft(10 + i * 10, 300);
            numBmpSmall[i].ShowBitmap();
            numBmp[i].SetTopLeft(10 + i * 20, 320);
            numBmp[i].ShowBitmap();
        }
    }
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
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
    ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
    border.LoadBitmap("Bitmaps/gameover/gameover_border.bmp", RGB(0, 255, 0));
    char str[80];
    ShowInitProgress(80);

    for (int i = 0; i < 10; i++) {		// 載入數字圖
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

void CGameStateOver::OnShow() {		// GAMEOVER 畫面顯示
    border.SetTopLeft(x, y);
    border.ShowBitmap();
    //
    int tempScore = score, tempLevel = level, tempAccuracy = int (accuracy * 100.0);
    int dotPos = 0;
    int scorePosX = 230, levelPosX = 130, accPosX = 155, // 定義各數字位置偏移量
        scorePosY =  45, levelPosY =  90, accPosY = 108,
        barPosX = 120, barPosY = 122;

    for (int i = 0; i < 5; i++) {					// 顯示分數數字bmp
        numBmp[tempScore % 10].SetTopLeft(x + scorePosX - 20 * i, y + scorePosY);
        numBmp[tempScore % 10].ShowBitmap();
        tempScore /= 10;
    }

    for (int i = 0; i < 2; i++) {					// 顯示關卡數字bmp
        numBmpSmall[tempLevel % 10].SetTopLeft(x + levelPosX - 10 * i, y + levelPosY);
        numBmpSmall[tempLevel % 10].ShowBitmap();
        tempLevel /= 10;
    }

    if (accuracy != 100) {								//若正確率非100%
        for (int i = 0, dotPos = 0; i < 4; i++) {		// 顯示正確率bmp
            numBmpSmall[tempAccuracy % 10].SetTopLeft(x + accPosX - 10 * i - dotPos, y + accPosY);
            numBmpSmall[tempAccuracy % 10].ShowBitmap();
            tempAccuracy /= 10;

            if (i == 1) {								// 顯示小數點
                dotPos = 5;
                numBmpSmall[11].SetTopLeft(x + accPosX - 10 * i - dotPos, y + accPosY);
                numBmpSmall[11].ShowBitmap();
            }

            tempLevel /= 10;
        }

        numBmpSmall[10].SetTopLeft(x + accPosX + 14, y + accPosY);
        numBmpSmall[10].ShowBitmap();					// 顯示百分比符號
    }
    else {												// 針對100%顯示
        numBmpSmall[1].SetTopLeft(x + accPosX - 35, y + accPosY);
        numBmpSmall[1].ShowBitmap();
        numBmpSmall[0].SetTopLeft(x + accPosX - 35 + 10, y + accPosY);
        numBmpSmall[0].ShowBitmap();
        numBmpSmall[0].SetTopLeft(x + accPosX - 35 + 10 * 2, y + accPosY);
        numBmpSmall[0].ShowBitmap();
        numBmpSmall[10].SetTopLeft(x + accPosX - 35 + 10 * 4, y + accPosY);
        numBmpSmall[10].ShowBitmap();					// 顯示百分比符號
    }

    for (int i = 0; i < 100; i++) {						// 顯示百分比的進度條
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
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////
CGameStateRun::CGameStateRun(CGame* g)
    : CGameState(g), LEVEL(30) {
    srand((unsigned)time(NULL));	// 亂數種子
    callEnemyCounter = maxCallEnemyCounter = 30;	// maxCallEnemyCounter 決定怪物生成速度
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
    CAudio::Instance()->Play(AUDIO_ROCK, true);			// 撥放 MIDI
    score.SetInteger(0);								// 設定SCORE為0
    score.SetTopLeft(SCORE_X, SCORE_Y);
    currEnemyNum = currBossANum = currBossBNum = 0;		// 初始化各關敵人已召喚數量
    lock = false;										// 取消鎖定
    targetEnemy = NULL;
    currLevel = 0;										// 初始化關卡
    enemyQueue.clear();									// 清空EQ
    lives = 3;
    totalKeyDownCount = totalCorrectKeyCount = 0;
    accuracy = 0;
    emp.SetEQ(&enemyQueue, &score, &lock, &targetEnemy);
    emp.SetEmpTimes(3);
    PublicData::me.SetState(0);
    totalEnemyNum = 0;
    levelChangeFlag = 0;
    levelChangeDelay = -1;
    levelChangeDelayMax = int( 3.5 * 30 );						// 設定關卡間delay 3秒

    //
    if (1) {	//【DEBUG區】 將第0關設定生成200只怪物，且召喚delay為0秒
        levelEnemyNum[0] = 200;
        levelChangeDelayMax = 0;
        callEnemyCounter = maxCallEnemyCounter = callBossACounter = maxCallBossACounter = callBossBCounter = maxCallBossBCounter = 0;
    }
}
void CGameStateRun::OnInit() {								// 遊戲的初值及圖形設定
    srand((unsigned)time(NULL));
    ShowInitProgress(33);	// 接個前一個狀態的進度，此處進度視為33%
    //
    // 繼續載入其他資料
    //
    score.LoadBitmap();
    map.LoadBitmap();
    emp.LoadBitmap();
    levelAni.LoadBitmap();
    CAudio::Instance()->Load(AUDIO_ROCK, "sounds\\The_Coming_Storm.mp3");	// 載入編號3的聲音The_Coming_Storm.mp3
    CAudio::Instance()->Load(AUDIO_SHOT, "sounds\\shot.mp3");
    ShowInitProgress(40);

    //
    // 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
    //
    for (int i = 0; i < 26; i++) {
        char str[50];
        sprintf(str, "Bitmaps/char4/%c.bmp", i + 97);
        letter.push_back(new CMovingBitmap);
        letter.back()->LoadBitmap(str, RGB(255, 255, 255));
    }

    //

    for (int i = 0; i < 22; i++) {	// 載入動畫
        char str[50];
        sprintf(str, "Bitmaps/target/target_s%d.bmp", i + 1);
        target.AddBitmap(str, RGB(0, 255, 0));
    }

    target.SetDelayCount(2);
    ShowInitProgress(50);
}
void CGameStateRun::OnMove() {						// 移動遊戲元素
    //
    // 如果希望修改cursor的樣式，則將下面程式的commment取消即可
    //
    //SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
    //
    ////////
    if (levelChangeDelay >= 0)	levelChangeDelay--;

    accuracy = (totalKeyDownCount != 0) ? \
               100 * double(totalCorrectKeyCount) / double(totalKeyDownCount) : \
               100;     // 計算正確率
    PublicData::score = score.GetInteger();
    PublicData::level = currLevel;
    PublicData::accuracy = accuracy;
    callEnemyCounter--;	//每隻怪物 生成間隔 之 counter
    callBossACounter--;
    callBossBCounter--;

    //==小怪==================================
    if (callEnemyCounter < 0 && currEnemyNum < levelEnemyNum[currLevel]) {	// counter 數到0後就開始召喚新怪
        callEnemyCounter = maxCallEnemyCounter;				// 把counter 調回max繼續數
        int randX = (rand() % (SIZE_X - 100)) ;				// SIZE_X - 100 為了不讓怪物的單字超出螢幕太多
        enemyQueue.push_back(new CEnemy(randX, 0, 3, true, &dictionary, 2, 7, &enemyQueue, &bombList, PublicData::me.GetX1(), PublicData::me.GetY1(), &letter) );
        enemyQueue.back()->LoadBitmap();
        currEnemyNum++;
        totalEnemyNum++;
    }

    //==BossA==================================
    if (callBossACounter < 0 && currBossANum < levelBossANum[currLevel]) {	// counter 數到0後就開始召喚新怪
        callBossACounter = maxCallBossACounter;				// 把counter 調回max繼續數
        int randX = (rand() % (SIZE_X - 150));
        enemyQueue.push_back(new CBossA(randX, 0, 5, true, &dictionary, 7, 20, &enemyQueue, &bombList, &letter));
        enemyQueue.back()->LoadBitmap();
        currBossANum++;
        totalEnemyNum++;
    }

    //==BossB==================================
    if (callBossBCounter < 0 && currBossBNum < levelBossBNum[currLevel]) {	// counter 數到0後就開始召喚新怪
        callBossBCounter = maxCallBossBCounter;				// 把counter 調回max繼續數
        int randX = (rand() % (SIZE_X - 150));
        enemyQueue.push_back(new CBossB(randX, 0, 5, true, &dictionary, 7, 20, &enemyQueue, &bombList, &letter));
        enemyQueue.back()->LoadBitmap();
        currBossBNum++;
        totalEnemyNum++;
    }

    //===判斷Me是否碰到Enemy===

    for (CEnemy* eq : enemyQueue) {
        if (eq->IsAlive() && eq->HitMe(&PublicData::me)) {
            lives--;
            (lives <= 0) ? GotoGameState(GAME_STATE_OVER) : 0;
        }

        // 當enemy飛出畫面外時kill()

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
        //若Enemy IsAlive=0, 則從vector中移除
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
        bl->OnMove();	// 移動bullet

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
        // 換 關卡
        if (!levelChangeFlag) {								// 先播放切換關卡動畫 並等待delay算完
            levelAni.Play(currLevel, score.GetInteger());
            levelChangeFlag = true;
            levelChangeDelay = levelChangeDelayMax;
        }

        if (levelChangeDelay < 0 && levelChangeFlag) {		// 當delay算完後 再實際切換關卡
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
    const char KEY_LEFT = 0x25;	// keyboard左箭頭
    const char KEY_UP = 0x26;	// keyboard上箭頭
    const char KEY_RIGHT = 0x27; // keyboard右箭頭
    const char KEY_DOWN = 0x28; // keyboard下箭頭

    if (key != nChar) {
        key = nChar;

        for (int unsigned i = 0; i < enemyQueue.size(); i++) {			// 跑目前關卡怪物的數量
            if (enemyQueue[i]->IsAlive()) {								// 回傳當前怪物是否存在
                if (!lock) {										// 尚未鎖定了
                    if (nChar + 32 == enemyQueue[i]->GetFirstWord()) {	// 若等於第一個字母:鎖住 and 目前字元位置+1
                        totalCorrectKeyCount++;							// 正確按鍵數+1
                        map.PlayFlash();
                        CAudio::Instance()->Play(AUDIO_SHOT, false);			// 撥放 射擊音效

                        if (enemyQueue[i]->GetVocabLeng() == 1) {
                            targetEnemy = enemyQueue[i];					// targetEnemy為指標->正在攻擊的敵人
                            bulletList.push_back(new CBullet(targetEnemy->GetX() + 10, targetEnemy->GetY() + 10));	// 射子彈
                            targetEnemy->kill();									// 成功殺害怪物
                            score.Add(targetEnemy->GetVocabLeng());				// 分數+= 怪物長度
                            targetEnemy = NULL;
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
                        map.PlayFlash();
                        CAudio::Instance()->Play(AUDIO_SHOT, false);	// 撥放 射擊音效

                        if (targetEnemy->GetCurrWordLeng() == targetEnemy->GetVocabLeng()) {	 // 若當前長度 等於 字母的長度
                            targetEnemy->kill();									// 成功殺害怪物
                            score.Add(targetEnemy->GetVocabLeng());					// 分數+= 怪物長度
                            targetEnemy = NULL;
                            lock = false;
                        }

                        break;
                    }
                }
            }
        }

        if (nChar >= 65 && nChar <= 90) totalKeyDownCount++;			// 總按鍵數++

        if (nChar == 13)
            emp.CallEmp();
    }
}
void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
    const char KEY_LEFT = 0x25; // keyboard左箭頭
    const char KEY_UP = 0x26; // keyboard上箭頭
    const char KEY_RIGHT = 0x27; // keyboard右箭頭
    const char KEY_DOWN = 0x28; // keyboard下箭頭
    key = NULL;

    // 數字鍵作為debug按鈕
    if (nChar == '1')showDebug = showDebug ? false : true;  // 按1 開關debug

    if (nChar == '2' && enemyQueue.size() > 0) {  // 按2 清除EQ最後一隻敵人
        enemyQueue.back()->kill();
        lock = 0;
    }

    if (nChar == '3' && enemyQueue.size() > 0) {  // 按3 清除EQ中所有敵人
        for (CEnemy* ce : enemyQueue) ce->kill();

        lock = 0;
    }

    if (nChar == '4')GotoGameState(GAME_STATE_INIT);	// 按4 GOTO 起始畫面

    if (nChar == '5')GotoGameState(GAME_STATE_OVER);    // 按5 GOTO 遊戲結束畫面
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
    map.OnShow();						// 貼上背景網子
    score.ShowBitmap();					// 貼上分數
    emp.OnShow();						// 貼上EMP（必殺技）
    PublicData::me.OnShow();			// 貼上主角
    levelAni.OnShow();					// 貼上關卡切換動畫

    for (CBomb* cb : bombList)	cb->OnShow();

    for (CEnemy* eq : enemyQueue)	eq->OnShow();

    for (CBullet* bl : bulletList)	bl->OnShow();

    if (lock && targetEnemy != NULL) {
        targetEnemy->OnShow();	// 加上這一行 讓被鎖定的怪物再次show, 以防被其他怪物蓋住

        if (targetEnemy->GetBossType() == "enemy")
            target.SetTopLeft(targetEnemy->GetX() - 2, targetEnemy->GetY() - 2);
        else
            target.SetTopLeft(targetEnemy->GetX() + 8, targetEnemy->GetY() + 8);

        target.OnShow();
    }

    if (showDebug) {		// 顯示debug資訊
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
            for (unsigned int i = 0; i < enemyQueue.size(); i++) {	// 顯示場上怪物之 單字,curr/length
                char temp[50];
                sprintf(temp, "%s %d/%d (x:%d,y:%d)", enemyQueue[i]->GetVocab().c_str(), enemyQueue[i]->GetCurrWordLeng(), enemyQueue[i]->GetVocabLeng(), enemyQueue[i]->GetX(), enemyQueue[i]->GetY());
                pDC->SetTextColor(RGB(180 + i, 180 + i, 180 + i));
                pDC->TextOut(20, i * 14 + 60, temp);
            }
        }

        ////
        pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
        CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
    }
}
}
