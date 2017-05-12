#pragma once

namespace game_framework {

class CCharacter {
    public:
        CCharacter(string name, string pathName[10], int bmpNum, int, int);
        // �W��, �Ϥ����|�}�C, �Ϥ��ƶq, X�b, Y�b
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void SetXY(int, int);
        int  GetX1(), GetY1(), GetX2(), GetY2();
        int  GetWidth(), GetHeight();
        string GetName();

    protected:
        CAnimation			animation;							// �D���ʵe
        int					bmpNum;								// ���ɼƶq
        string				pathName[10];						// �ɮ׸��|
        string				name;								// �W��
        int					x, y;
};
}
