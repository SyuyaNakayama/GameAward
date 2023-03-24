#include "Camera.h"
#include "Input.h"

void Camera::Initialize(WorldTransform* parent)
{
	worldTransform.Initialize();
	//WorldTransform::SetViewProjection(&viewProjection);
	worldTransform.translation = { 0,10.0f,-10.0f };
	viewProjection.farZ = 75.0f;
	worldTransform.parent = parent;
}

void Camera::Update()
{
	Vector2 cameraMove = 
	{
		Input::GetInstance()->Move(Key::Left,Key::Right,10.0f),
		Input::GetInstance()->Move(Key::Up,Key::Down,10.0f)
	};

	Vector2 scale = { 1.0f / WindowsAPI::WIN_SIZE.x, 1.0f / WindowsAPI::WIN_SIZE.y };
	Vector2 angle;
	angle = -Vector2((float)cameraMove.y * scale.x, (float)cameraMove.x * scale.y) * PI;

	// 追加回転分のQuaternionを生成
	Quaternion rotQNew;
	rotQNew = Quaternion::MakeAxisAngle(Vector3::MakeYAxis(), -angle.y);
	rotQNew *= Quaternion::MakeAxisAngle(Vector3::MakeXAxis(), -angle.x);

	// 累積のQuaternionを合成
	rotQ *= rotQNew;

	// 注視点から視点へのベクトルと、上方向ベクトル
	Vector3 vTargetEye = { 0.0f, 0.0f, -10 };
	Vector3 vUp = { 0.0f, 1.0f };

	// ベクトルを回転
	vTargetEye = Quaternion::RotateVector(vTargetEye, rotQ);
	vUp = Quaternion::RotateVector(vUp, rotQ);

	// 注視点からずらした位置に視点座標を決定
	viewProjection.target = worldTransform.parent->GetWorldPosition();
	viewProjection.eye = viewProjection.target + vTargetEye;
	viewProjection.up = vUp;

	worldTransform.Update();
	viewProjection.Update();
}