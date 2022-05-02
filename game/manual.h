#ifndef _MANUAL_H_
#define _MANUAL_H_

#include "DxLib.h"


class Manual
{
public:

	Manual();
	~Manual();

	void Update(float _deltaTime);			// �X�V.
	void Draw();	
	// �摜�̃i���o�[�̃Q�b�^�[�Z�b�^�[
	int GetImgNumber() { return m_imgNumber; }
	void SetImgNumber(int Number) { m_imgNumber = Number; }

private:
	int m_manual[9];			//�}�j���A���̉摜�n���h��
	int m_imgNumber;			//�\���������摜�̔ԍ�
	bool m_clickLeftFlag, m_clickRightFlag;
	int m_nowInputLeftKye;
	int m_nowInputRightKye;

	int m_beforeInputLeftKye;
	int m_beforeInputRightKye;

	int m_manualSoundHandle;
};
#endif // _FISH_H_

