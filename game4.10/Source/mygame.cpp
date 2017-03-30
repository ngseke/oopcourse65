/*
3/24
已知BUG :		[解決了]1. 當有連續的單字(bee), 打到第二個e就會直接消失. 可能是keydown太靈敏.
				[解決了]2. 比如場上兩隻單字頂真(appl"e" "e"gg), 在打完apple最後的"e"時, 他會直接接到egg開頭的"e"
				[解決了]3. 為了讓單字不和場上重複, 會有已經new完怪物後, 但卻需要重新讓emeny SetVocab,
	    		   這造成了eneny在LoadBitmap, 對話框長度是停留在原先的length, 而非新SetVocab
			       後的那個單字長度. 但也不能重新LoadBitmap, 會出錯, 暫時讓對話框長度都一樣.
				4. 怪物越多的時候會開始當掉

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
    : CGameState(g) {
}

void CGameStateInit::OnInit() {
    ShowInitProgress(0);	// 一開始的loading進度為0%
    //
    // 開始載入資料
    //
    //logo.LoadBitmap(IDB_BACKGROUND);
    typing_logo.LoadBitmap("Bitmaps/start_logo.bmp", RGB(0, 255, 0));
    text1.LoadBitmap("Bitmaps/text1_start.bmp", RGB(0, 255, 0));
    // note1.LoadBitmap("Bitmaps/note1_start.bmp", RGB(0, 255, 0));
    char* note1_filename[6] = { "Bitmaps/note1_start_1.bmp",		//開頭說明圖片d
                                "Bitmaps/note1_start_2.bmp",
                                "Bitmaps/note1_start_3.bmp",
                                "Bitmaps/note1_start_4.bmp",
                                "Bitmaps/note1_start_5.bmp",
                                "Bitmaps/note1_start_6.bmp",
                              };

    for (int i = 0; i < 6; i++)		// 載入動畫(由4張圖形構成)
        note1.AddBitmap(note1_filename[i], RGB(0, 255, 0));

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
    note1.OnMove();
}
void CGameStateInit::OnShow() {
    //
    // 貼上logo
    //
    //logo.SetTopLeft((SIZE_X - logo.Width()) / 2, SIZE_Y / 8);
    //logo.ShowBitmap();
    typing_logo.SetTopLeft((SIZE_X - typing_logo.Width()) / 2, SIZE_Y / 5);
    typing_logo.ShowBitmap();
    text1.SetTopLeft((SIZE_X - text1.Width()) / 2, SIZE_Y / 5 + typing_logo.Height() + 180);
    text1.ShowBitmap();
    note1.SetTopLeft((SIZE_X - text1.Width()) / 8, SIZE_Y / 5 + typing_logo.Height());
    note1.OnShow();
    //
    // Demo螢幕字型的使用，不過開發時請盡量避免直接使用字型，改用CMovingBitmap比較好
    //
    CDC* pDC = CDDraw::GetBackCDC();			// 取得 Back Plain 的 CDC
    CFont f, *fp;
    f.CreatePointFont(160, "Consolas");	// 產生 font f; 160表示16 point的字
    fp = pDC->SelectObject(&f);					// 選用 font f
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetBkMode(TRANSPARENT);
    pDC->SetTextColor(RGB(41, 171, 226));
    //pDC->TextOut(180, 350, "Huang Xingqiao / Yu kaici");  //test text
    //if (ENABLE_GAME_PAUSE)
    //    pDC->TextOut(5, 425, "Press Ctrl-Q to pause the Game.");
    //pDC->TextOut(5, 455, "Press Alt-F4 or ESC to Quit.");
    ////
    pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
    CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
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
    f.CreatePointFont(160, "Times New Roman");	// 產生 font f; 160表示16 point的字
    fp = pDC->SelectObject(&f);					// 選用 font f
    pDC->SetBkColor(RGB(0, 0, 0));
    pDC->SetTextColor(RGB(255, 255, 0));
    char str[80];								// Demo 數字對字串的轉換
    sprintf(str, "你 死 了 ! (%d)", counter / 30);
    pDC->TextOut(240, 210, str);
    //	顯示分數 (not done)
    // char scoreChr[80];
    // sprintf(str, "SCORE: %d", score);
    pDC->TextOut(240, 250, str);
    //
    pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
    CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
}

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
/////////////////////////////////////////////////////////////////////////////

CGameStateRun::CGameStateRun(CGame* g)
    : CGameState(g), NUMBALLS(28), LEVEL(10) {
    srand((unsigned)time(NULL));	//亂數種子
    picX = picY = 0;
    callEnemyCounter = maxCallEnemyCounter = 20;	// maxCallEnemyCounter 決定怪物生成速度
    currEnemyNum = 0;
    lock = 0;
    currLevel = 0;
}

CGameStateRun::~CGameStateRun() {
    //delete[] ball;
    ////////
    //delete[] &enemy1;
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
    /* 老師的示範球
    for (int i = 0; i < NUMBALLS; i++) {				// 設定球的起始座標
        int x_pos = i % BALL_PER_ROW;
        int y_pos = i / BALL_PER_ROW;
        ball[i].SetXY(x_pos * BALL_GAP + BALL_XY_OFFSET, y_pos * BALL_GAP + BALL_XY_OFFSET);
        ball[i].SetDelay(x_pos);
        ball[i].SetIsAlive(true);
    }
    */
    eraser.Initialize();
    //background.SetTopLeft(BACKGROUND_X, 0);				// 設定背景的起始座標
    help.SetTopLeft(0, SIZE_Y - help.Height());			// 設定說明圖的起始座標
    //hits_left.SetInteger(HITS_LEFT);					// 指定剩下的撞擊數
    //hits_left.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);		// 指定剩下撞擊數的座標
    //CAudio::Instance()->Play(AUDIO_LAKE, true);			// 撥放 WAVE
    //CAudio::Instance()->Play(AUDIO_DING, false);		// 撥放 WAVE
    //CAudio::Instance()->Play(AUDIO_NTUT, true);			// 撥放 MIDI

    /////// SET Eneny's 初始值

    for (int i = 0; i < levelEnemyNum[currLevel]; i++) {
        //enemy1[i]->SetXY(0, 0);		//useless
        //enemy1[i]->SetDelay(10); //useless
        //enemy1[i]->SetIsAlive(false);
    }

    score.SetInteger(0);			//設定SCORE為0;
    score.SetTopLeft(HITS_LEFT_X, HITS_LEFT_Y);
}

