#include "TutorialScene.h"
#include "SceneManager.h"
#include <imgui.h>

UINT16 UIBox::uiBoxNum = 0;

void UIReset();

void TutorialScene::Initialize()
{
	for (size_t i = 0; i < LightGroup::DIR_LIGHT_NUM; i++)
	{
		Model::GetLightGroup()->SetDirLightActive(i, false);
	}

	Stage::SetStageNum((int)Stage::StageNum::Tutorial);
	stage.Initialize();

	UIReset();

	uiBoxes[0].Initialize({ -33,9,-60 }, { 10,10,15 }, (size_t)UIType::Tutorial::tutorialText1);	//�ړ������_�ړ�
	uiBoxes[1].Initialize({ -60,9,-60 }, { 14,10,13 }, (size_t)UIType::Tutorial::tutorialText2);	//�߂��̐C��𓔂���
	uiBoxes[2].Initialize({ -60,9,-42 }, { 15,10,3 }, (size_t)UIType::Tutorial::tutorialText3);		//�W�����v
	uiBoxes[3].Initialize({ -60,9,-21 }, { 15,10,11 }, (size_t)UIType::Tutorial::tutorialText4);	//HP�������
	uiBoxes[4].Initialize({ -60,9,0 }, { 15,10,8 }, (size_t)UIType::Tutorial::tutorialText5);		//�V���ȐC��𓔂���
	uiBoxes[5].Initialize({ -60,9,21 }, { 15,10,11 }, (size_t)UIType::Tutorial::tutorialText6);	//�΂�؂�ւ���
	uiBoxes[6].Initialize({ -48,9,60 }, { 9,10,15 }, (size_t)UIType::Tutorial::tutorialText7);		//�΂̐F�ɂ���ĕς���
	uiBoxes[7].Initialize({ -24,9,60 }, { 9,10,15 }, (size_t)UIType::Tutorial::tutorialText8);		//�X�C�b�`���ړ����悤
	uiBoxes[8].Initialize({ 18,9,60 }, { 9,10,15 }, (size_t)UIType::Tutorial::tutorialText9);		//�X�C�b�`���͈��������Ȃ�
	uiBoxes[9].Initialize({ 60,9,0 }, { 15,10,9 }, (size_t)UIType::Tutorial::tutorialText10);		//��֍s������
	uiBoxes[10].Initialize({ -6,31,60 }, { 9,10,15 }, (size_t)UIType::Tutorial::tutorialText11);	//�����擾���Ă݂悤
	uiBoxes[11].Initialize({ 18,31,60 }, { 9,10,15 }, (size_t)UIType::Tutorial::tutorialText12);	//���̌��Ђ������
	uiBoxes[12].Initialize({ 42,31,60 }, { 9,10,15 }, (size_t)UIType::Tutorial::tutorialText13);	//�����g���Ă݂悤
	uiBoxes[13].Initialize({ 36,31,24 }, { 21,10,15 }, (size_t)UIType::Tutorial::tutorialText14);	//���̃h�A���S�[����

}

void TutorialScene::Update()
{
	stage.Update();
	if (input->IsTrigger(Key::R)) { sceneManager_->ChangeScene(Scene::Play); } // ���g���C
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
	ui->SetPosition({ 1940/2,100 });
}