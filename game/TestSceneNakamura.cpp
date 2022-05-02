#include "TestSceneNakamura.h"
#include "Player.h"
#include "Hitchecker.h"
#include "Camera.h"
#include "DxLib.h"
#include "Effect.h"
#include "Result.h"
#include "Fish.h"
#include "manual.h"
#include "mouse.h"
#include "score.h"

//	�X�N���[���̃T�C�Y
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

//	�t�F�[�h�C���̑��x
const int FADE_IN_SPEED = 3;
//	�t�F�[�h�A�E�g�̑��x
const int FADE_OUT_SPEED = 3;

// ���ʒ���
const int GONG_VOLUME_PAL = 30;
const int DOOR_VOLUME_PAL = 40;

//�f�o�b�N�p�ł������ϐ��V���[�Y
const int FISH_HP = 1000;
const float PLAYER_HP = 1000.0f;
const int PIN_DOWN_POWER = 20;		//�s�����E�ɏオ���
const int PIN_UP_POWER = 10;		//�s�������ɉ������

const VECTOR FISH_ON_POS = { 0.0f, 0.0f, 0.0f };
const VECTOR FISH_TWO_POS = { -5.0f, 0.0f, 0.0f };
const VECTOR FISH_THREE_POS ={  5.0f, 0.0f, 1.0f  };		//���̂��ꂼ��̏����|�W�V����
const VECTOR FISH_FOUR_POS = { -3.0f, 0.0f, 10.0f };
const VECTOR FISH_FIVE_POS = { 1.0f, 0.0f, 8.0f   };

const int FISH_NUMBER = 5;								//���̑S���̐�

const float FISH_POWER = 1.5f;

const int PIN_POSX = SCREEN_SIZE_W / 4+200;


const float movePower = 300;
const float moveMax = 1000;
const float speedPower = 0.5f;
const float DownPin = 800;

const float HPdiminish = 300;
const float HPplayerDiminish = 5;

const float MiniTimer = 100;

TestSceneNakamura::TestSceneNakamura()
	:m_gameSceneFlag(false)
	,m_player(nullptr)
	,m_startMiniGameFlag(false)
	,m_pinNowPower(0)
	,m_hitDrawTimer(0)
	, HpGauge(1.0f)
	, m_playerDirFlag(false)
	, m_manualDrawFlag(false)
	, m_manualBigDrawFlag(false)
	, FishOverview(false)
	, m_resultFlag(false)
	, FishSize(0)
	, m_pinSpeed(0)
	, m_fadeInFinishFlag(false)
	, m_fadeOutFlag(false)
	, m_fadeOutFinishFlag(false)
	,FishSizeScore(0)
{
	m_pinPosX = PIN_POSX;		//�s���̈ʒu�����傤�ǃQ�[�W�̐^�񒆂ɗ���悤�ɂ���
	//m_fishHP = FISH_HP;						//���̋���HP�i�f�o�b�N�p�j�i���Ƃŋ���cpp�̕��ɈڐA����j
	m_playerHP = PLAYER_HP;				//�v���C���[��HP��ݒ�

	for (int i = 0; i < FISH_NUMBER; i++)
	{
		m_fish[i] = nullptr;
	}
	m_miniTimer = 0;


	m_fishGauge = 0.0f;

	m_pinMovePower = movePower;

	m_pinSpeed = speedPower;
}

TestSceneNakamura::~TestSceneNakamura()
{
	DeleteSoundMem(m_fishSoundHandle);
	DeleteSoundMem(m_BGMSoundHandle);
	delete m_player;	//	�v���C���[�̃|�C���^�����o�ϐ�������
	for (int i = 0; i < FISH_NUMBER; i++)
	{
		delete m_fish[i];
	}
}

