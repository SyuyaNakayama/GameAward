#include "Camera.h"
#include "Input.h"
#include "Quaternion.h"

void Camera::Initialize()
{
	worldTransform.Initialize();
	//WorldTransform::SetViewProjection(&viewProjection);
}

void Camera::Update()
{
	Input::MouseMove mouseMove = Input::GetInstance()->GetMouseMove();
	angleTarget += (float)mouseMove.lX / 100.0f;
	Quaternion rotQ = Quaternion::MakeAxisAngle(Vector3::MakeYAxis(), angleTarget);
	Vector3 forward = Normalize(Quaternion::RotateVector(Vector3::MakeZAxis(), rotQ));

	worldTransform.Update();
	viewProjection.eye = worldTransform.GetWorldPosition();
	viewProjection.target = viewProjection.eye + forward;
	viewProjection.Update();
}
