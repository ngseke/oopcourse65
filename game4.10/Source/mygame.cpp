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
int PublicData::CorrectKeyCount = 0;
int PublicData::totalKeyCount = 0;
double PublicData::accuracy = 0.0;
bool PublicData::musicOnOff = 1;
bool PublicData::newUnlock = false;
bool PublicData::debugMode = false;
CFile PublicData::file;
CMe	PublicData::me;

CGameStateInit::CGameStateInit(CGame* g)
    : CGameState(g), NOTE_TEXT_X(60), NOTE_TEXT_Y(280), MENU_Y(320),
      MENU_ITEM_NUM(5), CHARACTER_Y(320) {
}

CGameStateInit::~CGameStateInit() {
    for (CMovingBitmap* mt : menuText) delete mt;

    for (CMovingBitmap* nt : note) delete nt;
}
void CGameStateInit::OnInit() {
    ShowInitProgress(0);	// 一開始的loading進度為0%
    currSelectItem = displayState = 0;					// 初始化選單選取項目
    noteDisplayState = statsDisplayState = aboutDisplayState = 0;			// 初始化“遊戲說明”及“統計”選取頁面項目
    statsPRItemNum = 0;									// 初始化統計頁面 最高記錄的選取項目
    wrongKeyNum = 0;
    exitGameCount = 0;
    cheatCode = "";

    if (0) {// DEBUG用
        displayState = 1;
        noteDisplayState = 6;
        statsDisplayState = 0;
        PublicData::newUnlock = 1;
    }

    PublicData::me.ReadUnlockCharacter();
    PublicData::me.LoadBitmap();											// 主角
    PublicData::me.SetSelectedChar(PublicData::file.ReadSelectedCharacter());	// 讀取上次選取的角色
    PublicData::musicOnOff = PublicData::file.ReadMusicOnOff();					// 讀取上次音樂的狀態
    map.LoadBitmap();														// 背景網狀動畫
    typing_logo.LoadBitmap("Bitmaps/start_logo1.bmp", RGB(0, 255, 0));		// logo
    taipin.LoadBitmap("Bitmaps/taipin.bmp", RGB(0, 255, 0));
    text1.LoadBitmap("Bitmaps/text1_start.bmp", RGB(0, 255, 0));			// 操作方式提示bitmap
    highScoreBorder.LoadBitmap("Bitmaps/menu/highscore_border.bmp", RGB(0, 255, 0));
    new_text.LoadBitmap("Bitmaps/menu/menu_new_text.bmp", RGB(0, 255, 0));
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
    exit.LoadBitmap("Bitmaps/menu/exit.bmp", RGB(0, 255, 0));
    ShowInitProgress(15);

    for (int i = 0; i < 6; i++) {	// 說明框裡面的按鍵動畫
        char str[50];
        sprintf(str, "Bitmaps/menu/note/note1_exkey_%d.bmp", i + 1);
        noteExkey.AddBitmap(str, RGB(0, 255, 0));
    }

    noteExkey.SetDelayCount(10);

    for (int i = 0; i < 9; i++) {		// 多頁的說明文字
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
        sprintf(str, "Bitmaps/level/num_white/%d.bmp", i);
        numBmp_White[i].LoadBitmap(str, RGB(0, 255, 0));
        sprintf(str, "Bitmaps/level/num_s_white/%d.bmp", i);
        numBmpSmall_White[i].LoadBitmap(str, RGB(0, 255, 0));
    }

    numBmpSmall[10].LoadBitmap("Bitmaps/level/num_s/per.bmp", RGB(0, 255, 0));
    numBmpSmall[11].LoadBitmap("Bitmaps/level/num_s/dot.bmp", RGB(0, 255, 0));
    numBmpSmall[12].LoadBitmap("Bitmaps/level/num_s/slash.bmp", RGB(0, 255, 0));
    numBmpSmall_White[10].LoadBitmap("Bitmaps/level/num_s_white/per.bmp", RGB(0, 255, 0));
    numBmpSmall_White[11].LoadBitmap("Bitmaps/level/num_s_white/dot.bmp", RGB(0, 255, 0));
    numBmpSmall_White[12].LoadBitmap("Bitmaps/level/num_s_white/slash.bmp", RGB(0, 255, 0));
    numBmpSmall_White[13].LoadBitmap("Bitmaps/level/num_s_white/colon.bmp", RGB(0, 255, 0));
    ShowInitProgress(20);
    // 載入角色選擇 元素
    characterBorder.LoadBitmap("Bitmaps/menu/character/character_border.bmp", RGB(0, 255, 0));
    characterArrow.LoadBitmap("Bitmaps/menu/character/character_arrow.bmp", RGB(0, 255, 0));
    // 載入統計元素
    statsBorder.LoadBitmap("Bitmaps/menu/stats/stats_border.bmp", RGB(0, 255, 0));
    statsBg[0].LoadBitmap("Bitmaps/menu/stats/stats_bg1.bmp", RGB(0, 255, 0));
    statsBg[1].LoadBitmap("Bitmaps/menu/stats/stats_bg2.bmp", RGB(0, 255, 0));
    statsArrow[0].LoadBitmap("Bitmaps/menu/stats/stats_arrow.bmp", RGB(0, 255, 0));
    statsArrow[1].LoadBitmap("Bitmaps/menu/stats/stats_arrow_right.bmp", RGB(0, 255, 0));
    statsArrow[2].LoadBitmap("Bitmaps/menu/stats/stats_arrow_left.bmp", RGB(0, 255, 0));
    statsArrowV[0].LoadBitmap("Bitmaps/menu/stats/stats_arrow_v.bmp", RGB(0, 255, 0));
    statsArrowV[1].LoadBitmap("Bitmaps/menu/stats/stats_arrow_v_up.bmp", RGB(0, 255, 0));
    statsArrowV[2].LoadBitmap("Bitmaps/menu/stats/stats_arrow_v_down.bmp", RGB(0, 255, 0));
    statsArrowV[3].LoadBitmap("Bitmaps/menu/stats/stats_arrow_v_none.bmp", RGB(0, 255, 0));

    for (int i = 0; i < 4; i++)
        statsArrowV[i].SetTopLeft((SIZE_X - statsArrow[0].Width()) / 2 + 570, NOTE_TEXT_Y + 163);

    statsText[0].LoadBitmap("Bitmaps/menu/stats/stats_text_tkc.bmp", RGB(0, 255, 0));
    statsText[1].LoadBitmap("Bitmaps/menu/stats/stats_text_hl.bmp", RGB(0, 255, 0));
    statsText[2].LoadBitmap("Bitmaps/menu/stats/stats_text_ckc.bmp", RGB(0, 255, 0));
    statsText[3].LoadBitmap("Bitmaps/menu/stats/stats_text_acc.bmp", RGB(0, 255, 0));
    statsNoRecord.LoadBitmap("Bitmaps/menu/stats/stats_no_record.bmp", RGB(0, 255, 0));
    // 載入關於元素
    aboutBorder.LoadBitmap("Bitmaps/menu/about/about_border.bmp", RGB(0, 255, 0)); // 介紹框線
    about.LoadBitmap("Bitmaps/menu/about/about_text_p2.bmp", RGB(0, 255, 0)); // 介紹文字
    delText.LoadBitmap("Bitmaps/menu/about/about_del_text.bmp", RGB(0, 255, 0));	// 確認刪除視窗
    musicOnOff[0].LoadBitmap("Bitmaps/menu/about/about_music_on.bmp", RGB(0, 255, 0));
    musicOnOff[1].LoadBitmap("Bitmaps/menu/about/about_music_off.bmp", RGB(0, 255, 0));
}

