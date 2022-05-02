#include "mouse.h"

int Mouse::m_nowInputLeftKye = 0;
int Mouse::m_nowInputRightKye = 0;
int Mouse::m_beforeInputLeftKye = 0;
int Mouse::m_beforeInputRightKye = 0;
bool Mouse::m_clickFlag = false;


Mouse::Mouse()
	:posX(0)
	,posY(0)
{
}

Mouse::~Mouse()
{
}

void Mouse::Update()
{
	// マウスを表示状態にする
	SetMouseDispFlag(TRUE);
	GetMousePoint(&posX, &posY);			//カーソルのポジション取得

	m_nowInputLeftKye = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
	m_beforeInputLeftKye = m_nowInputLeftKye;

}

void Mouse::Draw()
{
}

int Mouse::GetMouseClickInputKye()
{
	if (m_beforeInputLeftKye == 0 && m_nowInputLeftKye == 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}
