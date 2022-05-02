#ifndef _SCORE_H_
#define _SCORE_H_

#include "DxLib.h"


class Score
{
public:



	Score();				// �R���X�g���N�^.
	~Score();				// �f�X�g���N�^.


	// �|�W�V������getter/setter.
	//void SetPosX(const int setX) { posX = setX; }

	//�X�R�A�̃Q�b�^�[
	static int GetScore() { return m_score; }
	//�X�R�A�����Z����
	static void AddScore(int _Score) { m_score += _Score; }

	static void SetScore(const float score) { m_score = score; }

	static void MaxScore();


private:

	static float m_score;
};

#endif // _SCORE_H_