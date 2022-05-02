#pragma once
#include "SceneBase.h"
#include "DxLib.h"


class Title : public SceneBase
{
public:


	Title();					//	�R���X�g���N�^
	~Title();					//	�f�X�g���N�^

	SceneBase* Update(float _deltaTime) override;
	void Draw() override;
	void Sound() override;
	void Load() override;

	void UpdateTransparent();

	//	�f���^�^�C���̃Z�b�^�[
	void SetDeltaTime(float _deltaTime) { m_deltaTime = _deltaTime; }
	//	�f���^�^�C���̃Q�b�^�[
	float GetDeltaTime() { return m_deltaTime; }
private:

	int m_backGraphHandle;				//	�^�C�g����ʁE�w�i�̃O���t�B�b�N�n���h��
	float m_deltaTime;				//	�f���^�^�C��
	int m_gameSceneFlag;			//	�Q�[���V�[���؂�ւ��t���O

	int m_logoHandle;				//���S�̉摜�n���h��
	int m_cursorHandle;				//�J�[�\���̉摜�̃n���h��
	int m_buttonHandle;				//�Q�[�����J�n����{�^���̉摜�n���h��
	float m_cursorTimer;
	float m_cursorPower;				//�J�[�\�����g��k������`�J��	
	int m_cursorPowerFlag;

	int mousuX, mousuY;				//�}�E�X�̍��W���i�[����ϐ�
	int m_bottonExpansion;			//�{�^���̊g��A�k����������ϐ�

	int m_nowInputLeftKye;
	int m_nowInputRightKye;
	int m_beforeInputLeftKye;
	int m_beforeInputRightKye;

	bool m_fadeInFinishFlag;
	bool m_fadeOutFlag;
	bool m_fadeOutFinishFlag;

	int m_SoundHandle;
	int m_gameStartHandle;
	int m_touchHandle;

	bool m_manualFlag;

	class Manual* m_manual;			//�}�j���A���̕\��

};