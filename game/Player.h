//-----------------------------------------------------------------------------
// @brief  プレイヤークラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "DxLib.h"

class Mouse;

class Player final
{
public:

	enum State
	{
		normal,		//なにもしていない！状態
		landing,	//あひるさんが着水した状態
		eat,		//魚にくいつかれた状態
		inGame,		//ミニゲーム中
		fish,		//釣った！
		manual,		//マニュアル読んでるときは何もできない！
		result,
	};

	State GetState() const { return state; }	//今のプレイヤーの状態を返す	
	void SetState(const State set) { state = set; }


	Player();				// コンストラクタ.
	~Player();				// デストラクタ.

	void Update(float _deltaTime,Mouse* m_mouse);			// 更新.
	void Draw();			// 描画.


	// 衝突処理.
	//void OnHitObstruct(ObstructBase& obstruct);


	// ポジションのgetter/setter.
	const VECTOR& GetPos() const { return pos; }
	void SetPos(const VECTOR set) { pos = set; }

	// アヒルのしゅつげんのgetter
	bool GetBuckFlag() { return buckDrawFlag; }


	// ディレクションのgetter/setter.
	const VECTOR& GetDir() const { return dir; }
	void SetDir(const VECTOR set) { dir = set; }

	// あたり判定半径の取得.
	float GetHitRadius() { return hitRadius; }

	//マウスがクリックした場所を取得する
	int GetMouseClickPosX() { return mousuX; }
	int GetMouseClickPosY() { return mousuY; }


private:

	State state;					//プレイヤーの今の状態

	int		m_modelHandle;			//	モデルハンドル
	VECTOR	pos;			// ポジション.
	VECTOR	velocity;		// 移動力.
	VECTOR	dir;			// 回転方向.
	VECTOR  playerDir;
	float	hitRadius;		// あたり判定の半径.

	int mousuX, mousuY;
	int buckZ;
	bool buckDrawFlag;
	int m_buckHandle;

	int m_shakePlayer;		//うきが浮き沈みしてるように見える動きをさせる変数
	bool m_shakePlayerFlag;
	class PlayEffect* m_playerRippleEfk;	//水しぶきのエフェクト

	VECTOR rod_tip;

	int m_nowInputLeftKye;
	int m_nowInputRightKye;
	int m_beforeInputLeftKye;
	int m_beforeInputRightKye;

	int m_landingSoundHandle;
	bool m_soundFlag;

	int m_hitSoundHandle;

	class PlayEffect* m_playerOrbitEfk;
	VECTOR m_efkDir;


};

#endif // _PLAYER_H_