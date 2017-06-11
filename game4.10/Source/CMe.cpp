#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include <sstream>
#include <fstream>
#include "CEnemy.h"
#include "CMe.h"
#include "CRecord.h"


namespace game_framework {
// CMe: ������ܥD��������

CMe::CMe(): CHARACTER_POS_Y(320) {
    x = 100;
    y = SIZE_Y - 60;
    currState = 1;
    selectedChar = 0;
    highScoreName = "";
    highScoreCharNum = 0;
    //file = NULL;
}
CMe::~CMe() {
    for (CCharacter* cc : character) delete cc;
}


void CMe::LoadBitmap() {
    unlockSign.LoadBitmap("Bitmaps/me/me_unlock.bmp", RGB(0, 255, 0));
    unlock_border.LoadBitmap("Bitmaps/menu/character/character_unlock_border.bmp", RGB(0, 255, 0));
    LoadCharacter();
}

void CMe::WriteUnlockCharacter(string name) {
    fstream	fp;
    fp.open("user/unlock.txt", ios::out | ios::app);
    fp << name << endl;
    fp.close();
}
void CMe::ReadUnlockCharacter() {
    for (CCharacter* cc : character)
        cc->SetIsUnlock(0);

    //
    fstream	fp;
    fp.open("user/unlock.txt", ios::in);
    char temp[100];

    if (fp) {
        while (!fp.eof()) {
            fp.getline(temp, sizeof(temp));

            for (CCharacter* cc : character) {
                if (cc->GetName() == temp)
                    cc->SetIsUnlock(true);
            }
        }
    }

    for (CCharacter* cc : character) TRACE("%d\n", cc->GetIsUnlock());

    fp.close();
}
bool CMe::JudgeUnlock(int ur0, int ur1, int ur2, int ur3) {
    bool atLeastOneUnlockFlag = false;		// �b�����P�_�� �ܤָ���F�@�ӷs������
    int data[4] = { ur0, ur1, ur2, ur3 };	// �N���P�_����� �s�J�}�C

    for (CCharacter* cc : character) {		// �j��]�Ҧ�����vector
        bool allGoodToUnlock = true;		// ���]�w �����F������� �� true

        for (int i = 0; i < 4; i++) {		// �j��]4���������
            if (!(data[i] >= cc->GetUnlockRequirement(i)))
                allGoodToUnlock = false;	// �Y�䤤�@�����ŦX �����F������� �K�]�w��false
        }

        if (allGoodToUnlock && !cc->GetIsUnlock()) {	// �Y �����F������� �B �|���Q����L �h����Ө���
            WriteUnlockCharacter(cc->GetName());	// ����Ө����q�� �g�Jtxt
            atLeastOneUnlockFlag = true;			// �]�w���ܤָ���@���⬰ true��
        }
    }

    ReadUnlockCharacter();
    return atLeastOneUnlockFlag;
}
bool CMe::GetSelectedCharIsUnlock() {
    return character[selectedChar]->GetIsUnlock();
}

void CMe::LoadCharacter() {
    // �������(�̧�)�G 1.�֭p�`���T����� 2.������� 3.������T�v 4.����F��̰����d
    character.push_back(new CCharacter("Iron Man",			"���K�H",		"me_ironman", 2,			 0, 0, 0, 0));
    character.push_back(new CCharacter("Captain American",	"���Q����",	"me_captain_american", 1,	 0, 300, 92, 0));
    character.push_back(new CCharacter("Hulk",				"�E�J",			"me_hulk", 1,				 0, 600, 87, 0));
    character.push_back(new CCharacter("Creeper",			"�W�O��",		"me_creeper", 1,			 0, 1000, 80, 0));
    character.push_back(new CCharacter("Cow",				"��",			"me_cow", 1,				 500, 0, 0, 0));
    character.push_back(new CCharacter("Minion",			"�p�p�L",		"me_minion", 1,				 0, 1500, 80, 10));
    character.push_back(new CCharacter("Doge",				"����",			"me_doge",	2,				0, 2000, 78, 0));
    character.push_back(new CCharacter("Tsai Ing-wen",		"���^��",		"me_Ing_wen", 10,			0, 3000, 75, 0));
    character.push_back(new CCharacter("Donald Trump",		"�t��",			"me_trump", 1,				0, 5000, 70, 0));
    character.push_back(new CCharacter("Bouncing Ball",		"���ʪ��y",		"me_ball", 4,				0, 0, 97, 5));
    character.push_back(new CCharacter("Eraser",			"���l",			"me_eraser", 4,				0, 0, 92, 15));
    character.push_back(new CCharacter("Zombie Brain",		"�L�͸�",		"me_zombie", 1,				1000, 0, 0, 0));
    character.push_back(new CCharacter("Pikachu",			"�֥d��",		"me_pikachu", 2,			1500, 0, 0, 0));
    character.push_back(new CCharacter("Mike Wazowski",		"���Q��դh",	"me_mike", 2,				2000, 0, 0, 0));
    character.push_back(new CCharacter("Mushroom",			"���O�ڸ̭���ۣ", "me_mushroom", 1,			2500, 0, 0, 0));
    character.push_back(new CCharacter("Kirby",				"�d��",			"me_kirby",	6,				3000, 0, 0, 0));
    character.push_back(new CCharacter("Finn",				"���_",			"me_finn", 1,				4000, 0, 0, 0));
    character.push_back(new CCharacter("Flappy Bird",		"������",		"me_flappy_bird", 2,		5000, 0, 0, 0));
    character.push_back(new CCharacter("ROC",				"���إ���",		"me_roc", 1,				10000, 0, 0, 0));
    character.push_back(new CCharacter("PRC",				"���ؤH���@�M��", "me_prc", 1,				34260, 0, 0, 0));
    character.push_back(new CCharacter("Japan",				"�饻",			"me_japan", 1,				50000, 0, 0, 0));
}

void CMe::OnMove() {
    if (currState == 0) character[selectedChar]->OnMove();
    else if (currState == 1) character[selectedChar]->OnMove();
    else if (currState == 3) {
        character[highScoreCharNum]->OnMove();
    }
    else if (currState == 4) {
        for (int i = 0; i < 3; i++) {
            if (playingRecordCharNum[i] != 999)
                character[playingRecordCharNum[i]]->OnMove();
        }
    }
}

void CMe::OnShow() {
    if (currState == 0) {	// �C�������
        x = (SIZE_X - character[selectedChar]->GetWidth()) / 2;
        y = SIZE_Y - 60;
        character[selectedChar]->SetXY(x, y);
        character[selectedChar]->OnShow();
    }
    else if (currState == 1) {	//��ܨ���e��
        x = (SIZE_X - character[selectedChar]->GetWidth()) / 2;
        y = CHARACTER_POS_Y + 70;

        if (character[selectedChar]->GetIsUnlock()) {					// ����
            character[selectedChar]->SetXY(x, y);
            character[selectedChar]->OnShow();
        }
        else {
            unlockSign.SetTopLeft((SIZE_X - unlockSign.Width()) / 2, y);
            unlockSign.ShowBitmap();
        }

        for (unsigned int i = 1; i <= 4; i++) {
            const int PADDING_CENTER = 40, PADDING_EACH = 80;
            int position = selectedChar + i;

            if (position >= 0 && position < int(character.size()) ) {	// �k��
                if (character[selectedChar + i]->GetIsUnlock()) {	// �Y�w�Q����
                    character[selectedChar + i]->SetXY(x + PADDING_CENTER + PADDING_EACH * i, y);
                    character[selectedChar + i]->OnShow();
                }
                else {												// �|���Q����
                    unlockSign.SetTopLeft((SIZE_X - unlockSign.Width()) / 2 + PADDING_CENTER + PADDING_EACH * i, y);
                    unlockSign.ShowBitmap();
                }
            }

            position = selectedChar - i;

            if (position >= 0 && position < int(character.size())) {	// ����
                if (character[selectedChar - i]->GetIsUnlock()) {
                    character[selectedChar - i]->SetXY(x - PADDING_CENTER - PADDING_EACH * i, y);
                    character[selectedChar - i]->OnShow();
                }
                else {
                    unlockSign.SetTopLeft((SIZE_X - unlockSign.Width()) / 2 - PADDING_CENTER - PADDING_EACH * i, y);
                    unlockSign.ShowBitmap();
                }
            }
        }

        ////
        if (character[selectedChar]->GetIsUnlock()) {
            CDC* pDC = CDDraw::GetBackCDC();
            CFont f, *fp;
            f.CreatePointFont(100, "�s�ө���");
            fp = pDC->SelectObject(&f);
            pDC->SetBkColor(RGB(0, 90, 130));
            pDC->SetBkMode(TRANSPARENT);
            char temp[20];
            pDC->SetTextColor(RGB(200, 200, 200));
            string name;
            name = character[selectedChar]->GetIsUnlock() ? character[selectedChar]->GetName() : "? ? ?";
            sprintf(temp, "%s", name.c_str());
            pDC->TextOut(350, CHARACTER_POS_Y + 60 + 60, temp);
            pDC->SetTextColor(RGB(255, 200, 15));
            name = character[selectedChar]->GetIsUnlock() ? character[selectedChar]->GetSubName() : "? ? ?";
            sprintf(temp, "%s", name.c_str());
            pDC->TextOut(350, CHARACTER_POS_Y + 60 + 74, temp);
            pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
            CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
            ////
        }
        else {	// ��ܸ������
            const int UNLOCK_BORDER_X = 470, UNLOCK_BORDER_Y = 280;
            unlock_border.SetTopLeft(UNLOCK_BORDER_X, UNLOCK_BORDER_Y);
            unlock_border.ShowBitmap();
            //
            CDC* pDC = CDDraw::GetBackCDC();
            CFont f, *fp;
            f.CreatePointFont(80, "�s�ө���");
            fp = pDC->SelectObject(&f);
            pDC->SetBkMode(TRANSPARENT);
            char temp[100];
            pDC->SetTextColor(RGB(200, 200, 200));
            int ulrPrintNum = 0;
            // �������(�̧�)�G 1.�֭p�`���T����� 2.������� 3.������T�v 4.����F��̰����d

            for (int i = 0; i < 4; i++) {
                int num = character[selectedChar]->GetUnlockRequirement(i);

                if (i == 0)  sprintf(temp, "%s%d%s", "�֭p�`����ƹF ", num, " ��");
                else if (i == 1) sprintf(temp, "%s%d%s", "������ƹF ", num, " ��");
                else if (i == 2) sprintf(temp, "%s%d%s", "������T�v���� ", num, " %");
                else if (i == 3) sprintf(temp, "%s%d%s", "����F��� ", num, " ���d");

                if (num != 0) {
                    if (i == 0) pDC->SetTextColor(RGB(255, 200, 15));
                    else		pDC->SetTextColor(RGB(200, 200, 200));

                    pDC->TextOut(UNLOCK_BORDER_X + 20, UNLOCK_BORDER_Y + 25 + ulrPrintNum * 14, temp);
                    ulrPrintNum++;
                }
            }

            //
            pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
            CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
        }
    }

    if (currState == 2) {	// �C���������
        x = (SIZE_X - character[selectedChar]->GetWidth()) / 2 + 200;
        y =  400;
        character[selectedChar]->SetXY(x, y);
        character[selectedChar]->OnShow();
    }

    if (currState == 3) {	// �̰��������
        x = 294;
        y = 420;
        character[highScoreCharNum]->SetXY(x - character[highScoreCharNum]->GetWidth() / 2, y );
        character[highScoreCharNum]->OnShow();
    }

    if (currState == 4) {	// �C���������
        x = 160;
        y = 400;
        const int LINE_MARGIN = 44;		// ���ⶡ��Y�b�Z��

        for (int i = 0; i < 3; i++) {
            if (playingRecordCharNum[i] != 999) {
                character[ playingRecordCharNum[i] ]->SetXY(x - character[playingRecordCharNum[i]]->GetWidth() / 2, \
                        y + LINE_MARGIN * i);
                character[ playingRecordCharNum[i] ]->OnShow();
            }
        }
    }

    if (currState == 5) {}
}

void CMe::AddSelectedChar(int num) {
    int result = selectedChar + num;

    if (result >= 0 && result < int(character.size()) ) selectedChar = result;
}
void CMe::SetSelectedChar(string characterName) {
    selectedChar = 0;

    for (unsigned int i = 0; i < character.size(); i++) {
        if (characterName == character[i]->GetName()) {
            selectedChar = i;
            break;
        }
    }
}
void CMe::SetXY(int nx, int ny) {
    x = nx;
    y = ny;
}
void CMe::SetHighScoreDisplay( string characterName) {
    this->highScoreName = characterName;

    for (unsigned int i = 0; i < character.size(); i++) {
        if (highScoreName == character[i]->GetName()) {
            highScoreCharNum = i;
            break;
        }
    }
}
void CMe::SetPlayingRecordDisplay(string s0, string s1, string s2) {
    playingRecordName[0] = s0;
    playingRecordName[1] = s1;
    playingRecordName[2] = s2;

    for (int j = 0; j < 3; j++) {
        for (unsigned int i = 0; i < character.size(); i++) {
            if (playingRecordName[j] == character[i]->GetName()) {	// �Y��J������W�MVector���ǰt
                playingRecordCharNum[j] = i;
                break;
            }
            else if (playingRecordName[j] == "") {	// ����ܥD��Bitmap
                playingRecordCharNum[j] = 999;	// 999 ��ܤ���ܥD��Bitmap
                break;
            }
        }
    }
}
int CMe::GetX1() {
    return character[selectedChar]->GetX1();
}
int CMe::GetY1() {
    return character[selectedChar]->GetY1();
}
int CMe::GetX2() {
    return character[selectedChar]->GetX2();
}
int CMe::GetY2() {
    return character[selectedChar]->GetY2();
}
void CMe::SetState(int state) {
    currState = state;
}
int CMe::GetselectedChar() {
    return selectedChar;
}
string CMe:: GetMeName() {
    return character[selectedChar]->GetName();
}
void CMe::SetselectedChar(int selectedChar) {
    this->selectedChar = selectedChar;
}
}