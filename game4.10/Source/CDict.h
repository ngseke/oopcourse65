namespace game_framework {

class CDict {
    public:
        CDict();
        string GetText();
    private:
        string temp;
        vector<string> dictionary;
};
}