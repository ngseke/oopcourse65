#pragma once
namespace game_framework {
// CDict:	�޲z�r����
class CDict {
    public:
        CDict();
        string GetText();
    private:
        vector<string> dictionary;
};
}
