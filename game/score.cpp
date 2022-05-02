#include "score.h"

float Score::m_score = 0;

Score::Score()
{
}

Score::~Score()
{
}

void Score::MaxScore()
{
	if (m_score > 999)
	{
		m_score = 999;
	}
}
