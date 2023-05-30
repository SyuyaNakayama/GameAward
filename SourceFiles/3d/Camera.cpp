#include "Camera.h"
#include "Input.h"
#include "Model.h"
#include <algorithm>
#include "SceneManager.h"

void Camera::Initialize(WorldTransform* parent)
{
	worldTransform.Initialize();
	Model::SetViewProjection(&viewProjection);
	worldTransform.translation = { 0,10.0f,-10.0f };
	viewProjection.farZ = 150.0f;
	worldTransform.parent = parent;
	viewProjection.Initialize();
	parentPrePos = parent->GetWorldPosition();
}

void Camera::Update()
{
	// 前フレーム座標取得
	prePos = worldTransform.GetWorldPosition();
	Vector2 cameraMove;

	Input* input = Input::GetInstance();
	if(!input->IsConnectGamePad())
	{
		cameraMove.x = Input::GetInstance()->Move(Key::Left, Key::Right, 10.0f);
		cameraMove.y = Input::GetInstance()->Move(Key::Up, Key::Down, 10.0f);
	}
	else
	{
		cameraMove = Input::GetInstance()->ConRStick(8.0f);
	}

	if (SceneManager::GetInstance()->GetNowScene() == Scene::Title)
	{
		cameraMove = {};
	}

	angle += cameraMove / 500.0f;
	angle.y = std::clamp(angle.y, -PI / 2.5f, PI / 2.5f);
	disToParent = std::clamp(disToParent, 0.1f, DISTANCE_MAX);
	if (disToParent < DISTANCE_MAX - 1.0e-4f)
	{
		float parentSpd = Length(parentPrePos - worldTransform.parent->GetWorldPosition());
		disToParent += parentSpd;
	}
	disToParent = std::clamp(disToParent, 0.1f, DISTANCE_MAX);
	Vector3 eyeVec = -Vector3::MakeAxis(Axis::Z) * Matrix4::Rotate({ angle.y,angle.x }) * disToParent;

	worldTransform.translation = eyeVec;
	worldTransform.Update();
	viewProjection.target = worldTransform.parent->GetWorldPosition();
	viewProjection.eye = worldTransform.GetWorldPosition();
	parentPrePos = worldTransform.parent->GetWorldPosition();
}