void CGameStateRun::OnMove() {						// 移動遊戲元素
    //
    // 如果希望修改cursor的樣式，則將下面程式的commment取消即可
    //
    //SetCursor(AfxGetApp()->LoadCursor(IDC_GAMECURSOR));
    //
    // 移動背景圖的座標
    //
    /*
    if (background.Top() > SIZE_Y)  // 當資訊工程系掉落到底部後 回到上面
    	background.SetTopLeft(60, -background.Height());

    background.SetTopLeft(background.Left(), background.Top() + 1);
    */
    //
    // 移動球
    //

    /*
    for (int i = 0; i < NUMBALLS; i++)
        ball[i].OnMove();
    */
    ////////////
    if (picX <= SIZE_Y) {
        picX += 5;
        picY += 5;
    }
    else {
        picX = picY = 0;
    }

    ////////
    callEnemyCounter--;	//每隻怪物 生成間隔 之 counter (50~0)

    if (callEnemyCounter < 0 && currEnemyNum < levelEnemyNum[currLevel]) {	// counter 數到0後就開始召喚新怪
        callEnemyCounter = maxCallEnemyCounter;									// 把counter 調回max繼續數
        int randX = (rand() % (SIZE_X - 100)) ;					// SIZE_X - 100 為了不讓怪物的單字超出螢幕太多
        ////
        //省喬改用把怪物放入vector的作法
        enemy1.push_back(new CEnemy(randX, 0, 5, 0));
        enemy1.back()->LoadBitmap();
        ////
        //enemy1[currEnemyNum]->SetXY(randX, 0);
        //enemy1[currEnemyNum]->SetDelay(5);							// 從0數到這個數字才會動
        ////

        while (1) {								// 此迴圈 檢查新召喚的怪物 是否跟場上現有的第一個字撞
            bool firstWordBounceFlag = 0;

            for (int i = enemy1.size() - 1; i >= 0 ; i--) {
                if (enemy1.back()->GetFirstWord() == enemy1[i]->GetFirstWord() && enemy1[i]->IsAlive())
                    firstWordBounceFlag = 1;
            }

            if (firstWordBounceFlag) enemy1.back()->SetVocab();
            else {
                break;
            }
        }

        enemy1.back()->LoadTextbox();	// 確定單字是是什麼後 才讀取textbox的bitmap
        enemy1.back()->SetIsAlive(true);
        currEnemyNum++;
    }

    for (unsigned int i = 0; i < enemy1.size(); i++)
        if (enemy1[i]->IsAlive())	enemy1[i]->OnMove();

    for (unsigned int i = 0; i < bulletList.size(); i++)
        bulletList[i]->OnMove();	// 移動bullet

    for (unsigned int i = 0; i < enemy1.size(); i++) {
        //若Enemy IsAlive=0, 則從vector中移除 但寫法不確定
        vector<CEnemy*>::iterator iterEnemy1 = enemy1.begin();

        if (!enemy1[i]->IsAlive()) {
            enemy1.erase(iterEnemy1 + i);
            break;
        }
    }

    for (int i = bulletList.size() - 1; i >= 0; i--) {
        //若bullet IsAlive=0, 則從vector中移除
        if (!bulletList[i]->IsAlive())	bulletList.erase(bulletList.begin());       //(不確定是不是這麼寫)
    }

    /////////////
    //
    // 移動擦子
    //
    //eraser.OnMove();
    //
    // 判斷擦子是否碰到球
    //
    /*
    for (int i = 0; i < NUMBALLS; i++)
        if (ball[i].IsAlive() && ball[i].HitEraser(&eraser)) {
            ball[i].SetIsAlive(false);
            CAudio::Instance()->Play(AUDIO_DING);
            hits_left.Add(-1);

            //
            // 若剩餘碰撞次數為0，則跳到Game Over狀態
            //
            if (hits_left.GetInteger() <= 0) {
                CAudio::Instance()->Stop(AUDIO_LAKE);	// 停止 WAVE
                CAudio::Instance()->Stop(AUDIO_NTUT);	// 停止 MIDI
                GotoGameState(GAME_STATE_OVER);
            }
        }
    */
    //
    // 移動彈跳的球
    //
    bball.OnMove();
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
    help.LoadBitmap(IDB_HELP, RGB(255, 255, 255));				// 載入說明的圖形
    corner.LoadBitmap(IDB_CORNER);							// 載入角落圖形
    //corner.ShowBitmap(background);							// 將corner貼到background
    bball.LoadBitmap();										// 載入圖形
    //hits_left.LoadBitmap();
    score.LoadBitmap();
    CAudio::Instance()->Load(AUDIO_DING, "sounds\\ding.wav");	// 載入編號0的聲音ding.wav
    CAudio::Instance()->Load(AUDIO_LAKE, "sounds\\lake.mp3");	// 載入編號1的聲音lake.mp3
    CAudio::Instance()->Load(AUDIO_NTUT, "sounds\\ntut.mid");	// 載入編號2的聲音ntut.mid
    //
    // 此OnInit動作會接到CGameStaterOver::OnInit()，所以進度還沒到100%
    //
}
void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
    const char KEY_LEFT = 0x25; // keyboard左箭頭
    const char KEY_UP = 0x26; // keyboard上箭頭
    const char KEY_RIGHT = 0x27; // keyboard右箭頭
    const char KEY_DOWN = 0x28; // keyboard下箭頭
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
    const char KEY_LEFT = 0x25; // keyboard左箭頭
    const char KEY_UP = 0x26; // keyboard上箭頭
    const char KEY_RIGHT = 0x27; // keyboard右箭頭
    const char KEY_DOWN = 0x28; // keyboard下箭頭

    if (nChar == KEY_LEFT)
        eraser.SetMovingLeft(false);

    if (nChar == KEY_RIGHT)
        eraser.SetMovingRight(false);

    if (nChar == KEY_UP)
        eraser.SetMovingUp(false);

    if (nChar == KEY_DOWN)
        eraser.SetMovingDown(false);

    for (int unsigned i = 0; i < enemy1.size(); i++) {		// 跑目前關卡怪物的數量
        if (enemy1[i]->IsAlive()) {								// 回傳當前怪物是否存在
            if (!lock) {										// 尚未鎖定了
                if (nChar + 32 == enemy1[i]->GetFirstWord()) {	// 若等於第一個字母:鎖住 and 目前字元位置+1
                    lock = true;
                    targetEnemy = enemy1[i];					// targetEnemy為指標->正在攻擊的敵人
                    targetEnemy->AddCurrWordLeng();
                    bulletList.push_back(new CBullet(targetEnemy->GetX() + 10, targetEnemy->GetY() + 10));
                }
            }
            else {											// 若已鎖定
                if (nChar + 32 == targetEnemy->GetVocab()[targetEnemy->GetCurrWordLeng()]) { 	// 若等於當前字母
                    targetEnemy->AddCurrWordLeng();
                    bulletList.push_back(new CBullet(targetEnemy->GetX(), targetEnemy->GetY()));

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
    //eraser.SetMovingLeft(true);
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
    //  注意：Show裡面千萬不要移動任何物件的座標，移動座標的工作應由Move做才對，
    //        否則當視窗重新繪圖時(OnDraw)，物件就會移動，看起來會很怪。換個術語
    //        說，Move負責MVC中的Model，Show負責View，而View不應更動Model。
    //
    //
    //
    help.ShowBitmap();					// 貼上說明圖
    //hits_left.ShowBitmap();

    /////////

    for (unsigned int i = 0; i < enemy1.size(); i++)
        if (enemy1[i]->IsAlive())enemy1[i]->OnShow();

    for (unsigned int i = 0; i < bulletList.size(); i++) {
        bulletList[i]->OnShow();
    }

    score.ShowBitmap();
    /////////
    //bball.OnShow();						// 貼上彈跳的球
    //eraser.OnShow();					// 貼上擦子
    //
    //  貼上左上及右下角落的圖
    //
    corner.SetTopLeft(0, 0);
    corner.ShowBitmap();
    corner.SetTopLeft(SIZE_X - corner.Width(), SIZE_Y - corner.Height());
    corner.ShowBitmap();

    //
    //
    //////debug yong
    if (SHOW_DEBUG) {		// 要顯示DEBUG資訊的話 去h檔把常數SHOW_DEBUG設TRUE
        CDC* pDC = CDDraw::GetBackCDC();
        CFont f, *fp;
        f.CreatePointFont(120, "New Times Roman");
        fp = pDC->SelectObject(&f);
        pDC->SetBkColor(RGB(0, 0, 0));
        pDC->SetBkMode(TRANSPARENT);
        //
        char temp[30];
        sprintf(temp, "Curr Enemy Numbers: %d", enemy1.size());
        pDC->SetTextColor(RGB(200, 0, 0));
        pDC->TextOut(20, 20, temp);

        for (unsigned int i = 0; i < enemy1.size(); i++) {	// 顯示場上怪物之 單字,curr/length
            char temp[30];
            sprintf(temp, "%s %d/%d", enemy1[i]->GetVocab().c_str(), enemy1[i]->GetCurrWordLeng(), enemy1[i]->GetVocabLeng());
            pDC->SetTextColor(RGB(200 + i, 200 + i, 200 + i));
            pDC->TextOut(20, i * 14 + 40, temp);
        }

        sprintf(temp, "Bullet Numbers: %d", bulletList.size());
        pDC->SetTextColor(RGB(200, 200, 200 ));
        pDC->TextOut(250, 20, temp);
        ////
        pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
        CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
    }
}
}
