#include "manual.h"

Manual::Manual()
	:m_imgNumber(0)
	,m_clickLeftFlag(false)
	,m_clickRightFlag(false)
	, m_beforeInputRightKye(0)
	,m_beforeInputLeftKye(0)
{
	m_manual[0] = LoadGraph("data/img/manual/マニュアル01.png");	//背景の画像
	m_manual[1] = LoadGraph("data/img/manual/マニュアル02.png");	//背景の画像
	m_manual[2] = LoadGraph("data/img/manual/マニュアル03.png");	//背景の画像
	m_manual[3] = LoadGraph("data/img/manual/マニュアル04.png");	//背景の画像
	m_manual[4] = LoadGraph("data/img/manual/マニュアル05.png");	//背景の画像
	m_manual[5] = LoadGraph("data/img/manual/マニュアル06.png");	//背景の画像
	m_manual[6] = LoadGraph("data/img/manual/マニュアル07.png");	//背景の画像
	m_manual[7] = LoadGraph("data/img/manual/マニュアル08.png");	//背景の画像
	m_manual[8] = LoadGraph("data/img/manual/マニュアル09.png");	//背景の画像

	m_manualSoundHandle = LoadSoundMem("data/sound/fish/マニュアル.mp3");


	//パスの中身書き換える
}

Manual::~Manual()
{
}

void Manual::Update(float _deltaTime)
{

	m_nowInputLeftKye = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0 && !m_clickLeftFlag;
	m_nowInputRightKye = (GetMouseInput() & MOUSE_INPUT_RIGHT) != 0 && !m_clickRightFlag;
	
	//左クリックして変数が減って右クリックで増える
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


	//数が一定数超えたり減りすぎたりしないようにもする

}

void Manual::Draw()
{
	//画像の番号の変数を作る
	DrawGraph(0, 0, m_manual[m_imgNumber], false);	//
}
