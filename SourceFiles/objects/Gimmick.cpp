#include <imgui.h>
#include <random>
#include "Gimmick.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "Stage.h"
#include "SceneManager.h"
#include "UIDrawer.h"

#pragma region �ÓI�����o�ϐ�
bool Gimmick::isStart_;
LightGroup* Gimmick::lightGroup = nullptr;
size_t Candle::lightNum = 0;
Player* Wall::player = nullptr;
// ���̐ÓI�����o�ϐ��̏�����
size_t KeyLock::keyNum = 0;
size_t KeyLock::collectKeyNum = 0;
bool KeyLock::isCollectAll = false;
UINT RoomDoor::roomNum = 1;
std::array<UINT, 3> RoomDoor::allNextRoomNums;
#pragma endregion

void Gimmick::Initialize(const GimmickParam& param)
{
	worldTransform.translation = param.pos;
	worldTransform.scale = param.scale;
	worldTransform.rotation = param.rot * (PI / 180);
}

#pragma region Door
void BaseDoor::Initialize(const GimmickParam& param)
{
	collisionAttribute = CollisionAttribute::Block;
	collisionMask = CollisionMask::Block;
	// �p�����[�^�Z�b�g
	Gimmick::Initialize(param);
	// ���f���ǂݍ���
	model = Model::Create("door", true);
	model_back = Model::Create("door_back");
	model->SetAnbient({ 1,1,1 });
	model->Update();
	// �e���f����worldTransform�������ƃ��f���̈ʒu����
	worldTransform.Initialize();
	worldTransform.translation += { 0, 1.35f };
	worldTransform.scale = { 1.8f,1.9f,2.0f };
	for (auto& w : door)
	{
		// �e�q�֌W
		w.parent = &worldTransform;
		w.Initialize();
		// �傫���𒲐�
		w.scale = { 1.1f,1.05f,1.0f };
		// �J����
		w.rotation.y = -PI / 2.0f;
	}
	// ���W�𒲐�
	door[(int)WTType::L].translation += {-1.4f, -1.3f, 0.0f};
	door[(int)WTType::R].translation += { 1.4f, -1.3f, 0.0f};
}

/// <summary>
/// �h�A�̍X�V����
/// </summary>
void BaseDoor::Update()
{
	for (auto& w : door) { w.Update(); }
	worldTransform.Update();
}

void BaseDoor::Draw()
{
	for (auto& w : door) { model->Draw(w); }
	model_back->Draw(worldTransform);
}

void GoalDoor::Update()
{
	assert(Move);
	(this->*Move)(); // ���𓮂���
	BaseDoor::Update();
}

/// <summary>
/// �h�A���J��
/// </summary>
void GoalDoor::Open()
{
	if (++rot >= 90) { Move = &GoalDoor::Opened; }

	door[(int)WTType::L].rotation.y = (rot + 180) * PI / 180;
	door[(int)WTType::R].rotation.y = -rot * PI / 180;
}

/// <summary>
/// �h�A�����
/// </summary>
void GoalDoor::Close()
{
	if (--rot <= 0)
	{
		isStart_ = true;
		Move = &GoalDoor::Closed;
	}

	door[(int)WTType::L].rotation.y = (rot + 180) * PI / 180;
	door[(int)WTType::R].rotation.y = -rot * PI / 180;
}

void GoalDoor::Opened()
{
	if (input->IsTrigger(Key::P)) { Move = &GoalDoor::Close; }	// ����߂�
}

void GoalDoor::Closed()
{
	if (input->IsTrigger(Key::O)) { Move = &GoalDoor::Open; }
	// �S�[������
	// Player�̃��C�g�C���f�b�N�X�͔�΂�
	for (size_t i = 1; i <= Candle::GetLightNum(); i++)
	{
		if (lightGroup->GetPointLightActive(i))
		{
			// ���C�g�����Ă��鎞
			Move = &GoalDoor::Open;
			continue;
		}
		// ���C�g�����Ă��Ȃ����͊֐����I������
		Move = &GoalDoor::Closed;
		return;
	}
}

/// <summary>
/// �h�A�ɓ���������
/// </summary>
void GoalDoor::OnCollision(BoxCollider* boxCollider)
{
	if (Move != &GoalDoor::Opened) { return; } // �h�A���󂢂Ă��鎞�S�[��
	SceneManager::GetInstance()->SetNextScene(Scene::Clear);
}

void SelectDoor::Closed()
{
	// ���C�g�����Ă��鎞
	if (lightGroup->GetPointLightActive(doorIndex)) { Move = &GoalDoor::Open; }
}

void SelectDoor::OnCollision(BoxCollider* boxCollider)
{
	if (Move != &GoalDoor::Opened) { return; } // �h�A���󂢂Ă��鎞
	Stage::SetStageNum(doorIndex);
	SceneManager::GetInstance()->SetNextScene(Scene::Play);
}

