#ifndef _RESULT_H_
#define _RESULT_H_

#include "SceneBase.h"

#include "DxLib.h"

class Result : public SceneBase
{
public:
	Result();			//	コンストラクタ
	~Result();							//　デストラクタ

	void UpdateTransparent();

	SceneBase* Update(float _deltaTime)override;	//	更新
	void Draw()override;				//	描画
	void Sound()override;				//	音楽
	void Load()override;				//	初期化
private:
	int m_backgroundGraphHandle;		//	背景のグラフィックハンドル

	int m_backGraphHandle;
	int m_logoHandle;
	int m_cursorHandle;
	int m_buttonHandle;			//ゲームオーバー画面で使っている画像ハンドル達

	int m_gameSceneFlag;			//	ゲームシーン切り替えフラグ

	int mousuX, mousuY;				//マウスの座標を格納する変数
	int m_bottonExpansion;			//ボタンの拡大、縮小をさせる変数
	float m_cursorPower;				//カーソルを拡大縮小するチカラ	
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

