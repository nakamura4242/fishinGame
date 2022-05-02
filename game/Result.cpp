#include "Result.h"
#include "Title.h"
#include "score.h"

const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;
const int VOLUME_PAL_SUP = 90;
// �ő哧�ߗ�
const int MAX_TRANSP_VAL = 200;
// ���ߗʕω��p�x�N�g��
const int transModeration = -1;
//	�t�F�[�h�C���̑��x
const int FADE_IN_SPEED = 3;
//	�t�F�[�h�A�E�g�̑��x
const int FADE_OUT_SPEED = 3;

const int SCORE_POS_X = SCREEN_SIZE_W / 2 + 150;
const int SCORE_POS_Y = SCREEN_SIZE_W / 3 - 150;

Result::Result()
	:m_gameSceneFlag(false)
	, m_cursorTimer(0)
	, m_cursorPower(0.5)
	, mousuX(0)
	, mousuY(0)
	, m_bottonExpansion(0)
	, m_nowInputLeftKye(0)
	, m_beforeInputLeftKye(0)
	, m_fadeInFinishFlag(false)
	, m_fadeOutFlag(false)
	, m_fadeOutFinishFlag(false)
	, m_soundVolume(150)
	, m_FretFede(0)
	, m_fedeFlag(false)
{
}

Result::~Result()
{
	DeleteSoundMem(m_SoundHandle);
	DeleteSoundMem(m_gameEndHandle);

}

SceneBase* Result::Update(float _deltaTime)
{
	// �}�E�X��\����Ԃɂ���
	SetMouseDispFlag(TRUE);

	GetMousePoint(&mousuX, &mousuY);			//�J�[�\���̃|�W�V�����擾

	m_nowInputLeftKye = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;

	if (m_FretFede > 255)
	{
		m_fedeFlag = false;
	}
	else if (m_FretFede < 100)
	{
		m_fedeFlag = true;
	}

	if (m_fedeFlag)
	{
		m_FretFede++;

	}
	else
	{
		m_FretFede--;
	}


	if ((SCREEN_SIZE_W / 4 + 60 < mousuX && SCREEN_SIZE_H / 2 + 150 < mousuY)
		&& ((SCREEN_SIZE_W / 4 + 879) + 60 > mousuX && (SCREEN_SIZE_H / 2 + 406) + 150 > mousuY))	//�{�^���͈̔͂ɃJ�[�\������������
	{
		m_bottonExpansion = 50;
		if (m_beforeInputLeftKye == 0 && m_nowInputLeftKye == 1)
		{
			ChangeVolumeSoundMem(m_soundVolume, m_SoundHandle);
			PlaySoundMem(m_gameEndHandle, DX_PLAYTYPE_BACK, false);
			WaitTimer(1500);
			m_soundVolume--;
			Score::SetScore(0);
			m_gameSceneFlag = true;
		}

	}
	else
	{
		m_bottonExpansion = 0;
	}

	m_beforeInputLeftKye = m_nowInputLeftKye;

	if (m_fadeOutFinishFlag)
	{

		return new Title;
	}
	return this;
}

void Result::Draw()
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
	DrawExtendGraph(506 - m_bottonExpansion, SCREEN_SIZE_H - 438 - m_bottonExpansion,
		506 +908 + m_bottonExpansion, SCREEN_SIZE_H  + m_bottonExpansion,
		m_buttonHandle, TRUE);
	DrawGraph(SCORE_POS_X, SCORE_POS_Y, m_scoreHandle[Score::GetScore() % 10], TRUE);//1����
	DrawGraph(SCORE_POS_X - 150, SCORE_POS_Y, m_scoreHandle[(Score::GetScore() % 100) / 10], TRUE);//2����
	DrawGraph(SCORE_POS_X - 300, SCORE_POS_Y, m_scoreHandle[Score::GetScore() / 100], TRUE);//2����
	//DrawGraph(0, 0, m_hyoukaHandle, TRUE);


	//DrawGraph(0, 0, m_cursorHandle, TRUE);
	//DrawExtendGraph((SCREEN_SIZE_W / 2 + 200) - m_cursorTimer, (SCREEN_SIZE_H / 2 + 250) - m_cursorTimer,
	//	(SCREEN_SIZE_W / 2 + 200 + 59) + m_cursorTimer, (SCREEN_SIZE_H / 2 + 250 + 76) + m_cursorTimer,
	//	m_cursorHandle, TRUE);

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

	// �t�F�[�h�A�E�g����
	if (m_gameSceneFlag)
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

	if (Score::GetScore() < 50)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_FretFede);

		DrawGraph(0, 0, m_evaluationHandle[0], TRUE);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 225);

	}
	if (Score::GetScore() >= 50 && Score::GetScore() < 200)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_FretFede);

		DrawGraph(0, 0, m_evaluationHandle[1], TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 225);

	}
	if (Score::GetScore() >= 200 && Score::GetScore() < 500)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_FretFede);

		DrawGraph(0, 0, m_evaluationHandle[2], TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 225);

	}
	if (Score::GetScore() >= 500)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_FretFede);

		DrawGraph(0, 0, m_evaluationHandle[3], TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 225);

	}


}

void Result::Sound()
{
	ChangeVolumeSoundMem(150, m_SoundHandle);
	PlaySoundMem(m_SoundHandle, DX_PLAYTYPE_LOOP, false);

}

void Result::Load()
{
	m_backGraphHandle	= LoadGraph("data/img/GameOver/OverBack.png");
	m_logoHandle		=LoadGraph("data/img/GameOver/Overlog.png");
	m_cursorHandle		=LoadGraph("data/img/GameOver/OverCursor.png");
	m_buttonHandle		=LoadGraph("data/img/GameOver/OverTitle.png");

	LoadDivGraph("data/img/GameOver/mapchip02.png", 10, 10, 1, 150, 150, m_scoreHandle);
	m_SoundHandle = LoadSoundMem("data/sound/fish/c2.mp3");		//�X�R�A�̌��ʉ��n���h��
	m_gameEndHandle = LoadSoundMem("data/sound/fish/yakuoto.mp3");		//�X�R�A�̌��ʉ��n���h��

	m_evaluationHandle[0]= LoadGraph("data/img/GameOver/�쎀.png");
	m_evaluationHandle[1]= LoadGraph("data/img/GameOver/������.png");
	m_evaluationHandle[2]= LoadGraph("data/img/GameOver/����.png");
	m_evaluationHandle[3]= LoadGraph("data/img/GameOver/�v����.png");
	m_hyoukaHandle = LoadGraph("data/img/GameOver/hyouka.png");

}
 //�_�ŃG�t�F�N�g�ɗp���铧�ߗʂ̍X�V����
void Result::UpdateTransparent()
{
}
