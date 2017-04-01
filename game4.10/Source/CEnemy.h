namespace game_framework {

class CEnemy {
    public:
        CEnemy();
        CEnemy(int x, int y, int delay, bool alive);

        bool HitEraser(CEraser* eraser);						// 是否碰到擦子
        bool IsAlive();											// 是否活著
        void LoadBitmap();										// 載入圖形
        void OnMove();											// 移動
        void OnShow();											// 將圖形貼到畫面
        void SetXY(int nx, int ny);								// 設定圓心的座標
        void SetIsAlive(bool alive);							// 設定是否活著
        void SetDelay(int d);									// 設定旋轉的速度
        /////////////

        void SetVocab();										// 隨機從dict中抓取一個單字到vocab裡面
        string GetVocab();										// 回傳整組單字(ex: "apple")
        char GetFirstWord();									// 以char回傳一個字 (ex: 'a')
        void AddCurrWordLeng();									// CurrWord++
        int GetCurrWordLeng();									// 回傳int型態的CurrWord
        int GetVocabLeng();
        void LoadTextbox();
        int GetX(), GetY();
        void MinusIndex(int num);								//	擊退怪物 num為擊退多少index


    protected:
        CMovingBitmap bmp;			// 球的圖
        CMovingBitmap bmp_center;	// 圓心的圖
        CMovingBitmap textCursor;	// 文字光標 圖
        //CMovingBitmap talkBox;		// 對話框 圖	(舊的)
        CMovingBitmap talkBoxL, talkBoxC, talkBoxR;		// 優化過的對話框 圖
        vector<CMovingBitmap*> letter;
        //          <  =  = ... =  >
        // talkBox: L  C  C ... C  R
        // L(Left), C(Center), R(Right)
        // 依照單字長度 生成不同數量的 talkBoxC
        //
        CAnimation		target;
        int x, y;					// 圓心的座標
        int dx, dy;					// 球距離圓心的位移量
        int index;					// 球的「角度」，0-17表示0-360度
        int delay_counter;			// 調整旋轉速度的計數器
        int delay;					// 旋轉的速度
        bool is_alive;				// 是否活著
        /////////
        string vocab;				// 儲存單字
        int length;					// 單字總長度
        int currWordLeng;			// 當前輸入文字的字元
        int xMoveDistance;			// 怪物X位移的總距離

    private:

        bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// 是否碰到參數範圍的矩形
};
}