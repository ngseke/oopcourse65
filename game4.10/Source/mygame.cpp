#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include <time.h>
#include "audio.h"
#include "gamelib.h"
#include "mygame.h"

namespace game_framework {
/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C���}�Y�e������
/////////////////////////////////////////////////////////////////////////////

int PublicData::score = 0;
int PublicData::level = 0;
int PublicData::CorrectKeyCount = 0;
int PublicData::totalKeyCount = 0;
double PublicData::accuracy = 0.0;
bool PublicData::musicOnOff = 1;
bool PublicData::newUnlock = false;
bool PublicData::debugMode = false;
CFile PublicData::file;
CMe	PublicData::me;

CGameStateInit::CGameStateInit(CGame* g)
    : CGameState(g), NOTE_TEXT_X(60), NOTE_TEXT_Y(280), MENU_Y(320),
      MENU_ITEM_NUM(5), CHARACTER_Y(320) {
}

CGameStateInit::~CGameStateInit() {
    for (CMovingBitmap* mt : menuText) delete mt;

    for (CMovingBitmap* nt : note) delete nt;
}
void CGameStateInit::OnInit() {
    ShowInitProgress(0);	// �@�}�l��loading�i�׬�0%
    currSelectItem = displayState = 0;					// ��l�ƿ��������
    noteDisplayState = statsDisplayState = aboutDisplayState = 0;			// ��l�ơ��C���������Ρ��έp�������������
    statsPRItemNum = 0;									// ��l�Ʋέp���� �̰��O�����������
    wrongKeyNum = 0;
    exitGameCount = 0;
    cheatCode = "";

    if (0) {// DEBUG��
        displayState = 1;
        noteDisplayState = 6;
        statsDisplayState = 0;
        PublicData::newUnlock = 1;
    }

    PublicData::me.ReadUnlockCharacter();
    PublicData::me.LoadBitmap();											// �D��
    PublicData::me.SetSelectedChar(PublicData::file.ReadSelectedCharacter());	// Ū���W�����������
    PublicData::musicOnOff = PublicData::file.ReadMusicOnOff();					// Ū���W�����֪����A
    map.LoadBitmap();														// �I�������ʵe
    typing_logo.LoadBitmap("Bitmaps/start_logo1.bmp", RGB(0, 255, 0));		// logo
    taipin.LoadBitmap("Bitmaps/taipin.bmp", RGB(0, 255, 0));
    text1.LoadBitmap("Bitmaps/text1_start.bmp", RGB(0, 255, 0));			// �ާ@�覡����bitmap
    highScoreBorder.LoadBitmap("Bitmaps/menu/highscore_border.bmp", RGB(0, 255, 0));
    new_text.LoadBitmap("Bitmaps/menu/menu_new_text.bmp", RGB(0, 255, 0));
    /////
    //
    // ���J��椸��
    ShowInitProgress(5);

    for (int i = 0; i < 5; i++) {		// 5�ӿ��
        char str[50];
        sprintf(str, "Bitmaps/menu/menu_t_%de.bmp", i + 1);
        menuText.push_back(new CMovingBitmap);
        menuText.back()->LoadBitmap(str, RGB(0, 255, 0));
    }

    ShowInitProgress(10);
    menuText.push_back(new CMovingBitmap);
    menuText.back()->LoadBitmap("Bitmaps/menu/menu_t_be.bmp", RGB(0, 255, 0));	// ��^���s
    menuBorder.LoadBitmap("Bitmaps/menu/menu_border.bmp", RGB(0, 255, 0));
    menuBorder_ckecked.LoadBitmap("Bitmaps/menu/menu_border_checked.bmp", RGB(0, 255, 0));
    // ���J�C����������
    noteBorder.LoadBitmap("Bitmaps/menu/note/note_text_border.bmp", RGB(0, 255, 0)); // �����ؽu
    noteArrow.LoadBitmap ("Bitmaps/menu/note/note_text_arraw.bmp", RGB(0, 255, 0));	// �����b�Y
    noteUnselected.LoadBitmap("Bitmaps/menu/note/note_unselected.bmp", RGB(0, 255, 0));
    noteSelected.LoadBitmap("Bitmaps/menu/note/note_selected.bmp", RGB(0, 255, 0));
    exit.LoadBitmap("Bitmaps/menu/exit.bmp", RGB(0, 255, 0));
    ShowInitProgress(15);

    for (int i = 0; i < 6; i++) {	// �����ظ̭�������ʵe
        char str[50];
        sprintf(str, "Bitmaps/menu/note/note1_exkey_%d.bmp", i + 1);
        noteExkey.AddBitmap(str, RGB(0, 255, 0));
    }

    noteExkey.SetDelayCount(10);

    for (int i = 0; i < 9; i++) {		// �h����������r
        char str[50];
        sprintf(str, "Bitmaps/menu/note/note_text_p%d.bmp", i + 1);
        note.push_back(new CMovingBitmap);
        note.back()->LoadBitmap(str, RGB(0, 255, 0));
    }

    for (int i = 0; i < 10; i++) {		// ���J�Ʀr��
        char str[50];
        sprintf(str, "Bitmaps/level/num/%d.bmp", i);
        numBmp[i].LoadBitmap(str, RGB(0, 255, 0));
        sprintf(str, "Bitmaps/level/num_s/%d.bmp", i);
        numBmpSmall[i].LoadBitmap(str, RGB(0, 255, 0));
        sprintf(str, "Bitmaps/level/num_white/%d.bmp", i);
        numBmp_White[i].LoadBitmap(str, RGB(0, 255, 0));
        sprintf(str, "Bitmaps/level/num_s_white/%d.bmp", i);
        numBmpSmall_White[i].LoadBitmap(str, RGB(0, 255, 0));
    }

    numBmpSmall[10].LoadBitmap("Bitmaps/level/num_s/per.bmp", RGB(0, 255, 0));
    numBmpSmall[11].LoadBitmap("Bitmaps/level/num_s/dot.bmp", RGB(0, 255, 0));
    numBmpSmall[12].LoadBitmap("Bitmaps/level/num_s/slash.bmp", RGB(0, 255, 0));
    numBmpSmall_White[10].LoadBitmap("Bitmaps/level/num_s_white/per.bmp", RGB(0, 255, 0));
    numBmpSmall_White[11].LoadBitmap("Bitmaps/level/num_s_white/dot.bmp", RGB(0, 255, 0));
    numBmpSmall_White[12].LoadBitmap("Bitmaps/level/num_s_white/slash.bmp", RGB(0, 255, 0));
    numBmpSmall_White[13].LoadBitmap("Bitmaps/level/num_s_white/colon.bmp", RGB(0, 255, 0));
    ShowInitProgress(20);
    // ���J������ ����
    characterBorder.LoadBitmap("Bitmaps/menu/character/character_border.bmp", RGB(0, 255, 0));
    characterArrow.LoadBitmap("Bitmaps/menu/character/character_arrow.bmp", RGB(0, 255, 0));
    // ���J�έp����
    statsBorder.LoadBitmap("Bitmaps/menu/stats/stats_border.bmp", RGB(0, 255, 0));
    statsBg[0].LoadBitmap("Bitmaps/menu/stats/stats_bg1.bmp", RGB(0, 255, 0));
    statsBg[1].LoadBitmap("Bitmaps/menu/stats/stats_bg2.bmp", RGB(0, 255, 0));
    statsArrow[0].LoadBitmap("Bitmaps/menu/stats/stats_arrow.bmp", RGB(0, 255, 0));
    statsArrow[1].LoadBitmap("Bitmaps/menu/stats/stats_arrow_right.bmp", RGB(0, 255, 0));
    statsArrow[2].LoadBitmap("Bitmaps/menu/stats/stats_arrow_left.bmp", RGB(0, 255, 0));
    statsArrowV[0].LoadBitmap("Bitmaps/menu/stats/stats_arrow_v.bmp", RGB(0, 255, 0));
    statsArrowV[1].LoadBitmap("Bitmaps/menu/stats/stats_arrow_v_up.bmp", RGB(0, 255, 0));
    statsArrowV[2].LoadBitmap("Bitmaps/menu/stats/stats_arrow_v_down.bmp", RGB(0, 255, 0));
    statsArrowV[3].LoadBitmap("Bitmaps/menu/stats/stats_arrow_v_none.bmp", RGB(0, 255, 0));

    for (int i = 0; i < 4; i++)
        statsArrowV[i].SetTopLeft((SIZE_X - statsArrow[0].Width()) / 2 + 570, NOTE_TEXT_Y + 163);

    statsText[0].LoadBitmap("Bitmaps/menu/stats/stats_text_tkc.bmp", RGB(0, 255, 0));
    statsText[1].LoadBitmap("Bitmaps/menu/stats/stats_text_hl.bmp", RGB(0, 255, 0));
    statsText[2].LoadBitmap("Bitmaps/menu/stats/stats_text_ckc.bmp", RGB(0, 255, 0));
    statsText[3].LoadBitmap("Bitmaps/menu/stats/stats_text_acc.bmp", RGB(0, 255, 0));
    statsNoRecord.LoadBitmap("Bitmaps/menu/stats/stats_no_record.bmp", RGB(0, 255, 0));
    // ���J���󤸯�
    aboutBorder.LoadBitmap("Bitmaps/menu/about/about_border.bmp", RGB(0, 255, 0)); // ���Юؽu
    about.LoadBitmap("Bitmaps/menu/about/about_text_p2.bmp", RGB(0, 255, 0)); // ���Ф�r
    delText.LoadBitmap("Bitmaps/menu/about/about_del_text.bmp", RGB(0, 255, 0));	// �T�{�R������
    musicOnOff[0].LoadBitmap("Bitmaps/menu/about/about_music_on.bmp", RGB(0, 255, 0));
    musicOnOff[1].LoadBitmap("Bitmaps/menu/about/about_music_off.bmp", RGB(0, 255, 0));
}

