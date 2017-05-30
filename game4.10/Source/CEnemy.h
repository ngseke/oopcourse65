#pragma once
#include "CBomb.h"
#include "CDict.h"
#include "CMe.h"
namespace game_framework {

class CEnemy {
    public:
        CEnemy();
        ~CEnemy();
        CEnemy(int x, int y, int delay, bool alive, CDict* dict, int minVL, int maxVL, \
               vector<CEnemy*>* enemyQueue, vector<CBomb*>* bombList, int endX, int endY, \
               vector<CMovingBitmap*>* letter);
        virtual void	LoadBitmap();					// 載入圖形
        virtual void	OnMove();						// 移動
        bool		IsAlive();							// 是否活著
        void		OnShow();							// 將圖形貼到畫面
        void		SetXY(int nx, int ny);				// 設定坐標
        void		SetIsAlive(bool alive);				// 設定是否活著
        void		SetDelay(int d);					// 設定掉落速度

        /////////////

        void		SetVocab();							// 隨機從dict中抓取一個單字到vocab裡面
        string		GetVocab();							// 回傳整組單字(ex: "apple")
        char		GetFirstWord();						// 以char回傳一個字 (ex: 'a')
        void		AddCurrWordLeng();					// CurrWord++
        int			GetCurrWordLeng();					// 回傳int型態的CurrWord
        int			GetVocabLeng();						// 回傳單字總長度
        int			GetX(), GetY();						// 取得X軸(x+dx), Y軸(y+dy)
        int			GetX2(), GetY2();					// 取得X2 (x+dx+bmp.width), Y2(y+dy+bmp.height)
        void		MinusIndex(int num);				// 擊退怪物 num為擊退多少index
        bool		HitMe(CMe* me);						// 是否碰到主角
        bool		IsBombed();							// 是否爆炸過了
        virtual void	 kill();
        string		GetBossType();

    protected:
        CMovingBitmap bmp;								// 球的圖
        CMovingBitmap bmp_center;						// 圓心的圖
        CMovingBitmap textCursor;						// 文字光標 圖
        CMovingBitmap talkBoxL, talkBoxC, talkBoxR;		// 優化過的對話框 圖
        vector<CEnemy*>* enemyQueue;
        vector<CBomb*>* bombList;
        vector<CMovingBitmap*>* letter;					// 文字圖檔
        CDict* dict;				// 字典檔指標
        string vocab;				// 儲存單字
        string bossType;			// 怪物的類型
        int x, y;					// 圓心的座標
        int dx, dy;					// 球距離圓心的位移量
        int endX, endY;				// 飛行的終點坐標
        int index;					// 將距離切成index等分
        int delay_counter;			// 掉落速度的計數器
        int delay;					// 掉落的速度
        bool is_alive;				// 是否活著
        bool is_bombed;				// 是否爆炸過了
        int length;					// 單字總長度
        int currWordLeng;			// 當前輸入文字的字元
        int minVocabLeng, maxVocabLeng;		// 規定怪物生成單字長度的區間( minVocabLeng ~ maxVocabLeng)

    private:
        bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形

};
}
