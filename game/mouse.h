#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "DxLib.h"


class Mouse
{
public:



	Mouse();				// �R���X�g���N�^.
	~Mouse();				// �f�X�g���N�^.
	void Update();			// �X�V.
	void Draw();			// �`��.


	// �|�W�V������getter/setter.
	void SetPosX(const int setX) { posX = setX; }
	void SetPosY(const int setY) { posY = setY; }

	//�}�E�X���N���b�N�����ꏊ���擾����
	int GetMouseClickPosX() { return posX; }
	int GetMouseClickPosY() { return posY; }

	static int GetMouseClickInputKye();


private:

	int posX, posY;			//�}�E�X�̃J�[�\���̃|�W�V����

	static int m_nowInputLeftKye;
	static int m_nowInputRightKye;
	static int m_beforeInputLeftKye;
	static int m_beforeInputRightKye;

	static bool m_clickFlag;

};

#endif // _PLAYER_H_