void CGameStateInit::OnBeginState() {
    text1_y = 550;
    text1_count = 0;
    PublicData::file.ReadHighScoreFile();
    PublicData::file.ReadRecordFile();
    statsPRItemNum = 0;		// �C���O�������ؼƦr�k�s�]�^��Ĥ@����ơ^
    PublicData::totalKeyCount = PublicData::file.ReadTotalKeyCount();
}

void CGameStateInit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
    PublicData::me.ReadUnlockCharacter();
    const char KEY_ESC = 27;
    const char KEY_SPACE = ' ';
    const char KEY_ENTER = 0xD;
    const char KEY_LEFT	 = 0x25;	// keyboard���b�Y
    const char KEY_UP	 = 0x26;	// keyboard�W�b�Y
    const char KEY_RIGHT = 0x27;	// keyboard�k�b�Y
    const char KEY_DOWN  = 0x28;	// keyboard�U�b�Y

    if (!(nChar == KEY_ESC || nChar == KEY_LEFT || nChar == KEY_UP || nChar == KEY_RIGHT || nChar == KEY_DOWN || nChar == KEY_ENTER || nChar == 'D' || nChar == 'Y' || nChar == 'N' || (nChar <= '5' && nChar >= '1'))) {
        //wrongKeyNum++;
    }

    if ((nChar <= '5' && nChar >= '1') && displayState == 0) {		// ���ѥH�Ʀr��1��5�Ӿ��a���
        if (nChar == '1')  GotoGameState(GAME_STATE_RUN);

        currSelectItem = displayState = nChar - '1';
        noteDisplayState = statsDisplayState = aboutDisplayState = 0;
        cheatCode = "";
    }

    if (nChar == KEY_ESC) {		// ESC��...
        if (!(displayState == 0) && !(displayState == 2 && !PublicData::me.GetSelectedCharIsUnlock()))
            displayState = 0;	// ��^�D���
        else if (!(displayState == 2) && !exitState) {
            exitState = true;
            return;
        }
    }

    if (exitState) {
        if (nChar == 'N' || nChar == KEY_ESC)exitState = false;
        else if (nChar == 'Y')	PostMessage(AfxGetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);
    }

    if (displayState == 0 ) {					// �b�D���...
        if (nChar == KEY_UP || nChar == KEY_DOWN) {		// ���ʥ���
            if (nChar == KEY_UP) currSelectItem--;
            else if (nChar == KEY_DOWN) currSelectItem++;

            if (currSelectItem < 0)currSelectItem = MENU_ITEM_NUM - 1;
            else if (currSelectItem > MENU_ITEM_NUM - 1)currSelectItem = 0;
        }
        else if (nChar == KEY_ENTER) {					// ���UENTER����...
            if (currSelectItem == 0) {
                GotoGameState(GAME_STATE_RUN);			// �}�l�C��
            }
            else {
                displayState = currSelectItem;				// �e���ҿ����state
                noteDisplayState = statsDisplayState = aboutDisplayState = 0;	// ��l�ƻ�����r��������� �� �C���O�������ؼƦr
                cheatCode = "";

                if (currSelectItem == 2) PublicData::newUnlock = false;
            }
        }
    }
    else if (displayState == 1 ) {							// [�C������]
        if (nChar == KEY_ENTER) displayState = 0;			//  ->��^�D���
        else if (nChar == KEY_LEFT || nChar == KEY_RIGHT) { // [�C������] ���k½���C������
            if		(nChar == KEY_LEFT)   noteDisplayState--;
            else if (nChar == KEY_RIGHT) noteDisplayState++;

            if (noteDisplayState < 0) noteDisplayState = int(note.size()) - 1;
            else if (noteDisplayState >  int(note.size()) - 1) noteDisplayState = 0;
        }
    }
    else if (displayState == 2) { // [������]
        if (nChar == KEY_ENTER) {
            if (PublicData::me.GetSelectedCharIsUnlock()) {
                PublicData::file.WriteSelectedCharacter(PublicData::me.GetMeName());
                displayState = 0;// ->��^�D���
            }
        }
        else if (nChar == KEY_LEFT || nChar == KEY_RIGHT) {
            if		(nChar == KEY_LEFT)   PublicData::me.AddSelectedChar(-1);
            else if (nChar == KEY_RIGHT)  PublicData::me.AddSelectedChar(1);
        }
    }
    else if (displayState == 3) { // [�έp]
        if (nChar == KEY_ENTER) displayState = 0;	// ->��^�D���
        else if (nChar == KEY_LEFT || nChar == KEY_RIGHT) { // [�έp] ����:�̰��O��; �k��:�C���O��
            if		(nChar == KEY_LEFT)	 statsDisplayState = 0;
            else if (nChar == KEY_RIGHT) statsDisplayState = 1;
        }

        if (statsDisplayState == 1) {				// �Y�� �C���O�����A
            if		(nChar == KEY_UP) {   // �V�W�d��O��
                if (statsPRItemNum > 0)statsPRItemNum--;
            }
            else if (nChar == KEY_DOWN) { // �V�U�d��O��
                if (statsPRItemNum < PublicData::file.GetRecordNum() - 3)statsPRItemNum++;
            }
        }
    }
    else if (displayState == 4 ) { // [����]
        if (aboutDisplayState == 0 && nChar == KEY_ENTER) displayState = 0;	// ->��^�D���

        if (aboutDisplayState == 0) {
            cheatCode = cheatCode + char(nChar);

            if (cheatCode == "104590029") {						// �@���X:����Ҧ�����
                PublicData::me.JudgeUnlock(99999, 99999, 100, 30);
                displayState = 0;
                cheatCode = "";
            }
            else if (cheatCode == "104590025") {				// �@���X:���\debug�Ҧ�
                PublicData::debugMode = true;
                displayState = 0;
                cheatCode = "";
            }
        }

        if (aboutDisplayState == 0 && nChar == 'M') {						// �}������
            PublicData::musicOnOff = PublicData::musicOnOff ? false : true;
            PublicData::file.WriteMusicOnOff(PublicData::musicOnOff);
        }

        if (aboutDisplayState == 0 && nChar == 'D' ) {
            aboutDisplayState = 1;    // ��ܲM���C����������
        }

        if (aboutDisplayState == 1) {
            if (nChar == 'Y') {	// ��YES�T�{�R���C������
                PublicData::file.DeleteAllData();			// �M��txt��
                PublicData::file.ReadHighScoreFile();		// ���s���J�C������
                PublicData::file.ReadRecordFile();
                PublicData::me.SetSelectedChar("Iron Man");	// ���m��������⬰IronMan
                displayState = 0;							// ��^�D���
            }
            else if (nChar == 'N') {	// ��NO����
                aboutDisplayState = 0;
            }
        }
    }
}

