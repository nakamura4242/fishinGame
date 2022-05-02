#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "DxLib.h"


class Mouse
{
public:



	Mouse();				// コンストラクタ.
	~Mouse();				// デストラクタ.
	void Update();			// 更新.
	void Draw();			// 描画.


	// ポジションのgetter/setter.
	void SetPosX(const int setX) { posX = setX; }
	void SetPosY(const int setY) { posY = setY; }

	//マウスがクリックした場所を取得する
	int GetMouseClickPosX() { return posX; }
	int GetMouseClickPosY() { return posY; }

	static int GetMouseClickInputKye();


private:

	int posX, posY;			//マウスのカーソルのポジション

	static int m_nowInputLeftKye;
	static int m_nowInputRightKye;
	static int m_beforeInputLeftKye;
	static int m_beforeInputRightKye;

	static bool m_clickFlag;

};

#endif // _PLAYER_H_