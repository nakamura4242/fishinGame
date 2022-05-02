//-----------------------------------------------------------------------------
// @brief  プレイヤークラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "Player.h"
#include "Effect.h"
#include "Camera.h"
#include "Fish.h"
#include "mouse.h"

#define SCREEN_SIZE_W 1920
#define SCREEN_SIZE_H 1080




//-----------------------------------------------------------------------------
// @brief  コンストラクタ.
//-----------------------------------------------------------------------------
Player::Player()
	:hitRadius(7.0f)
	, m_shakePlayer(0)
	, m_shakePlayerFlag(false)
	, m_nowInputLeftKye(0)
	, m_nowInputRightKye(0)
	, m_beforeInputLeftKye(0)
	, m_beforeInputRightKye(0)
	, m_soundFlag(false)
{	
	//アヒルさんのモデルをロードします
	m_buckHandle = MV1LoadModel("data/model/duck/duck.mv1");
	//アヒルさんのｘの角度を５０度傾けます		
	//MV1SetRotationXYZ(m_buckHandle, VGet(-50.0f * DX_PI_F / 180.0f, 0.0f, 0.0f));
	//アヒルのモデルが大きいので縮小します
	MV1SetScale(m_buckHandle, VGet(0.05, 0.05, 0.05));

	pos = VGet(100, 100, 100);

	state = normal;
	MV1SetPosition(m_buckHandle, pos);			//クリックした座標をアヒルさんのモデルにセットする
	velocity = VGet(0, 0, 0);
	rod_tip = VGet(0, 0,-10);

	m_landingSoundHandle = LoadSoundMem("data/sound/fish/水・落水・着水・ぽっちゃん04.mp3");	
	m_hitSoundHandle = LoadSoundMem("data/sound/fish/バタ足で泳ぐ.mp3");


	//// 水しぶきエフェクト読み込み
	//m_playerRippleEfk = new PlayEffect("data/effects/swim/hamon01.efk", 10);
	//m_efkDir = VGet(0.0f, 3.0f, 0.0f);

}

//-----------------------------------------------------------------------------
// @brief  デストラクタ.
//-----------------------------------------------------------------------------
Player::~Player()
{
	//m_playerRippleEfk->StopEffect();
	//m_playerRippleEfk->Delete();
	//delete m_playerRippleEfk;
	DeleteSoundMem(m_hitSoundHandle);
	DeleteSoundMem(m_landingSoundHandle);

	MV1DeleteModel(m_buckHandle);
}

//-----------------------------------------------------------------------------
// @brief  更新.
//-----------------------------------------------------------------------------
void Player::Update(float _deltaTime,Mouse* m_mouse)
{

	m_nowInputLeftKye = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
	m_nowInputRightKye = (GetMouseInput() & MOUSE_INPUT_RIGHT) != 0;


	if (m_shakePlayer > 50)
	{
		m_shakePlayerFlag = false;
	}
	else if (m_shakePlayer < 1)
	{
		m_shakePlayerFlag = true;
	}

	if (m_shakePlayerFlag)
	{
		pos.y += 0.01;
		m_shakePlayer++;
	}
	else
	{
		pos.y -= 0.01;
		m_shakePlayer--;
	}

	//ここにうきが浮き沈みする動きを入れる

	//左クリックしたらアヒルさんが出現
	if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0 && state== normal)	//何もしてない状態にクリックするとその場所にアヒルさん出現
	{
		mousuX = m_mouse->GetMouseClickPosX();
		mousuY = m_mouse->GetMouseClickPosY();

		pos = ConvScreenPosToWorldPos_ZLinear(VGet(mousuX, mousuY, SCREEN_SIZE_H - mousuY));
		MV1SetPosition(m_buckHandle, pos);			//クリックした座標をアヒルさんのモデルにセットする

		state = landing;		//アヒルさんが着水した状態に状態を更新する

		m_soundFlag = true;
	}

	if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0&&state==landing)
	{
	
		state = normal;

	}

	if (state == normal)
	{
		velocity = VGet(0, 0, 0);

		pos = VGet(100, 100, 100);
	}

	if (state == fish)
	{
		velocity = VGet(0, 100, 0);
		pos.y += velocity.y * _deltaTime;
		if (pos.y > 80)
		{
			state = result;
		}


	}

	if (state == inGame)
	{

	}

	pos = VAdd(pos, VScale( velocity, _deltaTime));
	MV1SetPosition(m_buckHandle, pos);
	//rod_tip = VGet(-pos.x, 80, -pos.z);

}

//-----------------------------------------------------------------------------
// @brief  描画.
//-----------------------------------------------------------------------------
void Player::Draw()
{
	MV1DrawModel(m_buckHandle);
	DrawLine3D(pos, rod_tip, GetColor(255, 255, 255));

	//DrawFormatString(0, 250, GetColor(255, 255, 255), "座標Ｘ %d　　座標Ｙ %d", mousuX, mousuY);
	//DrawFormatString(0, 550, GetColor(255, 255, 255), "座標Ｘ %f　　座標Ｙ %f   座標ｚ　%f", pos.x, pos.y,pos.z);
	//DrawSphere3D(pos, hitRadius, 5, 0x00ffff, 0x00ffff, false);

	if (m_soundFlag)
	{
		ChangeVolumeSoundMem(150, m_landingSoundHandle);
		PlaySoundMem(m_landingSoundHandle, DX_PLAYTYPE_BACK);
		m_soundFlag = false;
	}

	if (state == inGame || state == eat)
	{
		ChangeVolumeSoundMem(150, m_hitSoundHandle);
		PlaySoundMem(m_hitSoundHandle, DX_PLAYTYPE_LOOP,false);

	}
	else
	{
		StopSoundMem(m_hitSoundHandle);
	}

	//if (state == eat || state == inGame)
	//{
	//	if (m_playerRippleEfk->GetNowPlaying() != 0)
	//	{
	//		m_playerRippleEfk->PlayEffekseer(VAdd(pos, VGet(0, 0, 0)));

	//	}

	//}
	//m_playerRippleEfk->SetPlayingEffectPos(pos);

}

//-----------------------------------------------------------------------------
// @brief  衝突処理.
//-----------------------------------------------------------------------------
//void Player::OnHitObstruct(ObstructBase& obstruct)
//{
//}

