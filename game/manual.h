#ifndef _MANUAL_H_
#define _MANUAL_H_

#include "DxLib.h"


class Manual
{
public:

	Manual();
	~Manual();

	void Update(float _deltaTime);			// 更新.
	void Draw();	
	// 画像のナンバーのゲッターセッター
	int GetImgNumber() { return m_imgNumber; }
	void SetImgNumber(int Number) { m_imgNumber = Number; }

private:
	int m_manual[9];			//マニュアルの画像ハンドル
	int m_imgNumber;			//表示したい画像の番号
	bool m_clickLeftFlag, m_clickRightFlag;
	int m_nowInputLeftKye;
	int m_nowInputRightKye;

	int m_beforeInputLeftKye;
	int m_beforeInputRightKye;

	int m_manualSoundHandle;
};
#endif // _FISH_H_

