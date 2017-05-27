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
        void SetState(int);				// 設定當前顯示的狀

        int GetselectedChar();			// 取得選中的角色編號
        void SetselectedChar(int);			// 設定選中的角色編號
        string GetMeName();				// 取得選中的角色名

        void SetHighScoreDisplay(string);
        void SetPlayingRecordDisplay(string, string, string);

    protected:
        const int CHARACTER_POS_Y;
        int x, y;						// 圖形左上角座標
        vector<CCharacter*> character;
        int  selectedChar;				// 選中的角色編號
        int  currState;					// 0: 遊戲中, 1: 選擇角色畫面, 2: GAMEOVER畫面, 3: 最高分畫面
        string highScoreName, playingRecordName[3];
        int highScoreCharNum, playingRecordCharNum[3];
};
}