SceneBase* TestSceneNakamura::Update(float _deltaTime)
{
	Score::MaxScore();

	for (int i = 0; i < FISH_NUMBER; i++)
	{
		m_fish[i]->Update(_deltaTime, m_player);
		HitChecker::Check(*m_player, *m_fish[i]);
		HitChecker::VisualCheck(*m_player, *m_fish[i]);

		if (m_fish[i]->GetState()==Fish::result)
		{
			m_resultFlag = true;
			if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0)
			{
				m_resultFlag = false;
				m_player->SetState(Player::normal);
				m_fish[i]->SetState(Fish::normal);
				m_fish[i]->SetPos();
				Score::AddScore(FishSize);

				if (FishSize > FishSizeScore)
				{
					FishSizeScore = FishSize;
				}

				m_pinMovePower = movePower;

				m_pinSpeed = speedPower;
				m_pinPosX = PIN_POSX;		//�s���̈ʒu�����傤�ǃQ�[�W�̐^�񒆂ɗ���悤�ɂ���

			}

		}

		if (m_startMiniGameFlag)
		{
			if (m_fish[i]->GetState() == Fish::hit)
			{

				m_miniTimer+= MiniTimer*_deltaTime;									//hit�̉摜���`�悳��n�߂���J�E���g�����
				if (m_miniTimer >= 200)						//��莞�Ԃ���������hit�̉摜�������ă~�j�Q�[�����n�܂�
				{
					m_startMiniGameFlag = false;
					m_miniTimer = 0;
					m_miniGameFlag = true;
					m_player->SetState(Player::inGame);
				}

			}
		}

		if (m_player->GetState() == Player::inGame)
		{
			if (m_fish[i]->GetState() == Fish::hit)
			{
				m_fishGauge = m_fish[i]->GetHitFishHP() / m_fish[i]->GetHitFishStartHP();

			}

			if (m_fish[i]->GetState() == Fish::hit)
			{
				//�l������������HP������A���������瑝����
				m_fish[i]->HitFishUpHp(m_pinNowPower * _deltaTime);

				//m_pinMovePower = FISH_POWER + m_fish[i]->GetHitFishStrength();

			}


		}


		//����HP���O�ȉ��ɂȂ�����ނ�グ��悤�ɂ���
		if (m_fish[i]->GetHitFishHP() <= 0 && m_fish[i]->GetState() == Fish::hit)
		{
			if ((GetMouseInput() & MOUSE_INPUT_RIGHT) != 0)
			{
				ChangeVolumeSoundMem(150, m_fishSoundHandle);
				PlaySoundMem(m_fishSoundHandle, DX_PLAYTYPE_BACK);


				FishOverview = true;
				m_miniGameFlag = false;
				m_player->SetState(Player::fish);
				m_fish[i]->SetState(Fish::get);
				FishSize = m_fish[i]->GetHitFishStartHP() / 10;
				if (FishSize == 0)
				{
					FishSize = 1;
				}
			}
		}

	}

	m_camera->Update();
	m_mouse->Update();
	if (!m_resultFlag)
	{
		HpGauge = m_playerHP / PLAYER_HP;	//����HP�̑S�̂��Q�[�W�̉摜�ɑ΂���S�̂̊����𒲂ׂ�
		m_playerHP = m_playerHP - HPplayerDiminish * _deltaTime;


	}


	if (SCREEN_SIZE_W - 168 < m_mouse->GetMouseClickPosX() &&
		SCREEN_SIZE_H - 221 < m_mouse->GetMouseClickPosY()
		&& m_player->GetState()== !Player::inGame)
	{
		m_manualBigDrawFlag = true;
		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0&&  m_player->GetState()== ! Player::inGame)
		{
			m_manualDrawFlag = true;
			m_player->SetState(Player::manual);
			//�}�j���A�����J�����Ƃ��v���C���[�͉����ł��Ȃ��悤�ɂ���

		}

	}
	else
	{
		m_manualBigDrawFlag = false;
	}

	if (m_manualDrawFlag)
	{
		m_manual->Update(_deltaTime);
		if (m_manual->GetImgNumber() == 9)
		{
			m_manual->SetImgNumber(0);
			m_manualDrawFlag = false;
			m_player->SetState(Player::normal);

		}

	}


	if (!m_resultFlag)
	{
		m_player->Update(_deltaTime, m_mouse);

	}

	//
	//�~�j�Q�[���̏���
	//

	if (m_player->GetState() == Player::eat)	//����hit������~�j�Q�[�����n�܂�
	{

		m_startMiniGameFlag = true;
		//m_player->SetState(Player::inGame);

	}

	//Hit�̕�������������~�j�Q�[�����n�܂��ăQ�[�W���o��
	if (m_player->GetState() == Player::inGame)
	{

		//�s�����Q�[�W�̘g�����E�ɍs���߂��Ă��܂����牟���߂�������
		if (m_pinPosX >= PIN_POSX + (502 / 2))
		{
			m_pinPosX = PIN_POSX + (502 / 2);
		}
		//�s�����Q�[�W�̘g�������ɍs���߂��Ă��܂����牟���߂�������
		if (m_pinPosX <= PIN_POSX - (502 / 2))
		{
			m_pinPosX = PIN_POSX - (502 / 2);
		}



		m_pinMovePower = m_pinMovePower + m_pinSpeed;

		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)//�~�j�Q�[�����ɍ��N���b�N�������
		{

			m_pinSpeed = m_pinSpeed + speedPower;
			m_pinMovePower = m_pinMovePower + m_pinSpeed;

			if (m_pinMovePower > moveMax)
			{
				m_pinMovePower = moveMax;
			}

			m_pinPosX += m_pinMovePower * _deltaTime;


		}
		else
		{
			m_pinMovePower = movePower;

			m_pinSpeed = speedPower;

			////m_pinSpeed = 0;	
			//m_pinMovePower = 0.8;
			m_pinPosX -= DownPin * _deltaTime;
		}

		m_pinNowPower = m_pinPosX - PIN_POSX;
		//m_pinNowPower = m_pinNowPower / 100;
		m_pinNowPower = m_pinNowPower * -1;
		//�s�����E�ɍs���߂�����v���C���[��HP������
		if (m_pinPosX >= PIN_POSX + (502 / 2))
		{
			m_playerHP-= HPdiminish * _deltaTime;
		}

	}


	if (m_fadeOutFinishFlag)
	{
		return new Result;        //���U���g�V�[���ɐ؂�ւ���
	}

	//if (m_gameSceneFlag)
	//{
	//	return new Result;        //���U���g�V�[���ɐ؂�ւ���
	//}
	return this;							//	�Q�[���V�[����\����������
}


