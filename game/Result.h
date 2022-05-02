#ifndef _RESULT_H_
#define _RESULT_H_

#include "SceneBase.h"

#include "DxLib.h"

class Result : public SceneBase
{
public:
	Result();			//	�R���X�g���N�^
	~Result();							//�@�f�X�g���N�^

	void UpdateTransparent();

	SceneBase* Update(float _deltaTime)override;	//	�X�V
	void Draw()override;				//	�`��
	void Sound()override;				//	���y
	void Load()override;				//	������
private:
	int m_backgroundGraphHandle;		//	�w�i�̃O���t�B�b�N�n���h��

	int m_backGraphHandle;
	int m_logoHandle;
	int m_cursorHandle;
	int m_buttonHandle;			//�Q�[���I�[�o�[��ʂŎg���Ă���摜�n���h���B

	int m_gameSceneFlag;			//	�Q�[���V�[���؂�ւ��t���O

	int mousuX, mousuY;				//�}�E�X�̍��W���i�[����ϐ�
	int m_bottonExpansion;			//�{�^���̊g��A�k����������ϐ�
	float m_cursorPower;				//�J�[�\�����g��k������`�J��	
	int m_cursorPowerFlag;
	float m_cursorTimer;
	int m_scoreHandle[10];

	int m_nowInputLeftKye;
	int m_nowInputRightKye;
	int m_beforeInputLeftKye;
	int m_beforeInputRightKye;

	bool m_fadeInFinishFlag;
	bool m_fadeOutFlag;
	bool m_fadeOutFinishFlag;
	int m_SoundHandle;
	int m_gameEndHandle;
	int m_soundVolume;

	int m_evaluationHandle[4];
	int m_FretFede;
	bool m_fedeFlag;
	int m_hyoukaHandle;
};

#endif // _RESULT_H_

