#include "TutorialScene.h"
#include <imgui.h>

UINT16 UIBox::uiBoxNum = 0;

void TutorialScene::Initialize()
{
	for (size_t i = 0; i < LightGroup::DIR_LIGHT_NUM; i++)
	{
		Model::GetLightGroup()->SetDirLightActive(i, false);
	}

	Stage::SetStageNum((int)Stage::StageNum::Tutorial);
	stage.Initialize();

	uiBoxes[0].Initialize({ -69,4,85 }, { 15,5,15 }, 0);
	uiBoxes[1].Initialize({ 80,0,65 }, { 10,4,15 }, 0);
	uiBoxes[2].Initialize({ 30,0,80 }, { 30,4,40 }, 0);

	// �C���UI
	// �`���ԏ�����
	size_t candleUIIndex = (size_t)UIType::Play::Candle;
	for (size_t i = candleUIIndex; i < candleUIIndex + 8; i++) { UIDrawer::GetUI(i)->SetIsInvisible(true); }
	for (size_t i = 0; i < Candle::GetLightNum(); i++)
	{
		Sprite* candleUI = nullptr;
		candleUI = UIDrawer::GetUI(candleUIIndex + i);
		candleUI->SetIsInvisible(false);
		candleUI->SetPosition({ candleUI->GetSize().x * i + 32,85 });
		candleUI->SetColor({ 1,1,1,0.5f });
	}
	// ����UI
	// �`���ԏ�����
	size_t keyUIIndex = (size_t)UIType::Play::Key;
	size_t keyPartsUIIndex = (size_t)UIType::Play::KeyParts;
	UIDrawer::GetUI(keyUIIndex)->SetIsInvisible(true);
	for (size_t i = keyPartsUIIndex; i < keyPartsUIIndex + 6; i++) { UIDrawer::GetUI(i)->SetIsInvisible(true); }
	for (size_t i = 0; i < KeyLock::GetKeyNum(); i++)
	{
		Sprite* keyUI = nullptr;
		if (KeyLock::GetKeyNum() == 1) { keyUI = UIDrawer::GetUI(keyUIIndex); }
		else { keyUI = UIDrawer::GetUI(keyPartsUIIndex + i); }
		keyUI->SetIsInvisible(false);
		keyUI->SetPosition({ keyUI->GetSize().x * i + 32, 165 });
		keyUI->SetColor({ 1,1,1,0.5f });
	}

	uiBoxes[0].Initialize({ -33,9,-60 }, { 12,10,15 }, 1);//�ړ������_�ړ�
	uiBoxes[1].Initialize({ -60,9,-60 }, { 15,10,15 }, 1);//�߂��̐C��𓔂���
	uiBoxes[2].Initialize({ -60,9,-21 }, { 15,10,3 }, 14);//�W�����v
	uiBoxes[3].Initialize({ -60,9,-12 }, { 15,10,21 }, 1);//HP�̑����ɂ���
	uiBoxes[4].Initialize({ -60,9,21 }, { 15,10,12 }, 0);//�ŏ������
	uiBoxes[5].Initialize({ -48,9,60 }, { 9,10,15 }, 0);//�Ԃŏ������
	uiBoxes[6].Initialize({ -24,9,60 }, { 9,10,15 }, 1);//�X�C�b�`*1
	uiBoxes[7].Initialize({ 18,9,60 }, { 9,10,15 }, 1);//�X�C�b�`*2
	uiBoxes[8].Initialize({ 60,9,0 }, { 15,10,9 }, 1);//�G���x�[�^�[
}

void TutorialScene::Update()
{
	stage.Update();
	// UI�̒���
	if (UIUpdate) { (this->*UIUpdate)(); }
	for (auto& uiSphere : uiBoxes) { uiSphere.Update(); }
}

// �΂�ς��鑀�����
void TutorialScene::UI_Dark()
{
	Player* pPlayer = stage.GetPlayer();
	// �v���C���[��������O�Ȃ�X�L�b�v
	if (pPlayer->GetWorldPosition().z < -8.0f) { return; }
	// �X�v���C�g�̎擾
	ui = UIDrawer::GetUI((size_t)UIType::Tutorial::LightChange + input->IsConnectGamePad());
	// �X�v���C�g�̐ݒ�
	ui->SetPosition(WindowsAPI::WIN_SIZE / 2.0f);
	ui->SetColor({ 0,0,0,1 });
	ui->SetIsInvisible(false);
	// �v���C���[�̉΂����Ȃ�
	if (pPlayer->IsBlueFire())
	{
		// UI�������đ���������I���
		ui->SetIsInvisible(true);
		UIUpdate = nullptr;
	}
}

void TutorialScene::Draw()
{
	Model::PreDraw();
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