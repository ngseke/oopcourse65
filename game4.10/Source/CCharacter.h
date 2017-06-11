#pragma once

namespace game_framework {

class CCharacter {
    public:
        CCharacter(string name, string subName, string fileName, int bmpNum, int, int, int, int);
        // �W��, �Ϥ����|�}�C, �Ϥ��ƶq,  UNLOCK����1~4
        // �������(�̧�)�G 1.�֭p�`���T����� 2.������� 3.������T�v 4.����F��̰����d
        void LoadBitmap();
        void OnMove();
        void OnShow();
        void SetXY(int, int);
        int  GetX1(), GetY1(), GetX2(), GetY2();
        int  GetWidth(), GetHeight();
        string GetName(), GetSubName();
        void SetIsUnlock(bool);
        bool GetIsUnlock();
        int GetUnlockRequirement(int);	// �^�ǲ�X�Ӹ������, X=0~3
    protected:
        CAnimation			animation;							// �D���ʵe
        int					bmpNum;								// ���ɼƶq
        string				fileName;							// �ɦW
        string				name, subName;						// �W��
        int					x, y;
        bool				isUnlock;							// �O�_����
        int					unlockRequirement[4];					// �������
};
}
