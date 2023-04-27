#include "GamePlayScene.h"
#include <imgui.h>
#include "ImGuiManager.h"
#include "WindowsAPI.h"
#include "SceneManager.h"

UINT16 UIBox::uiBoxNum = 0;

void GamePlayScene::Initialize()
{
	for (size_t i = 0; i < LightGroup::DIR_LIGHT_NUM; i++)
	{
		Model::GetLightGroup()->SetDirLightActive(i, false);
	}
	input = Input::GetInstance();
	//debugCamera.Initialize({ 180,50 });
	debugCamera.Initialize({ 350,30 });
	WorldTransform::SetViewProjection(&debugCamera.GetViewProjection());
	stage.Initialize();
	// �C���UI
	for (size_t i = 0; i < candleUIs.size(); i++)
	{
		// �`���ԏ�����
		UIDrawer::GetUI(5 + i)->SetIsInvisible(true);
	}
	for (size_t i = 0; i < Candle::GetLightNum(); i++)
	{
		candleUIs[i] = UIDrawer::GetUI(5 + i);
		candleUIs[i]->SetIsInvisible(false);
		candleUIs[i]->SetPosition({ candleUIs[i]->GetSize().x * i,40 });
		candleUIs[i]->SetColor({ 1,1,1,0.5f });
	}
	// ����UI
	for (size_t i = 0; i < keyUIs.size(); i++)
	{
		// �`���ԏ�����
		UIDrawer::GetUI(16 + i)->SetIsInvisible(true);
	}
	for (size_t i = 0; i < KeyLock::GetKeyNum(); i++)
	{
		keyUIs[i] = UIDrawer::GetUI(16 + i);
		keyUIs[i]->SetIsInvisible(false);
		keyUIs[i]->SetPosition({ keyUIs[i]->GetSize().x - 75, 120 });
		keyUIs[i]->SetColor({ 1,1,1,0.5f });
	}

	player.Initialize(stage.GetStartPos(), stage.GetStartRot());
	// Block�N���X��Player�̃|�C���^�𑗂�
	Block::SetPlayerAddress(&player);
	//�X�e�[�W�J�n�̃J�����̏����ʒu
	viewProjection.target = stage.GetDoorPos();
	viewProjection.eye = stage.GetDoorPos() + Vector3{ 0,10,-15 };
	viewProjection.farZ = 1500.0f;
	viewProjection.Initialize();

	uiBoxes[0].Initialize({ -69,4,85 }, { 15,5,15 }, 0);
	uiBoxes[1].Initialize({ 80,0,65 }, { 10,4,15 }, 0);
	uiBoxes[2].Initialize({ 30,0,80 }, { 30,4,40 }, 13);
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
	// ����UI�̐F�ύX
	size_t collectedKeyNum = KeyLock::GetCollectedKeyNum();
	if (collectedKeyNum != 0)
	{
		keyUIs[collectedKeyNum - 1]->SetColor({ 1,1,1,1 });
	}
	for (auto& uiSphere : uiBoxes) { uiSphere.Update(); }
	if (input->IsTrigger(Key::_0)) 
	{
		sceneManager_->SetNextScene(Scene::Title); 
		Stage::SetStageNum(0);
	}
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

void UIBox::Initialize(Vector3 pos, Vector3 rad, UINT16 uiIndex)
{
	if (Stage::GetStageNum() != (int)Stage::StageNum::Tutorial) { return; }
	worldTransform.Initialize();
	worldTransform.translation = pos;
	worldTransform.scale = rad;
	worldTransform.Update();
	ui = UIDrawer::GetUI(uiIndex + Input::GetInstance()->IsConnectGamePad());
	collisionAttribute = CollisionAttribute::UI;
	collisionMask = CollisionMask::UI;
	index = uiBoxNum++;
}

void UIBox::Update()
{
	if (Stage::GetStageNum() != (int)Stage::StageNum::Tutorial) { return; }
	ui->SetIsInvisible(true);
}

void UIBox::OnCollision(BoxCollider* collider)
{
	if (Stage::GetStageNum() != (int)Stage::StageNum::Tutorial) { return; }
	Player* pPlayer = dynamic_cast<Player*>(collider);
	if (!pPlayer) { return; }
	if (pPlayer->IsBlueFire()) { return; }
	if (index == 2)
	{
		if (Input::GetInstance()->IsTrigger(Key::Return)) { isOpeEnd = true; }
	}
	if (isOpeEnd) { return; }
	ui->SetIsInvisible(false);
	ui->SetPosition(To2DVector(worldTransform.GetWorldPosition() + Vector3(0, -10, 0)));
}
