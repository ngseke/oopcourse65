#pragma once

namespace game_framework {

class CCharacter {
    public:
        CCharacter(string name, string subName, string fileName, int bmpNum);
        // �W��, �Ϥ����|�}�C, �Ϥ��ƶq, X�b, Y�b
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void SetXY(int, int);
        int  GetX1(), GetY1(), GetX2(), GetY2();
        int  GetWidth(), GetHeight();
        string GetName(), GetSubName();


    protected:
        CAnimation			animation;							// �D���ʵe
        int					bmpNum;								// ���ɼƶq
        string				fileName;							// �ɦW
        string				name, subName;						// �W��
        int					x, y;
};
}
