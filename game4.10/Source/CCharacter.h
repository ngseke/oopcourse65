#pragma once

namespace game_framework {

class CCharacter {
    public:
        CCharacter(string name, string subName, string fileName, int bmpNum, int, int, int, int);
        // 名稱, 圖片路徑陣列, 圖片數量,  UNLOCK條件1~4
        // 解鎖條件(依序)： 1.累計總正確按鍵數 2.單場分數 3.單場正確率 4.單場達到最高關卡
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void SetXY(int, int);
        int  GetX1(), GetY1(), GetX2(), GetY2();
        int  GetWidth(), GetHeight();
        string GetName(), GetSubName();
        void SetIsUnlock(bool);
        bool GetIsUnlock();
        int GetUnlockRequirement(int);	// 回傳第X個解鎖條件, X=0~3
    protected:
        CAnimation			animation;							// 主角動畫
        int					bmpNum;								// 圖檔數量
        string				fileName;							// 檔名
        string				name, subName;						// 名稱
        int					x, y;
        bool				isUnlock;							// 是否解鎖
        int					unlockRequirement[4];					// 解鎖條件
};
}
