#include "GamePlayScene.h"
#include <imgui.h>
#include "SpriteCommon.h"
#include "CollisionManager.h"
#include "ImGuiManager.h"

void GamePlayScene::Initialize()
{
	lightGroup = LightGroup::Create();
	for (size_t i = 0; i < LightGroup::DIR_LIGHT_NUM; i++)
	{
		//lightGroup->SetDirLightActive(i, false);
	}
	debugCamera.Initialize();
	WorldTransform::SetViewProjection(&debugCamera.GetViewProjection());
	Model::SetLightGroup(lightGroup.get());

	skydome.Initialize(100.0f);
	stage.Initialize(player.GetIsLight());
	player.Initialize(stage.GetStartPos());
	player.SetStageSize(stage.GetStageSize());
	input = Input::GetInstance();

	//�X�e�[�W�J�n�̃J�����̏����ʒu
	viewProjection.target = stage.GetDoorPos();
	viewProjection.eye = stage.GetDoorPos() + Vector3{ 0,10,-15 };
	viewProjection.farZ = 1500.0f;
}

void GamePlayScene::StartScene()
{
	if (!Gimmick::GetIsStart()) { return; }
	// �J������ԊJ�n
	float time = 100; // �J�������ړ����鎞��
	float timeRate = ++timer / time;
	viewProjection.eye = Lerp(stage.GetDoorPos() + Vector3{ 0,10,-15 }, { 0,60,-85 }, timeRate);
	viewProjection.target = Lerp(stage.GetDoorPos(), {}, timeRate);
	if (timer < time) { return; }
	// �J������ԏI��
	timer = 0;
	Gimmick::SetIsStart(false);
}

void GamePlayScene::Update()
{
	StartScene();
	skydome.Update();
	viewProjection.Update();
	player.Update();
	debugCamera.Update();
	stage.Update();
	CollisionManager::CheckAllCollisions();
	lightGroup->Update();
	Vector3 uiPos = { -7,0,17 };
	UIDrawer::GetUI(0)->SetPosition(To2DVector(uiPos));

	if (WorldTransform::GetViewProjection() != &viewProjection && input->IsTrigger(Mouse::Right) && !player.IsCameraChange())
	{
		WorldTransform::SetViewProjection(&viewProjection);
	}
}

void GamePlayScene::Draw()
{
	Model::PreDraw();
	//skydome.Draw();
	player.Draw();
	stage.Draw();
	Model::PostDraw();
}