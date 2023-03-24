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

	// �ǉ���]����Quaternion�𐶐�
	Quaternion rotQNew;
	rotQNew = Quaternion::MakeAxisAngle(Vector3::MakeYAxis(), -angle.y);
	rotQNew *= Quaternion::MakeAxisAngle(Vector3::MakeXAxis(), -angle.x);

	// �ݐς�Quaternion������
	rotQ *= rotQNew;

	// �����_���王�_�ւ̃x�N�g���ƁA������x�N�g��
	Vector3 vTargetEye = { 0.0f, 0.0f, -10 };
	Vector3 vUp = { 0.0f, 1.0f };

	// �x�N�g������]
	vTargetEye = Quaternion::RotateVector(vTargetEye, rotQ);
	vUp = Quaternion::RotateVector(vUp, rotQ);

	// �����_���炸�炵���ʒu�Ɏ��_���W������
	viewProjection.target = worldTransform.parent->GetWorldPosition();
	viewProjection.eye = viewProjection.target + vTargetEye;
	viewProjection.up = vUp;

	worldTransform.Update();
	viewProjection.Update();
}