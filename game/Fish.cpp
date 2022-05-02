#include "Fish.h"
#include "Player.h"
#include <math.h>
#include "score.h"
#include "Fade.h"
#include "Effect.h"
#include <time.h>

#define SCREEN_SIZE_W 1920	//��ʂ̉���
#define SCREEN_SIZE_H 1080	//��ʂ̏c��


Fish::Fish(const int posx,const int posy)
	:hitRadius(7.0f)
	, VisualRadius(60.0f)
	,m_fishDrowFlag(true)
	, m_fishSwim(0)
	, m_fishSwimPower(0.08)
	,m_fishSwimFlag(false)
	, m_FretFede(0)
	, m_fedeFlag(false)
	, m_fishSwimPosX(0)
	, m_fishSwimPosY(0)
	, m_resultFlag(false)

{
	//���f���̃��[�h�����܂�
	m_fishModelHandle = MV1LoadModel("data/model/sakana/sakana.mv1");
	m_startPosX = posx;
	m_startPosY = posy;
	//���̍��W���Z�b�g	
	//pos = ConvScreenPosToWorldPos_ZLinear(VGet(posx, posy, SCREEN_Y - posy));
	pos = ConvScreenPosToWorldPos_ZLinear(VGet(m_startPosX, m_startPosY, SCREEN_SIZE_H - m_startPosY));


	//���f���̃|�W�V�������Z�b�g���܂�
	MV1SetPosition(m_fishModelHandle, pos);

	//��]������p�x * DX_PI_F / 180.0f�Ŋp�x�������ł���
	MV1SetRotationXYZ(m_fishModelHandle, VGet(-50.0f * DX_PI_F / 180.0f, 0.0f, 0.0f));

	//���̃��f���̃e�N�X�`����ǂݍ���
	m_TextureHandle = LoadGraph("data/img/Texture/fish01.png");
	//�e�N�X�`���̔ԍ��H�����f���Ɠ����H���Ă���̂�������܂���
	m_texIndex = MV1GetMaterialDifMapTexture(m_fishModelHandle, 0);
	//���f���Ƀe�N�X�`����\��܂�
	MV1SetTextureGraphHandle(m_fishModelHandle, m_texIndex, m_TextureHandle, FALSE);

	velocity = VGet(0, 0, 0);

	//���̏�Ԃ��m�[�}���ɂ��܂�
	state = normal;
	//����HP��ݒ肵�܂�
	m_fishHP = rand()%800;
	if (m_fishHP < 100)
	{
		m_fishHP = 100;
	}
	m_startHP = m_fishHP;
	m_fishstrength = m_fishHP / 800;
	m_fishFretHandle= LoadGraph("data/img/gameScene/ase.png");

	m_fishSwimPosX = rand() % SCREEN_SIZE_W;
	m_fishSwimPosY = rand() % SCREEN_SIZE_H;

	//���f�����������̂ő傫�����܂�
	MV1SetScale(m_fishModelHandle, VGet(20+ m_fishHP/10, 20 + m_fishHP / 10, 20 + m_fishHP / 10));

	//m_fishOrbitEfk = new PlayEffect("data/effects/swim/kira.efk", 1);

}

Fish::~Fish()
{
	MV1DeleteModel(m_fishModelHandle);
	DeleteGraph(m_fishFretHandle);
	DeleteGraph(m_TextureHandle);

}

