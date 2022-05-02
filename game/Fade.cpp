#include "Fade.h"
#include "Dxlib.h"


const int FADE_SPEED = 3;


Fade::Fade()
{
}

//�t�F�[�h�C���֐��B�t�F�[�h�C���������摜������΃n���h��������B�Ȃ����false������
void Fade::FadeIn(int _graphHandle)
{
	for (int i = 0; i < 255; i += FADE_SPEED)
	{
		// �`��P�x���Z�b�g
		SetDrawBright(i, i, i);

		DrawGraph(0, 0, _graphHandle, TRUE);
		
		// �O���t�B�b�N��`��
		ScreenFlip();
	}
}

//�t�F�[�h�A�E�g�֐��B�t�F�[�h�A�E�g�������摜������΃n���h��������B�Ȃ����false������
void Fade::FadeOut( int _graphHandle)
{
	for (int i = 0; i < 255; i += FADE_SPEED)
	{
		// �`��P�x���Z�b�g
		SetDrawBright(255 - i, 255 - i, 255 - i);

		DrawGraph(0, 0, _graphHandle, TRUE);

		// �O���t�B�b�N��`��
		ScreenFlip();
	}
}
