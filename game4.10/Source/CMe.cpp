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
}


void CMe::LoadBitmap() {
    LoadCharacter();
    /*
    int character = 2;


    if (character == 0) {
        animation.AddBitmap("Bitmaps/me_ironman.bmp", RGB(255, 255, 255));
        animation.AddBitmap("Bitmaps/me_ironman1.bmp", RGB(255, 255, 255));
        animation.SetTopLeft(x, y);
    }
    else if (character == 1) {
        animation.AddBitmap("Bitmaps/me_hulk.bmp", RGB(255, 255, 255));
        animation.SetTopLeft(x - 4, y - 2);
    }
    else if (character == 2) {
        animation.AddBitmap("Bitmaps/me_captian_american.bmp", RGB(0, 255, 0));
        animation.SetTopLeft(x, y);
    }
    */
}

void CMe::LoadCharacter() {
    string file1[2] = { "Bitmaps/me_ironman.bmp", "Bitmaps/me_ironman1.bmp" };
    character.push_back(new CCharacter("Iron Man", file1, 2, x, y));
    //
    string file2[1] = { "Bitmaps/me_captain_american.bmp" };
    character.push_back(new CCharacter("Captain American", file2, 1, x, y));
    //
    string file3[1] = { "Bitmaps/me_hulk.bmp" };
    character.push_back(new CCharacter("Hulk", file3, 1, x, y));
    //
    string file4[1] = { "Bitmaps/me_cow.bmp" };
    character.push_back(new CCharacter("Cow", file4, 1, x, y));
    string file5[1] = { "Bitmaps/me_cow.bmp" };
    character.push_back(new CCharacter("Cow", file5, 1, x, y));
}

void CMe::OnMove() {
    if (currState == 0) character[selectedChar]->OnMove();
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
        pDC->SetBkMode(TRANSPARENT);
        char temp[20];
        sprintf(temp, "%s", character[selectedChar]->GetName().c_str());
        pDC->SetTextColor(RGB(200, 200, 200));
        pDC->TextOut(350, CHARACTER_POS_Y + 70 + 60, temp);
        pDC->SelectObject(fp);						// 放掉 font f (千萬不要漏了放掉)
        CDDraw::ReleaseBackCDC();					// 放掉 Back Plain 的 CDC
        ////
    }
}
void CMe::addSelectedChar(int num) {
    int result = selectedChar + num;

    if (result >= 0 && result < int(character.size()) ) selectedChar = result;
}
void CMe::SetXY(int nx, int ny) {
    x = nx;
    y = ny;
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

void CMe::setState(int state) {
    currState = state;
}

}