#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CEnemy.h"
#include "CMe.h"

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// CMe: 主角
/////////////////////////////////////////////////////////////////////////////

CMe::CMe(): CHARACTER_POS_Y(320) {
    x = 100;
    y = SIZE_Y - 60;
    selectedChar = 0;
    currState = 1;
    highScoreName = "";
    highScoreCharNum = 0;
}
CMe::~CMe() {
    for (CCharacter* cc : character) delete cc;
}

void CMe::LoadBitmap() {
    LoadCharacter();
}

void CMe::LoadCharacter() {
    character.push_back(new CCharacter("Iron Man",			"鋼鐵人",		"me_ironman", 2, x, y));
    character.push_back(new CCharacter("Captain American",	"美利堅隊長",	"me_captain_american.bmp", 1, x, y));
    character.push_back(new CCharacter("Hulk",				"浩克",			"me_hulk.bmp", 1, x, y));
    character.push_back(new CCharacter("Creeper",			"苦力怕",		"me_creeper.bmp", 1, x, y));
    character.push_back(new CCharacter("Minion",			"小小兵",		"me_minion.bmp", 1, x, y));
    character.push_back(new CCharacter("Zombie Brain",		"殭屍腦",		"me_zombie.bmp", 1, x, y));
    character.push_back(new CCharacter("Cow",				"牛",			"me_cow.bmp", 1, x, y));
    character.push_back(new CCharacter("Bouncing Ball",		"跳動的球",		"me_ball", 4, x, y));
}

void CMe::OnMove() {
    if (currState == 0) character[selectedChar]->OnMove();
    else if (currState == 1) character[selectedChar]->OnMove();
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
        y = CHARACTER_POS_Y + 70;			// 待修改
        character[selectedChar]->SetXY(x, y);
        character[selectedChar]->OnShow();

        for (unsigned int i = 1; i <= 4; i++) {
            const int PADDING_CENTER = 40, PADDING_EACH = 80;
            int position = selectedChar + i;

            if (position >= 0 && position < int(character.size()) ) {
                character[selectedChar + i]->SetXY(x + PADDING_CENTER + PADDING_EACH * i, y);
                character[selectedChar + i]->OnShow();
            }

            position = selectedChar - i;

            if (position >= 0 && position < int(character.size())) {
                character[selectedChar - i]->SetXY(x - PADDING_CENTER - PADDING_EACH * i, y);
                character[selectedChar - i]->OnShow();
            }
        }

        ////
        CDC* pDC = CDDraw::GetBackCDC();
        CFont f, *fp;
        f.CreatePointFont(100, "新細明體");
        fp = pDC->SelectObject(&f);
        pDC->SetBkColor(RGB(0, 90, 130));
        pDC->SetBkMode(TRANSPARENT);
        char temp[20];
        pDC->SetTextColor(RGB(200, 200, 200));
        sprintf(temp, "%s", character[selectedChar]->GetName().c_str());
        pDC->TextOut(350, CHARACTER_POS_Y + 60 + 60, temp);
        sprintf(temp, "%s", character[selectedChar]->GetSubName().c_str());
        pDC->TextOut(350, CHARACTER_POS_Y + 60 + 74, temp);
        pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
        CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
        ////
    }

    if (currState == 2) {	// 遊戲結束顯示
        x = (SIZE_X - character[selectedChar]->GetWidth()) / 2 + 200;
        y =  400;
        character[selectedChar]->SetXY(x, y);
        character[selectedChar]->OnShow();
    }

    if (currState == 3) {	// 最高紀錄顯示
        x = 282;
        y = 420;
        character[highScoreCharNum]->SetXY(x, y);
        character[highScoreCharNum]->OnShow();
    }

    if (currState == 4) {	// 遊玩紀錄顯示
        x = 160;
        y = 400;
        const int LINE_MARGIN = 44;

        for (int i = 0; i < 3; i++) {
            character[ playingRecordCharNum[i] ]->SetXY(x - character[playingRecordCharNum[i]]->GetWidth() / 2, \
                    y + LINE_MARGIN * i);
            character[ playingRecordCharNum[i] ]->OnShow();
        }
    }
}
void CMe::AddSelectedChar(int num) {
    int result = selectedChar + num;

    if (result >= 0 && result < int(character.size()) ) selectedChar = result;
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
            if (playingRecordName[j] == character[i]->GetName()) {
                playingRecordCharNum[j] = i;
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

}