void CGameStateInit::OnBeginState() {
    text1_y = 550;
    text1_count = 0;
    PublicData::file.ReadHighScoreFile();
    PublicData::file.ReadRecordFile();
    statsPRItemNum = 0;		// 遊玩記錄的項目數字歸零（回到第一筆資料）
    PublicData::totalKeyCount = PublicData::file.ReadTotalKeyCount();
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
    PublicData::me.ReadUnlockCharacter();
    const char KEY_ESC = 27;
    const char KEY_SPACE = ' ';
    const char KEY_ENTER = 0xD;
    const char KEY_LEFT	 = 0x25;	// keyboard左箭頭
    const char KEY_UP	 = 0x26;	// keyboard上箭頭
    const char KEY_RIGHT = 0x27;	// keyboard右箭頭
    const char KEY_DOWN  = 0x28;	// keyboard下箭頭

    if (!(nChar == KEY_ESC || nChar == KEY_LEFT || nChar == KEY_UP || nChar == KEY_RIGHT || nChar == KEY_DOWN || nChar == KEY_ENTER || nChar == 'D' || nChar == 'Y' || nChar == 'N' || (nChar <= '5' && nChar >= '1'))) {
        //wrongKeyNum++;
    }

    if ((nChar <= '5' && nChar >= '1') && displayState == 0) {		// 提供以數字鍵1～5來操縱選單
        if (nChar == '1')  GotoGameState(GAME_STATE_RUN);

        currSelectItem = displayState = nChar - '1';
        noteDisplayState = statsDisplayState = aboutDisplayState = 0;
        cheatCode = "";
    }

    if (nChar == KEY_ESC) {		// ESC鍵...
        if (!(displayState == 0) && !(displayState == 2 && !PublicData::me.GetSelectedCharIsUnlock()))
            displayState = 0;	// 返回主選單
        else if (!(displayState == 2) && !exitState) {
            exitState = true;
            return;
        }
    }

    if (exitState) {
        if (nChar == 'N' || nChar == KEY_ESC)exitState = false;
        else if (nChar == 'Y')	PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);
    }

    if (displayState == 0 ) {					// 在主選單...
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
            else {
                displayState = currSelectItem;				// 前往所選取的state
                noteDisplayState = statsDisplayState = aboutDisplayState = 0;	// 初始化說明文字的選取項目 及 遊玩記錄的項目數字
                cheatCode = "";

                if (currSelectItem == 2) PublicData::newUnlock = false;
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
        if (nChar == KEY_ENTER) {
            if (PublicData::me.GetSelectedCharIsUnlock()) {
                PublicData::file.WriteSelectedCharacter(PublicData::me.GetMeName());
                displayState = 0;// ->返回主選單
            }
        }
        else if (nChar == KEY_LEFT || nChar == KEY_RIGHT) {
            if		(nChar == KEY_LEFT)   PublicData::me.AddSelectedChar(-1);
            else if (nChar == KEY_RIGHT)  PublicData::me.AddSelectedChar(1);
        }
    }
    else if (displayState == 3) { // [統計]
        if (nChar == KEY_ENTER) displayState = 0;	// ->返回主選單
        else if (nChar == KEY_LEFT || nChar == KEY_RIGHT) { // [統計] 左頁:最高記錄; 右頁:遊玩記錄
            if		(nChar == KEY_LEFT)	 statsDisplayState = 0;
            else if (nChar == KEY_RIGHT) statsDisplayState = 1;
        }

        if (statsDisplayState == 1) {				// 若為 遊玩記錄狀態
            if		(nChar == KEY_UP) {   // 向上查找記錄
                if (statsPRItemNum > 0)statsPRItemNum--;
            }
            else if (nChar == KEY_DOWN) { // 向下查找記錄
                if (statsPRItemNum < PublicData::file.GetRecordNum() - 3)statsPRItemNum++;
            }
        }
    }
    else if (displayState == 4 ) { // [關於]
        if (aboutDisplayState == 0 && nChar == KEY_ENTER) displayState = 0;	// ->返回主選單

        if (aboutDisplayState == 0) {
            cheatCode = cheatCode + char(nChar);

            if (cheatCode == "104590029") {						// 作弊碼:解鎖所有角色
                PublicData::me.JudgeUnlock(99999, 99999, 100, 30);
                displayState = 0;
                cheatCode = "";
            }
            else if (cheatCode == "104590025") {				// 作弊碼:允許debug模式
                PublicData::debugMode = true;
                displayState = 0;
                cheatCode = "";
            }
        }

        if (aboutDisplayState == 0 && nChar == 'M') {						// 開關音樂
            PublicData::musicOnOff = PublicData::musicOnOff ? false : true;
            PublicData::file.WriteMusicOnOff(PublicData::musicOnOff);
        }

        if (aboutDisplayState == 0 && nChar == 'D' ) {
            aboutDisplayState = 1;    // 顯示清除遊玩紀錄視窗
        }

        if (aboutDisplayState == 1) {
            if (nChar == 'Y') {	// 選YES確認刪除遊戲紀錄
                PublicData::file.DeleteAllData();			// 清空txt檔
                PublicData::file.ReadHighScoreFile();		// 重新載入遊戲紀錄
                PublicData::file.ReadRecordFile();
                PublicData::me.SetSelectedChar("Iron Man");	// 重置選取的角色為IronMan
                displayState = 0;							// 返回主選單
            }
            else if (nChar == 'N') {	// 選NO取消
                aboutDisplayState = 0;
            }
        }
    }
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point) {
    wrongKeyNum++;
}
void CGameStateInit::OnMouseMove(UINT nFlags, CPoint point) {}
void CGameStateInit::OnMove() {
    map.OnMove();
    noteExkey.OnMove();
    PublicData::me.OnMove();

    if (exitGameCount != 0 && exitGameCount < 15)
        exitGameCount++;
    else
        exitGameCount = 0;

    if (text1_count < 400) text1_count++;

    if (text1_count > 5 * 30)  text1_y += int((text1_count - 5 * 30) * 1.1);

    if (wrongKeyNum > 3) {	// 若多次按下不相關按鈕，則友善提醒重新播放說明文字
        text1_y = 550;
        text1_count = wrongKeyNum = 0;
    }

    if (0) GotoGameState(GAME_STATE_OVER);
}
void CGameStateInit::OnShow() {
    map.OnShow();
    typing_logo.SetTopLeft((SIZE_X - typing_logo.Width()) / 2, 100);
    typing_logo.ShowBitmap();
    taipin.SetTopLeft((SIZE_X + typing_logo.Width()) / 2 - 60, 100 + 31);
    taipin.ShowBitmap();

    //
    if (displayState == 0) {	// 顯示主選單
        if (!exitState) {
            const int MENU_MARGIN_BTM = 40;
            menuBorder_ckecked.SetTopLeft((SIZE_X - menuBorder.Width()) / 2, MENU_Y + MENU_MARGIN_BTM * currSelectItem);
            menuBorder_ckecked.ShowBitmap();

            for (int i = 0; i < MENU_ITEM_NUM; i++) {
                menuBorder.SetTopLeft((SIZE_X - menuBorder.Width()) / 2, MENU_Y + MENU_MARGIN_BTM * i);
                menuBorder.ShowBitmap();
                menuText[i]->SetTopLeft((SIZE_X - menuText[i]->Width()) / 2, MENU_Y + 7 + MENU_MARGIN_BTM * i);
                menuText[i]->ShowBitmap();
            }

            int HIGHSCORE_POS_X = (SIZE_X + menuBorder.Width()) / 2 + 8;
            int HIGHSCORE_POS_Y = MENU_Y + 10;

            if (PublicData::file.isHighScoreExist()) {					// 顯示最高分(bitmap)
                int tempScore = PublicData::file.ReadHighScore_Score();
                highScoreBorder.SetTopLeft(HIGHSCORE_POS_X, HIGHSCORE_POS_Y);
                highScoreBorder.ShowBitmap();

                for (int i = 0; i < 5; i++) {					// 顯示分數數字bmp
                    numBmpSmall[tempScore % 10].SetTopLeft( HIGHSCORE_POS_X - 10 * i + 130, HIGHSCORE_POS_Y + 5);
                    numBmpSmall[tempScore % 10].ShowBitmap();
                    tempScore /= 10;
                }
            }

            if (PublicData::newUnlock) {
                new_text.SetTopLeft((SIZE_X - menuBorder.Width()) / 2 + 130, MENU_Y + MENU_MARGIN_BTM * 2);
                new_text.ShowBitmap();
            }
        }
        else {
            exit.SetTopLeft((SIZE_X - exit.Width()) / 2, (NOTE_TEXT_Y + exit.Height() / 2));
            exit.ShowBitmap();
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
            noteExkey.SetTopLeft((SIZE_X - noteExkey.Width()) / 2, NOTE_TEXT_Y + 160);
            noteExkey.OnShow();
        }
    }
    else if (displayState == 2) {      // 顯示 選擇角色 頁面
        characterBorder.SetTopLeft((SIZE_X - characterBorder.Width()) / 2, CHARACTER_Y);
        characterBorder.ShowBitmap();
        characterArrow.SetTopLeft((SIZE_X - characterArrow.Width()) / 2, CHARACTER_Y + characterBorder.Height() / 2);
        characterArrow.ShowBitmap();
        PublicData::me.SetState(1);
        PublicData::me.OnShow();
    }
    else if (displayState == 3) {    	// 顯示 統計 頁面
        const int STATS_POS_X = (SIZE_X - statsBorder.Width()) / 2;	// 統計框之位置
        const int STATS_TEXT_POS_Y = 110, STATS_TEXT_POS_X = 310;	// 統計頁文字之位置
        const int STATS_TEXT_MARGIN = 30;							// 文字之 行距
        statsBorder.SetTopLeft(STATS_POS_X, NOTE_TEXT_Y);
        statsBorder.ShowBitmap();									// 顯示統計頁之框
        PublicData::me.SetState(statsDisplayState + 3);

        if (statsDisplayState == 0) {	// 左頁 最高記錄
            statsBg[0].SetTopLeft(STATS_POS_X, NOTE_TEXT_Y);
            statsBg[0].ShowBitmap();
            statsArrow[1].SetTopLeft((SIZE_X - statsArrow[0].Width()) / 2, NOTE_TEXT_Y + (statsBorder.Height() - statsArrow[0].Height()) / 2 + 4);
            statsArrow[1].ShowBitmap();

            //
            //從 CFile 中取得最高分內容並顯示
            if (!PublicData::file.isHighScoreExist()) {		// 若最高分不存在
                PublicData::me.SetState(5);
                statsNoRecord.SetTopLeft((SIZE_X - statsNoRecord.Width()) / 2 + 115, NOTE_TEXT_Y + 150);
                statsNoRecord.ShowBitmap();
            }
            else {
                int tempScore = PublicData::file.ReadHighScore_Score(),
                    tempTotalKeyCount = PublicData::totalKeyCount,
                    tempLevel = PublicData::file.ReadHighScore_Level(),
                    tempCorrectKeyCount = PublicData::file.ReadHighScore_CorrectKeyCount(),
                    tempAccuracy = int(PublicData::file.ReadHighScore_Accuracy() * 100.0);
                PublicData::me.SetHighScoreDisplay(PublicData::file.ReadHighScore_Character());

                //
                for (int i = 0; i < 5; i++) {					// 顯示分數數字bmp
                    numBmp[tempScore % 10].SetTopLeft(STATS_POS_X + 227 - 20 * i, NOTE_TEXT_Y + 196 );
                    numBmp[tempScore % 10].ShowBitmap();
                    tempScore /= 10;
                }

                for (int j = 0; j < 4; j++) {					// 顯示項目文字（高關、按鍵、正確）及數字
                    statsText[j].SetTopLeft(STATS_POS_X + STATS_TEXT_POS_X, \
                                            NOTE_TEXT_Y + STATS_TEXT_POS_Y + STATS_TEXT_MARGIN  * j);
                    statsText[j].ShowBitmap();					// 顯示項目文字
                    const int  STATS_TEXT_MARGIN_R = 170;		// 項目文字 與 數字 之距離

                    if (j == 0) {		// 0為顯示 累計總按鍵數
                        for (int i = 0; i < 7; i++) {		// 顯示總按鍵數數字bmp
                            numBmpSmall[tempTotalKeyCount % 10].SetTopLeft(STATS_POS_X + STATS_TEXT_POS_X + STATS_TEXT_MARGIN_R - 10 * i, \
                                    NOTE_TEXT_Y + STATS_TEXT_POS_Y + STATS_TEXT_MARGIN * j + 1);
                            numBmpSmall[tempTotalKeyCount % 10].ShowBitmap();
                            tempTotalKeyCount /= 10;

                            if (tempTotalKeyCount == 0)break;
                        }
                    }

                    if (j == 1) {		// 1為顯示 關卡數字
                        for (int i = 0; i < 2; i++) {		// 顯示關卡數字bmp
                            numBmpSmall_White[tempLevel % 10].SetTopLeft(STATS_POS_X + STATS_TEXT_POS_X + STATS_TEXT_MARGIN_R - 10 * i, \
                                    NOTE_TEXT_Y + STATS_TEXT_POS_Y + STATS_TEXT_MARGIN * j + 1);
                            numBmpSmall_White[tempLevel % 10].ShowBitmap();
                            tempLevel /= 10;
                        }
                    }
                    else if (j == 2) {	// 2為顯示 總按鍵數
                        for (int i = 0; i < 5; i++) {		// 顯示總按鍵數數字bmp
                            numBmpSmall_White[tempCorrectKeyCount % 10].SetTopLeft(STATS_POS_X + STATS_TEXT_POS_X + STATS_TEXT_MARGIN_R - 10 * i, \
                                    NOTE_TEXT_Y + STATS_TEXT_POS_Y + STATS_TEXT_MARGIN * j + 1);
                            numBmpSmall_White[tempCorrectKeyCount % 10].ShowBitmap();
                            tempCorrectKeyCount /= 10;

                            if (tempCorrectKeyCount == 0)break;
                        }
                    }
                    else if (j == 3) {	// 3為顯示 正確率
                        if (tempAccuracy != 10000) {						//若正確率非100%
                            for (int i = 0, dotPos = 0; i < 4; i++) {		// 顯示正確率bmp
                                numBmpSmall_White[tempAccuracy % 10].SetTopLeft \
                                (STATS_POS_X + STATS_TEXT_POS_X + STATS_TEXT_MARGIN_R - 10 * i - dotPos, \
                                 NOTE_TEXT_Y + STATS_TEXT_POS_Y + STATS_TEXT_MARGIN * j + 1);
                                numBmpSmall_White[tempAccuracy % 10].ShowBitmap();
                                tempAccuracy /= 10;

                                if (i == 1) {								// 顯示小數點
                                    dotPos = 5;
                                    numBmpSmall_White[11].SetTopLeft\
                                    (STATS_POS_X + STATS_TEXT_POS_X + STATS_TEXT_MARGIN_R - 10 * i - dotPos, \
                                     NOTE_TEXT_Y + STATS_TEXT_POS_Y + STATS_TEXT_MARGIN * j + 1);
                                    numBmpSmall_White[11].ShowBitmap();
                                }

                                tempLevel /= 10;
                            }
                        }
                        else {												// 針對100%顯示
                            numBmpSmall_White[1].SetTopLeft(STATS_POS_X + STATS_TEXT_POS_X + STATS_TEXT_MARGIN_R - 10 * 2, \
                                                            NOTE_TEXT_Y + STATS_TEXT_POS_Y + STATS_TEXT_MARGIN * j + 1);
                            numBmpSmall_White[1].ShowBitmap();
                            numBmpSmall_White[0].SetTopLeft(STATS_POS_X + STATS_TEXT_POS_X + STATS_TEXT_MARGIN_R - 10, \
                                                            NOTE_TEXT_Y + STATS_TEXT_POS_Y + STATS_TEXT_MARGIN * j + 1);
                            numBmpSmall_White[0].ShowBitmap();
                            numBmpSmall_White[0].SetTopLeft(STATS_POS_X + STATS_TEXT_POS_X + STATS_TEXT_MARGIN_R, \
                                                            NOTE_TEXT_Y + STATS_TEXT_POS_Y + STATS_TEXT_MARGIN * j + 1);
                            numBmpSmall_White[0].ShowBitmap();
                        }

                        numBmpSmall_White[10].SetTopLeft(STATS_POS_X + STATS_TEXT_POS_X + STATS_TEXT_MARGIN_R + 14, \
                                                         NOTE_TEXT_Y + STATS_TEXT_POS_Y + STATS_TEXT_MARGIN * j + 1);
                        numBmpSmall_White[10].ShowBitmap();					// 顯示百分比符號
                    }
                }
            }
        }
        else if (statsDisplayState == 1) {		// 右頁 遊玩記錄
            statsBg[1].SetTopLeft(STATS_POS_X, NOTE_TEXT_Y);
            statsBg[1].ShowBitmap();
            statsArrow[2].SetTopLeft((SIZE_X - statsArrow[0].Width()) / 2, NOTE_TEXT_Y + (statsBorder.Height() - statsArrow[0].Height()) / 2 + 4);
            statsArrow[2].ShowBitmap();

            // 設定顯示的箭頭樣式
            if (PublicData::file.GetRecordNum() <= 3 && PublicData::file.GetRecordNum() >= 0) {
                statsArrowV[3].ShowBitmap();		// 箭頭樣式：全暗
            }
            else {
                if (statsPRItemNum == 0)
                    statsArrowV[2].ShowBitmap();	// 箭頭樣式：下亮
                else if (statsPRItemNum == PublicData::file.GetRecordNum() - 3)
                    statsArrowV[1].ShowBitmap();	// 箭頭樣式：下亮
                else
                    statsArrowV[0].ShowBitmap();	// 箭頭樣式：全亮
            }

            if (PublicData::file.GetRecordNum() == 0) {		// 當查無遊戲記錄時
                PublicData::me.SetState(5);
                statsNoRecord.SetTopLeft((SIZE_X - statsNoRecord.Width()) / 2, NOTE_TEXT_Y + 163);
                statsNoRecord.ShowBitmap();
            }

            PublicData::me.SetPlayingRecordDisplay	// 顯示該筆紀錄的ME(若不存在則不顯示)
            (statsPRItemNum >= PublicData::file.GetRecordNum() ? "" :
             PublicData::file.ReadRecord_Character(statsPRItemNum),
             statsPRItemNum + 1 >= PublicData::file.GetRecordNum() ? "" :
             PublicData::file.ReadRecord_Character(statsPRItemNum + 1),
             statsPRItemNum + 2 >= PublicData::file.GetRecordNum() ? "" :
             PublicData::file.ReadRecord_Character(statsPRItemNum + 2) );

            for (int j = 0; j < 3; j++) {
                if (statsPRItemNum + j >= PublicData::file.GetRecordNum())
                    break;

                int tempScore = PublicData::file.ReadRecord_Score(statsPRItemNum + j),
                    tempLevel = PublicData::file.ReadRecord_Level(statsPRItemNum + j),
                    tempCorrectKeyCount = PublicData::file.ReadRecord_CorrectKeyCount(statsPRItemNum + j),
                    tempAccuracy = int(PublicData::file.ReadRecord_Accuracy(statsPRItemNum + j) * 100.0);
                string tempDate = PublicData::file.ReadRecord_Date(statsPRItemNum + j);
                const int STATS_PR_NUM_POS_X = 135, STATS_PR_NUM_PER_POS_X = 380;
                const int LINE_MARGIN = 44;

                for (int i = 0, signNum = 0; i < 12; i++) {		// 顯示日期時間
                    numBmpSmall_White[tempDate[i] - '0'].SetTopLeft(STATS_POS_X + STATS_PR_NUM_POS_X - 40 + 10 * i + signNum * 8,
                            NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                    numBmpSmall_White[tempDate[i] - '0'].ShowBitmap();

                    if (i == 3) {
                        numBmpSmall_White[12].SetTopLeft(STATS_POS_X + STATS_PR_NUM_POS_X - 40 + 10 * i + 10 + signNum * 8,
                                                         NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                        numBmpSmall_White[12].ShowBitmap();
                        signNum++;
                    }

                    if (i == 5) {
                        numBmpSmall_White[12].SetTopLeft(STATS_POS_X + STATS_PR_NUM_POS_X - 40 + 10 * i + 10 + signNum * 8,
                                                         NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                        numBmpSmall_White[12].ShowBitmap();
                        signNum++;
                    }

                    if (i == 7)  signNum++;

                    if (i == 9) {	// 冒號
                        numBmpSmall_White[13].SetTopLeft(STATS_POS_X + STATS_PR_NUM_POS_X - 40 + 10 * i + 12 + signNum * 8,
                                                         NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                        numBmpSmall_White[13].ShowBitmap();
                        signNum++;
                    }
                }

                for (int i = 0; i < 5; i++) {		// 顯示分數數字bmp
                    numBmpSmall[tempScore % 10].SetTopLeft(STATS_POS_X + STATS_PR_NUM_POS_X + 170 - 10 * i,
                                                           NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                    numBmpSmall[tempScore % 10].ShowBitmap();
                    tempScore /= 10;
                }

                for (int i = 0; i < 2; i++) {		// 顯示關卡數字bmp
                    numBmpSmall_White[tempLevel % 10].SetTopLeft(STATS_POS_X + STATS_PR_NUM_POS_X + 222 - 10 * i, \
                            NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                    numBmpSmall_White[tempLevel % 10].ShowBitmap();
                    tempLevel /= 10;
                }

                for (int i = 0; i < 5; i++) {		// 顯示總按鍵數bmp
                    numBmpSmall_White[tempCorrectKeyCount % 10].SetTopLeft(STATS_POS_X + STATS_PR_NUM_POS_X + 306 - 10 * i,
                            NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                    numBmpSmall_White[tempCorrectKeyCount % 10].ShowBitmap();
                    tempCorrectKeyCount /= 10;
                }

                if (tempAccuracy != 10000) {						//若正確率非100%
                    for (int i = 0, dotPos = 0; i < 4; i++) {		// 顯示正確率bmp
                        numBmpSmall_White[tempAccuracy % 10].SetTopLeft \
                        (STATS_POS_X + STATS_PR_NUM_POS_X + STATS_PR_NUM_PER_POS_X - 10 * i - dotPos, \
                         NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                        numBmpSmall_White[tempAccuracy % 10].ShowBitmap();
                        tempAccuracy /= 10;

                        if (i == 1) {								// 顯示小數點
                            dotPos = 5;
                            numBmpSmall_White[11].SetTopLeft\
                            (STATS_POS_X + STATS_PR_NUM_POS_X + STATS_PR_NUM_PER_POS_X - 10 * i - dotPos, \
                             NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                            numBmpSmall_White[11].ShowBitmap();
                        }

                        tempLevel /= 10;
                    }
                }
                else {												// 針對100%顯示
                    numBmpSmall_White[1].SetTopLeft(STATS_POS_X + STATS_PR_NUM_POS_X + STATS_PR_NUM_PER_POS_X - 10 * 2, \
                                                    NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                    numBmpSmall_White[1].ShowBitmap();
                    numBmpSmall_White[0].SetTopLeft(STATS_POS_X + STATS_PR_NUM_POS_X + STATS_PR_NUM_PER_POS_X - 10, \
                                                    NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                    numBmpSmall_White[0].ShowBitmap();
                    numBmpSmall_White[0].SetTopLeft(STATS_POS_X + STATS_PR_NUM_POS_X + STATS_PR_NUM_PER_POS_X, \
                                                    NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                    numBmpSmall_White[0].ShowBitmap();
                }

                numBmpSmall_White[10].SetTopLeft(STATS_POS_X + STATS_PR_NUM_POS_X + STATS_PR_NUM_PER_POS_X + 14, \
                                                 NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                numBmpSmall_White[10].ShowBitmap();			// 顯示百分比符號
            }
        }

        PublicData::me.OnShow();									// 顯示統計頁之角色（最高分和遊玩記錄）
    }
    else if (displayState == 4) {      // 顯示關於頁面
        if (aboutDisplayState == 0) {
            aboutBorder.SetTopLeft((SIZE_X - aboutBorder.Width()) / 2, NOTE_TEXT_Y);
            aboutBorder.ShowBitmap(); // 顯示關於框
            about.SetTopLeft((SIZE_X - aboutBorder.Width()) / 2, NOTE_TEXT_Y + 11);
            about.ShowBitmap();		  // 顯示關於文字

            if (PublicData::musicOnOff) {
                musicOnOff[0].SetTopLeft((SIZE_X - aboutBorder.Width()) / 2 + 458, NOTE_TEXT_Y + 180);
                musicOnOff[0].ShowBitmap();
            }
            else {
                musicOnOff[1].SetTopLeft((SIZE_X - aboutBorder.Width()) / 2 + 458, NOTE_TEXT_Y + 180);
                musicOnOff[1].ShowBitmap();
            }
        }
        else if (aboutDisplayState == 1) {	// 刪除遊戲紀錄頁
            delText.SetTopLeft((SIZE_X - delText.Width()) / 2, (NOTE_TEXT_Y + delText.Height() / 2) );
            delText.ShowBitmap();
        }
    }

    text1.SetTopLeft((SIZE_X - text1.Width()) / 2, text1_y);
    text1.ShowBitmap();

    if (0) {		// 展示顯示數字及字體
        for (int i = 0; i < 13; i++) {
            numBmpSmall[i].SetTopLeft(10 + i * 10, 300);
            numBmpSmall[i].ShowBitmap();
            numBmpSmall_White[i].SetTopLeft(10 + i * 10, 310);
            numBmpSmall_White[i].ShowBitmap();

            if (i < 10) {
                numBmp[i].SetTopLeft(10 + i * 20, 320);
                numBmp[i].ShowBitmap();
                numBmp_White[i].SetTopLeft(10 + i * 20, 340);
                numBmp_White[i].ShowBitmap();
            }
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
    if (isHighScore)	newHS_text.OnMove();	// 移動破紀錄動畫

    //if (isUnlock)newChar_text.OnMove();

    if (counter < 0)	GotoGameState(GAME_STATE_INIT);

    if (barCounter < 200 && barCounter < accuracy) {
        barCounter +=  2;
    }
}
void CGameStateOver::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
    const char KEY_ENTER = 0xD;
    (nChar == KEY_ENTER) ? GotoGameState(GAME_STATE_INIT) : 0;	// 按下Enter鍵返回開頭頁面
}
void CGameStateOver::OnBeginState() {
    counter = 1000 * 30;
    barCounter = 0;
    isHighScore = isUnlock = false;
    //
    score = PublicData::score;
    level = PublicData::level;
    accuracy = PublicData::accuracy;
    //
    PublicData::me.SetState(2);
    PublicData::file.ReadHighScoreFile();

    if (PublicData::file.GetRecordNum() == 0)					// 若不曾有遊玩記錄
        PublicData::totalKeyCount = PublicData::CorrectKeyCount;
    else {
        PublicData::totalKeyCount = PublicData::file.ReadTotalKeyCount();
        PublicData::totalKeyCount += PublicData::CorrectKeyCount;
    }

    if (score > PublicData::file.ReadHighScore_Score()) {		// 若本次分數大於 最高分則寫入
        PublicData::file.WriteHighScore(score, level, accuracy, PublicData::me.GetMeName(), PublicData::CorrectKeyCount);
        isHighScore = true;
    }

    PublicData::file.WriteRecord(score, level, accuracy, PublicData::me.GetMeName(), PublicData::CorrectKeyCount);	// 寫入 單筆遊玩記錄
    PublicData::file.WriteTotalKeyCount(PublicData::totalKeyCount);		// 寫入 總按鍵數
    PublicData::file.ReadRecordFile();									// 讀 遊玩記錄

    //
    if (PublicData::me.JudgeUnlock(PublicData::totalKeyCount, score, int(accuracy), level)) {// 判斷是否達成解鎖要素
        isUnlock = PublicData::newUnlock = true;
    }

    if (PublicData::musicOnOff) {
        CAudio::Instance()->Stop(AUDIO_ROCK);						// 暫停 背景音效
        CAudio::Instance()->Play(AUDIO_GAMEOVER, false);			// 播放 GAMEOVER音效
    }
}
void CGameStateOver::OnInit() {
    ShowInitProgress(66);	// 接個前一個狀態的進度，此處進度視為66%
    border.LoadBitmap("Bitmaps/gameover/gameover_border.bmp", RGB(0, 255, 0));
    newHS_text.AddBitmap("Bitmaps/gameover/gameover_new_hs1.bmp", RGB(0, 255, 0));
    newHS_text.AddBitmap("Bitmaps/gameover/gameover_new_hs2.bmp", RGB(0, 255, 0));
    newHS_text.SetDelayCount(20);
    newChar_text.AddBitmap("Bitmaps/gameover/gameover_new_char1.bmp", RGB(0, 255, 0));
    newChar_text.AddBitmap("Bitmaps/gameover/gameover_new_char2.bmp", RGB(0, 255, 0));
    newChar_text.SetDelayCount(20);
    ShowInitProgress(80);
    isHighScore = isUnlock = 0;

    for (int i = 0; i < 10; i++) {		// 載入數字圖
        char str[80];
        sprintf(str, "Bitmaps/level/num/%d.bmp", i);
        numBmp[i].LoadBitmap(str, RGB(0, 255, 0));
        sprintf(str, "Bitmaps/level/num_s/%d.bmp", i);
        numBmpSmall[i].LoadBitmap(str, RGB(0, 255, 0));
    }

    bar[0].LoadBitmap("Bitmaps/gameover/bar_0.bmp", RGB(0, 255, 0));
    bar[1].LoadBitmap("Bitmaps/gameover/bar_1.bmp", RGB(0, 255, 0));
    numBmpSmall[10].LoadBitmap("Bitmaps/level/num_s/per.bmp", RGB(0, 255, 0));	// 載入百分比圖片
    numBmpSmall[11].LoadBitmap("Bitmaps/level/num_s/dot.bmp", RGB(0, 255, 0));	// 載入小數點圖片
    x = (SIZE_X - border.Width()) / 2;
    y = (SIZE_Y - border.Height()) / 2;
    ShowInitProgress(100);

    if (PublicData::musicOnOff)
        CAudio::Instance()->Load(AUDIO_GAMEOVER, "sounds\\gameover.mp3");
}
void CGameStateOver::OnShow() {		// GAMEOVER 畫面顯示
    border.SetTopLeft(x, y);
    border.ShowBitmap();

    if (isHighScore) {	// 顯示破紀錄動畫
        newHS_text.SetTopLeft((SIZE_X - newHS_text.Width()) / 2, y + border.Height() + 20);
        newHS_text.OnShow();
    }

    if (isUnlock) {
        newChar_text.SetTopLeft((SIZE_X - newHS_text.Width()) / 2, y + border.Height() + 20 + 35 * isHighScore);
        newChar_text.OnShow();
    }

    int tempScore = score, tempLevel = level, tempAccuracy = int (accuracy * 100.0);
    int dotPos = 0;
    int scorePosX = 230, levelPosX = 130, accPosX = 155, // 定義各數字位置偏移量
        scorePosY =  45, levelPosY =  90, accPosY = 108,
        barPosX	  = 120, barPosY   = 122;

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
    callEnemyCounter = maxCallEnemyCounter = 20;	// maxCallEnemyCounter 決定怪物生成速度
    callBossACounter = maxCallBossACounter = 65;
    callBossBCounter = maxCallBossBCounter = 75;
}
CGameStateRun::~CGameStateRun() {
    for (CEnemy* eq : enemyQueue) delete eq;

    for (CBullet* bl : bulletList) delete bl;

    for (CBomb* cb : bombList) delete cb;
}
void CGameStateRun::OnBeginState() {
    const int SCORE_X = 240, SCORE_Y = 240;

    if (PublicData::musicOnOff)
        CAudio::Instance()->Play(AUDIO_ROCK, true);			// 撥放 MIDI

    score.SetInteger(0);								// 設定SCORE為0
    score.SetTopLeft(SCORE_X, SCORE_Y);
    currEnemyNum = currBossANum = currBossBNum = 0;		// 初始化各關敵人已召喚數量
    lock = false;										// 取消鎖定
    targetEnemy = NULL;
    currLevel = 0;										// 初始化關卡
    enemyQueue.clear();									// 清空EQ
    lives = 3;
    totalKeyDownCount = PublicData::CorrectKeyCount = 0;
    accuracy = 0;
    emp.SetEQ(&enemyQueue, &score, &lock, &targetEnemy);
    emp.SetEmpTimes(3);
    PublicData::me.SetState(0);
    totalEnemyNum = 0;
    levelChangeFlag = 0;
    levelChangeDelay = -1;
    levelChangeDelayMax = int( 3.5 * 30 );						// 設定關卡間delay 3秒
    pause = false;
}
void CGameStateRun::OnInit() {								// 遊戲的初值及圖形設定
    srand((unsigned)time(NULL));
    ShowInitProgress(33);
    //
    // 繼續載入其他資料
    //
    score.LoadBitmap();		// 載入分數顯示器
    map.LoadBitmap();		// 載入背景
    emp.LoadBitmap();		// 載入EMP
    levelAni.LoadBitmap();	// 載入切換關卡過場動畫
    pauseText.LoadBitmap("Bitmaps/menu/pause.bmp", RGB(0, 255, 0));
    debugText.LoadBitmap("Bitmaps/debug_text.bmp", RGB(0, 255, 0));

    if (1) {
        CAudio::Instance()->Load(AUDIO_ROCK, "sounds\\The_Coming_Storm.mp3");
        CAudio::Instance()->Load(AUDIO_SHOT, "sounds\\shot.mp3");
        CAudio::Instance()->Load(AUDIO_ERROR, "sounds\\error.mp3");
        CAudio::Instance()->Load(AUDIO_CONGRATULATION, "sounds\\congratulation.mp3");
        CAudio::Instance()->Load(AUDIO_CONGRATULATION2, "sounds\\congratulation2.mp3");
    }

    ShowInitProgress(40);

    for (int i = 0; i < 26; i++) {		// 載入字型圖片
        char str[50];
        sprintf(str, "Bitmaps/char4/%c.bmp", i + 97);
        letter.push_back(new CMovingBitmap);
        letter.back()->LoadBitmap(str, RGB(255, 255, 255));
    }

    for (int i = 0; i < 22; i++) {		// 載入鎖定敵人瞄準動畫
        char str[50];
        sprintf(str, "Bitmaps/target/target_s%d.bmp", i + 1);
        target.AddBitmap(str, RGB(0, 255, 0));
    }

    target.SetDelayCount(2);
    ShowInitProgress(50);
}
void CGameStateRun::OnMove() {			// 移動遊戲元素
    if (pause)return;

    if (levelChangeDelay >= 0)	levelChangeDelay--;		// 關卡與關卡間延遲的計數器

    accuracy = (totalKeyDownCount != 0) ? \
               100 * double(PublicData::CorrectKeyCount) / double(totalKeyDownCount) : \
               100;     // 計算正確率
    PublicData::score = score.GetInteger();
    PublicData::level = currLevel;
    PublicData::accuracy = accuracy;
    callEnemyCounter--;	// 每隻怪物 生成間隔 之 計數器
    callBossACounter--;
    callBossBCounter--;

    if (quickCall) {	//【DEBUG區】 將第0關設定生成200只怪物，且召喚delay為0秒
        //levelEnemyNum[0] = 200;
        levelChangeDelayMax = 0;
        maxCallEnemyCounter = maxCallBossACounter  = maxCallBossBCounter = 0;
    }
    else {
        levelChangeDelayMax = int(3.5 * 30);
        maxCallEnemyCounter = 25;
        maxCallBossACounter = 75;
        maxCallBossBCounter = 90;
    }

    //==小怪==================================
    if (callEnemyCounter < 0 && currEnemyNum < levelEnemyNum[currLevel]) {	// counter 數到0後就開始召喚新怪
        callEnemyCounter = maxCallEnemyCounter;				// 把counter 調回max繼續數
        int randX = (rand() % (SIZE_X - 100)) ;				// SIZE_X - 100 為了不讓怪物的單字超出螢幕太多
        enemyQueue.push_back(new CEnemy(randX, 0, 3, true, &dictionary, 2, 5, &enemyQueue, &bombList, \
                                        PublicData::me.GetX1() - 30 + rand() % 60, PublicData::me.GetY1(), &letter) ); // 將召喚的新怪放入vecotr內
        enemyQueue.back()->LoadBitmap();	// 載入召喚的新怪
        currEnemyNum++;						// 在本關卡已召喚的怪物計數器
        totalEnemyNum++;					// 總以召喚的怪物數量
    }

    //==BossA==================================
    if (callBossACounter < 0 && currBossANum < levelBossANum[currLevel]) {
        callBossACounter = maxCallBossACounter;
        int randX = (rand() % (SIZE_X - 350) + 200);
        enemyQueue.push_back(new CBossA(randX, 0, 5, true, &dictionary, 6, 12, &enemyQueue, &bombList, &letter));
        enemyQueue.back()->LoadBitmap();
        currBossANum++;
        totalEnemyNum++;
    }

    //==BossB==================================
    if (callBossBCounter < 0 && currBossBNum < levelBossBNum[currLevel]) {
        callBossBCounter = maxCallBossBCounter;
        int randX = (rand() % (SIZE_X - 350) + 200);
        enemyQueue.push_back(new CBossB(randX, 0, 5, true, &dictionary, 6, 12, &enemyQueue, &bombList, &letter));
        enemyQueue.back()->LoadBitmap();
        currBossBNum++;
        totalEnemyNum++;
    }

    //===判斷Me是否碰到Enemy===

    for (CEnemy* eq : enemyQueue) {
        if (eq->IsAlive() && eq->HitMe(&PublicData::me)) {
            lives--;
            (lives <= 0) ? GotoGameState(GAME_STATE_OVER) : 0;	// 若生命值為0則GOTO遊戲結束的STATE
        }

        if (eq->GetX() > SIZE_X + 40 || eq->GetX() < -40 || eq->GetY() > SIZE_Y + 40) {
            eq->kill();	// 當enemy飛出畫面外時殺掉怪物
        }
    }

    // ===Enemy===
    bool enemyAllDead = true;

    for (unsigned int i = 0; i < enemyQueue.size(); i++) { // 移動VECTOR內的所有怪物
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
        bl->OnMove();	// 移動BULLET

        if (bl->IsAlive()) bulletAllDead = false;
    }

    if (bulletAllDead) {	// 若VECTOR內的子彈皆播放完畢，釋放記憶體並清空VECOTR
        for (CBullet* bl : bulletList) delete bl;

        bulletList.clear();
    }

    ////===bomb===
    bool bombAllDead = true;

    for (CBomb* cb : bombList) {
        cb->OnMove();	// 移動BOMB

        if (cb->IsAlive()) bombAllDead = false;
    }

    if (bombAllDead) {
        for (CBomb* cb : bombList) {	// 若VECTOR內的爆炸皆播放完畢，釋放記憶體並清空VECOTR
            delete cb;
            cb = NULL;
        }

        bombList.clear();
    }

    ////===Change Level===
    if (currEnemyNum >= levelEnemyNum[currLevel] && currBossANum >= levelBossANum[currLevel] \
            && currBossBNum >= levelBossBNum[currLevel] && enemyQueue.size() == 0) {
        // 換 關卡
        if (!levelChangeFlag) {											// 等待delay算完
            levelAni.Play(currLevel, score.GetInteger());				// 播放切換關卡動畫
            levelChangeFlag = true;
            levelChangeDelay = levelChangeDelayMax;						// 將計數器調回

            if (PublicData::musicOnOff) {
                //CAudio::Instance()->Stop(AUDIO_ROCK);						// 暫停 背景音效
                //CAudio::Instance()->Play(AUDIO_CONGRATULATION, false);		// 撥放 過關音效
                //CAudio::Instance()->Play(AUDIO_CONGRATULATION2, false);		// 撥放 過關音效2
            }
        }

        if (levelChangeDelay < 0 && levelChangeFlag) {					// 當delay算完後 再實際切換關卡
            if (PublicData::musicOnOff) {
                //CAudio::Instance()->Stop(AUDIO_CONGRATULATION);				//暫停 過關音效
                //CAudio::Instance()->Stop(AUDIO_CONGRATULATION2);			//暫停 過關音效2
                //CAudio::Instance()->Play(AUDIO_ROCK, true);					// 撥放 背景音效
            }

            currLevel++;

            if (currLevel > LEVEL)GotoGameState(GAME_STATE_OVER);		// 若當前關卡大於最大關卡則GOTO遊戲結束的STATE

            currEnemyNum = currBossANum = currBossBNum = 0;				// 重置該關卡已召喚的怪物數量
            callEnemyCounter = maxCallEnemyCounter;						// 調回召喚怪物的計數器
            callBossACounter = maxCallBossACounter;
            callBossBCounter = maxCallBossBCounter;
            levelChangeFlag = false;
        }
    }

    //
    map.OnMove();				// 移動背景
    emp.OnMove();				// 移動EMP
    PublicData::me.OnMove();	// 移動主角
    levelAni.OnMove();			// 移動關卡切換動畫

    if (lock && targetEnemy != NULL) {
        target.OnMove();		// 移動鎖定目標動畫
    }
}
void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
    const char KEY_LEFT = 0x25;	// keyboard左箭頭
    const char KEY_UP = 0x26;	// keyboard上箭頭
    const char KEY_RIGHT = 0x27; // keyboard右箭頭
    const char KEY_DOWN = 0x28; // keyboard下箭頭

    if (pause) return;

    if (key != nChar) {
        key = nChar;	// 變數key來儲存按下的按鍵，防止按鍵彈跳

        for (int unsigned i = 0; i < enemyQueue.size(); i++) {			// 跑目前關卡怪物的數量
            if (enemyQueue[i]->IsAlive()) {								// 回傳當前怪物是否存在
                if (!lock) {											// 尚未鎖定
                    if (nChar + 32 == enemyQueue[i]->GetFirstWord()) {	// 若等於第一個字母:鎖住 and 目前字元位置+1
                        PublicData::CorrectKeyCount++;							// 正確按鍵數+1
                        map.PlayFlash();

                        if (PublicData::musicOnOff)
                            CAudio::Instance()->Play(AUDIO_SHOT, false);	// 撥放 射擊音效

                        if (enemyQueue[i]->GetVocabLeng() == 1) {		// A. 針對1字小怪攻擊
                            targetEnemy = enemyQueue[i];				// targetEnemy為指標->正在攻擊的敵人
                            bulletList.push_back(new CBullet(targetEnemy->GetX() + 10, targetEnemy->GetY() + 10));	// 射子彈
                            targetEnemy->kill();						// 成功殺害怪物
                            score.Add(int(targetEnemy->GetVocabLeng() * (1 + accuracy / 100)));		// 分數+= 怪物長度
                            targetEnemy = NULL;
                            break;
                        }
                        else {											// B. 針對一般的怪物攻擊
                            lock = true;								// 已鎖定某只怪物
                            targetEnemy = enemyQueue[i];				// targetEnemy為指標->正在攻擊的敵人
                            targetEnemy->AddCurrWordLeng();				// 已輸入到的單子數++
                            bulletList.push_back(new CBullet(targetEnemy->GetX() + 10, targetEnemy->GetY() + 10));	// 射子彈
                            targetEnemy->MinusIndex(2);					// 擊退怪物
                            break;
                        }
                    }
                    else {
                        if (nChar >= 65 && nChar <= 90)
                            if (PublicData::musicOnOff)
                                CAudio::Instance()->Play(AUDIO_ERROR, false);	// 撥放 射擊音效
                    }
                }
                else {													// 若已鎖定
                    if (nChar + 32 == targetEnemy->GetVocab()[targetEnemy->GetCurrWordLeng()]) { 	// 若等於當前字母
                        targetEnemy->AddCurrWordLeng();
                        bulletList.push_back(new CBullet(targetEnemy->GetX(), targetEnemy->GetY()));
                        targetEnemy->MinusIndex(rand() % 2 + 1);		// 擊退怪物
                        PublicData::CorrectKeyCount++;							// 正確按鍵數+1
                        map.PlayFlash();

                        if (PublicData::musicOnOff)
                            CAudio::Instance()->Play(AUDIO_SHOT, false);	// 撥放 射擊音效

                        if (targetEnemy->GetCurrWordLeng() == targetEnemy->GetVocabLeng()) {	 // 若當前長度 等於 字母的長度
                            targetEnemy->kill();						// 成功殺害怪物
                            score.Add(int(targetEnemy->GetVocabLeng() * (1 + accuracy / 100)));		// 分數+= 怪物長度
                            targetEnemy = NULL;							// 因怪物已被殺害，將targetEnemy指標指向NULL
                            lock = false;								// 取消鎖定怪物
                        }

                        break;
                    }
                    else {
                        if (nChar >= 65 && nChar <= 90)
                            if (PublicData::musicOnOff)
                                CAudio::Instance()->Play(AUDIO_ERROR, false);	// 撥放 射擊音效
                    }
                }
            }
        }

        if (nChar >= 65 && nChar <= 90) totalKeyDownCount++;			// 總按鍵數++

        if (nChar == 13) {// 若按下ENTER則發動EMP攻擊.
            emp.CallEmp(PublicData::musicOnOff);
        }
    }
}
void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
    const char KEY_LEFT = 0x25; // keyboard左箭頭
    const char KEY_UP = 0x26; // keyboard上箭頭
    const char KEY_RIGHT = 0x27; // keyboard右箭頭
    const char KEY_DOWN = 0x28; // keyboard下箭頭
    const char KEY_ESC = 27;
    key = NULL;

    if (nChar == KEY_ESC) pause = pause ? false : true;

    if (nChar == 'B' && pause) {
        CAudio::Instance()->Pause();
        GotoGameState(GAME_STATE_INIT);
    }

    if (PublicData::debugMode) {	// 允許使用DEBUG按鍵
        if (nChar == '1')showDebug = showDebug ? false : true;  // 按1 開關debug

        if (nChar == '2' && enemyQueue.size() > 0) {			// 按2 清除EQ最後一隻敵人
            enemyQueue.back()->kill();
            lock = 0;
        }

        if (nChar == '3' && enemyQueue.size() > 0) {			// 按3 清除EQ中所有敵人
            for (CEnemy* ce : enemyQueue) ce->kill();

            lock = 0;
        }

        if (nChar == '4')GotoGameState(GAME_STATE_INIT);		// 按4 GOTO 起始畫面

        if (nChar == '5')GotoGameState(GAME_STATE_OVER);		// 按5 GOTO 遊戲結束畫面

        if (nChar == '6')quickCall = quickCall ? false : true;	// 按6 開關快速召喚

        if (nChar == '7')score.Add(100);						// 按7 加百分
    }
}
void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point) {}
void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point) {}
void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point) {}
void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point) {}
void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point) {}
void CGameStateRun::OnShow() {
    map.OnShow();						// 顯示背景
    score.ShowBitmap();					// 顯示分數
    emp.OnShow();						// 顯示EMP（電磁波）
    PublicData::me.OnShow();			// 顯示主角
    levelAni.OnShow();					// 顯示關卡切換動畫

    for (CBomb* cb : bombList)	cb->OnShow();		//顯示VECTOR中所有的 爆炸

    for (CBullet* bl : bulletList)	bl->OnShow();	//顯示VECTOR中所有的 子彈

    for (CEnemy* eq : enemyQueue)	eq->OnShow();	//顯示VECTOR中所有的 怪物

    if (lock && targetEnemy != NULL) {
        targetEnemy->OnShow();						// 加上這一行 讓被鎖定的怪物再次顯示, 以防被其他怪物蓋住

        if (targetEnemy->GetBossType() == "enemy")
            target.SetTopLeft(targetEnemy->GetX() - 2, targetEnemy->GetY() - 2);	// 設定普通怪物 瞄準動畫的位置
        else
            target.SetTopLeft(targetEnemy->GetX() + 8, targetEnemy->GetY() + 8);	// 設定BOSS 瞄準動畫的位置

        target.OnShow();															// 顯示瞄準動畫
    }

    if (PublicData::debugMode) {													// 顯示debug操作說明
        debugText.SetTopLeft(SIZE_X - debugText.Width() - 20, 20);
        debugText.ShowBitmap();
    }

    if (pause) {																	// 顯示暫停視窗
        pauseText.SetTopLeft((SIZE_X - pauseText.Width()) / 2, (SIZE_Y - pauseText.Height() ) / 2);
        pauseText.ShowBitmap();
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
                totalKeyDownCount, PublicData::CorrectKeyCount, accuracy);
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
