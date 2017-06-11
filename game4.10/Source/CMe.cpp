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
// CMe: 控制顯示主角的中樞

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
    bool atLeastOneUnlockFlag = false;		// 在本次判斷中 至少解鎖了一個新的角色
    int data[4] = { ur0, ur1, ur2, ur3 };	// 將欲判斷的資料 存入陣列

    for (CCharacter* cc : character) {		// 迴圈跑所有角色vector
        bool allGoodToUnlock = true;		// 先設定 “全達解鎖條件” 為 true

        for (int i = 0; i < 4; i++) {		// 迴圈跑4項解鎖條件
            if (!(data[i] >= cc->GetUnlockRequirement(i)))
                allGoodToUnlock = false;	// 若其中一項不符合 “全達解鎖條件” 便設定為false
        }

        if (allGoodToUnlock && !cc->GetIsUnlock()) {	// 若 “全達解鎖條件” 且 尚未被解鎖過 則解鎖該角色
            WriteUnlockCharacter(cc->GetName());	// 解鎖該角色手段為 寫入txt
            atLeastOneUnlockFlag = true;			// 設定“至少解鎖一角色為 true”
        }
    }

    ReadUnlockCharacter();
    return atLeastOneUnlockFlag;
}
bool CMe::GetSelectedCharIsUnlock() {
    return character[selectedChar]->GetIsUnlock();
}