void RoomDoor::Initialize(const GimmickParam& param)
{
	BaseDoor::Initialize(param);
	// ����
	std::random_device rnd;
	std::mt19937 rnddev(rnd());
	std::uniform_int_distribution<UINT> rand(1, 5);

	// �����̏d���𖳂���
	bool isExistNextRoomNum = false;
	do
	{
		nextRoomNum = rand(rnddev);
		for (size_t i = 0; i < doorIndex - 1; i++)
		{
			// �����̒l�����X�g�ɓo�^����Ă����ꍇ
			if (allNextRoomNums[i] == nextRoomNum)
			{
				isExistNextRoomNum = true;
				break;
			}
			else { isExistNextRoomNum = false; }
		}
	} while (isExistNextRoomNum);
	
	// 3�ڂ̃h�A�̎�
	if (doorIndex >= allNextRoomNums.size())
	{
		// ���̕����������h�A�����邩�`�F�b�N
		bool isExistRightNextRoomNum = false;
		for (UINT nextRoomNumTemp : allNextRoomNums)
		{
			if (nextRoomNumTemp == roomNum + 1)
			{
				isExistRightNextRoomNum = true;
				break;
			}
		}
		// �Ȃ������ꍇ�͎��̕����ԍ����Z�b�g
		if (!isExistRightNextRoomNum) { nextRoomNum = roomNum + 1; }
	}
	// ������z��ɓo�^
	allNextRoomNums[doorIndex - 1] = nextRoomNum;
}

void RoomDoor::Update()
{
	std::array<std::string, 3> strings = { "Left","Center","Right" };

	BaseDoor::Update();
	ImGui::Text("nextRoomNum(%s) : %d", strings[doorIndex - 1].c_str(), nextRoomNum);
}

void RoomDoor::OnCollision(BoxCollider* boxCollider)
{
	SceneManager::GetInstance()->SetNextScene(Scene::Play);
	roomNum++;
}
#pragma endregion

#pragma region KeyLock
void KeyLock::Initialize(const GimmickParam& param)
{
	// �����蔻��ݒ�
	collisionAttribute = CollisionAttribute::Block;
	collisionMask = CollisionMask::Block;
	// ���f���ǂݍ���
	model = Model::Create("cube", true);
	// ������
	worldTransform.Initialize();
	// �p�����[�^�Z�b�g
	Gimmick::Initialize(param);
	// ���Ђ̐��𑝂₷
	keyNum++;
}

void KeyLock::Update()
{
	// ����S�ďW�߂���t���O���I���ɂ���
	if (!isCollectAll && collectKeyNum == keyNum) {
		isCollectAll = true;
	}
}

void KeyLock::OnCollision(BoxCollider* boxCollider)
{
	// �擾�������Ђ̐��𑝂₷
	collectKeyNum++;
	// ���W�ς݃t���O���I���ɂ���
	isCollect = true;
	// �����蔻����Ȃ���
	collisionMask = CollisionMask::None;
}

void KeyLock::Draw()
{
	// �܂��擾����ĂȂ��Ȃ�`�悷��
	if (!isCollect) { model->Draw(worldTransform); }
}
#pragma endregion

#pragma region Candle
void Candle::Initialize(const GimmickParam& param)
{
	// �p�����[�^�Z�b�g
	Gimmick::Initialize(param);
	model = Model::Create("candle", true);
	worldTransform.Initialize();
	worldTransform.translation.y -= 1.0f;
	worldTransform.scale = { 2.0f,2.0f,2.0f };
	lightGroup = Model::GetLightGroup();
	lightPos = worldTransform.translation + Vector3(0, worldTransform.scale.y + 1.2f);
	particleProp =
	{
		lightPos,{0,0.01f,0},{0,0.0005f,0},
		0.025f,0.001f,0,40,0.4f
	};
	lightGroup->SetPointLightAtten(lightIndex, { 0.2f, 0.01f });
	lightGroup->SetPointLightColor(lightIndex, { 1,0.5f,0.5f });
	ui = UIDrawer::GetUI((size_t)2 + Input::GetInstance()->IsConnectGamePad());
	if (param.flag != 0)
	{
		isExist = false;
		collisionMask = CollisionMask::None;
	}
}

void Candle::Update()
{
	// �X�e�[�W2�̏ꍇ
	if (Stage::GetStageNum() == (UINT)Stage::StageNum::Stage2)
	{
		// ���݂̕����ԍ��ȉ��̃C���f�b�N�X�̏ꍇ�o������
		isExist = lightIndex <= RoomDoor::GetRoomNumber();
	}
	if (!isExist) { return; }
	worldTransform.Update();
	(this->*Fire)();
	model->Update();
	ui->SetIsInvisible(true);
}

