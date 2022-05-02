//-----------------------------------------------------------------------------
// @brief  メイン処理.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "DxLib.h"
#include "Camera.h"
#include "SceneMgr.h"
#include "Title.h"
#include "EffekseerForDXLib.h"
#include "TestSceneNakamura.h"
#include "Result.h"

void InitializeEffekseer();

//-----------------------------------------------------------------------------
// @brief  メイン関数.
//-----------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) 
{
	// ＤＸライブラリ初期化処理
	if (DxLib_Init() == -1)		
	{
		return -1;	// エラーが起きたら直ちに終了
	}
	//InitializeEffekseer();
	// 画面モードのセット
	SetGraphMode(1920, 1080, 16);
	ChangeWindowMode(TRUE);
	SetDrawScreen(DX_SCREEN_BACK);		//	裏画面の設定


	InitializeEffekseer();

	//	デルタタイム管理用の変数をセット
	LONGLONG NowTime;
	LONGLONG Time;
	float DeltaTime;
	int FPS;
	int FPSCounter;
	LONGLONG FPSCheckTime;


	//	システム時間を取得
	Time = GetNowHiPerformanceCount();

	//	最初の経過時間は仮に0.0000001f秒にしておく
	DeltaTime = 0.000001f;

	// FPS計測関係の初期化
	FPSCheckTime = GetNowHiPerformanceCount();
	FPS = 0;
	FPSCounter = 0;



	//	シーンマネージャークラスのインスタンスを生成
	SceneMgr* Scene = new SceneMgr;

	//	タイトルシーンをセット
	Scene->SetScene(new Title);
	//Scene->SetScene(new TestSceneNakamura);
	////Scene->SetScene(new Result);

	// エスケープキーが押されるかウインドウが閉じられるまでループ
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		//	画面消去
		ClearDrawScreen();

		Effekseer_Sync3DSetting();
		// シーン制御
		Scene->Update(DeltaTime);

		// 描画処理
		Scene->Draw();

		// FPSの描画
		//DrawFormatString(0, 0, GetColor(255, 255, 255), "FPS:%d", FPS);
		//// FPSの描画
		//DrawFormatString(0, 50, GetColor(255, 255, 255), "deltaTime:%f", DeltaTime);


		//// Effekseer側のカメラとDxライブラリ側のカメラを同期する
		//Effekseer_Sync3DSetting();
		// Effekseerの更新
		UpdateEffekseer3D();
		// Effekseerの描画
		DrawEffekseer3D();


		//// Effekseerにより再生中のエフェクトを更新する。
		//UpdateEffekseer2D();

		//// Effekseerにより再生中のエフェクトを描画する。
		//DrawEffekseer2D();

		// BGM処理
		Scene->Sound();

		// 裏画面の内容を表画面に反映させる
		ScreenFlip();

		//	現在のシステム時間を取得
		NowTime = GetNowHiPerformanceCount();

		// 前回取得した時間からの経過時間を秒に変換してセット
		// ( GetNowHiPerformanceCount で取得できる値はマイクロ秒単位なので 1000000 で割ることで秒単位になる )
		DeltaTime = (NowTime - Time) / 1000000.0f;

		//	今回取得した時間を保存
		Time = NowTime;

		// FPS関係の処理( 1秒経過する間に実行されたメインループの回数を FPS とする )
		FPSCounter++;
		if (NowTime - FPSCheckTime > 1000000)
		{
			FPS = FPSCounter;
			FPSCounter = 0;
			FPSCheckTime = NowTime;
		}


	}

	// シーンを削除
	delete(Scene);

	// Effekseerの終了
	Effkseer_End();
	// ＤＸライブラリの後始末
	DxLib_End();

	// ソフトの終了
	return 0;
}

/// <summary>
/// Effekseerの初期化
/// </summary>
void InitializeEffekseer()
{
	// DXライブラリとEffekseerの初期化処理
	if (Effekseer_Init(8000) == -1)
	{
		printf("Effekseer初期化に失敗！\n");			                              // エラーが起きたら直ちに終了
	}

	//---------------------------------------------------+
	// Effekseer関連の初期化
	//---------------------------------------------------+
	SetUseDirect3DVersion(DX_DIRECT3D_11);                    // DirectX11を使用
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	SetUseZBuffer3D(TRUE);                                    // ZBufferを使用
	SetWriteZBuffer3D(TRUE);                                  // ZBufferへの書き込みを許可
	//Effekseer_Set2DSetting(1920, 1080);

}