void TestSceneNakamura::Draw()
{
	if (!m_fadeInFinishFlag)
	{
		// �t�F�[�h�C������
		for (int i = 0; i < 255; i += FADE_IN_SPEED)
		{
			// �`��P�x���Z�b�g
			SetDrawBright(i, i, i);
			DrawGraph(0, 0, m_backGraphHandle, TRUE);
			ScreenFlip();
		}
		m_fadeInFinishFlag = true;
	}


	DrawGraph(0, 0, m_backGraphHandle, TRUE);	//�w�i�̉摜
	//DrawBillboard3D(VGet(0,0,0), 0.5f, 0.5f, SCREEN_SIZE_W, 0.0f, m_backGraphHandle, true);

	m_player->Draw();
	for (int i = 0; i < FISH_NUMBER; i++)
	{
		m_fish[i]->Draw();
	}

	m_mouse->Draw();

	if (m_manualBigDrawFlag)
	{
		DrawExtendGraph(SCREEN_SIZE_W - 168 - 120, SCREEN_SIZE_H - 221 - 120,
			SCREEN_SIZE_W - 20, SCREEN_SIZE_H - 20,
			m_manualIconHandle, TRUE);

	}
	else
	{
		DrawExtendGraph(SCREEN_SIZE_W - 168 - 50, SCREEN_SIZE_H - 221 - 50,
			SCREEN_SIZE_W - 50, SCREEN_SIZE_H - 50,
			m_manualIconHandle, TRUE);

		if (m_player->GetState() == Player::inGame)
		{
			SetDrawBlendMode(DX_BLENDMODE_SUB, 122);

			DrawExtendGraph(SCREEN_SIZE_W - 168 - 50, SCREEN_SIZE_H - 221 - 50,
				SCREEN_SIZE_W - 50, SCREEN_SIZE_H - 50,
				m_manualIconHandle, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 225);

		}

	}

	DrawGraph(50, SCREEN_SIZE_H - 57 - 50, m_playerGaugeZeroHandle, TRUE);

	DrawRectGraph(50, SCREEN_SIZE_H - 57 - 50, 0, 0,
		HpGauge * 509.0f, 154, m_playerGaugeHandle, true, false);	//�~�j�Q�[���̃Q�[�W��\��������
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - HpGauge * 255);
	DrawRectGraph(50, SCREEN_SIZE_H - 57 - 50, 0, 0,
		HpGauge * 509.0f, 154, m_playerpinchHandle, true, false);	//�~�j�Q�[���̃Q�[�W��\��������
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 225);

	DrawGraph(200 + 150, SCREEN_SIZE_H - 57 - 220, m_scoreHandle[Score::GetScore() % 10], TRUE);//1����
	DrawGraph(200, SCREEN_SIZE_H - 57 - 220, m_scoreHandle[(Score::GetScore() % 100) / 10], TRUE);//2����
	DrawGraph(200 - 150, SCREEN_SIZE_H - 57 - 220, m_scoreHandle[Score::GetScore() / 100], TRUE);//2����

	if (m_startMiniGameFlag)
	{
		DrawGraph(0, 0, m_hitGraphHandle, TRUE);	//�~�j�Q�[���̃Q�[�W��\��������

	}


	for (int i = 0; i < FISH_NUMBER; i++)
	{

		//Hit�̕�������������~�j�Q�[�����n�܂��ăQ�[�W���o��
		if (m_player->GetState() == Player::inGame)
		{
			DrawGraph(PIN_POSX, 100, m_HPzeroGaugeHandle, TRUE);	//�~�j�Q�[���̃Q�[�W��\��������
			//DrawRectGraph(SCREEN_SIZE_W / 4, 0, 0, 0, m_playerHP, 154, m_miniGameGaugeHandle, true, false);	//�~�j�Q�[���̃Q�[�W��\��������
			DrawRectGraph(PIN_POSX, 100, 0, 0,
				m_fishGauge * 509.0f, 154, m_miniGameGaugeHandle, true, false);	//�~�j�Q�[���̃Q�[�W��\��������



			DrawGraph(m_pinPosX, 100, m_pinHandle, TRUE);				//�~�j�Q�[���̃s����\��������
			DrawGraph(0, 0, m_gaugeIconHandle, TRUE);				//�~�j�Q�[���̃s����\��������



			//����HP���O�ȉ��ɂȂ�����ނ�グ��悤�ɂ���
			if (m_fish[i]->GetHitFishHP() <= 0 && m_fish[i]->GetState() == Fish::hit)
			{
				DrawGraph(0, 0, m_mouseFishHandle, TRUE);
			}
		}
	}

	if (m_playerHP <= 0)
	{
		m_playerDirFlag = true;
	}

	//for (int i = 0; i < 2; i++)
	//{
	//	MV1DrawModel(hasuHandle[i]);
	//}

	//for (int i = 0; i < FISH_NUMBER; i++)
	//{
	//	DrawFormatString(0, 100 * i, GetColor(255, 255, 255), "%dfishHP %f", i,m_fish[i]->GetHitFishHP());
	//	DrawFormatString(0, 250 + i*100, GetColor(255, 255, 255), "%d start %f", i,m_fish[i]->GetHitFishStartHP());

	//}
	//DrawGraph(SCREEN_SIZE_W - 168, SCREEN_SIZE_H - 221, m_manualIconHandle, TRUE);
	if (m_manualDrawFlag)	//�{�^���͈̔͂ɃJ�[�\������������
	{
		m_manual->Draw();

	}


	if (m_resultFlag)
	{
		DrawGraph(0, 0, m_paperHandle, TRUE);//�ꌅ��
		//DrawGraph(0, 0, m_FishPrintHandle, TRUE);//�ꌅ��

		DrawExtendGraph(800 - 5 * FishSize, 300 - 5 * FishSize,
			800 + 255 + 5 * FishSize, 300 + 144 + 5 * FishSize,
			m_FishPrintHandle, TRUE);


		DrawGraph(800 + 150, 680, m_scoreHandle[FishSize % 10], TRUE);//�ꌅ��
		DrawGraph(800, 680, m_scoreHandle[(FishSize % 100)/10], TRUE);//2����
		DrawGraph(800 - 150, 680, m_scoreHandle[FishSize / 100], TRUE);//3����

		DrawGraph(800 + 300, 680, cmHandle, TRUE);//�ꌅ��
		if (FishSize > FishSizeScore)
		{
			DrawGraph(0, 0, m_highScore, TRUE);//�ꌅ��


		}

	}

	//FishSize = m_fish[0]->GetHitFishHP()/10;

	//DrawFormatString(0, 600, GetColor(255, 255, 255), "speed %f", HpGauge);
	//DrawFormatString(0, 700, GetColor(255, 255, 255), "move %f", m_miniTimer);
	//DrawFormatString(0, 700, GetColor(255, 255, 255), "size/10 %d", FishSize % 10);

	//DrawFormatString(0, 800, GetColor(255, 255, 255), "miniTimer %d", m_miniTimer);
	//DrawFormatString(0, 400, GetColor(255, 255, 255), "fishHP %d", m_fishHP);
	//DrawFormatString(0, 500, GetColor(255, 255, 255), "playerHP %f", m_playerHP);

		// �t�F�[�h�A�E�g����
	if (m_playerDirFlag)
	{
		for (int i = 255; i > 0; i -= FADE_IN_SPEED)
		{
			// �`��P�x���Z�b�g
			SetDrawBright(i, i, i);
			DrawGraph(0, 0, m_backGraphHandle, false);
			ScreenFlip();
		}
		m_fadeOutFinishFlag = true;
	}

}

