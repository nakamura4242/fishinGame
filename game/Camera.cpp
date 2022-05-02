
//-----------------------------------------------------------------------------
// @brief  カメラクラス.
// 2016 Takeru Yui All Rights Reserved.
//-----------------------------------------------------------------------------
#include "Player.h"
#include "Camera.h"

#define USE_LERP_CAMERA 1

#define SCREEN_SIZE_W 1920
#define SCREEN_SIZE_H 1080


VECTOR Camera::dir = VGet(0, 0, 0);
bool Camera::m_compeDiveFlag = false;

//-----------------------------------------------------------------------------
// @brief  コンストラクタ.
//-----------------------------------------------------------------------------
Camera::Camera()
	:m_cameraPosX(0.0f)
	, m_cameraPosY(0.0f)
	, m_cameraPosZ(0.0f)
	, m_playerState(0)
{
	//奥行0.1～1000までをカメラの描画範囲とする
	// ３D空間に何かを描画する際に、カメラからどれだけ離れたところ（Near）から、
	// どこまで（Far）のものを描画するかを設定する(FOV)
	SetCameraNearFar(0.50f, 150);

	// Vgetはベクトルの取得
	// 原点の値をセット
	pos = VGet(0, 0, 0);

	// lerpを使用して実装.
// lerp(VECTOR a, VECTOR b, float t)は
// answer = a + ((b-a) * t)
	VECTOR aimPos = VAdd(VGet(0, 0, 0), VScale(VGet(0, -1.5, 1), -50));

	aimPos = VAdd(aimPos, VGet(0, 0.0f, 0));
	VECTOR posToAim = VSub(aimPos, pos);
	VECTOR scaledPosToAim = VScale(posToAim, 0.1f);
	pos = VAdd(pos, scaledPosToAim);

	SetCameraPositionAndTarget_UpVecY(pos, VGet(0, 0, 0));

}

//-----------------------------------------------------------------------------
// @brief  デストラクタ.
//-----------------------------------------------------------------------------
Camera::~Camera()
{
	// 処理なし.
}

//-----------------------------------------------------------------------------
// @brief  更新.
//-----------------------------------------------------------------------------
void Camera::Update()
{
#if !USE_LERP_CAMERA
	// z軸上で、プレイヤーから一定距離離れた状態でプレイヤーを常に見続けるよう位置調整
	//pos = VGet(0, player.GetPos().y + 20.0f, player.GetPos().z - 30.0f);

#else
#endif

}
