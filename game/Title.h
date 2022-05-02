#pragma once
#include "SceneBase.h"
#include "DxLib.h"


class Title : public SceneBase
{
public:


	Title();					//	コンストラクタ
	~Title();					//	デストラクタ

	SceneBase* Update(float _deltaTime) override;
	void Draw() override;
	void Sound() override;
	void Load() override;

	void UpdateTransparent();

	//	デルタタイムのセッター
	void SetDeltaTime(float _deltaTime) { m_deltaTime = _deltaTime; }
	//	デルタタイムのゲッター
	float GetDeltaTime() { return m_deltaTime; }
private:

	int m_backGraphHandle;				//	タイトル画面・背景のグラフィックハンドル
	float m_deltaTime;				//	デルタタイム
	int m_gameSceneFlag;			//	ゲームシーン切り替えフラグ

	int m_logoHandle;				//ロゴの画像ハンドル
	int m_cursorHandle;				//カーソルの画像のハンドル
	int m_buttonHandle;				//ゲームが開始するボタンの画像ハンドル
	float m_cursorTimer;
	float m_cursorPower;				//カーソルを拡大縮小するチカラ	
	int m_cursorPowerFlag;

	int mousuX, mousuY;				//マウスの座標を格納する変数
	int m_bottonExpansion;			//ボタンの拡大、縮小をさせる変数

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

	class Manual* m_manual;			//マニュアルの表示

};