#include "manual.h"

Manual::Manual()
	:m_imgNumber(0)
	,m_clickLeftFlag(false)
	,m_clickRightFlag(false)
	, m_beforeInputRightKye(0)
	,m_beforeInputLeftKye(0)
{
	m_manual[0] = LoadGraph("data/img/manual/�}�j���A��01.png");	//�w�i�̉摜
	m_manual[1] = LoadGraph("data/img/manual/�}�j���A��02.png");	//�w�i�̉摜
	m_manual[2] = LoadGraph("data/img/manual/�}�j���A��03.png");	//�w�i�̉摜
	m_manual[3] = LoadGraph("data/img/manual/�}�j���A��04.png");	//�w�i�̉摜
	m_manual[4] = LoadGraph("data/img/manual/�}�j���A��05.png");	//�w�i�̉摜
	m_manual[5] = LoadGraph("data/img/manual/�}�j���A��06.png");	//�w�i�̉摜
	m_manual[6] = LoadGraph("data/img/manual/�}�j���A��07.png");	//�w�i�̉摜
	m_manual[7] = LoadGraph("data/img/manual/�}�j���A��08.png");	//�w�i�̉摜
	m_manual[8] = LoadGraph("data/img/manual/�}�j���A��09.png");	//�w�i�̉摜

	m_manualSoundHandle = LoadSoundMem("data/sound/fish/�}�j���A��.mp3");


	//�p�X�̒��g����������
}

Manual::~Manual()
{
}

void Manual::Update(float _deltaTime)
{

	m_nowInputLeftKye = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0 && !m_clickLeftFlag;
	m_nowInputRightKye = (GetMouseInput() & MOUSE_INPUT_RIGHT) != 0 && !m_clickRightFlag;
	
	//���N���b�N���ĕϐ��������ĉE�N���b�N�ő�����
	if (m_beforeInputLeftKye == 0 && m_nowInputLeftKye == 1)
	{
		ChangeVolumeSoundMem(255, m_manualSoundHandle);
		PlaySoundMem(m_manualSoundHandle, DX_PLAYTYPE_BACK);

		m_beforeInputLeftKye = m_nowInputLeftKye;
		m_imgNumber--;
		if (m_imgNumber < 0)
		{
			m_imgNumber = 0;

		}
	}

	if (m_beforeInputRightKye == 0 && m_nowInputRightKye == 1)
	{
		ChangeVolumeSoundMem(255, m_manualSoundHandle);
		PlaySoundMem(m_manualSoundHandle, DX_PLAYTYPE_BACK);

		m_beforeInputRightKye = m_nowInputRightKye;
		m_imgNumber++;
	}
	m_beforeInputRightKye = m_nowInputRightKye;
	m_beforeInputLeftKye = m_nowInputLeftKye;


	//������萔�������茸�肷�����肵�Ȃ��悤�ɂ�����

}

void Manual::Draw()
{
	//�摜�̔ԍ��̕ϐ������
	DrawGraph(0, 0, m_manual[m_imgNumber], false);	//
}
