#pragma once
#include "CBomb.h"
#include "CDict.h"
#include "CMe.h"
namespace game_framework {

class CEnemy {
    public:
        CEnemy();
        CEnemy(int x, int y, int delay, bool alive, CDict* dict, int minVL, int maxVL, vector<CEnemy*>* enemyQueue, vector<CBomb*>* bombList, int endX, int endY);
        bool IsAlive();											// 是否活著
        virtual void LoadBitmap();								// 載入圖形
        virtual void OnMove();									// 移動
        void OnShow();											// 將圖形貼到畫面
        void SetXY(int nx, int ny);								// 設定圓心的座標
        void SetIsAlive(bool alive);							// 設定是否活著
        void SetDelay(int d);									// 設定掉落速度

        /////////////

        void SetVocab();										// 隨機從dict中抓取一個單字到vocab裡面
        string GetVocab();										// 回傳整組單字(ex: "apple")
        char GetFirstWord();									// 以char回傳一個字 (ex: 'a')
        void AddCurrWordLeng();									// CurrWord++
        int GetCurrWordLeng();									// 回傳int型態的CurrWord
        int GetVocabLeng();										// 回傳單字總長度
        int GetX(), GetY();										// 取得X軸(x+dx), Y軸(y+dy)
        int GetX2(), GetY2();
        void MinusIndex(int num);								// 擊退怪物 num為擊退多少index
        bool HitMe(CMe* me);
        bool IsBombed();										// 是否爆炸過了
        virtual void kill();

    protected:
        CMovingBitmap bmp;								// 球的圖
        CMovingBitmap bmp_center;						// 圓心的圖
        CMovingBitmap textCursor;						// 文字光標 圖
        CMovingBitmap talkBoxL, talkBoxC, talkBoxR;		// 優化過的對話框 圖
        vector<CMovingBitmap*> letter;					// 文字圖檔
        //          <  =  = ... =  >
        // talkBox: L  C  C ... C  R
        // L(Left), C(Center), R(Right)
        // 依照單字長度 生成不同數量的 talkBoxC
        //
        CAnimation	target;			// 鎖定的動畫
        int x, y;					// 圓心的座標
        int dx, dy;					// 球距離圓心的位移量
        int index;					// 球的「角度」，0-17表示0-360度
        int delay_counter;			// 掉落速度的計數器
        int delay;					// 掉落的速度
        bool is_alive;				// 是否活著
        //int xMoveDistance, yMoveDistance;
        /////////
        string vocab;				// 儲存單字
        int length;					// 單字總長度
        int currWordLeng;			// 當前輸入文字的字元
        CDict* dict;				// 字典檔指標
        int minVocabLeng, maxVocabLeng;		// 規定怪物生成單字長度的區間( minVocabLeng ~ maxVocabLeng)
        int targetX, targetY;
        vector<CBomb*>* bombList;
        bool is_bombed;
        int endX, endY;
        vector<CEnemy*>* enemyQueue;
    private:
        bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形

};
}
