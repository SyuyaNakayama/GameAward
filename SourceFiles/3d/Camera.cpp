#include "Camera.h"
#include "Input.h"
#include "ImGuiManager.h"
#include "Model.h"
#include <algorithm>

void Camera::Initialize(WorldTransform* parent)
{
	worldTransform.Initialize();
	Model::SetViewProjection(&viewProjection);
	worldTransform.translation = { 0,10.0f,-10.0f };
	viewProjection.farZ = 150.0f;
	worldTransform.parent = parent;
	viewProjection.Initialize();
}

void Camera::Update()
{
	Vector2 cameraMove =
	{
		Input::GetInstance()->Move(Key::Left, Key::Right, 10.0f),
		Input::GetInstance()->Move(Key::Up, Key::Down, 10.0f)
	};

	angle += cameraMove / 500.0f;
	angle.y = std::clamp(angle.y, -PI / 2.5f, PI / 2.5f);
	Vector3 eyeVec = -Vector3::MakeAxis(Axis::Z) * Matrix4::Rotate({ angle.y,angle.x }) * 20.0f;

	worldTransform.translation = eyeVec;
	worldTransform.Update();
	viewProjection.target = worldTransform.parent->GetWorldPosition();
	viewProjection.eye = worldTransform.GetWorldPosition();
}