void CGameStateInit::OnLButtonDown(UINT nFlags, CPoint point) {
    wrongKeyNum++;
}
void CGameStateInit::OnMouseMove(UINT nFlags, CPoint point) {}
void CGameStateInit::OnMove() {
    map.OnMove();
    noteExkey.OnMove();
    PublicData::me.OnMove();

    if (exitGameCount != 0 && exitGameCount < 15)
        exitGameCount++;
    else
        exitGameCount = 0;

    if (text1_count < 400) text1_count++;

    if (text1_count > 5 * 30)  text1_y += int((text1_count - 5 * 30) * 1.1);

    if (wrongKeyNum > 3) {	// �Y�h�����U���������s�A�h�͵��������s���񻡩���r
        text1_y = 550;
        text1_count = wrongKeyNum = 0;
    }

    if (0) GotoGameState(GAME_STATE_OVER);
}
void CGameStateInit::OnShow() {
    map.OnShow();
    typing_logo.SetTopLeft((SIZE_X - typing_logo.Width()) / 2, 100);
    typing_logo.ShowBitmap();
    taipin.SetTopLeft((SIZE_X + typing_logo.Width()) / 2 - 60, 100 + 31);
    taipin.ShowBitmap();

    //
    if (displayState == 0) {	// ��ܥD���
        if (!exitState) {
            const int MENU_MARGIN_BTM = 40;
            menuBorder_ckecked.SetTopLeft((SIZE_X - menuBorder.Width()) / 2, MENU_Y + MENU_MARGIN_BTM * currSelectItem);
            menuBorder_ckecked.ShowBitmap();

            for (int i = 0; i < MENU_ITEM_NUM; i++) {
                menuBorder.SetTopLeft((SIZE_X - menuBorder.Width()) / 2, MENU_Y + MENU_MARGIN_BTM * i);
                menuBorder.ShowBitmap();
                menuText[i]->SetTopLeft((SIZE_X - menuText[i]->Width()) / 2, MENU_Y + 7 + MENU_MARGIN_BTM * i);
                menuText[i]->ShowBitmap();
            }

            int HIGHSCORE_POS_X = (SIZE_X + menuBorder.Width()) / 2 + 8;
            int HIGHSCORE_POS_Y = MENU_Y + 10;

            if (PublicData::file.isHighScoreExist()) {					// ��̰ܳ���(bitmap)
                int tempScore = PublicData::file.ReadHighScore_Score();
                highScoreBorder.SetTopLeft(HIGHSCORE_POS_X, HIGHSCORE_POS_Y);
                highScoreBorder.ShowBitmap();

                for (int i = 0; i < 5; i++) {					// ��ܤ��ƼƦrbmp
                    numBmpSmall[tempScore % 10].SetTopLeft( HIGHSCORE_POS_X - 10 * i + 130, HIGHSCORE_POS_Y + 5);
                    numBmpSmall[tempScore % 10].ShowBitmap();
                    tempScore /= 10;
                }
            }

            if (PublicData::newUnlock) {
                new_text.SetTopLeft((SIZE_X - menuBorder.Width()) / 2 + 130, MENU_Y + MENU_MARGIN_BTM * 2);
                new_text.ShowBitmap();
            }
        }
        else {
            exit.SetTopLeft((SIZE_X - exit.Width()) / 2, (NOTE_TEXT_Y + exit.Height() / 2));
            exit.ShowBitmap();
        }
    }
    else if (displayState == 1) {	// ��ܻ�����r
        // �����ؽu
        noteBorder.SetTopLeft((SIZE_X - noteBorder.Width()) / 2, NOTE_TEXT_Y);
        noteBorder.ShowBitmap();
        // �����b�Y
        noteArrow.SetTopLeft((SIZE_X - noteArrow.Width()) / 2, NOTE_TEXT_Y + (noteBorder.Height()  - noteArrow.Height()) / 2 + 11);
        noteArrow.ShowBitmap();
        // ������r
        note[noteDisplayState]->SetTopLeft((SIZE_X - noteBorder.Width()) / 2, NOTE_TEXT_Y + (noteBorder.Height() - note[noteDisplayState]->Height() ) / 2 + 11);
        note[noteDisplayState]->ShowBitmap();

        // ������r ���ܿO
        for (unsigned int i = 0; i < note.size(); i++) {
            noteUnselected.SetTopLeft((SIZE_X - noteBorder.Width()) / 2 + noteBorder.Width() - 25 - 8 * i, NOTE_TEXT_Y + noteBorder.Height() - 20);
            noteUnselected.ShowBitmap();
        }

        noteSelected.SetTopLeft((SIZE_X - noteBorder.Width()) / 2 + noteBorder.Width() - 25 - 8 * (note.size() - noteDisplayState - 1), NOTE_TEXT_Y + noteBorder.Height() - 20);
        noteSelected.ShowBitmap();

        // �������r�ʵe
        if (noteDisplayState == 0) {
            noteExkey.SetTopLeft((SIZE_X - noteExkey.Width()) / 2, NOTE_TEXT_Y + 160);
            noteExkey.OnShow();
        }
    }
    else if (displayState == 2) {      // ��� ��ܨ��� ����
        characterBorder.SetTopLeft((SIZE_X - characterBorder.Width()) / 2, CHARACTER_Y);
        characterBorder.ShowBitmap();
        characterArrow.SetTopLeft((SIZE_X - characterArrow.Width()) / 2, CHARACTER_Y + characterBorder.Height() / 2);
        characterArrow.ShowBitmap();
        PublicData::me.SetState(1);
        PublicData::me.OnShow();
    }
    else if (displayState == 3) {    	// ��� �έp ����
        const int STATS_POS_X = (SIZE_X - statsBorder.Width()) / 2;	// �έp�ؤ���m
        const int STATS_TEXT_POS_Y = 110, STATS_TEXT_POS_X = 310;	// �έp����r����m
        const int STATS_TEXT_MARGIN = 30;							// ��r�� ��Z
        statsBorder.SetTopLeft(STATS_POS_X, NOTE_TEXT_Y);
        statsBorder.ShowBitmap();									// ��ܲέp������
        PublicData::me.SetState(statsDisplayState + 3);

        if (statsDisplayState == 0) {	// ���� �̰��O��
            statsBg[0].SetTopLeft(STATS_POS_X, NOTE_TEXT_Y);
            statsBg[0].ShowBitmap();
            statsArrow[1].SetTopLeft((SIZE_X - statsArrow[0].Width()) / 2, NOTE_TEXT_Y + (statsBorder.Height() - statsArrow[0].Height()) / 2 + 4);
            statsArrow[1].ShowBitmap();

            //
            //�q CFile �����o�̰������e�����
            if (!PublicData::file.isHighScoreExist()) {		// �Y�̰������s�b
                PublicData::me.SetState(5);
                statsNoRecord.SetTopLeft((SIZE_X - statsNoRecord.Width()) / 2 + 115, NOTE_TEXT_Y + 150);
                statsNoRecord.ShowBitmap();
            }
            else {
                int tempScore = PublicData::file.ReadHighScore_Score(),
                    tempTotalKeyCount = PublicData::totalKeyCount,
                    tempLevel = PublicData::file.ReadHighScore_Level(),
                    tempCorrectKeyCount = PublicData::file.ReadHighScore_CorrectKeyCount(),
                    tempAccuracy = int(PublicData::file.ReadHighScore_Accuracy() * 100.0);
                PublicData::me.SetHighScoreDisplay(PublicData::file.ReadHighScore_Character());

                //
                for (int i = 0; i < 5; i++) {					// ��ܤ��ƼƦrbmp
                    numBmp[tempScore % 10].SetTopLeft(STATS_POS_X + 227 - 20 * i, NOTE_TEXT_Y + 196 );
                    numBmp[tempScore % 10].ShowBitmap();
                    tempScore /= 10;
                }

                for (int j = 0; j < 4; j++) {					// ��ܶ��ؤ�r�]�����B����B���T�^�μƦr
                    statsText[j].SetTopLeft(STATS_POS_X + STATS_TEXT_POS_X, \
                                            NOTE_TEXT_Y + STATS_TEXT_POS_Y + STATS_TEXT_MARGIN  * j);
                    statsText[j].ShowBitmap();					// ��ܶ��ؤ�r
                    const int  STATS_TEXT_MARGIN_R = 170;		// ���ؤ�r �P �Ʀr ���Z��

                    if (j == 0) {		// 0����� �֭p�`�����
                        for (int i = 0; i < 7; i++) {		// ����`����ƼƦrbmp
                            numBmpSmall[tempTotalKeyCount % 10].SetTopLeft(STATS_POS_X + STATS_TEXT_POS_X + STATS_TEXT_MARGIN_R - 10 * i, \
                                    NOTE_TEXT_Y + STATS_TEXT_POS_Y + STATS_TEXT_MARGIN * j + 1);
                            numBmpSmall[tempTotalKeyCount % 10].ShowBitmap();
                            tempTotalKeyCount /= 10;

                            if (tempTotalKeyCount == 0)break;
                        }
                    }

                    if (j == 1) {		// 1����� ���d�Ʀr
                        for (int i = 0; i < 2; i++) {		// ������d�Ʀrbmp
                            numBmpSmall_White[tempLevel % 10].SetTopLeft(STATS_POS_X + STATS_TEXT_POS_X + STATS_TEXT_MARGIN_R - 10 * i, \
                                    NOTE_TEXT_Y + STATS_TEXT_POS_Y + STATS_TEXT_MARGIN * j + 1);
                            numBmpSmall_White[tempLevel % 10].ShowBitmap();
                            tempLevel /= 10;
                        }
                    }
                    else if (j == 2) {	// 2����� �`�����
                        for (int i = 0; i < 5; i++) {		// ����`����ƼƦrbmp
                            numBmpSmall_White[tempCorrectKeyCount % 10].SetTopLeft(STATS_POS_X + STATS_TEXT_POS_X + STATS_TEXT_MARGIN_R - 10 * i, \
                                    NOTE_TEXT_Y + STATS_TEXT_POS_Y + STATS_TEXT_MARGIN * j + 1);
                            numBmpSmall_White[tempCorrectKeyCount % 10].ShowBitmap();
                            tempCorrectKeyCount /= 10;

                            if (tempCorrectKeyCount == 0)break;
                        }
                    }
                    else if (j == 3) {	// 3����� ���T�v
                        if (tempAccuracy != 10000) {						//�Y���T�v�D100%
                            for (int i = 0, dotPos = 0; i < 4; i++) {		// ��ܥ��T�vbmp
                                numBmpSmall_White[tempAccuracy % 10].SetTopLeft \
                                (STATS_POS_X + STATS_TEXT_POS_X + STATS_TEXT_MARGIN_R - 10 * i - dotPos, \
                                 NOTE_TEXT_Y + STATS_TEXT_POS_Y + STATS_TEXT_MARGIN * j + 1);
                                numBmpSmall_White[tempAccuracy % 10].ShowBitmap();
                                tempAccuracy /= 10;

                                if (i == 1) {								// ��ܤp���I
                                    dotPos = 5;
                                    numBmpSmall_White[11].SetTopLeft\
                                    (STATS_POS_X + STATS_TEXT_POS_X + STATS_TEXT_MARGIN_R - 10 * i - dotPos, \
                                     NOTE_TEXT_Y + STATS_TEXT_POS_Y + STATS_TEXT_MARGIN * j + 1);
                                    numBmpSmall_White[11].ShowBitmap();
                                }

                                tempLevel /= 10;
                            }
                        }
                        else {												// �w��100%���
                            numBmpSmall_White[1].SetTopLeft(STATS_POS_X + STATS_TEXT_POS_X + STATS_TEXT_MARGIN_R - 10 * 2, \
                                                            NOTE_TEXT_Y + STATS_TEXT_POS_Y + STATS_TEXT_MARGIN * j + 1);
                            numBmpSmall_White[1].ShowBitmap();
                            numBmpSmall_White[0].SetTopLeft(STATS_POS_X + STATS_TEXT_POS_X + STATS_TEXT_MARGIN_R - 10, \
                                                            NOTE_TEXT_Y + STATS_TEXT_POS_Y + STATS_TEXT_MARGIN * j + 1);
                            numBmpSmall_White[0].ShowBitmap();
                            numBmpSmall_White[0].SetTopLeft(STATS_POS_X + STATS_TEXT_POS_X + STATS_TEXT_MARGIN_R, \
                                                            NOTE_TEXT_Y + STATS_TEXT_POS_Y + STATS_TEXT_MARGIN * j + 1);
                            numBmpSmall_White[0].ShowBitmap();
                        }

                        numBmpSmall_White[10].SetTopLeft(STATS_POS_X + STATS_TEXT_POS_X + STATS_TEXT_MARGIN_R + 14, \
                                                         NOTE_TEXT_Y + STATS_TEXT_POS_Y + STATS_TEXT_MARGIN * j + 1);
                        numBmpSmall_White[10].ShowBitmap();					// ��ܦʤ���Ÿ�
                    }
                }
            }
        }
        else if (statsDisplayState == 1) {		// �k�� �C���O��
            statsBg[1].SetTopLeft(STATS_POS_X, NOTE_TEXT_Y);
            statsBg[1].ShowBitmap();
            statsArrow[2].SetTopLeft((SIZE_X - statsArrow[0].Width()) / 2, NOTE_TEXT_Y + (statsBorder.Height() - statsArrow[0].Height()) / 2 + 4);
            statsArrow[2].ShowBitmap();

            // �]�w��ܪ��b�Y�˦�
            if (PublicData::file.GetRecordNum() <= 3 && PublicData::file.GetRecordNum() >= 0) {
                statsArrowV[3].ShowBitmap();		// �b�Y�˦��G���t
            }
            else {
                if (statsPRItemNum == 0)
                    statsArrowV[2].ShowBitmap();	// �b�Y�˦��G�U�G
                else if (statsPRItemNum == PublicData::file.GetRecordNum() - 3)
                    statsArrowV[1].ShowBitmap();	// �b�Y�˦��G�U�G
                else
                    statsArrowV[0].ShowBitmap();	// �b�Y�˦��G���G
            }

            if (PublicData::file.GetRecordNum() == 0) {		// ��d�L�C���O����
                PublicData::me.SetState(5);
                statsNoRecord.SetTopLeft((SIZE_X - statsNoRecord.Width()) / 2, NOTE_TEXT_Y + 163);
                statsNoRecord.ShowBitmap();
            }

            PublicData::me.SetPlayingRecordDisplay	// ��ܸӵ�������ME(�Y���s�b�h�����)
            (statsPRItemNum >= PublicData::file.GetRecordNum() ? "" :
             PublicData::file.ReadRecord_Character(statsPRItemNum),
             statsPRItemNum + 1 >= PublicData::file.GetRecordNum() ? "" :
             PublicData::file.ReadRecord_Character(statsPRItemNum + 1),
             statsPRItemNum + 2 >= PublicData::file.GetRecordNum() ? "" :
             PublicData::file.ReadRecord_Character(statsPRItemNum + 2) );

            for (int j = 0; j < 3; j++) {
                if (statsPRItemNum + j >= PublicData::file.GetRecordNum())
                    break;

                int tempScore = PublicData::file.ReadRecord_Score(statsPRItemNum + j),
                    tempLevel = PublicData::file.ReadRecord_Level(statsPRItemNum + j),
                    tempCorrectKeyCount = PublicData::file.ReadRecord_CorrectKeyCount(statsPRItemNum + j),
                    tempAccuracy = int(PublicData::file.ReadRecord_Accuracy(statsPRItemNum + j) * 100.0);
                string tempDate = PublicData::file.ReadRecord_Date(statsPRItemNum + j);
                const int STATS_PR_NUM_POS_X = 135, STATS_PR_NUM_PER_POS_X = 380;
                const int LINE_MARGIN = 44;

                for (int i = 0, signNum = 0; i < 12; i++) {		// ��ܤ���ɶ�
                    numBmpSmall_White[tempDate[i] - '0'].SetTopLeft(STATS_POS_X + STATS_PR_NUM_POS_X - 40 + 10 * i + signNum * 8,
                            NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                    numBmpSmall_White[tempDate[i] - '0'].ShowBitmap();

                    if (i == 3) {
                        numBmpSmall_White[12].SetTopLeft(STATS_POS_X + STATS_PR_NUM_POS_X - 40 + 10 * i + 10 + signNum * 8,
                                                         NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                        numBmpSmall_White[12].ShowBitmap();
                        signNum++;
                    }

                    if (i == 5) {
                        numBmpSmall_White[12].SetTopLeft(STATS_POS_X + STATS_PR_NUM_POS_X - 40 + 10 * i + 10 + signNum * 8,
                                                         NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                        numBmpSmall_White[12].ShowBitmap();
                        signNum++;
                    }

                    if (i == 7)  signNum++;

                    if (i == 9) {	// �_��
                        numBmpSmall_White[13].SetTopLeft(STATS_POS_X + STATS_PR_NUM_POS_X - 40 + 10 * i + 12 + signNum * 8,
                                                         NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                        numBmpSmall_White[13].ShowBitmap();
                        signNum++;
                    }
                }

                for (int i = 0; i < 5; i++) {		// ��ܤ��ƼƦrbmp
                    numBmpSmall[tempScore % 10].SetTopLeft(STATS_POS_X + STATS_PR_NUM_POS_X + 170 - 10 * i,
                                                           NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                    numBmpSmall[tempScore % 10].ShowBitmap();
                    tempScore /= 10;
                }

                for (int i = 0; i < 2; i++) {		// ������d�Ʀrbmp
                    numBmpSmall_White[tempLevel % 10].SetTopLeft(STATS_POS_X + STATS_PR_NUM_POS_X + 222 - 10 * i, \
                            NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                    numBmpSmall_White[tempLevel % 10].ShowBitmap();
                    tempLevel /= 10;
                }

                for (int i = 0; i < 5; i++) {		// ����`�����bmp
                    numBmpSmall_White[tempCorrectKeyCount % 10].SetTopLeft(STATS_POS_X + STATS_PR_NUM_POS_X + 306 - 10 * i,
                            NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                    numBmpSmall_White[tempCorrectKeyCount % 10].ShowBitmap();
                    tempCorrectKeyCount /= 10;
                }

                if (tempAccuracy != 10000) {						//�Y���T�v�D100%
                    for (int i = 0, dotPos = 0; i < 4; i++) {		// ��ܥ��T�vbmp
                        numBmpSmall_White[tempAccuracy % 10].SetTopLeft \
                        (STATS_POS_X + STATS_PR_NUM_POS_X + STATS_PR_NUM_PER_POS_X - 10 * i - dotPos, \
                         NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                        numBmpSmall_White[tempAccuracy % 10].ShowBitmap();
                        tempAccuracy /= 10;

                        if (i == 1) {								// ��ܤp���I
                            dotPos = 5;
                            numBmpSmall_White[11].SetTopLeft\
                            (STATS_POS_X + STATS_PR_NUM_POS_X + STATS_PR_NUM_PER_POS_X - 10 * i - dotPos, \
                             NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                            numBmpSmall_White[11].ShowBitmap();
                        }

                        tempLevel /= 10;
                    }
                }
                else {												// �w��100%���
                    numBmpSmall_White[1].SetTopLeft(STATS_POS_X + STATS_PR_NUM_POS_X + STATS_PR_NUM_PER_POS_X - 10 * 2, \
                                                    NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                    numBmpSmall_White[1].ShowBitmap();
                    numBmpSmall_White[0].SetTopLeft(STATS_POS_X + STATS_PR_NUM_POS_X + STATS_PR_NUM_PER_POS_X - 10, \
                                                    NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                    numBmpSmall_White[0].ShowBitmap();
                    numBmpSmall_White[0].SetTopLeft(STATS_POS_X + STATS_PR_NUM_POS_X + STATS_PR_NUM_PER_POS_X, \
                                                    NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                    numBmpSmall_White[0].ShowBitmap();
                }

                numBmpSmall_White[10].SetTopLeft(STATS_POS_X + STATS_PR_NUM_POS_X + STATS_PR_NUM_PER_POS_X + 14, \
                                                 NOTE_TEXT_Y + 130 + j * LINE_MARGIN);
                numBmpSmall_White[10].ShowBitmap();			// ��ܦʤ���Ÿ�
            }
        }

        PublicData::me.OnShow();									// ��ܲέp��������]�̰����M�C���O���^
    }
    else if (displayState == 4) {      // ������󭶭�
        if (aboutDisplayState == 0) {
            aboutBorder.SetTopLeft((SIZE_X - aboutBorder.Width()) / 2, NOTE_TEXT_Y);
            aboutBorder.ShowBitmap(); // ��������
            about.SetTopLeft((SIZE_X - aboutBorder.Width()) / 2, NOTE_TEXT_Y + 11);
            about.ShowBitmap();		  // ��������r

            if (PublicData::musicOnOff) {
                musicOnOff[0].SetTopLeft((SIZE_X - aboutBorder.Width()) / 2 + 458, NOTE_TEXT_Y + 180);
                musicOnOff[0].ShowBitmap();
            }
            else {
                musicOnOff[1].SetTopLeft((SIZE_X - aboutBorder.Width()) / 2 + 458, NOTE_TEXT_Y + 180);
                musicOnOff[1].ShowBitmap();
            }
        }
        else if (aboutDisplayState == 1) {	// �R���C��������
            delText.SetTopLeft((SIZE_X - delText.Width()) / 2, (NOTE_TEXT_Y + delText.Height() / 2) );
            delText.ShowBitmap();
        }
    }

    text1.SetTopLeft((SIZE_X - text1.Width()) / 2, text1_y);
    text1.ShowBitmap();

    if (0) {		// �i����ܼƦr�Φr��
        for (int i = 0; i < 13; i++) {
            numBmpSmall[i].SetTopLeft(10 + i * 10, 300);
            numBmpSmall[i].ShowBitmap();
            numBmpSmall_White[i].SetTopLeft(10 + i * 10, 310);
            numBmpSmall_White[i].ShowBitmap();

            if (i < 10) {
                numBmp[i].SetTopLeft(10 + i * 20, 320);
                numBmp[i].ShowBitmap();
                numBmp_White[i].SetTopLeft(10 + i * 20, 340);
                numBmp_White[i].ShowBitmap();
            }
        }
    }
}
/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����������A(Game Over)
/////////////////////////////////////////////////////////////////////////////
CGameStateOver::CGameStateOver(CGame* g)
    : CGameState(g) {
}
void CGameStateOver::OnMove() {
    if (isHighScore)	newHS_text.OnMove();	// ���ʯ}�����ʵe

    //if (isUnlock)newChar_text.OnMove();

    if (counter < 0)	GotoGameState(GAME_STATE_INIT);

    if (barCounter < 200 && barCounter < accuracy) {
        barCounter +=  2;
    }
}
void CGameStateOver::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
    const char KEY_ENTER = 0xD;
    (nChar == KEY_ENTER) ? GotoGameState(GAME_STATE_INIT) : 0;	// ���UEnter���^�}�Y����
}
void CGameStateOver::OnBeginState() {
    counter = 1000 * 30;
    barCounter = 0;
    isHighScore = isUnlock = false;
    //
    score = PublicData::score;
    level = PublicData::level;
    accuracy = PublicData::accuracy;
    //
    PublicData::me.SetState(2);
    PublicData::file.ReadHighScoreFile();

    if (PublicData::file.GetRecordNum() == 0)					// �Y�������C���O��
        PublicData::totalKeyCount = PublicData::CorrectKeyCount;
    else {
        PublicData::totalKeyCount = PublicData::file.ReadTotalKeyCount();
        PublicData::totalKeyCount += PublicData::CorrectKeyCount;
    }

    if (score > PublicData::file.ReadHighScore_Score()) {		// �Y�������Ƥj�� �̰����h�g�J
        PublicData::file.WriteHighScore(score, level, accuracy, PublicData::me.GetMeName(), PublicData::CorrectKeyCount);
        isHighScore = true;
    }

    PublicData::file.WriteRecord(score, level, accuracy, PublicData::me.GetMeName(), PublicData::CorrectKeyCount);	// �g�J �浧�C���O��
    PublicData::file.WriteTotalKeyCount(PublicData::totalKeyCount);		// �g�J �`�����
    PublicData::file.ReadRecordFile();									// Ū �C���O��

    //
    if (PublicData::me.JudgeUnlock(PublicData::totalKeyCount, score, int(accuracy), level)) {// �P�_�O�_�F������n��
        isUnlock = PublicData::newUnlock = true;
    }

    if (PublicData::musicOnOff) {
        CAudio::Instance()->Stop(AUDIO_ROCK);						// �Ȱ� �I������
        CAudio::Instance()->Play(AUDIO_GAMEOVER, false);			// ���� GAMEOVER����
    }
}
void CGameStateOver::OnInit() {
    ShowInitProgress(66);	// ���ӫe�@�Ӫ��A���i�סA���B�i�׵���66%
    border.LoadBitmap("Bitmaps/gameover/gameover_border.bmp", RGB(0, 255, 0));
    newHS_text.AddBitmap("Bitmaps/gameover/gameover_new_hs1.bmp", RGB(0, 255, 0));
    newHS_text.AddBitmap("Bitmaps/gameover/gameover_new_hs2.bmp", RGB(0, 255, 0));
    newHS_text.SetDelayCount(20);
    newChar_text.AddBitmap("Bitmaps/gameover/gameover_new_char1.bmp", RGB(0, 255, 0));
    newChar_text.AddBitmap("Bitmaps/gameover/gameover_new_char2.bmp", RGB(0, 255, 0));
    newChar_text.SetDelayCount(20);
    ShowInitProgress(80);
    isHighScore = isUnlock = 0;

    for (int i = 0; i < 10; i++) {		// ���J�Ʀr��
        char str[80];
        sprintf(str, "Bitmaps/level/num/%d.bmp", i);
        numBmp[i].LoadBitmap(str, RGB(0, 255, 0));
        sprintf(str, "Bitmaps/level/num_s/%d.bmp", i);
        numBmpSmall[i].LoadBitmap(str, RGB(0, 255, 0));
    }

    bar[0].LoadBitmap("Bitmaps/gameover/bar_0.bmp", RGB(0, 255, 0));
    bar[1].LoadBitmap("Bitmaps/gameover/bar_1.bmp", RGB(0, 255, 0));
    numBmpSmall[10].LoadBitmap("Bitmaps/level/num_s/per.bmp", RGB(0, 255, 0));	// ���J�ʤ���Ϥ�
    numBmpSmall[11].LoadBitmap("Bitmaps/level/num_s/dot.bmp", RGB(0, 255, 0));	// ���J�p���I�Ϥ�
    x = (SIZE_X - border.Width()) / 2;
    y = (SIZE_Y - border.Height()) / 2;
    ShowInitProgress(100);

    if (PublicData::musicOnOff)
        CAudio::Instance()->Load(AUDIO_GAMEOVER, "sounds\\gameover.mp3");
}
void CGameStateOver::OnShow() {		// GAMEOVER �e�����
    border.SetTopLeft(x, y);
    border.ShowBitmap();

    if (isHighScore) {	// ��ܯ}�����ʵe
        newHS_text.SetTopLeft((SIZE_X - newHS_text.Width()) / 2, y + border.Height() + 20);
        newHS_text.OnShow();
    }

    if (isUnlock) {
        newChar_text.SetTopLeft((SIZE_X - newHS_text.Width()) / 2, y + border.Height() + 20 + 35 * isHighScore);
        newChar_text.OnShow();
    }

    int tempScore = score, tempLevel = level, tempAccuracy = int (accuracy * 100.0);
    int dotPos = 0;
    int scorePosX = 230, levelPosX = 130, accPosX = 155, // �w�q�U�Ʀr��m�����q
        scorePosY =  45, levelPosY =  90, accPosY = 108,
        barPosX	  = 120, barPosY   = 122;

    for (int i = 0; i < 5; i++) {					// ��ܤ��ƼƦrbmp
        numBmp[tempScore % 10].SetTopLeft(x + scorePosX - 20 * i, y + scorePosY);
        numBmp[tempScore % 10].ShowBitmap();
        tempScore /= 10;
    }

    for (int i = 0; i < 2; i++) {					// ������d�Ʀrbmp
        numBmpSmall[tempLevel % 10].SetTopLeft(x + levelPosX - 10 * i, y + levelPosY);
        numBmpSmall[tempLevel % 10].ShowBitmap();
        tempLevel /= 10;
    }

    if (accuracy != 100) {								//�Y���T�v�D100%
        for (int i = 0, dotPos = 0; i < 4; i++) {		// ��ܥ��T�vbmp
            numBmpSmall[tempAccuracy % 10].SetTopLeft(x + accPosX - 10 * i - dotPos, y + accPosY);
            numBmpSmall[tempAccuracy % 10].ShowBitmap();
            tempAccuracy /= 10;

            if (i == 1) {								// ��ܤp���I
                dotPos = 5;
                numBmpSmall[11].SetTopLeft(x + accPosX - 10 * i - dotPos, y + accPosY);
                numBmpSmall[11].ShowBitmap();
            }

            tempLevel /= 10;
        }

        numBmpSmall[10].SetTopLeft(x + accPosX + 14, y + accPosY);
        numBmpSmall[10].ShowBitmap();					// ��ܦʤ���Ÿ�
    }
    else {												// �w��100%���
        numBmpSmall[1].SetTopLeft(x + accPosX - 35, y + accPosY);
        numBmpSmall[1].ShowBitmap();
        numBmpSmall[0].SetTopLeft(x + accPosX - 35 + 10, y + accPosY);
        numBmpSmall[0].ShowBitmap();
        numBmpSmall[0].SetTopLeft(x + accPosX - 35 + 10 * 2, y + accPosY);
        numBmpSmall[0].ShowBitmap();
        numBmpSmall[10].SetTopLeft(x + accPosX - 35 + 10 * 4, y + accPosY);
        numBmpSmall[10].ShowBitmap();					// ��ܦʤ���Ÿ�
    }

    for (int i = 0; i < 100; i++) {						// ��ܦʤ��񪺶i�ױ�
        bar[0].SetTopLeft(x + barPosX + i, y + barPosY);
        bar[0].ShowBitmap();

        if (i < barCounter) {
            bar[1].SetTopLeft(x + barPosX + i, y + barPosY);
            bar[1].ShowBitmap();
        }
    }

    PublicData::me.OnShow();
}
/////////////////////////////////////////////////////////////////////////////
// �o��class���C�����C�����檫��A�D�n���C���{�����b�o��
/////////////////////////////////////////////////////////////////////////////
CGameStateRun::CGameStateRun(CGame* g)
    : CGameState(g), LEVEL(30) {
    srand((unsigned)time(NULL));	// �üƺؤl
    callEnemyCounter = maxCallEnemyCounter = 20;	// maxCallEnemyCounter �M�w�Ǫ��ͦ��t��
    callBossACounter = maxCallBossACounter = 65;
    callBossBCounter = maxCallBossBCounter = 75;
}
CGameStateRun::~CGameStateRun() {
    for (CEnemy* eq : enemyQueue) delete eq;

    for (CBullet* bl : bulletList) delete bl;

    for (CBomb* cb : bombList) delete cb;
}
void CGameStateRun::OnBeginState() {
    const int SCORE_X = 240, SCORE_Y = 240;

    if (PublicData::musicOnOff)
        CAudio::Instance()->Play(AUDIO_ROCK, true);			// ���� MIDI

    score.SetInteger(0);								// �]�wSCORE��0
    score.SetTopLeft(SCORE_X, SCORE_Y);
    currEnemyNum = currBossANum = currBossBNum = 0;		// ��l�ƦU���ĤH�w�l��ƶq
    lock = false;										// ������w
    targetEnemy = NULL;
    currLevel = 0;										// ��l�����d
    enemyQueue.clear();									// �M��EQ
    lives = 3;
    totalKeyDownCount = PublicData::CorrectKeyCount = 0;
    accuracy = 0;
    emp.SetEQ(&enemyQueue, &score, &lock, &targetEnemy);
    emp.SetEmpTimes(3);
    PublicData::me.SetState(0);
    totalEnemyNum = 0;
    levelChangeFlag = 0;
    levelChangeDelay = -1;
    levelChangeDelayMax = int( 3.5 * 30 );						// �]�w���d��delay 3��
    pause = false;
}
void CGameStateRun::OnInit() {								// �C������Ȥιϧγ]�w
    srand((unsigned)time(NULL));
    ShowInitProgress(33);
    //
    // �~����J��L���
    //
    score.LoadBitmap();		// ���J������ܾ�
    map.LoadBitmap();		// ���J�I��
    emp.LoadBitmap();		// ���JEMP
    levelAni.LoadBitmap();	// ���J�������d�L���ʵe
    pauseText.LoadBitmap("Bitmaps/menu/pause.bmp", RGB(0, 255, 0));
    debugText.LoadBitmap("Bitmaps/debug_text.bmp", RGB(0, 255, 0));

    if (1) {
        CAudio::Instance()->Load(AUDIO_ROCK, "sounds\\The_Coming_Storm.mp3");
        CAudio::Instance()->Load(AUDIO_SHOT, "sounds\\shot.mp3");
        CAudio::Instance()->Load(AUDIO_ERROR, "sounds\\error.mp3");
        CAudio::Instance()->Load(AUDIO_CONGRATULATION, "sounds\\congratulation.mp3");
        CAudio::Instance()->Load(AUDIO_CONGRATULATION2, "sounds\\congratulation2.mp3");
    }

    ShowInitProgress(40);

    for (int i = 0; i < 26; i++) {		// ���J�r���Ϥ�
        char str[50];
        sprintf(str, "Bitmaps/char4/%c.bmp", i + 97);
        letter.push_back(new CMovingBitmap);
        letter.back()->LoadBitmap(str, RGB(255, 255, 255));
    }

    for (int i = 0; i < 22; i++) {		// ���J��w�ĤH�˷ǰʵe
        char str[50];
        sprintf(str, "Bitmaps/target/target_s%d.bmp", i + 1);
        target.AddBitmap(str, RGB(0, 255, 0));
    }

    target.SetDelayCount(2);
    ShowInitProgress(50);
}
void CGameStateRun::OnMove() {			// ���ʹC������
    if (pause)return;

    if (levelChangeDelay >= 0)	levelChangeDelay--;		// ���d�P���d�����𪺭p�ƾ�

    accuracy = (totalKeyDownCount != 0) ? \
               100 * double(PublicData::CorrectKeyCount) / double(totalKeyDownCount) : \
               100;     // �p�⥿�T�v
    PublicData::score = score.GetInteger();
    PublicData::level = currLevel;
    PublicData::accuracy = accuracy;
    callEnemyCounter--;	// �C���Ǫ� �ͦ����j �� �p�ƾ�
    callBossACounter--;
    callBossBCounter--;

    if (quickCall) {	//�iDEBUG�ϡj �N��0���]�w�ͦ�200�u�Ǫ��A�B�l��delay��0��
        //levelEnemyNum[0] = 200;
        levelChangeDelayMax = 0;
        maxCallEnemyCounter = maxCallBossACounter  = maxCallBossBCounter = 0;
    }
    else {
        levelChangeDelayMax = int(3.5 * 30);
        maxCallEnemyCounter = 25;
        maxCallBossACounter = 75;
        maxCallBossBCounter = 90;
    }

    //==�p��==================================
    if (callEnemyCounter < 0 && currEnemyNum < levelEnemyNum[currLevel]) {	// counter �ƨ�0��N�}�l�l��s��
        callEnemyCounter = maxCallEnemyCounter;				// ��counter �զ^max�~���
        int randX = (rand() % (SIZE_X - 100)) ;				// SIZE_X - 100 ���F�����Ǫ�����r�W�X�ù��Ӧh
        enemyQueue.push_back(new CEnemy(randX, 0, 3, true, &dictionary, 2, 5, &enemyQueue, &bombList, \
                                        PublicData::me.GetX1() - 30 + rand() % 60, PublicData::me.GetY1(), &letter) ); // �N�l�ꪺ�s�ǩ�Jvecotr��
        enemyQueue.back()->LoadBitmap();	// ���J�l�ꪺ�s��
        currEnemyNum++;						// �b�����d�w�l�ꪺ�Ǫ��p�ƾ�
        totalEnemyNum++;					// �`�H�l�ꪺ�Ǫ��ƶq
    }

    //==BossA==================================
    if (callBossACounter < 0 && currBossANum < levelBossANum[currLevel]) {
        callBossACounter = maxCallBossACounter;
        int randX = (rand() % (SIZE_X - 350) + 200);
        enemyQueue.push_back(new CBossA(randX, 0, 5, true, &dictionary, 6, 12, &enemyQueue, &bombList, &letter));
        enemyQueue.back()->LoadBitmap();
        currBossANum++;
        totalEnemyNum++;
    }

    //==BossB==================================
    if (callBossBCounter < 0 && currBossBNum < levelBossBNum[currLevel]) {
        callBossBCounter = maxCallBossBCounter;
        int randX = (rand() % (SIZE_X - 350) + 200);
        enemyQueue.push_back(new CBossB(randX, 0, 5, true, &dictionary, 6, 12, &enemyQueue, &bombList, &letter));
        enemyQueue.back()->LoadBitmap();
        currBossBNum++;
        totalEnemyNum++;
    }

    //===�P�_Me�O�_�I��Enemy===

    for (CEnemy* eq : enemyQueue) {
        if (eq->IsAlive() && eq->HitMe(&PublicData::me)) {
            lives--;
            (lives <= 0) ? GotoGameState(GAME_STATE_OVER) : 0;	// �Y�ͩR�Ȭ�0�hGOTO�C��������STATE
        }

        if (eq->GetX() > SIZE_X + 40 || eq->GetX() < -40 || eq->GetY() > SIZE_Y + 40) {
            eq->kill();	// ��enemy���X�e���~�ɱ����Ǫ�
        }
    }

    // ===Enemy===
    bool enemyAllDead = true;

    for (unsigned int i = 0; i < enemyQueue.size(); i++) { // ����VECTOR�����Ҧ��Ǫ�
        enemyQueue[i]->OnMove();
        enemyQueue[i]->IsAlive() ? enemyAllDead = false : 0;
    }

    for (unsigned int i = 0; i < enemyQueue.size(); i++) {
        //�YEnemy IsAlive=0, �h�qvector������
        if (!enemyQueue[i]->IsAlive() && enemyQueue[i]->IsBombed()) {
            vector<CEnemy*>::iterator iterenemyQueue = enemyQueue.begin();
            delete enemyQueue[i];
            enemyQueue[i] = NULL;
            enemyQueue.erase(iterenemyQueue + i);
            i = 0;
        }
    }

    ////===bullet===
    bool bulletAllDead = true;

    for (CBullet* bl : bulletList) {
        bl->OnMove();	// ����BULLET

        if (bl->IsAlive()) bulletAllDead = false;
    }

    if (bulletAllDead) {	// �YVECTOR�����l�u�Ҽ��񧹲��A����O����òM��VECOTR
        for (CBullet* bl : bulletList) delete bl;

        bulletList.clear();
    }

    ////===bomb===
    bool bombAllDead = true;

    for (CBomb* cb : bombList) {
        cb->OnMove();	// ����BOMB

        if (cb->IsAlive()) bombAllDead = false;
    }

    if (bombAllDead) {
        for (CBomb* cb : bombList) {	// �YVECTOR�����z���Ҽ��񧹲��A����O����òM��VECOTR
            delete cb;
            cb = NULL;
        }

        bombList.clear();
    }

    ////===Change Level===
    if (currEnemyNum >= levelEnemyNum[currLevel] && currBossANum >= levelBossANum[currLevel] \
            && currBossBNum >= levelBossBNum[currLevel] && enemyQueue.size() == 0) {
        // �� ���d
        if (!levelChangeFlag) {											// ����delay�⧹
            levelAni.Play(currLevel, score.GetInteger());				// ����������d�ʵe
            levelChangeFlag = true;
            levelChangeDelay = levelChangeDelayMax;						// �N�p�ƾ��զ^

            if (PublicData::musicOnOff) {
                //CAudio::Instance()->Stop(AUDIO_ROCK);						// �Ȱ� �I������
                //CAudio::Instance()->Play(AUDIO_CONGRATULATION, false);		// ���� �L������
                //CAudio::Instance()->Play(AUDIO_CONGRATULATION2, false);		// ���� �L������2
            }
        }

        if (levelChangeDelay < 0 && levelChangeFlag) {					// ��delay�⧹�� �A��ڤ������d
            if (PublicData::musicOnOff) {
                //CAudio::Instance()->Stop(AUDIO_CONGRATULATION);				//�Ȱ� �L������
                //CAudio::Instance()->Stop(AUDIO_CONGRATULATION2);			//�Ȱ� �L������2
                //CAudio::Instance()->Play(AUDIO_ROCK, true);					// ���� �I������
            }

            currLevel++;

            if (currLevel > LEVEL)GotoGameState(GAME_STATE_OVER);		// �Y��e���d�j��̤j���d�hGOTO�C��������STATE

            currEnemyNum = currBossANum = currBossBNum = 0;				// ���m�����d�w�l�ꪺ�Ǫ��ƶq
            callEnemyCounter = maxCallEnemyCounter;						// �զ^�l��Ǫ����p�ƾ�
            callBossACounter = maxCallBossACounter;
            callBossBCounter = maxCallBossBCounter;
            levelChangeFlag = false;
        }
    }

    //
    map.OnMove();				// ���ʭI��
    emp.OnMove();				// ����EMP
    PublicData::me.OnMove();	// ���ʥD��
    levelAni.OnMove();			// �������d�����ʵe

    if (lock && targetEnemy != NULL) {
        target.OnMove();		// ������w�ؼаʵe
    }
}
void CGameStateRun::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
    const char KEY_LEFT = 0x25;	// keyboard���b�Y
    const char KEY_UP = 0x26;	// keyboard�W�b�Y
    const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
    const char KEY_DOWN = 0x28; // keyboard�U�b�Y

    if (pause) return;

    if (key != nChar) {
        key = nChar;	// �ܼ�key���x�s���U������A�������u��

        for (int unsigned i = 0; i < enemyQueue.size(); i++) {			// �]�ثe���d�Ǫ����ƶq
            if (enemyQueue[i]->IsAlive()) {								// �^�Ƿ�e�Ǫ��O�_�s�b
                if (!lock) {											// �|����w
                    if (nChar + 32 == enemyQueue[i]->GetFirstWord()) {	// �Y����Ĥ@�Ӧr��:��� and �ثe�r����m+1
                        PublicData::CorrectKeyCount++;							// ���T�����+1
                        map.PlayFlash();

                        if (PublicData::musicOnOff)
                            CAudio::Instance()->Play(AUDIO_SHOT, false);	// ���� �g������

                        if (enemyQueue[i]->GetVocabLeng() == 1) {		// A. �w��1�r�p�ǧ���
                            targetEnemy = enemyQueue[i];				// targetEnemy������->���b�������ĤH
                            bulletList.push_back(new CBullet(targetEnemy->GetX() + 10, targetEnemy->GetY() + 10));	// �g�l�u
                            targetEnemy->kill();						// ���\���`�Ǫ�
                            score.Add(int(targetEnemy->GetVocabLeng() * (1 + accuracy / 100)));		// ����+= �Ǫ�����
                            targetEnemy = NULL;
                            break;
                        }
                        else {											// B. �w��@�몺�Ǫ�����
                            lock = true;								// �w��w�Y�u�Ǫ�
                            targetEnemy = enemyQueue[i];				// targetEnemy������->���b�������ĤH
                            targetEnemy->AddCurrWordLeng();				// �w��J�쪺��l��++
                            bulletList.push_back(new CBullet(targetEnemy->GetX() + 10, targetEnemy->GetY() + 10));	// �g�l�u
                            targetEnemy->MinusIndex(2);					// ���h�Ǫ�
                            break;
                        }
                    }
                    else {
                        if (nChar >= 65 && nChar <= 90)
                            if (PublicData::musicOnOff)
                                CAudio::Instance()->Play(AUDIO_ERROR, false);	// ���� �g������
                    }
                }
                else {													// �Y�w��w
                    if (nChar + 32 == targetEnemy->GetVocab()[targetEnemy->GetCurrWordLeng()]) { 	// �Y�����e�r��
                        targetEnemy->AddCurrWordLeng();
                        bulletList.push_back(new CBullet(targetEnemy->GetX(), targetEnemy->GetY()));
                        targetEnemy->MinusIndex(rand() % 2 + 1);		// ���h�Ǫ�
                        PublicData::CorrectKeyCount++;							// ���T�����+1
                        map.PlayFlash();

                        if (PublicData::musicOnOff)
                            CAudio::Instance()->Play(AUDIO_SHOT, false);	// ���� �g������

                        if (targetEnemy->GetCurrWordLeng() == targetEnemy->GetVocabLeng()) {	 // �Y��e���� ���� �r��������
                            targetEnemy->kill();						// ���\���`�Ǫ�
                            score.Add(int(targetEnemy->GetVocabLeng() * (1 + accuracy / 100)));		// ����+= �Ǫ�����
                            targetEnemy = NULL;							// �]�Ǫ��w�Q���`�A�NtargetEnemy���Ы��VNULL
                            lock = false;								// ������w�Ǫ�
                        }

                        break;
                    }
                    else {
                        if (nChar >= 65 && nChar <= 90)
                            if (PublicData::musicOnOff)
                                CAudio::Instance()->Play(AUDIO_ERROR, false);	// ���� �g������
                    }
                }
            }
        }

        if (nChar >= 65 && nChar <= 90) totalKeyDownCount++;			// �`�����++

        if (nChar == 13) {// �Y���UENTER�h�o��EMP����.
            emp.CallEmp(PublicData::musicOnOff);
        }
    }
}
void CGameStateRun::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
    const char KEY_LEFT = 0x25; // keyboard���b�Y
    const char KEY_UP = 0x26; // keyboard�W�b�Y
    const char KEY_RIGHT = 0x27; // keyboard�k�b�Y
    const char KEY_DOWN = 0x28; // keyboard�U�b�Y
    const char KEY_ESC = 27;
    key = NULL;

    if (nChar == KEY_ESC) pause = pause ? false : true;

    if (nChar == 'B' && pause) {
        CAudio::Instance()->Pause();
        GotoGameState(GAME_STATE_INIT);
    }

    if (PublicData::debugMode) {	// ���\�ϥ�DEBUG����
        if (nChar == '1')showDebug = showDebug ? false : true;  // ��1 �}��debug

        if (nChar == '2' && enemyQueue.size() > 0) {			// ��2 �M��EQ�̫�@���ĤH
            enemyQueue.back()->kill();
            lock = 0;
        }

        if (nChar == '3' && enemyQueue.size() > 0) {			// ��3 �M��EQ���Ҧ��ĤH
            for (CEnemy* ce : enemyQueue) ce->kill();

            lock = 0;
        }

        if (nChar == '4')GotoGameState(GAME_STATE_INIT);		// ��4 GOTO �_�l�e��

        if (nChar == '5')GotoGameState(GAME_STATE_OVER);		// ��5 GOTO �C�������e��

        if (nChar == '6')quickCall = quickCall ? false : true;	// ��6 �}���ֳt�l��

        if (nChar == '7')score.Add(100);						// ��7 �[�ʤ�
    }
}
void CGameStateRun::OnLButtonDown(UINT nFlags, CPoint point) {}
void CGameStateRun::OnLButtonUp(UINT nFlags, CPoint point) {}
void CGameStateRun::OnMouseMove(UINT nFlags, CPoint point) {}
void CGameStateRun::OnRButtonDown(UINT nFlags, CPoint point) {}
void CGameStateRun::OnRButtonUp(UINT nFlags, CPoint point) {}
void CGameStateRun::OnShow() {
    map.OnShow();						// ��ܭI��
    score.ShowBitmap();					// ��ܤ���
    emp.OnShow();						// ���EMP�]�q�Ϫi�^
    PublicData::me.OnShow();			// ��ܥD��
    levelAni.OnShow();					// ������d�����ʵe

    for (CBomb* cb : bombList)	cb->OnShow();		//���VECTOR���Ҧ��� �z��

    for (CBullet* bl : bulletList)	bl->OnShow();	//���VECTOR���Ҧ��� �l�u

    for (CEnemy* eq : enemyQueue)	eq->OnShow();	//���VECTOR���Ҧ��� �Ǫ�

    if (lock && targetEnemy != NULL) {
        targetEnemy->OnShow();						// �[�W�o�@�� ���Q��w���Ǫ��A�����, �H���Q��L�Ǫ��\��

        if (targetEnemy->GetBossType() == "enemy")
            target.SetTopLeft(targetEnemy->GetX() - 2, targetEnemy->GetY() - 2);	// �]�w���q�Ǫ� �˷ǰʵe����m
        else
            target.SetTopLeft(targetEnemy->GetX() + 8, targetEnemy->GetY() + 8);	// �]�wBOSS �˷ǰʵe����m

        target.OnShow();															// ��ܺ˷ǰʵe
    }

    if (PublicData::debugMode) {													// ���debug�ާ@����
        debugText.SetTopLeft(SIZE_X - debugText.Width() - 20, 20);
        debugText.ShowBitmap();
    }

    if (pause) {																	// ��ܼȰ�����
        pauseText.SetTopLeft((SIZE_X - pauseText.Width()) / 2, (SIZE_Y - pauseText.Height() ) / 2);
        pauseText.ShowBitmap();
    }

    if (showDebug) {		// ���debug��T
        CDC* pDC = CDDraw::GetBackCDC();
        CFont f, *fp;
        f.CreatePointFont(100, "Fixedsys");
        fp = pDC->SelectObject(&f);
        pDC->SetBkColor(RGB(0, 0, 0));
        //
        char temp[100];
        sprintf(temp, "TotalKeyNum: %d TotalCorrectKeyNum: %d , Accuracy: %.2lf%%", \
                totalKeyDownCount, PublicData::CorrectKeyCount, accuracy);
        pDC->SetTextColor(RGB(50, 200, 200));
        pDC->TextOut(20, 20, temp);
        //
        char temp1[100];
        sprintf(temp1, "EQ size: %d,  Live: %d,  Level: %d,  LOCK: %d,  TotalEnemyNum: %d", \
                enemyQueue.size(), lives, currLevel, bool(lock), totalEnemyNum);
        pDC->SetTextColor(RGB(255, 255, 255));
        pDC->TextOut(20, 40, temp1);

        //

        if (1) {
            for (unsigned int i = 0; i < enemyQueue.size(); i++) {	// ��ܳ��W�Ǫ��� ��r,curr/length
                char temp[50];
                sprintf(temp, "%s %d/%d (x:%d,y:%d)", enemyQueue[i]->GetVocab().c_str(), enemyQueue[i]->GetCurrWordLeng(), enemyQueue[i]->GetVocabLeng(), enemyQueue[i]->GetX(), enemyQueue[i]->GetY());
                pDC->SetTextColor(RGB(180 + i, 180 + i, 180 + i));
                pDC->TextOut(20, i * 14 + 60, temp);
            }
        }

        ////
        pDC->SelectObject(fp);						// �� font f (�d�U���n�|�F��)
        CDDraw::ReleaseBackCDC();					// �� Back Plain �� CDC
    }
}
}
