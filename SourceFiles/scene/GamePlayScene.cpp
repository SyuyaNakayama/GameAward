#include "GamePlayScene.h"
#include <imgui.h>
#include "SpriteCommon.h"
#include "ImGuiManager.h"
#include "WindowsAPI.h"

void GamePlayScene::Initialize()
{
	lightGroup = Model::GetLightGroup();
	for (size_t i = 0; i < LightGroup::DIR_LIGHT_NUM; i++)
	{
		lightGroup->SetDirLightActive(i, false);
	}
	debugCamera.Initialize();
	WorldTransform::SetViewProjection(&debugCamera.GetViewProjection());

	skydome.Initialize(100.0f);
	stage.Initialize();
	player.Initialize(stage.GetStartPos());
	player.SetStageSize(stage.GetStageSize());
	input = Input::GetInstance();

	// Wall�N���X��Player�̃|�C���^�𑗂�
	Wall::SetPlayerAddress(&player);

	//�X�e�[�W�J�n�̃J�����̏����ʒu
	viewProjection.target = stage.GetDoorPos();
	viewProjection.eye = stage.GetDoorPos() + Vector3{ 0,10,-15 };
	viewProjection.farZ = 1500.0f;
	viewProjection.Initialize();
}

void GamePlayScene::StartScene()
{
	if (!Gimmick::GetIsStart()) { return; }
	// �J������ԊJ�n
	float time = 100; // �J�������ړ����鎞��
	float timeRate = ++timer / time;
	viewProjection.eye = Lerp(stage.GetDoorPos() + Vector3{ 0,10,-15 }, { 0,50,-75 }, timeRate);
	viewProjection.target = Lerp(stage.GetDoorPos(), {}, timeRate);
	if (timer < time) { return; }
	// �J������ԏI��
	timer = 0;
	Gimmick::SetIsStart(false);
	// �`���[�g���A���Ȃ瑀�������L����
	if (Stage::GetStageNum() == (UINT16)Stage::StageNum::Tutorial) { UIUpdate = &GamePlayScene::UI_Camera; }
}

void GamePlayScene::Update()
{
	StartScene();
	skydome.Update();
	player.Update();
	debugCamera.Update();
	stage.Update();
	// UI�̒���
	if (UIUpdate) { (this->*UIUpdate)(); }

	if (WorldTransform::GetViewProjection() != &viewProjection && input->IsTrigger(Mouse::Right) && !player.IsCameraChange())
	{
		WorldTransform::SetViewProjection(&viewProjection);
	}
}

// ���_�؂�ւ��̑������
void GamePlayScene::UI_Camera()
{
	// �X�v���C�g�̎擾
	ui = UIDrawer::GetUI((size_t)4 + input->IsConnectGamePad());
	// �X�v���C�g�̐ݒ�
	ui->SetIsInvisible(false);
	ui->SetPosition(WindowsAPI::WIN_SIZE / 2.0f);
	// �}�E�X���E�N���b�N���ꂽ��
	if (input->IsTrigger(Mouse::Right))
	{
		// UI�������Ď��̑��������
		ui->SetIsInvisible(true);
		UIUpdate = &GamePlayScene::UI_Dark;
	}
}

// �΂������������
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
	//skydome.Draw();
	player.Draw();
	stage.Draw();
	Model::PostDraw();
}
