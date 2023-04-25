#include "GamePlayScene.h"
#include <imgui.h>
#include "ImGuiManager.h"
#include "WindowsAPI.h"
#include "SceneManager.h"

void GamePlayScene::Initialize()
{
	for (size_t i = 0; i < LightGroup::DIR_LIGHT_NUM; i++)
	{
		Model::GetLightGroup()->SetDirLightActive(i, false);
	}
	input = Input::GetInstance();
	debugCamera.Initialize({ 180,50 });
	WorldTransform::SetViewProjection(&debugCamera.GetViewProjection());
	stage.Initialize();
	// �C���UI
	for (size_t i = 0; i < candleUIs.size(); i++)
	{
		candleUIs[i] = UIDrawer::GetUI(5 + i);
	}
	for (size_t i = 0; i < Candle::GetLightNum(); i++)
	{
		candleUIs[i]->SetIsInvisible(false);
		candleUIs[i]->SetPosition({ candleUIs[i]->GetSize().x * i,72 });
		candleUIs[i]->SetColor({ 1,1,1,0.5f });
	}

	player.Initialize(stage.GetStartPos(), stage.GetStartRot());
	// Block�N���X��Player�̃|�C���^�𑗂�
	Block::SetPlayerAddress(&player);
	//�X�e�[�W�J�n�̃J�����̏����ʒu
	viewProjection.target = stage.GetDoorPos();
	viewProjection.eye = stage.GetDoorPos() + Vector3{ 0,10,-15 };
	viewProjection.farZ = 1500.0f;
	viewProjection.Initialize();

	uiSpheres[0].Initialize({ -69,4,85 }, { 15,5,15 });
	uiSpheres[1].Initialize({ 80,0,65 }, { 10,4,15 });
}

void GamePlayScene::Update()
{
	player.Update();
	debugCamera.Update();
	stage.Update();
	// ���g���C
	if (input->IsTrigger(Key::R)) { sceneManager_->SetNextScene(Scene::Play); }
	// UI�̒���
	if (UIUpdate) { (this->*UIUpdate)(); }
	// �C���UI�̐F�ύX
	size_t lightedNum = Candle::GetLightedNum();
	if (lightedNum != 0)
	{
		candleUIs[lightedNum - 1]->SetColor({ 1,1,1,1 });
	}
	for (auto& uiSphere : uiSpheres) { uiSphere.Update(); }
}

// �΂�ς��鑀�����
void GamePlayScene::UI_Dark()
{
	// �v���C���[��������O�Ȃ�X�L�b�v
	if (player.GetWorldPosition().z < -8.0f) { return; }
	// �X�v���C�g�̎擾
	ui = UIDrawer::GetUI((size_t)0 + input->IsConnectGamePad());
	// �X�v���C�g�̐ݒ�
	ui->SetPosition(WindowsAPI::WIN_SIZE / 2.0f);
	ui->SetColor({ 0,0,0,1 });
	ui->SetIsInvisible(false);
	// �v���C���[�̉΂����Ȃ�
	if (player.IsBlueFire())
	{
		// UI�������đ���������I���
		ui->SetIsInvisible(true);
		UIUpdate = nullptr;
	}
}

void GamePlayScene::Draw()
{
	Model::PreDraw();
	player.Draw();
	stage.Draw();
	Model::PostDraw();
}

void UISphere::Initialize(Vector3 pos, Vector3 rad)
{
	if (Stage::GetStageNum() != (int)Stage::StageNum::Tutorial) { return; }
	worldTransform.Initialize();
	worldTransform.translation = pos;
	worldTransform.scale = rad;
	worldTransform.Update();
	collisionAttribute = CollisionAttribute::UI;
	collisionMask = CollisionMask::UI;
}

void UISphere::Update()
{
	UIDrawer::GetUI(Input::GetInstance()->IsConnectGamePad())->SetIsInvisible(true);
}

void UISphere::OnCollision(BoxCollider* collider)
{
	Player* pPlayer = dynamic_cast<Player*>(collider);
	if (!pPlayer) { return; }
	if (pPlayer->IsBlueFire()) { return; }
	Sprite* ui = UIDrawer::GetUI(Input::GetInstance()->IsConnectGamePad());
	ui->SetIsInvisible(false);
	ui->SetPosition(To2DVector(worldTransform.GetWorldPosition() + Vector3(0, -10, 0)));
}