void Fish::Update(float _deltaTime, Player* m_player)
{
	//���̃A�C�R���𓧉߂��鏈��
	if (m_fishHP < 100)
	{
		if (m_FretFede > 255)
		{
			m_fedeFlag = false;
		}
		else if (m_FretFede < 1)
		{
			m_fedeFlag = true;
		}

		if (m_fedeFlag)
		{
			m_FretFede++;

		}
		else
		{
			m_FretFede--;
		}

	}


	MV1SetRotationXYZ(m_fishModelHandle, VGet(0.0f, m_fishSwim * DX_PI_F / 180.0f, 0.0f));

	
	if (m_fishSwim > 30)
	{
		m_fishSwimFlag = false;
	}
	else if(m_fishSwim <1)
	{
		m_fishSwimFlag = true;
	}

	if (m_fishSwimFlag)
	{
		m_fishSwim += m_fishSwimPower;

	}
	else
	{
		m_fishSwim -= m_fishSwimPower;

	}



	//�A�q��������Ԏ��ɋ�����͂����ɋ߂Â�
	if (m_player->GetState() == Player::landing && state== assault)
	{
		VECTOR Ppos = m_player->GetPos();	//�v���C���[�̍��W�̎擾
		VECTOR Fpos = VSub( Ppos , pos);	//�v���C���[�Ƌ��̍��W�̍������v�Z
		/*VECTOR Dot = VDot(Ppos, pos);*/	//�O�ς��g���ăv���C���[�̏ꏊ�ɋ��������悤�ɂ���
		VNorm(Fpos);

		float Angle = atan2(Fpos.x, Fpos.z);
		MV1SetRotationXYZ(m_fishModelHandle, VGet(0.0f, Angle + DX_PI_F / 2, 0.0f));

		//DrawFormatString(0, 500, GetColor(255, 255, 255), "sub:�w %f  sub:y %f  sub:z %f", Fpos.x, Fpos.y, Fpos.z);

		dir = Fpos;
		//VNorm(dir);
		//MV1SetRotationXYZ(m_fishModelHandle, dir);

		velocity = VScale(dir, 1);


		//velocity = VScale(velocity,deltaTime);
	}

	if (m_player->GetState() == Player::normal && state == assault)
	{
		state = normal;

	}

	if ((m_player->GetState() == Player::eat || m_player->GetState() == Player::inGame)&& state == assault)
	{
		state = normal;

	}

	//���������̂Ƃ���܂ł��ǂ�����Ƃ����̏�Ɏ~�܂�
	if (state == hit)
	{
		VECTOR swimPos = ConvScreenPosToWorldPos_ZLinear(VGet(m_fishSwimPosX, m_fishSwimPosY, SCREEN_SIZE_H - m_fishSwimPosY));

		VECTOR GorlDir = VSub(swimPos, pos);
		VNorm(GorlDir);

		float Angle = atan2(GorlDir.x, GorlDir.z);
		MV1SetRotationXYZ(m_fishModelHandle, VGet(0.0f, Angle + DX_PI_F / 2, 0.0f));

		if (swimPos.z > pos.z)
		{
			velocity = VScale(GorlDir, 2);
			//velocity = VScale(velocity,deltaTime);

		}
		else
		{
			velocity = VScale(GorlDir, 4);
			//velocity = VScale(velocity,deltaTime);

		}

		if (VSize(GorlDir) < 50)
		{
			m_fishSwimPosX = rand() % SCREEN_SIZE_W;
			m_fishSwimPosY = rand() % SCREEN_SIZE_H;

		}

		m_player->SetPos(pos);

	}
	//�����ނꂽ�狛�̃��f���̕`��Ǘ������Ă���t���O��off�ɂ���
	if (state == get)
	{
		velocity = VGet(0, 100, 0);

		pos.y += velocity.y*_deltaTime;


		if (pos.y > 80)
		{
			m_fishHP = rand() % 1500;
			m_startHP = m_fishHP;
			MV1SetScale(m_fishModelHandle, VGet(20 + m_fishHP / 10, 20 + m_fishHP / 10, 20 + m_fishHP / 10));

			state = result;
			//m_player->SetState(Player::normal);
			//srand((unsigned int)time(NULL));


		}

	}


	if (state == normal)
	{

		VECTOR swimPos = ConvScreenPosToWorldPos_ZLinear(VGet(m_fishSwimPosX, m_fishSwimPosY, SCREEN_SIZE_H - m_fishSwimPosY));
		VECTOR GorlDir = VSub(swimPos, pos);
		VNorm(GorlDir);

		float Angle = atan2(GorlDir.x, GorlDir.z);
		MV1SetRotationXYZ(m_fishModelHandle, VGet(0.0f, Angle + DX_PI_F / 2, 0.0f));
		velocity = VScale(GorlDir, 0.1);
		//velocity = VScale(velocity,deltaTime);
		if (VSize(GorlDir) < 50)
		{
			m_fishSwimPosX = rand() % SCREEN_SIZE_W;
			m_fishSwimPosY = rand() % SCREEN_SIZE_H;

		}


	}

	if (m_startHP < m_fishHP)//�����AHP�̏���𒴂��Ă��܂����猳�ɖ߂�
	{
		m_fishHP = m_startHP;
	}

	//velocity = VGet(velocity.x * _deltaTime, velocity.y * _deltaTime, velocity.z * _deltaTime);
	pos = VAdd(pos, VScale( velocity, _deltaTime));
	//velocity.x = velocity.x ;
	//velocity.y = velocity.y ;
	//velocity.z = velocity.z ;


	//pos = VAdd(pos, velocity);

	//pos.x = pos.x + velocity.x* _deltaTime;
	//pos.y = pos.y + velocity.y* _deltaTime;
	//pos.z = pos.z + velocity.z* _deltaTime;

	//���f���̃|�W�V�������Z�b�g���܂�
	MV1SetPosition(m_fishModelHandle, pos);

}

void Fish::Draw()
{
	MV1DrawModel(m_fishModelHandle);
	VECTOR FretHandle =  ConvWorldPosToScreenPos(pos);
	if (m_fishHP < 100)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_FretFede);

		DrawGraph(FretHandle.x+100, FretHandle.y-100, m_fishFretHandle, TRUE);//
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 225);
	}

	//if (m_fishOrbitEfk->GetNowPlaying() != 0)
	//{
	//	m_fishOrbitEfk->PlayEffekseer(VAdd(pos, VGet(0, 0, 0)));

	//}
	//m_fishOrbitEfk->SetPlayingEffectPos(pos);


	//�����蔻�肪������悤�ɕ`�悵�Ă��炤
	//DrawSphere3D(pos, hitRadius, 5, 0x00ffff, 0x00ffff, false);

	//DrawFormatString(0, 650, GetColor(255, 255, 255), "fade %d", m_FretFede);

	//DrawFormatString(0, 500, GetColor(255, 255, 255), "velocity:�w %f  velocity:y %f  velocity:z %f", velocity.x,velocity.y,velocity.z);

}

void Fish::OnHitFish()
{
	//state = hit;
}

void Fish::SetPos()
{
	m_fishSwimPosX = rand() % SCREEN_SIZE_W;
	m_fishSwimPosY = rand() % SCREEN_SIZE_H;

	pos = ConvScreenPosToWorldPos_ZLinear(VGet(m_startPosX, m_startPosY, SCREEN_SIZE_H - m_startPosY));

}
