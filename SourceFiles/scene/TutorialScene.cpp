#include "TutorialScene.h"
#include "SceneManager.h"
#include <imgui.h>

UINT16 UIBox::uiBoxNum = 0;

void UIReset();

void TutorialScene::Initialize()
{
	Stage::SetStageNum((int)Stage::StageNum::Tutorial);
	stage.Initialize();
	UIDrawer::GetUI((size_t)UIType::Play::Reset + !input->IsConnectGamePad())->SetIsInvisible(true);

	UIReset();

	// ��������̐؂�ւ�
	std::array<size_t, 3> uiIndex{};
	if (input->IsConnectGamePad())
	{
		uiIndex[0] = (size_t)UIType::Tutorial::tutorialText1;
		uiIndex[1] = (size_t)UIType::Tutorial::tutorialText3;
		uiIndex[2] = (size_t)UIType::Tutorial::tutorialText6;
	}
	else
	{
		uiIndex[0] = (size_t)UIType::Tutorial::tutorialText1_2;
		uiIndex[1] = (size_t)UIType::Tutorial::tutorialText3_2;
		uiIndex[2] = (size_t)UIType::Tutorial::tutorialText6_2;
	}

	uiBoxes[0].Initialize({ -33,9,-60 }, { 10,10,15 }, uiIndex[0]);	// �ړ������_�ړ�
	uiBoxes[1].Initialize({ -60,9,-60 }, { 14,10,13 }, (size_t)UIType::Tutorial::tutorialText2);	// �߂��̐C��𓔂���
	uiBoxes[2].Initialize({ -60,9,-42 }, { 15,10,3 }, uiIndex[1]);		// �W�����v
	uiBoxes[3].Initialize({ -60,9,-21 }, { 15,10,11 }, (size_t)UIType::Tutorial::tutorialText4);	// HP�������
	uiBoxes[4].Initialize({ -60,9,0 }, { 15,10,8 }, (size_t)UIType::Tutorial::tutorialText5);		// �V���ȐC��𓔂���
	uiBoxes[5].Initialize({ -60,9,21 }, { 15,10,11 }, uiIndex[2]);		// �΂�؂�ւ���
	uiBoxes[6].Initialize({ -48,9,60 }, { 9,10,15 }, (size_t)UIType::Tutorial::tutorialText7);		// �΂̐F�ɂ���ĕς���
	uiBoxes[7].Initialize({ -24,9,60 }, { 9,10,15 }, (size_t)UIType::Tutorial::tutorialText8);		// �X�C�b�`���ړ����悤
	uiBoxes[8].Initialize({ 18,9,60 }, { 9,10,15 }, (size_t)UIType::Tutorial::tutorialText9);		// �X�C�b�`�͈��������Ȃ�
	uiBoxes[9].Initialize({ 60,9,54 }, { 15,10,9 }, (size_t)UIType::Tutorial::tutorialText10);		// ��֍s������
	uiBoxes[10].Initialize({ 42,31,60 }, { 9,10,15 }, (size_t)UIType::Tutorial::tutorialText11);	// �����擾���Ă݂悤
	uiBoxes[11].Initialize({ 18,31,60 }, { 9,10,15 }, (size_t)UIType::Tutorial::tutorialText12);	// ���̌��Ђ������
	uiBoxes[12].Initialize({ -6,31,60 }, { 9,10,15 }, (size_t)UIType::Tutorial::tutorialText13);	// �����g���Ă݂悤
	uiBoxes[13].Initialize({ -36,31,24 }, { 21,10,15 }, (size_t)UIType::Tutorial::tutorialText14);	// ���̃h�A���S�[����
}

void TutorialScene::Update()
{
	stage.Update();
	if (input->IsTrigger(Key::R) || input->IsTrigger(JoyPad::X)) { sceneManager_->ChangeScene(Scene::Tutorial); } // ���g���C
	// UI�̍X�V
	for (auto& uiSphere : uiBoxes) { uiSphere.Update(); }
}

void TutorialScene::Draw()
{
	Model::PreDraw();
	stage.Draw();
	Model::PostDraw();
}

void UIBox::Initialize(const Vector3& pos, const Vector3& rad, size_t uiIndex, const Vector3& uipos_)
{
	worldTransform.Initialize();
	worldTransform.translation = pos;
	worldTransform.scale = rad;
	worldTransform.Update();
	ui = UIDrawer::GetUI(uiIndex);
	collisionAttribute = CollisionAttribute::UI;
	collisionMask = CollisionMask::UI;
	index = uiBoxNum++;
	uipos = uipos_;
}

void UIBox::Update()
{
	ui->SetIsInvisible(true);
}

void UIBox::OnCollision(BoxCollider* collider)
{
	ui->SetIsInvisible(false);
	ui->SetPosition(uipos);
}