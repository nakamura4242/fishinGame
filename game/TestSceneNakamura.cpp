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

//	スクリーンのサイズ
const int SCREEN_SIZE_W = 1920;
const int SCREEN_SIZE_H = 1080;

//	フェードインの速度
const int FADE_IN_SPEED = 3;
//	フェードアウトの速度
const int FADE_OUT_SPEED = 3;

// 音量調整
const int GONG_VOLUME_PAL = 30;
const int DOOR_VOLUME_PAL = 40;

//デバック用でつくった変数シリーズ
const int FISH_HP = 1000;
const float PLAYER_HP = 1000.0f;
const int PIN_DOWN_POWER = 20;		//ピンが右に上がる力
const int PIN_UP_POWER = 10;		//ピンが左に下がる力

const VECTOR FISH_ON_POS = { 0.0f, 0.0f, 0.0f };
const VECTOR FISH_TWO_POS = { -5.0f, 0.0f, 0.0f };
const VECTOR FISH_THREE_POS ={  5.0f, 0.0f, 1.0f  };		//魚のそれぞれの初期ポジション
const VECTOR FISH_FOUR_POS = { -3.0f, 0.0f, 10.0f };
const VECTOR FISH_FIVE_POS = { 1.0f, 0.0f, 8.0f   };

const int FISH_NUMBER = 5;								//魚の全員の数

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
	m_pinPosX = PIN_POSX;		//ピンの位置がちょうどゲージの真ん中に来るようにする
	//m_fishHP = FISH_HP;						//仮の魚のHP（デバック用）（あとで魚のcppの方に移植する）
	m_playerHP = PLAYER_HP;				//プレイヤーのHPを設定

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
	delete m_player;	//	プレイヤーのポインタメンバ変数を消去
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
				m_pinPosX = PIN_POSX;		//ピンの位置がちょうどゲージの真ん中に来るようにする

			}

		}

		if (m_startMiniGameFlag)
		{
			if (m_fish[i]->GetState() == Fish::hit)
			{

				m_miniTimer+= MiniTimer*_deltaTime;									//hitの画像が描画され始めたらカウントされる
				if (m_miniTimer >= 200)						//一定時間がたったらhitの画像が消えてミニゲームが始まる
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
				//値が負だったらHPが減る、正だったら増える
				m_fish[i]->HitFishUpHp(m_pinNowPower * _deltaTime);

				//m_pinMovePower = FISH_POWER + m_fish[i]->GetHitFishStrength();

			}


		}


		//魚のHPが０以下になったら釣り上げるようにする
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
		HpGauge = m_playerHP / PLAYER_HP;	//今のHPの全体がゲージの画像に対する全体の割合を調べる
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
			//マニュアルを開いたときプレイヤーは何もできないようにする

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
	//ミニゲームの処理
	//

	if (m_player->GetState() == Player::eat)	//魚がhitしたらミニゲームが始まる
	{

		m_startMiniGameFlag = true;
		//m_player->SetState(Player::inGame);

	}

	//Hitの文字が消えたらミニゲームが始まってゲージが出る
	if (m_player->GetState() == Player::inGame)
	{

		//ピンがゲージの枠よりも右に行き過ぎてしまったら押し戻しをする
		if (m_pinPosX >= PIN_POSX + (502 / 2))
		{
			m_pinPosX = PIN_POSX + (502 / 2);
		}
		//ピンがゲージの枠よりも左に行き過ぎてしまったら押し戻しをする
		if (m_pinPosX <= PIN_POSX - (502 / 2))
		{
			m_pinPosX = PIN_POSX - (502 / 2);
		}



		m_pinMovePower = m_pinMovePower + m_pinSpeed;

		if ((GetMouseInput() & MOUSE_INPUT_LEFT) != 0)//ミニゲーム中に左クリックをすると
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
		//ピンが右に行き過ぎたらプレイヤーのHPが減る
		if (m_pinPosX >= PIN_POSX + (502 / 2))
		{
			m_playerHP-= HPdiminish * _deltaTime;
		}

	}


	if (m_fadeOutFinishFlag)
	{
		return new Result;        //リザルトシーンに切り替える
	}

	//if (m_gameSceneFlag)
	//{
	//	return new Result;        //リザルトシーンに切り替える
	//}
	return this;							//	ゲームシーンを表示し続ける
}


