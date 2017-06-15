#pragma once
#include "CCharacter.h"
#include "CMe.h"
#include "CDict.h"
#include "CBomb.h"
#include "CEnemy.h"
#include "CBullet.h"
#include "CMap.h"
#include "CBossA.h"
#include "CBossB.h"
#include "CEmp.h"
#include "CLevel.h"
#include "CRecord.h"
#include "CFile.h"

namespace game_framework {
// Constants
enum AUDIO_ID {				// 定義各種音效的編號
    AUDIO_DING,				// 0
    AUDIO_LAKE,				// 1
    AUDIO_NTUT,				// 2
    //===========================================
    AUDIO_ROCK,             // 3
    AUDIO_SHOT,             // 4
    AUDIO_ERROR,			// 5
    AUDIO_CONGRATULATION,	// 6
    AUDIO_CONGRATULATION2,	// 6
    AUDIO_GAMEOVER			// 7

};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲開頭畫面物件
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////
class PublicData {
    public:
        static int			score;						// 共用變數：儲存分數
        static int			level;						// 共用變數：儲存關卡
        static double		accuracy;					// 共用變數：正確率
        static CMe			me;							// 共用變數：主角參數
        static CFile		file;

        static int			CorrectKeyCount;		// 共用變數：正確按鍵數
        static int			totalKeyCount;			// 共用變數：正確按鍵數(會累積的)
        static bool			musicOnOff;					// 共用變數：音樂開關
        static bool			newUnlock;					// 共用變數：是否有新的解鎖角色