void CMe::LoadCharacter() {
    // 解鎖條件(依序)： 1.累計總正確按鍵數 2.單場分數 3.單場正確率 4.單場達到最高關卡
    character.push_back(new CCharacter("Iron Man",			"鋼鐵人",		"me_ironman", 2,			 0, 0, 0, 0));
    character.push_back(new CCharacter("Captain American",	"美利堅隊長",	"me_captain_american", 1,	 0, 300, 92, 0));
    character.push_back(new CCharacter("Hulk",				"浩克",			"me_hulk", 1,				 0, 600, 87, 0));
    character.push_back(new CCharacter("Creeper",			"苦力怕",		"me_creeper", 1,			 0, 1000, 80, 0));
    character.push_back(new CCharacter("Cow",				"牛",			"me_cow", 1,				 500, 0, 0, 0));
    character.push_back(new CCharacter("Minion",			"小小兵",		"me_minion", 1,				 0, 1500, 80, 10));
    character.push_back(new CCharacter("Doge",				"狗狗",			"me_doge",	2,				0, 2000, 78, 0));
    character.push_back(new CCharacter("Tsai Ing-wen",		"蔡英文",		"me_Ing_wen", 10,			0, 3000, 75, 0));
    character.push_back(new CCharacter("Donald Trump",		"川普",			"me_trump", 1,				0, 5000, 70, 0));
    character.push_back(new CCharacter("Bouncing Ball",		"跳動的球",		"me_ball", 4,				0, 0, 97, 5));
    character.push_back(new CCharacter("Eraser",			"擦子",			"me_eraser", 4,				0, 0, 92, 15));
    character.push_back(new CCharacter("Zombie Brain",		"殭屍腦",		"me_zombie", 1,				1000, 0, 0, 0));
    character.push_back(new CCharacter("Pikachu",			"皮卡撐",		"me_pikachu", 2,			1500, 0, 0, 0));
    character.push_back(new CCharacter("Mike Wazowski",		"反霸凌博士",	"me_mike", 2,				2000, 0, 0, 0));
    character.push_back(new CCharacter("Mushroom",			"馬力歐裡面的菇", "me_mushroom", 1,			2500, 0, 0, 0));
    character.push_back(new CCharacter("Kirby",				"卡比",			"me_kirby",	6,				3000, 0, 0, 0));
    character.push_back(new CCharacter("Finn",				"阿寶",			"me_finn", 1,				4000, 0, 0, 0));
    character.push_back(new CCharacter("Flappy Bird",		"像素鳥",		"me_flappy_bird", 2,		5000, 0, 0, 0));
    character.push_back(new CCharacter("ROC",				"中華民國",		"me_roc", 1,				10000, 0, 0, 0));
    character.push_back(new CCharacter("PRC",				"中華人民共和國", "me_prc", 1,				34260, 0, 0, 0));
    character.push_back(new CCharacter("Japan",				"日本",			"me_japan", 1,				50000, 0, 0, 0));
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
    if (currState == 0) {	// 遊戲中顯示
        x = (SIZE_X - character[selectedChar]->GetWidth()) / 2;
        y = SIZE_Y - 60;
        character[selectedChar]->SetXY(x, y);
        character[selectedChar]->OnShow();
    }
    else if (currState == 1) {	//選擇角色畫面
        x = (SIZE_X - character[selectedChar]->GetWidth()) / 2;
        y = CHARACTER_POS_Y + 70;

        if (character[selectedChar]->GetIsUnlock()) {					// 中間
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

            if (position >= 0 && position < int(character.size()) ) {	// 右邊
                if (character[selectedChar + i]->GetIsUnlock()) {	// 若已被解鎖
                    character[selectedChar + i]->SetXY(x + PADDING_CENTER + PADDING_EACH * i, y);
                    character[selectedChar + i]->OnShow();
                }
                else {												// 尚未被解鎖
                    unlockSign.SetTopLeft((SIZE_X - unlockSign.Width()) / 2 + PADDING_CENTER + PADDING_EACH * i, y);
                    unlockSign.ShowBitmap();
                }
            }

            position = selectedChar - i;

            if (position >= 0 && position < int(character.size())) {	// 左邊
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
            f.CreatePointFont(100, "新細明體");
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
            pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
            CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
            ////
        }
        else {	// 顯示解鎖條件
            const int UNLOCK_BORDER_X = 470, UNLOCK_BORDER_Y = 280;
            unlock_border.SetTopLeft(UNLOCK_BORDER_X, UNLOCK_BORDER_Y);
            unlock_border.ShowBitmap();
            //
            CDC* pDC = CDDraw::GetBackCDC();
            CFont f, *fp;
            f.CreatePointFont(80, "新細明體");
            fp = pDC->SelectObject(&f);
            pDC->SetBkMode(TRANSPARENT);
            char temp[100];
            pDC->SetTextColor(RGB(200, 200, 200));
            int ulrPrintNum = 0;
            // 解鎖條件(依序)： 1.累計總正確按鍵數 2.單場分數 3.單場正確率 4.單場達到最高關卡

            for (int i = 0; i < 4; i++) {
                int num = character[selectedChar]->GetUnlockRequirement(i);

                if (i == 0)  sprintf(temp, "%s%d%s", "累計總按鍵數達 ", num, " 次");
                else if (i == 1) sprintf(temp, "%s%d%s", "單場分數達 ", num, " 分");
                else if (i == 2) sprintf(temp, "%s%d%s", "單場正確率高於 ", num, " %");
                else if (i == 3) sprintf(temp, "%s%d%s", "單場達到第 ", num, " 關卡");

                if (num != 0) {
                    if (i == 0) pDC->SetTextColor(RGB(255, 200, 15));
                    else		pDC->SetTextColor(RGB(200, 200, 200));

                    pDC->TextOut(UNLOCK_BORDER_X + 20, UNLOCK_BORDER_Y + 25 + ulrPrintNum * 14, temp);
                    ulrPrintNum++;
                }
            }

            //
            pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
            CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
        }
    }

    if (currState == 2) {	// 遊戲結束顯示
        x = (SIZE_X - character[selectedChar]->GetWidth()) / 2 + 200;
        y =  400;
        character[selectedChar]->SetXY(x, y);
        character[selectedChar]->OnShow();
    }

    if (currState == 3) {	// 最高紀錄顯示
        x = 294;
        y = 420;
        character[highScoreCharNum]->SetXY(x - character[highScoreCharNum]->GetWidth() / 2, y );
        character[highScoreCharNum]->OnShow();
    }

    if (currState == 4) {	// 遊玩紀錄顯示
        x = 160;
        y = 400;
        const int LINE_MARGIN = 44;		// 角色間的Y軸距離

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
            if (playingRecordName[j] == character[i]->GetName()) {	// 若輸入的角色名和Vector內匹配
                playingRecordCharNum[j] = i;
                break;
            }
            else if (playingRecordName[j] == "") {	// 不顯示主角Bitmap
                playingRecordCharNum[j] = 999;	// 999 表示不顯示主角Bitmap
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