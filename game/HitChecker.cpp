
//-----------------------------------------------------------------------------
// @brief  ヒットチェック.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------

#include "HitChecker.h"
#include "Player.h"
#include "Fish.h"

//-----------------------------------------------------------------------------
// @brief  ヒットチェック処理.
//-----------------------------------------------------------------------------
void HitChecker::Check(Player& player, Fish& _fish)
{
	// Z軸とX軸の二次元座標としてあたり判定を行う.
	VECTOR yZeroPlayer = VGet(player.GetPos().x, player.GetPos().y, player.GetPos().z);
	bool isHit = true;

	// 一度でもぶつかったら位置が変わるのでヒット計算をしなおす.
	while (isHit)
	{
		isHit = false;

		VECTOR yZeroObstruct = VGet(_fish.GetPos().x, _fish.GetPos().y , _fish.GetPos().z);

		// お互いのポジションと半径の距離が重なったら当たっていることになる.
		VECTOR playerToObs = VSub(yZeroObstruct, yZeroPlayer);

		if (VSize(playerToObs) < player.GetHitRadius() + _fish.GetHitRadius())
		{
			//printfDx("playerToObs %d \n", VSize(playerToObs));

			//_fish.OnHitFish();
			if (_fish.GetState() == Fish::assault &&player.GetState()==Player::landing)
			{
				_fish.SetState(Fish::hit);
				player.SetState(Player::eat);
			}
			isHit = true;
			
			break;
		}
		if (isHit)
		{
			yZeroPlayer = VGet(player.GetPos().x, 0, player.GetPos().z);
			break;
		}

		//return false;
	}
}

void HitChecker::VisualCheck(Player& player, Fish& _fish)
{
	// Z軸とX軸の二次元座標としてあたり判定を行う.
	VECTOR yZeroPlayer = VGet(player.GetPos().x, player.GetPos().y, player.GetPos().z);
	bool isHit = true;

	// 一度でもぶつかったら位置が変わるのでヒット計算をしなおす.
	while (isHit)
	{
		isHit = false;

		VECTOR yZeroObstruct = VGet(_fish.GetPos().x, _fish.GetPos().y, _fish.GetPos().z);

		// お互いのポジションと半径の距離が重なったら当たっていることになる.
		VECTOR playerToObs = VSub(yZeroObstruct, yZeroPlayer);

		if (VSize(playerToObs) < player.GetHitRadius() + _fish.GetVisualRadius())
		{
			//printfDx("playerToObs %d \n", VSize(playerToObs));

			//_fish.OnHitFish();
			if (_fish.GetState() == Fish::normal && player.GetState() == Player::landing)
			{
				_fish.SetState(Fish::assault);
			}
			isHit = true;

			break;
		}
		if (isHit)
		{
			yZeroPlayer = VGet(player.GetPos().x, 0, player.GetPos().z);
			break;
		}

		//return false;
	}
}