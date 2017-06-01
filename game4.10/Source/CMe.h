#pragma once
#include "CCharacter.h"


namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// 控制所有的主角CHARACTER
/////////////////////////////////////////////////////////////////////////////


class CMe {
    public:
        CMe();
        ~CMe();
        void LoadBitmap();				// 載入圖形
        void OnMove();					// 移動圖形
        void OnShow();					// 將圖形圖形貼到畫面
        int  GetX1();					// ME左上角 x 座標
        int  GetY1();					// ME左上角 y 座標
        int  GetX2();					// ME右下角 x 座標
        int  GetY2();					// ME右下角 y 座標
        void SetXY(int nx, int ny);		// 設定左上角座標
        void LoadCharacter();			// 載入角色
        void AddSelectedChar(int);		// 選擇角色時移動游標
        void SetState(int);				// 設定當前顯示的狀態

        int GetselectedChar();			// 取得選中的角色編號
        void SetselectedChar(int);		// 設定選中的角色編號
        string GetMeName();				// 取得選中的角色名
        void SetSelectedChar(string);	// 設定遊玩中選擇的角色
        //
        void SetHighScoreDisplay(string);						// 設定 【最高記錄】頁面的角色顯示
        void SetPlayingRecordDisplay(string, string, string);	// 設定 【遊玩記錄】頁面的角色顯示×3
        //
        void WriteUnlockCharacter(string);		// 寫入已解鎖的角色
        void ReadUnlockCharacter();				// 讀已解鎖的角色
        bool JudgeUnlock(int, int, int, int);	// 帶入本次遊玩的成績, 查看是否符合解鎖條件, 並回傳一個bool
        bool GetSelectedCharIsUnlock();			// 回傳選定的角色是否已解鎖

    protected:
        vector<CCharacter*> character;
        CFile*				file;
        string highScoreName, playingRecordName[3];
        CMovingBitmap		unlockSign, unlock_border;

        const int CHARACTER_POS_Y;
        int x, y;						// 圖形左上角座標
        int  selectedChar;				// 選中的角色編號
        int  currState;					// 0: 遊戲中, 1: 選擇角色畫面, 2: GAMEOVER畫面, 3: 最高記錄畫面, 4: 遊玩記錄畫面
        int highScoreCharNum, playingRecordCharNum[3];
};
}