void TestSceneNakamura::Draw()
{
	if (!m_fadeInFinishFlag)
	{
		// フェードイン処理
		for (int i = 0; i < 255; i += FADE_IN_SPEED)
		{
			// 描画輝度をセット
			SetDrawBright(i, i, i);
			DrawGraph(0, 0, m_backGraphHandle, TRUE);
			ScreenFlip();
		}
		m_fadeInFinishFlag = true;
	}


	DrawGraph(0, 0, m_backGraphHandle, TRUE);	//背景の画像
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
		HpGauge * 509.0f, 154, m_playerGaugeHandle, true, false);	//ミニゲームのゲージを表示させる
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 - HpGauge * 255);
	DrawRectGraph(50, SCREEN_SIZE_H - 57 - 50, 0, 0,
		HpGauge * 509.0f, 154, m_playerpinchHandle, true, false);	//ミニゲームのゲージを表示させる
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 225);

	DrawGraph(200 + 150, SCREEN_SIZE_H - 57 - 220, m_scoreHandle[Score::GetScore() % 10], TRUE);//1桁目
	DrawGraph(200, SCREEN_SIZE_H - 57 - 220, m_scoreHandle[(Score::GetScore() % 100) / 10], TRUE);//2桁目
	DrawGraph(200 - 150, SCREEN_SIZE_H - 57 - 220, m_scoreHandle[Score::GetScore() / 100], TRUE);//2桁目

	if (m_startMiniGameFlag)
	{
		DrawGraph(0, 0, m_hitGraphHandle, TRUE);	//ミニゲームのゲージを表示させる

	}


	for (int i = 0; i < FISH_NUMBER; i++)
	{

		//Hitの文字が消えたらミニゲームが始まってゲージが出る
		if (m_player->GetState() == Player::inGame)
		{
			DrawGraph(PIN_POSX, 100, m_HPzeroGaugeHandle, TRUE);	//ミニゲームのゲージを表示させる
			//DrawRectGraph(SCREEN_SIZE_W / 4, 0, 0, 0, m_playerHP, 154, m_miniGameGaugeHandle, true, false);	//ミニゲームのゲージを表示させる
			DrawRectGraph(PIN_POSX, 100, 0, 0,
				m_fishGauge * 509.0f, 154, m_miniGameGaugeHandle, true, false);	//ミニゲームのゲージを表示させる



			DrawGraph(m_pinPosX, 100, m_pinHandle, TRUE);				//ミニゲームのピンを表示させる
			DrawGraph(0, 0, m_gaugeIconHandle, TRUE);				//ミニゲームのピンを表示させる



			//魚のHPが０以下になったら釣り上げるようにする
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
	if (m_manualDrawFlag)	//ボタンの範囲にカーソルが入ったら
	{
		m_manual->Draw();

	}


	if (m_resultFlag)
	{
		DrawGraph(0, 0, m_paperHandle, TRUE);//一桁目
		//DrawGraph(0, 0, m_FishPrintHandle, TRUE);//一桁目

		DrawExtendGraph(800 - 5 * FishSize, 300 - 5 * FishSize,
			800 + 255 + 5 * FishSize, 300 + 144 + 5 * FishSize,
			m_FishPrintHandle, TRUE);


		DrawGraph(800 + 150, 680, m_scoreHandle[FishSize % 10], TRUE);//一桁目
		DrawGraph(800, 680, m_scoreHandle[(FishSize % 100)/10], TRUE);//2桁目
		DrawGraph(800 - 150, 680, m_scoreHandle[FishSize / 100], TRUE);//3桁目

		DrawGraph(800 + 300, 680, cmHandle, TRUE);//一桁目
		if (FishSize > FishSizeScore)
		{
			DrawGraph(0, 0, m_highScore, TRUE);//一桁目


		}

	}

	//FishSize = m_fish[0]->GetHitFishHP()/10;

	//DrawFormatString(0, 600, GetColor(255, 255, 255), "speed %f", HpGauge);
	//DrawFormatString(0, 700, GetColor(255, 255, 255), "move %f", m_miniTimer);
	//DrawFormatString(0, 700, GetColor(255, 255, 255), "size/10 %d", FishSize % 10);

	//DrawFormatString(0, 800, GetColor(255, 255, 255), "miniTimer %d", m_miniTimer);
	//DrawFormatString(0, 400, GetColor(255, 255, 255), "fishHP %d", m_fishHP);
	//DrawFormatString(0, 500, GetColor(255, 255, 255), "playerHP %f", m_playerHP);

		// フェードアウト処理
	if (m_playerDirFlag)
	{
		for (int i = 255; i > 0; i -= FADE_IN_SPEED)
		{
			// 描画輝度をセット
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

	m_backGraphHandle = LoadGraph("data/img/gameScene/GameScene.png");	//背景の画像
	m_hitGraphHandle= LoadGraph("data/img/gameScene/Hit02.png");			//hit表記をする画像

	m_miniGameGaugeHandle = LoadGraph("data/img/gameScene/ゲージ.png");	//ミニゲームのゲージ
	m_HPzeroGaugeHandle= LoadGraph("data/img/gameScene/ゲージ0.png");		//HPがゼロのゲージ画像
	m_pinHandle = LoadGraph("data/img/gameScene/pin.png");				//ミニゲームのピンの画像
	m_mouseFishHandle= LoadGraph("data/img/gameScene/mouse.png");

	m_manualIconHandle = LoadGraph("data/img/gameScene/マニュアルアイコン.png");

	LoadDivGraph("data/img/gameScene/mapchip02.png", 10, 10, 1, 150, 150, m_scoreHandle);
	LoadDivGraph("data/img/GameOver/mapchip02.png", 10, 10, 1, 150, 150, m_fishScoreHandle);

	m_paperHandle = LoadGraph("data/img/Fish/kami01.png");	//背景の画像
	m_FishPrintHandle = LoadGraph("data/img/Fish/gyotaku03.png");	//背景の画像

	cmHandle = LoadGraph("data/img/gameScene/ｃｍ.png");	//背景の画像

	m_fishSoundHandle = LoadSoundMem("data/sound/fish/水・ざぶーん04.mp3");
	m_BGMSoundHandle = LoadSoundMem("data/sound/fish/VSQSE_1041_river_22.mp3");

	m_fishResultSound01Handle = LoadSoundMem("data/sound/fish/ふつ・・う？.mp3");
	m_fishResultSound02Handle = LoadSoundMem("data/sound/fish/get.mp3");
	m_fishResultSound03Handle = LoadSoundMem("data/sound/fish/和風サウンドロゴ場面転換001.mp3");


	m_gaugeIconHandle = LoadGraph("data/img/gameScene/ゲージアイコン.png");

	m_playerGaugeHandle = LoadGraph("data/img/gameScene/プレイヤーゲージ.png");
	m_playerGaugeZeroHandle = LoadGraph("data/img/gameScene/プレイヤーゲージ０.png");
	m_playerpinchHandle = LoadGraph("data/img/gameScene/プレイヤーピンチ.png");
	m_highScore = LoadGraph("data/img/Fish/ハイスコア.png");
	////二個ハスの葉を描画したい
	//for (int i = 0; i < 2; i++)
	//{
	//	//ハスのモデルをロードする
	//	hasuHandle[i] = MV1LoadModel("data/model/hasu/hasu.mv1");

	//	//回転させる角度 * DX_PI_F / 180.0fで角度調整ができる
	//	//MV1SetRotationXYZ(hasuHandle[i], VGet(-50.0f * DX_PI_F / 180.0f, 0.0f, 0.0f));
	//	// 
	//	//ハスのモデルのテクスチャを読み込む
	//	m_lotusTextureHandle = LoadGraph("data/img/Texture/lotus.png");
	//	//テクスチャの番号？をモデルと統合？しているのかもしれません
	//	m_texIndex = MV1GetMaterialDifMapTexture(hasuHandle[i], 0);
	//	//モデルにテクスチャを貼ります
	//	MV1SetTextureGraphHandle(hasuHandle[i], m_texIndex, m_lotusTextureHandle, FALSE);
	//}

	////ハスのモデルが小さいので拡大します
	//MV1SetScale(hasuHandle[0], VGet(200, 200, 200));
	//MV1SetScale(hasuHandle[1], VGet(200, 200, 200));

	////ポジションをセットする
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
	// 確認用
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
