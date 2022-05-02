#pragma once

#include "SceneBase.h"
#include "DxLib.h"

class TestSceneNakamura :public SceneBase
{
public:


	TestSceneNakamura();				//	コンストラクタ
	~TestSceneNakamura();			//	デストラクタ

	SceneBase* Update(float _deltaTime)override;	//	更新
	void Draw()override;			//	描画
	void Sound()override;			//	音楽
	void Load()override;			//	初期化
	void DebugKey();


private:
	class Player* m_player;			//	プレイヤークラスへのポインタメンバ変数
	class Fish* m_fish[5];				//　魚
	//class Fish* m_fish;
	class Camera* m_camera;
	class Manual* m_manual;			//マニュアルの表示
	class Mouse* m_mouse;			//マウスのポインタメンバ変数
	
	int m_backGraphHandle;
	int m_hitGraphHandle;			//　ヒットした時の画像ハンドル
	int m_gameSceneFlag;
	int hasuHandle[2];					//   ハスのモデル
	int m_lotusTextureHandle;			//   ハスのテクスチャ
	int m_texIndex;
	VECTOR pos;

	int m_fishModelHandle;			//魚の３Dモデルハンドル
	int m_miniGameGaugeHandle;		//ミニゲームの画像ハンドル
	int m_HPzeroGaugeHandle;			//プレイヤーのHP画像ハンドル

	int m_pinHandle;				//ミニゲームのピンの画像ハンドル
	int m_pinPosX;					//ピンの座標の変数
	float m_pinNowPower;				//ピンの現在の力

	//int m_fishHP;					//魚のHP
	float m_playerHP;					//プレイヤーのHP

	int m_startMiniGameFlag;		//hitした時の画像が表示されるフラグ
	int m_miniGameFlag;				//ミニゲームが始まるフラグ
	float m_miniTimer;				//hitした時間を計測する変数
	int m_hitDrawTimer;				//hitした画像の描画切り替えをするための変数

	float HpGauge;					//プレイヤーのHPの全体がゲージの何パーセントかを格納する変数

	bool m_playerDirFlag;				//プレイヤーが死んだかどうかのフラグ
	int m_mouseFishHandle;			//釣り上げるやり方のマニュアルを表示する画像ハンドル
	float m_pinMovePower;				//ピンを動かすチカラ

	int m_manualIconHandle;			//マニュアルを表示するアイコンの画像ハンドル
	bool m_manualDrawFlag;			//マニュアルを今表示しているかどうか
	bool m_manualBigDrawFlag;

	int m_scoreHandle[10];
	int m_fishScoreHandle[10];
	bool FishOverview;

	bool m_resultFlag;
	int m_FishPrintHandle;
	int m_paperHandle;

	int FishSize;
	int cmHandle;

	float m_pinSpeed;

	bool m_fadeInFinishFlag;
	bool m_fadeOutFlag;
	bool m_fadeOutFinishFlag;
	int m_fishSoundHandle;
	int m_BGMSoundHandle;

	int m_gaugeIconHandle;

	int m_playerGaugeHandle;
	int m_playerGaugeZeroHandle;

	float m_fishGauge;
	int m_playerpinchHandle;

	int m_fishResultSound01Handle;
	int m_fishResultSound02Handle;
	int m_fishResultSound03Handle;

	int FishSizeScore;
	int m_highScore;
};