void TestSceneNakamura::Sound()
{
	ChangeVolumeSoundMem(150, m_BGMSoundHandle);
	PlaySoundMem(m_BGMSoundHandle, DX_PLAYTYPE_LOOP,false);

	if (m_resultFlag)
	{
		if (FishSize < 50 && CheckSoundMem(m_fishResultSound01Handle) == 0)
		{
			ChangeVolumeSoundMem(150, m_fishResultSound01Handle);
			PlaySoundMem(m_fishResultSound01Handle, DX_PLAYTYPE_BACK);

		}
		if (FishSize >= 50 && CheckSoundMem(m_fishResultSound02Handle) == 0 && FishSize < 100)
		{
			ChangeVolumeSoundMem(150, m_fishResultSound02Handle);
			PlaySoundMem(m_fishResultSound02Handle, DX_PLAYTYPE_BACK);

		}
		if (FishSize >= 100 && CheckSoundMem(m_fishResultSound03Handle) == 0 && FishSize > 50)
		{
			ChangeVolumeSoundMem(150, m_fishResultSound03Handle);
			PlaySoundMem(m_fishResultSound03Handle, DX_PLAYTYPE_BACK);

		}


	}
	else
	{
		StopSoundMem(m_fishResultSound01Handle);
		StopSoundMem(m_fishResultSound02Handle);
		StopSoundMem(m_fishResultSound03Handle);

	}

}

