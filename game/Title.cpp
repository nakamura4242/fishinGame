#include "Title.h"
#include "DxLib.h"
#include "TestSceneNakamura.h"
#include "manual.h"

//	�X�N���[���̃T�C�Y
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

//	�t�F�[�h�C���̑��x
const int FADE_IN_SPEED = 3;
//	�t�F�[�h�A�E�g�̑��x
const int FADE_OUT_SPEED = 3;



Title::Title()
	:m_gameSceneFlag(false)
	, m_cursorTimer(0)
	,m_cursorPower(0.5)
	,mousuX(0)
	,mousuY(0)
	, m_bottonExpansion(0)
	, m_nowInputLeftKye(0)
	, m_nowInputRightKye(0)
	, m_beforeInputLeftKye(0)
	, m_beforeInputRightKye(0)
	, m_fadeInFinishFlag(false)
	, m_fadeOutFlag(false)
	, m_fadeOutFinishFlag(false)
	, m_manualFlag(false)

{
}

Title::~Title()
{
	DeleteSoundMem(m_SoundHandle);
	DeleteSoundMem(m_gameStartHandle);

}

/// <summary>
/// �X�V����
/// </summary>
/// <returns> �V�[���̃|�C���^ </returns>
SceneBase* Title::Update(float _deltaTime)
{
	// �}�E�X��\����Ԃɂ���
	SetMouseDispFlag(TRUE);

	GetMousePoint(&mousuX, &mousuY);			//�J�[�\���̃|�W�V�����擾

	m_nowInputLeftKye = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;


	if ((SCREEN_SIZE_W / 4 + 60 < mousuX && SCREEN_SIZE_H / 2 + 150 < mousuY)
		&&((SCREEN_SIZE_W / 4 + 879) + 60>mousuX && (SCREEN_SIZE_H / 2 + 406) + 150>mousuY))	//�{�^���͈̔͂ɃJ�[�\������������
	{
		//ChangeVolumeSoundMem(150, m_touchHandle);
		//PlaySoundMem(m_touchHandle, DX_PLAYTYPE_BACK);


		m_bottonExpansion = 50;
		if (m_beforeInputLeftKye == 0 && m_nowInputLeftKye == 1)
		{
			ChangeVolumeSoundMem(150, m_gameStartHandle);
			PlaySoundMem(m_gameStartHandle,
				DX_PLAYTYPE_NORMAL);

			m_gameSceneFlag = true;
		}

	}
	else 
	{
		m_bottonExpansion = 0;
	}

	if (m_gameSceneFlag)
	{
		m_manual->Update(_deltaTime);

		if (m_manual->GetImgNumber() == 9)
		{
			m_manual->SetImgNumber(0);
			m_manualFlag = true;
			m_gameSceneFlag = false;
		}

	}

	if (m_fadeOutFinishFlag)
	{
		return new TestSceneNakamura();
	}


	return this;
}

void Title::Draw()
{

	if (!m_fadeInFinishFlag)
	{
		// �t�F�[�h�C������
		for (int i = 0; i < 255; i += FADE_IN_SPEED)
		{
			// �`��P�x���Z�b�g
			SetDrawBright(i, i, i);
			DrawGraph(0, 0, m_backGraphHandle, TRUE);
			ScreenFlip();
		}
		m_fadeInFinishFlag = true;
	}

	DrawGraph(0, 0, m_backGraphHandle, TRUE);
	DrawGraph(0, 0, m_logoHandle, TRUE);
	DrawExtendGraph((SCREEN_SIZE_W / 4 )+60 - m_bottonExpansion, (SCREEN_SIZE_H / 2)+150 - m_bottonExpansion,
		(SCREEN_SIZE_W / 4 +879)+60 + m_bottonExpansion, (SCREEN_SIZE_H / 2 +406)+150 + m_bottonExpansion, 
		m_buttonHandle, TRUE);
	//DrawGraph(0, 0, m_cursorHandle, TRUE);
	DrawExtendGraph((SCREEN_SIZE_W/2+250)- m_cursorTimer, (SCREEN_SIZE_H/2+400) - m_cursorTimer, 
		(SCREEN_SIZE_W / 2+250+59) + m_cursorTimer, (SCREEN_SIZE_H / 2+400+76) + m_cursorTimer, 
		m_cursorHandle, TRUE);
	//�J�[�\���̊g��k�����ł��Ă��Ȃ��A�Q�[���I�[�o�[��ʂ��ł��Ă��Ȃ�
	if (m_cursorTimer > 50)
	{
		m_cursorPowerFlag = true;
	}
	else if (m_cursorTimer < 1)
	{
		m_cursorPowerFlag = false;
	}

	if (m_cursorPowerFlag)
	{
		m_cursorTimer -= m_cursorPower;
	}
	else
	{
		m_cursorTimer += m_cursorPower;
	}

	if (m_gameSceneFlag)
	{
		m_manual->Draw();
	}

	// �t�F�[�h�A�E�g����
	if (m_manualFlag)
	{
		for (int i = 255; i > 0; i -= FADE_IN_SPEED)
		{
			// �`��P�x���Z�b�g
			SetDrawBright(i, i, i);
			DrawGraph(0, 0, m_backGraphHandle, false);
			ScreenFlip();
		}
		m_fadeOutFinishFlag = true;
	}

}

/// <summary>
/// �T�E���h�̍Đ�����
/// </summary>
void Title::Sound()
{

	ChangeVolumeSoundMem(150, m_SoundHandle);
	PlaySoundMem(m_SoundHandle, DX_PLAYTYPE_LOOP,false);

}

/// <summary>
/// �ǂݍ��ݏ���
/// </summary>
void Title::Load()
{
	m_backGraphHandle = LoadGraph("data/img/Title/TitleBack.png");
	m_logoHandle = LoadGraph("data/img/Title/Titlelog.png");
	m_cursorHandle = LoadGraph("data/img/Title/TitleCursor.png");
	m_buttonHandle = LoadGraph("data/img/Title/TitleGamestate.png");

	m_SoundHandle = LoadSoundMem("data/sound/fish/���̗���.mp3");		//�X�R�A�̌��ʉ��n���h��
	m_gameStartHandle = LoadSoundMem("data/sound/fish/���ʂɕ����яオ��.mp3");

	m_touchHandle = LoadSoundMem("data/sound/fish/tyokotyoko.mp3");

	m_manual = new Manual;

}

/// <summary>
/// �_�ŃG�t�F�N�g�ɗp���铧�ߗʂ̍X�V����
/// </summary>
void Title::UpdateTransparent()
{
}
