//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------

#pragma once

class Player;
class Fish;

class HitChecker
{

public:
	static void Check(Player& player, Fish& fish);
	static void VisualCheck(Player& player, Fish& fish);

};