        static bool			debugMode;
};

class CGameStateInit : public CGameState {
    public:
        CGameStateInit(CGame* g);
        ~CGameStateInit();
        void OnInit();  								// 遊戲的初值及圖形設定
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnKeyUp(UINT, UINT, UINT); 				// 處理鍵盤Up的動作
        void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
        void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作
        void OnMove();
    protected:
        void OnShow();									// 顯示這個狀態的遊戲畫面
    private:
        const int NOTE_TEXT_X, NOTE_TEXT_Y;				// 定義 遊戲說明 擺放的位置
        const int MENU_Y;								// 定義 MENU 的Y軸
        const int MENU_ITEM_NUM;						// 定義 MENU 項目的數量
        const int CHARACTER_Y;							// 定義 角色選擇 之 角色Y軸
        CMovingBitmap typing_logo, taipin;				// typing typing 精美的LOGO
        CMovingBitmap text1;							// 說明文字
        int text1_y, text1_count;						// 說明文字移出效果
        CMap map;										// 背景
        int currSelectItem, displayState;				// 當前選擇的MENU, 當前顯示的狀態
        int noteDisplayState, statsDisplayState, aboutDisplayState;		// 當前顯示的說明狀態
        int statsPRItemNum;								// 遊玩記錄的項目數字
        int wrongKeyNum;								// 錯誤按鍵數
        int exitGameCount;								// 關閉遊戲計數
        bool exitState = false;
        //主選單元素
        vector<CMovingBitmap*>	menuText;						// 選單文字VECTOR
        CMovingBitmap			menuBorder, menuBorder_ckecked;	// 頁面指示燈
        CMovingBitmap			userBorder, highScoreBorder;	// 最高分的框
        CMovingBitmap			numBmp[10], numBmpSmall[14];	// 數字圖檔
        CMovingBitmap			numBmp_White[10], numBmpSmall_White[14];	// 數字圖檔（白色）
        CMovingBitmap			new_text;
        CMovingBitmap			exit;
        // 遊戲說明 元素
        CAnimation				noteExkey;						// 遊戲說明裡面的 打字動畫
        CMovingBitmap			noteBorder, noteArrow;			// 框線, 箭頭
        CMovingBitmap			noteSelected, noteUnselected;	// 指示燈
        vector<CMovingBitmap*>	note;							// 多頁的說明文字
        // 角色選擇 元素
        CMovingBitmap			characterBorder, characterArrow;// 角色選擇框 箭頭
        // 統計	   元素
        CMovingBitmap			statsBorder, statsBg[2];		// 統計頁面框, 統計頁面左右頁
        CMovingBitmap			statsArrow[3], statsArrowV[4];	// 左右箭頭, 上下四種狀態箭頭
        CMovingBitmap			statsText [4];					// 左頁項目文字
        CMovingBitmap			statsNoRecord;					// 無記錄 文字
        // 介紹頁面 元素
        string					cheatCode;						// 儲存作弊碼
        CMovingBitmap			aboutBorder, about;				// 關於框, 關於文字
        CMovingBitmap			delText;						// 確認刪除文字
        CMovingBitmap			musicOnOff[2];

};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的遊戲執行物件，主要的遊戲程式都在這裡
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateRun : public CGameState {
    public:
        CGameStateRun(CGame* g);
        ~CGameStateRun();
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnInit();  								// 遊戲的初值及圖形設定
        void OnKeyDown(UINT, UINT, UINT);
        void OnKeyUp(UINT, UINT, UINT);
        void OnLButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
        void OnLButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
        void OnMouseMove(UINT nFlags, CPoint point);	// 處理滑鼠的動作
        void OnRButtonDown(UINT nFlags, CPoint point);  // 處理滑鼠的動作
        void OnRButtonUp(UINT nFlags, CPoint point);	// 處理滑鼠的動作
    protected:
        void OnMove();									// 移動遊戲元素
        void OnShow();									// 顯示這個狀態的遊戲畫面
    private:
        bool			showDebug = false;				// 是否顯示debug資訊
        bool			quickCall = false;				// 是否開啟快速召喚
        CMovingBitmap	pauseText;										// 暫停視窗文字
        CMovingBitmap	debugText;										// debug提示文字
        CAnimation		target;											// 鎖定的動畫
        vector<CMovingBitmap*>	letter;
        vector<CEnemy*> enemyQueue;										// 儲存所有敵人的Vector
        vector<CBullet*>bulletList;										// 儲存飛行中的子彈的Vector
        vector<CBomb*>  bombList;										// 儲存爆炸效果的vector
        //
        CEnemy* 		targetEnemy;									// 指標 用於指向瞄準的敵人
        CDict			dictionary;										// 所有怪物共用的字典
        CMap			map;											// 背景圖
        CInteger		score;											// 分數顯示器
        CEmp			emp;											// 電磁波
        CLevel			levelAni;										// 切換關卡時的動畫
        //
        const int		LEVEL;											// 關卡總數
        char			key;											// 記錄所按下的按鍵 用於防止彈跳
        bool			lock;											// 判斷是否鎖住第一個字母了
        bool			pause;
        int				lives;											// 生命值
        int				levelEnemyNum[30] = { 4,  5,  5,  6,  7,  7,  7,  7,  8,  9, 10, 10, 10, 11, 11, 11, 12, 12, 12, 13, 13, 13, 14, 14, 14, 14, 15, 15, 15, 17 };		// 該關卡最大的敵人數
        int				levelBossANum[30] = { 0,  0,  1,  1,  2,  1,  1,  1,  1,  2,  1,  2,  2,  2,  3,  3,  3,  3,  3,  3,  4,  4,  4,  5,  5,  5,  5,  5,  5,  7 };
        int				levelBossBNum[30] = { 0,  0,  0,  0,  0,  1,  1,  2,  2,  1,  2,  1,  2,  2,  2,  2,  2,  2,  2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4 };
        int				callEnemyCounter, maxCallEnemyCounter;			// 召喚怪物間隔計數器, 召喚怪物間隔; maxCallEnemyCounter 決定怪物生成速度 越小速度越快
        int				callBossACounter, maxCallBossACounter;
        int				callBossBCounter, maxCallBossBCounter;
        int				currEnemyNum;									// 當前該關卡 已召喚的敵人數量
        int				currBossANum, currBossBNum;						// 當前該關卡 已召喚的BossA & BossB數量
        int				currLevel;										// 當前關卡
        int				totalEnemyNum;									// 總召喚的敵人數量
        int				levelChangeFlag, levelChangeDelay, levelChangeDelayMax; // 關卡和關卡間的delay
        int				totalKeyDownCount;								// 總按鍵數, 總正確按鍵數
        double			accuracy;										// 正確率

};

/////////////////////////////////////////////////////////////////////////////
// 這個class為遊戲的結束狀態(Game Over)
// 每個Member function的Implementation都要弄懂
/////////////////////////////////////////////////////////////////////////////

class CGameStateOver : public CGameState {
    public:
        CGameStateOver(CGame* g);
        void OnBeginState();							// 設定每次重玩所需的變數
        void OnInit();
        void OnKeyUp(UINT, UINT, UINT);
    protected:
        void OnMove();									// 移動遊戲元素
        void OnShow();									// 顯示這個狀態的遊戲畫面
    private:
        CMovingBitmap		border;								// 框線
        CMovingBitmap		numBmp[10], numBmpSmall[12];		// 數字圖檔
        CMovingBitmap		bar[2];			// 進度條
        CAnimation			newHS_text;		// 破紀錄文字
        CAnimation			newChar_text;
        bool				isHighScore;	// 本次遊玩的是否破紀錄
        bool				isUnlock;		// 本次遊玩的是否解鎖新角色
        int					counter;		// 倒數之計數器
        int					x, y;			// 圖檔顯示位置
        int					score, level;	// 分數, 關卡
        double				accuracy;		// 正確率
        int					barCounter;		// 進度條的計數器

};

}