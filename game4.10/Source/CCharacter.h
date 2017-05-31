#pragma once

namespace game_framework {

class CCharacter {
    public:
        CCharacter(string name, string subName, string fileName, int bmpNum);
        // 名稱, 圖片路徑陣列, 圖片數量, X軸, Y軸
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void SetXY(int, int);
        int  GetX1(), GetY1(), GetX2(), GetY2();
        int  GetWidth(), GetHeight();
        string GetName(), GetSubName();


    protected:
        CAnimation			animation;							// 主角動畫
        int					bmpNum;								// 圖檔數量
        string				fileName;							// 檔名
        string				name, subName;						// 名稱
        int					x, y;
};
}