void TestSceneNakamura::Load()
{

	m_backGraphHandle = LoadGraph("data/img/gameScene/GameScene.png");	//�w�i�̉摜
	m_hitGraphHandle= LoadGraph("data/img/gameScene/Hit02.png");			//hit�\�L������摜

	m_miniGameGaugeHandle = LoadGraph("data/img/gameScene/�Q�[�W.png");	//�~�j�Q�[���̃Q�[�W
	m_HPzeroGaugeHandle= LoadGraph("data/img/gameScene/�Q�[�W0.png");		//HP���[���̃Q�[�W�摜
	m_pinHandle = LoadGraph("data/img/gameScene/pin.png");				//�~�j�Q�[���̃s���̉摜
	m_mouseFishHandle= LoadGraph("data/img/gameScene/mouse.png");

	m_manualIconHandle = LoadGraph("data/img/gameScene/�}�j���A���A�C�R��.png");

	LoadDivGraph("data/img/gameScene/mapchip02.png", 10, 10, 1, 150, 150, m_scoreHandle);
	LoadDivGraph("data/img/GameOver/mapchip02.png", 10, 10, 1, 150, 150, m_fishScoreHandle);

	m_paperHandle = LoadGraph("data/img/Fish/kami01.png");	//�w�i�̉摜
	m_FishPrintHandle = LoadGraph("data/img/Fish/gyotaku03.png");	//�w�i�̉摜

	cmHandle = LoadGraph("data/img/gameScene/����.png");	//�w�i�̉摜

	m_fishSoundHandle = LoadSoundMem("data/sound/fish/���E���ԁ[��04.mp3");
	m_BGMSoundHandle = LoadSoundMem("data/sound/fish/VSQSE_1041_river_22.mp3");

	m_fishResultSound01Handle = LoadSoundMem("data/sound/fish/�ӂE�E���H.mp3");
	m_fishResultSound02Handle = LoadSoundMem("data/sound/fish/get.mp3");
	m_fishResultSound03Handle = LoadSoundMem("data/sound/fish/�a���T�E���h���S��ʓ]��001.mp3");


	m_gaugeIconHandle = LoadGraph("data/img/gameScene/�Q�[�W�A�C�R��.png");

	m_playerGaugeHandle = LoadGraph("data/img/gameScene/�v���C���[�Q�[�W.png");
	m_playerGaugeZeroHandle = LoadGraph("data/img/gameScene/�v���C���[�Q�[�W�O.png");
	m_playerpinchHandle = LoadGraph("data/img/gameScene/�v���C���[�s���`.png");
	m_highScore = LoadGraph("data/img/Fish/�n�C�X�R�A.png");
	////��n�X�̗t��`�悵����
	//for (int i = 0; i < 2; i++)
	//{
	//	//�n�X�̃��f�������[�h����
	//	hasuHandle[i] = MV1LoadModel("data/model/hasu/hasu.mv1");

	//	//��]������p�x * DX_PI_F / 180.0f�Ŋp�x�������ł���
	//	//MV1SetRotationXYZ(hasuHandle[i], VGet(-50.0f * DX_PI_F / 180.0f, 0.0f, 0.0f));
	//	// 
	//	//�n�X�̃��f���̃e�N�X�`����ǂݍ���
	//	m_lotusTextureHandle = LoadGraph("data/img/Texture/lotus.png");
	//	//�e�N�X�`���̔ԍ��H�����f���Ɠ����H���Ă���̂�������܂���
	//	m_texIndex = MV1GetMaterialDifMapTexture(hasuHandle[i], 0);
	//	//���f���Ƀe�N�X�`����\��܂�
	//	MV1SetTextureGraphHandle(hasuHandle[i], m_texIndex, m_lotusTextureHandle, FALSE);
	//}

	////�n�X�̃��f�����������̂Ŋg�債�܂�
	//MV1SetScale(hasuHandle[0], VGet(200, 200, 200));
	//MV1SetScale(hasuHandle[1], VGet(200, 200, 200));

	////�|�W�V�������Z�b�g����
	//MV1SetPosition(hasuHandle[0], VGet(50, -150, -10));
	//MV1SetPosition(hasuHandle[1], VGet(-80, -150, 60));

	//m_fish[0] = new Fish(VGet(0.0f, 0.0f, 0.0f));
	//m_fish[1] = new Fish(VGet(-50.0f, 0.0f, 0.0f));
	//m_fish[2] = new Fish(VGet(50.0f, 0.0f, 10.0f));
	//m_fish[3] = new Fish(VGet(-30.0f, 0.0f, 100.0f));
	//m_fish[4] = new Fish(VGet(10.0f, 0.0f, 80.0f));



	m_player = new Player;
	m_camera = new Camera;
	m_manual = new Manual;
	m_mouse = new Mouse;

	m_fish[0] = new Fish(100, 500);
	m_fish[1] = new Fish(1000, 800);
	m_fish[2] = new Fish(500, 200);
	m_fish[3] = new Fish(1500, 400);
	m_fish[4] = new Fish(700, 800);

}

void TestSceneNakamura::DebugKey()
{
	// �m�F�p
	if (CheckHitKey(KEY_INPUT_A))
	{
	}
	if (CheckHitKey(KEY_INPUT_B))
	{
	}
	if (CheckHitKey(KEY_INPUT_RETURN))
	{
	}
}
