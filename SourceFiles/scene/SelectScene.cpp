#include "SelectScene.h"

void SelectScene::Initialize()
{
	Stage::SetStageNum(0);
	stage.Initialize();

	std::string stageName[] = { "1", "2", "3", "4" };
	for (size_t i = 0; i < STAGE_NUM; i++)
	{
		stagePlanes[i].Initialize("Stage" + stageName[i] + ".png", { -30 + 20 * (float)i,7.5f,23.99f }, { 5,1,1e-5f });

		clearPlanes[i].Initialize("clear.png", { -30 + 20 * (float)i,7.5f,1e+5f }, { 2.5f,1,1e-5f });
		// クリア判定のビット演算
		if (Stage::GetClearFlag() & (1 << i))
		{
			stagePlanes[i].model->SetAnbient({ 0.25f,0.25f,0.25f });
			stagePlanes[i].model->Update();
			clearPlanes[i].worldTransform.rotation.z = Angle(30);
			clearPlanes[i].worldTransform.translation.z = 23.9f;
			clearPlanes[i].worldTransform.Update();
		}
	}
	AudioManager::Play(BGMName::Select);
	AudioManager::Stop(SEName::DoorOpen); 
	RoomDoor::ResetRoomNum();
}

void SelectScene::Update()
{
	stage.Update();
	AudioManager::Play(BGMName::Select);
}

void SelectScene::Draw()
{
	Model::PreDraw();
	stage.Draw();
	for (auto& stagePlane : stagePlanes) { stagePlane.Draw(); }
	for (auto& clearPlane : clearPlanes) { clearPlane.Draw(); }
	Model::PostDraw();
}

void StagePlane::Initialize(const std::string& textureName, const Vector3& pos, const Vector3& scale)
{
	model = Model::Create("cube");
	model->SetAnbient({ 0.75f,0.75f,0.75f });
	std::unique_ptr<Sprite> texture = Sprite::Create("UI/stage/" + textureName);
	Vector2 textureSize = texture->GetSize();
	textureSize.x = -textureSize.x;
	texture->SetSize(textureSize);
	model->SetSprite(std::move(texture));
	worldTransform.Initialize();
	worldTransform.translation = pos;
	worldTransform.scale = scale;
	model->Update();
	worldTransform.Update();
}