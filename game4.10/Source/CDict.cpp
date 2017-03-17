#include "stdafx.h"
#include "Resource.h"
#include <mmsystem.h>
#include <ddraw.h>
#include "audio.h"
#include "gamelib.h"
#include "CEraser.h"
#include "CBall.h"
#include "CDict.h"
#include <fstream>
#include <string>
#include <vector>
namespace game_framework {

	void CDict::GetText() {
		fstream file;
		file.open("text.txt", ios::in);
		string text;
		vector<string> temp;
		if (!file) {}
		else {
			while (file >> text) {
				temp.push_back(text);
			}
		}
	}


}
