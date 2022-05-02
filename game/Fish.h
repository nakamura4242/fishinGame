#ifndef _FISH_H_
#define _FISH_H_

#include "DxLib.h"

class Player;

class Fish
{
public:

	enum State
	{
		normal,		//�Ȃɂ����Ă��Ȃ��I���
		assault,	//�����Ɍ������ēˌ����Ă�����
		hit,		//�����H���������
		get,		//�����ނ�グ��ꂽ���
		result,
	};
	State GetState() const { return state; }	//���̃v���C���[�̏�Ԃ�Ԃ�	
	void SetState(const State set) { state = set; }


	Fish(const int posx, const int posy);
	~Fish();

	void Update(float _deltaTime ,Player* m_player);			// �X�V.
	void Draw();							// �`��.

	// �|�W�V������getter/setter.
	const VECTOR& GetPos() const { return pos; }
	void SetPos(const VECTOR set) { pos = set; }

	// �����蔻�蔼�a�̎擾.
	float GetHitRadius() { return hitRadius; }
	// ���̎��o���蔼�a�̎擾
	float GetVisualRadius() { return VisualRadius; }
	//���̗̑͂�����
	void HitFishDownHP(int power) { m_fishHP-=power; }
	//���̗̑͂��񕜂���
	void HitFishUpHp(int power) { m_fishHP+=power; }
	//���̗̑͂̎擾
	float GetHitFishHP() { return m_fishHP; }
	void SetHitFishHp(int HP) { m_fishHP = HP; }
	//���̋����̒l��n���Q�b�^�[
	float GetHitFishStrength() { return m_fishstrength; }

	void OnHitFish();

	void SetPos();

	//���̍ŏ��ɐݒ肳�ꂽ�̗͂̎擾
	float GetHitFishStartHP() { return m_startHP; }


private:

	State state;			//���̏��
	int		m_fishModelHandle;			//	���f���n���h��
	VECTOR	pos;			// �|�W�V����.
	VECTOR	SetPosStart;			// �|�W�V����.

	VECTOR	velocity;		// �ړ���.
	VECTOR	dir;			// ��]����.

	int m_startPosX, m_startPosY;		//�ŏ��̍��W
	float m_fishstrength;						//�ŏ���HP
	float m_startHP;

	float	hitRadius;		// �����蔻��̔��a.
	float	VisualRadius;	//���̎��o�͈̔�

	int m_TextureHandle;//�e�N�X�`���̉摜�n���h��
	int m_texIndex; //�e�N�X�`���̔ԍ�

	float m_fishHP;			//����HP

	bool m_fishDrowFlag;		//����`�悷�邩���Ȃ���

	float m_fishSwim;			//�����j���悤�Ɍ�����悤�ɍ�����ϐ�
	bool m_fishSwimFlag;		//�����ǂ����̊p�x�Ɍ������̃t���O
	float m_fishSwimPower;

	int m_fishFretHandle;
	int m_FretFede;
	bool m_fedeFlag;
	
	int m_fishSwimPosX;
	int m_fishSwimPosY;			//�����j���Ƃ��ɖڎw�����W

	bool m_resultFlag;
	int m_FishPrintHandle;
	int m_paperHandle;

	class PlayEffect* m_fishOrbitEfk;

};
#endif // _FISH_H_

