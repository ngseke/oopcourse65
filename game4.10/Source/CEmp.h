#pragma once

namespace game_framework {

enum AUDIO_EMP_ID {
    AUDIO_EMP
};

class CEmp {
    public:
        CEmp();
        int  GetX1();					// 左上角 x 座標
        int  GetY1();					// 左上角 y 座標
        int  GetX2();					// 右下角 x 座標
        int  GetY2();					// 右下角 y 座標
        void Initialize();				// 設定初始值
        void LoadBitmap();				// 載入圖形
        void OnMove();
        void OnShow();
        void SetXY(int nx, int ny);		// 設定左上角座標
        void SetEQ(vector<CEnemy*>* enemyQueue, CInteger* score, bool* lock, CEnemy** targetEnemy);
        void CallEmp(bool);
        void SetEmpTimes(int);			// 設定電磁波可使用的次數
        int GetEmpTimes();
    protected:
        CMovingBitmap		displayBG, displayNumber[10];
        CAnimation			emp;				// 衝擊波
        vector<CEnemy*>*	enemyQueue;			// [指標] 儲存所有敵人的Vector
        CInteger*			score;				// [指標] 分數
        bool*				lock;				// [指標] 是否已鎖定敵人
        CEnemy**			targetEnemy;		// [指標] 被鎖定的敵人
        bool				state;				// 是否處於 技能播放中
        int					x, y;				// 左上角座標
        int					empTimes;			// 剩餘可召喚的EMP次數
    private:
        bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形

};
}
