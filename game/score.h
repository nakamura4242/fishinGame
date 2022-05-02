#ifndef _SCORE_H_
#define _SCORE_H_

#include "DxLib.h"


class Score
{
public:



	Score();				// コンストラクタ.
	~Score();				// デストラクタ.


	// ポジションのgetter/setter.
	//void SetPosX(const int setX) { posX = setX; }

	//スコアのゲッター
	static int GetScore() { return m_score; }
	//スコアを加算する
	static void AddScore(int _Score) { m_score += _Score; }

	static void SetScore(const float score) { m_score = score; }

	static void MaxScore();


private:

	static float m_score;
};

#endif // _SCORE_H_