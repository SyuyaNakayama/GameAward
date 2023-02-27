#include "DebugCamera.h"
#include "Input.h"
#include "WindowsAPI.h"

void DebugCamera::Initialize(Vector3 targetPos, float distance_)
{
	distance = distance_;
	viewProjection.target = viewProjection.eye = targetPos;
	viewProjection.eye.z -= distance;
}

void DebugCamera::Update()
{
	bool dirty = false;
	Vector2 angle;
	Vector2 scale = { 1.0f / WindowsAPI::WIN_SIZE.x, 1.0f / WindowsAPI::WIN_SIZE.y };
	Input* input = Input::GetInstance();

	// マウスの入力を取得
	Input::MouseMove mouseMove = input->GetMouseMove();

	// マウスの左ボタンが押されていたらカメラを回転させる
	if (input->IsInput(Mouse::Left))
	{
		angle = -Vector2(mouseMove.lY * scale.x, mouseMove.lX * scale.y) * PI;
		dirty = true;
	}

	// マウスの中ボタンが押されていたらカメラを並行移動させる
	if (input->IsInput(Mouse::Middle))
	{
		Vector3 move = Vector3(-mouseMove.lX, mouseMove.lY) / 100.0f;
		move = Quaternion::RotateVector(move, rotQ);

		viewProjection.CameraMove(move);
		dirty = true;
	}

	// ホイール入力で距離を変更
	if (mouseMove.lZ != 0)
	{
		distance -= mouseMove.lZ / 100.0f;
		distance = max(distance, 1.0f);
		dirty = true;
	}

	if (dirty)
	{
		// 追加回転分のQuaternionを生成
		Quaternion rotQNew;
		rotQNew = Quaternion::MakeAxisAngle(Vector3::MakeYAxis(), -angle.y);
		rotQNew *= Quaternion::MakeAxisAngle(Vector3::MakeXAxis(), -angle.x);

		// 累積のQuaternionを合成
		rotQ *= rotQNew;

		// 注視点から視点へのベクトルと、上方向ベクトル
		Vector3 vTargetEye = { 0.0f, 0.0f, -distance };
		Vector3 vUp = { 0.0f, 1.0f };

		// ベクトルを回転
		vTargetEye = Quaternion::RotateVector(vTargetEye, rotQ);
		vUp = Quaternion::RotateVector(vUp, rotQ);

		// 注視点からずらした位置に視点座標を決定
		viewProjection.eye = viewProjection.target + vTargetEye;
		viewProjection.up = vUp;
	}
	viewProjection.Update();
}