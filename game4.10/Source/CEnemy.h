namespace game_framework {

class CEnemy {
    public:
        CEnemy();
        //CEnemy(int x, int y, int delay, bool alive, CDict* dict);
        CEnemy(int x, int y, int delay, bool alive, CDict* dict, int minVL, int maxVL);
        bool HitEraser(CEraser* eraser);						// �O�_�I�����l
        bool IsAlive();											// �O�_����
        virtual void LoadBitmap();								// ���J�ϧ�
        virtual void OnMove();									// ����
        void OnShow();											// �N�ϧζK��e��
        void SetXY(int nx, int ny);								// �]�w��ߪ��y��
        void SetIsAlive(bool alive);							// �]�w�O�_����
        void SetDelay(int d);									// �]�w���઺�t��

        /////////////

        void SetVocab();										// �H���qdict������@�ӳ�r��vocab�̭�
        string GetVocab();										// �^�Ǿ�ճ�r(ex: "apple")
        char GetFirstWord();									// �Hchar�^�Ǥ@�Ӧr (ex: 'a')
        void AddCurrWordLeng();									// CurrWord++
        int GetCurrWordLeng();									// �^��int���A��CurrWord
        int GetVocabLeng();
        int GetX(), GetY();
        void MinusIndex(int num);								//	���h�Ǫ� num�����h�h��index
        bool HitMe(CMe* me);

    protected:
        CMovingBitmap bmp;			// �y����
        CMovingBitmap bmp_center;	// ��ߪ���
        CMovingBitmap textCursor;	// ��r���� ��
        //CMovingBitmap talkBox;		// ��ܮ� ��	(�ª�)
        CMovingBitmap talkBoxL, talkBoxC, talkBoxR;		// �u�ƹL����ܮ� ��
        vector<CMovingBitmap*> letter;
        //          <  =  = ... =  >
        // talkBox: L  C  C ... C  R
        // L(Left), C(Center), R(Right)
        // �̷ӳ�r���� �ͦ����P�ƶq�� talkBoxC
        //
        CAnimation		target;		// ��w���ʵe
        int x, y;					// ��ߪ��y��
        int dx, dy;					// �y�Z����ߪ��첾�q
        int index;					// �y���u���סv�A0-17���0-360��
        int delay_counter;			// �վ����t�ת��p�ƾ�
        int delay;					// ���઺�t��
        bool is_alive;				// �O�_����
        //int xMoveDistance, yMoveDistance;
        /////////
        string vocab;				// �x�s��r
        int length;					// ��r�`����
        int currWordLeng;			// ��e��J��r���r��
        CDict* dict;
        int minVocabLeng, maxVocabLeng;		// �W�w�Ǫ��ͦ���r���ת��϶�( minVocabLeng ~ maxVocabLeng)
        int targetX, targetY;

    private:

        bool HitRectangle(int tx1, int ty1, int tx2, int ty2);	// �O�_�I��Ѽƽd�򪺯x��
};
}