void Candle::Dark()
{
	lightGroup->SetPointLightActive(lightIndex, false);
	model->SetAnbient({ 0.1f,0.1f,0.1f });
}

void Candle::PreLight()
{
	// ��ԕύX
	if (--particleTimer <= 0)
	{
		Fire = &Candle::PostLight;
		lightGroup->SetPointLightActive(lightIndex, true); // �_��
		model->SetAnbient({ 0.7f,0.3f,0.3f }); // �}�e���A������
		// �p�[�e�B�N������
		lightPos = worldTransform.translation + Vector3(0, worldTransform.scale.y + 1.2f);
		particleProp.posOffset = lightPos;
		lightGroup->SetPointLightPos(lightIndex, lightPos);
	}
	// ��������
	std::random_device rnd;
	std::mt19937 rnddev(rnd());
	std::uniform_real_distribution<float> randRadius(0, 2.0f);
	std::uniform_real_distribution<float> randAngle(-PI / 2.0f, PI / 2.0f);
	// �p�[�e�B�N���ݒ�
	DirectionalParticle::AddProp particleProp =
	{
		playerPos,worldTransform.translation + Vector3(0, worldTransform.scale.y + 1.2f),
		0.5f,2,randAngle(rnddev),randRadius(rnddev),60
	};
	// �p�[�e�B�N���ǉ�
	ParticleManager::Add(particleProp);
}

void Candle::PostLight()
{
	// �p�[�e�B�N���ǉ�
	ParticleManager::Add(particleProp);
}

void Candle::OnCollision(RayCollider* rayCollider)
{
	if (Length(rayCollider->GetWorldPosition() - worldTransform.GetWorldPosition()) >= 8.0f) { return; }
	if (Stage::GetStageNum() == (UINT16)Stage::StageNum::Tutorial)
	{
		ui->SetIsInvisible(Fire != &Candle::Dark);
		ui->SetPosition(To2DVector(worldTransform.GetWorldPosition() + Vector3(0, 1, 0)));
	}
	if (!Input::GetInstance()->IsTrigger(Mouse::Left)) { return; }
	Fire = &Candle::PreLight;
	particleTimer = 60;
	playerPos = rayCollider->GetWorldPosition();
}
#pragma endregion

#pragma region Wall
void Wall::Initialize(const GimmickParam& param)
{
	// �����蔻��ݒ�
	collisionAttribute = CollisionAttribute::Block;
	collisionMask = CollisionMask::Block;
	// �p�����[�^�Z�b�g
	Gimmick::Initialize(param);
	if (param.flag == 0) { wallState = (int)WallStatus::NORMAL; }
	else if (param.flag == 1) { wallState = (int)WallStatus::MOVE; }
	else if (param.flag == 2) { wallState = (int)WallStatus::VANISH_RED; }
	else if (param.flag == 3) { wallState = (int)WallStatus::VANISH_BLUE; }
	// ���f���ǂݍ���
	model = Model::Create("cube");
	std::unique_ptr<Sprite> sprite = Sprite::Create("stages/floor.png");
	//std::unique_ptr<Sprite> sprite = Sprite::Create("white1x1.png");
	sprite->SetSize(sprite->GetSize() / max(max(param.scale.x, param.scale.y), param.scale.z) * 10.0f);
	model->SetSprite(std::move(sprite));
	model->Update();
	// ������
	worldTransform.Initialize();
}

void Wall::Update()
{
	// �����蔻��ݒ�
	if ((wallState & (int)WallStatus::VANISH_BLUE) && !player->IsBlueFire()) { collisionMask = CollisionMask::None; }
	//else if (wallState & (int)WallStatus::VANISH_RED && player->IsRedFire()) { collisionMask = CollisionMask::None; }
	else { collisionMask = CollisionMask::Block; }
	// ����
	if (wallState & (int)WallStatus::MOVE) {
		Move();
	}
	// �X�V
	worldTransform.Update();
}

void Wall::Draw()
{
	// �����Ȃ���
	if (!(wallState & (int)WallStatus::VANISH_RED) && !(wallState & (int)WallStatus::VANISH_BLUE)) { Gimmick::Draw(); return; }
	// ���̂Ƃ��`�悳����
	if (wallState & (int)WallStatus::VANISH_BLUE && player->IsBlueFire()) { Gimmick::Draw(); }
}

void Wall::Move()
{
	worldTransform.translation.y += speed;
	if (interval > 0) { interval--; return; }
	if (worldTransform.translation.y > 20.0f) { speed = -speed; interval = 120; }
	if (worldTransform.translation.y < 1.0f) { speed = -speed; interval = 120; }
}
#pragma endregion