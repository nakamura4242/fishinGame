#ifndef _FISH_H_
#define _FISH_H_

#include "DxLib.h"

class Player;

class Fish
{
public:

	enum State
	{
		normal,		//なにもしていない！状態
		assault,	//うきに向かって突撃している状態
		hit,		//魚が食いついた状態
		get,		//魚が釣り上げられた状態
		result,
	};
	State GetState() const { return state; }	//今のプレイヤーの状態を返す	
	void SetState(const State set) { state = set; }


	Fish(const int posx, const int posy);
	~Fish();

	void Update(float _deltaTime ,Player* m_player);			// 更新.
	void Draw();							// 描画.

	// ポジションのgetter/setter.
	const VECTOR& GetPos() const { return pos; }
	void SetPos(const VECTOR set) { pos = set; }

	// あたり判定半径の取得.
	float GetHitRadius() { return hitRadius; }
	// 魚の視覚判定半径の取得
	float GetVisualRadius() { return VisualRadius; }
	//魚の体力が減る
	void HitFishDownHP(int power) { m_fishHP-=power; }
	//魚の体力が回復する
	void HitFishUpHp(int power) { m_fishHP+=power; }
	//魚の体力の取得
	float GetHitFishHP() { return m_fishHP; }
	void SetHitFishHp(int HP) { m_fishHP = HP; }
	//魚の強さの値を渡すゲッター
	float GetHitFishStrength() { return m_fishstrength; }

	void OnHitFish();

	void SetPos();

	//魚の最初に設定された体力の取得
	float GetHitFishStartHP() { return m_startHP; }


private:

	State state;			//魚の状態
	int		m_fishModelHandle;			//	モデルハンドル
	VECTOR	pos;			// ポジション.
	VECTOR	SetPosStart;			// ポジション.

	VECTOR	velocity;		// 移動力.
	VECTOR	dir;			// 回転方向.

	int m_startPosX, m_startPosY;		//最初の座標
	float m_fishstrength;						//最初のHP
	float m_startHP;

	float	hitRadius;		// あたり判定の半径.
	float	VisualRadius;	//魚の視覚の範囲

	int m_TextureHandle;//テクスチャの画像ハンドル
	int m_texIndex; //テクスチャの番号

	float m_fishHP;			//魚のHP

	bool m_fishDrowFlag;		//魚を描画するかしないか

	float m_fishSwim;			//魚が泳ぐように見えるように作った変数
	bool m_fishSwimFlag;		//魚がどっちの角度に向くかのフラグ
	float m_fishSwimPower;

	int m_fishFretHandle;
	int m_FretFede;
	bool m_fedeFlag;
	
	int m_fishSwimPosX;
	int m_fishSwimPosY;			//魚が泳ぐときに目指す座標

	bool m_resultFlag;
	int m_FishPrintHandle;
	int m_paperHandle;

	class PlayEffect* m_fishOrbitEfk;

};
#endif // _FISH_H_

