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
size_t Candle::lightedNum = 0;
Player* Block::player = nullptr;
// ���̐ÓI�����o�ϐ��̏�����
size_t KeyLock::keyNum = 0;
size_t KeyLock::collectKeyNum = 0;
bool KeyLock::isCollectAll = false;
UINT RoomDoor::roomNum = 1;
std::array<UINT, 3> RoomDoor::allNextRoomNums;
// �X�C�b�`�̐ÓI�����o�ϐ��̏�����
std::vector<Switch::SwitchParam> Switch::switches;
size_t Switch::switchNum = 0;
#pragma endregion

// �e�h���N���X���ʏ���
void Gimmick::Initialize(const GimmickParam& param)
{
	// �����蔻��ݒ�
	collisionAttribute = CollisionAttribute::Block;
	collisionMask = CollisionMask::Block;
	// worldTransform�������ƃp�����[�^���
	worldTransform.Initialize();
	worldTransform.translation = param.pos;
	worldTransform.scale = param.scale;
	worldTransform.rotation = param.rot * (PI / 180);
}

void Gimmick::CheckIsCameraCapture()
{
	isCameraCapture = true;
	ViewProjection* vp = WorldTransform::GetViewProjection();
	// �X�P�[���ɂ���Ă͔͈͓��ł��`�悳��Ȃ��ꍇ�����邽�߁A
	// �X�P�[����farZ�ȏ�̂Ƃ��͔��肵�Ȃ�
	for (size_t i = 0; i < 3; i++)
	{
		if (worldTransform.scale[i] < vp->farZ) { continue; }
		return;
	}
	// �J�����ʒu����I�u�W�F�N�g�܂ł̋������v�Z
	float dis = Length(worldTransform.GetWorldPosition() - vp->eye);
	// ������farZ�ȏ�̂Ƃ��͍X�V�E�`�悵�Ȃ�
	if (dis >= vp->farZ) { isCameraCapture = false; }
}

#pragma region Door
void BaseDoor::Initialize(const GimmickParam& param)
{
	// �p�����[�^�Z�b�g
	Gimmick::Initialize(param);
	// ���f���ǂݍ���
	model = Model::Create("door", true);
	model_back = Model::Create("door_back");
	model->SetAnbient({ 1,1,1 });
	model->Update();
	// �e���f����worldTransform�������ƃ��f���̈ʒu����
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
	CheckIsCameraCapture();
	worldTransform.Update();
	for (auto& w : door) { w.Update(); }
}

void BaseDoor::Draw()
{
	if (!isCameraCapture) { return; }
	model_back->Draw(worldTransform);
	for (auto& w : door) { model->Draw(w); }
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
	if (Candle::GetLightNum() == Candle::GetLightedNum()) { Move = &GoalDoor::Open; }
}

/// <summary>
/// �h�A�ɓ���������
/// </summary>
void GoalDoor::OnCollision(BoxCollider* boxCollider)
{
	if (Move != &GoalDoor::Opened) { return; } // �h�A���󂢂Ă��鎞�S�[��
	SceneManager::GetInstance()->SetNextScene(Scene::Title);
	Stage::SetStageNum(0);
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
	// ���̕����ԍ��̐ݒ�
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

	// ���̕����ԍ������o��
	std::unique_ptr<Sprite> sprite = Sprite::Create("ui/candleui.png");
	candlePlaneObj.Initialize();
	candlePlaneObj.translation = worldTransform.translation;
	candlePlaneObj.translation.y += 10;
	candlePlaneObj.scale.x = (float)nextRoomNum;
	candlePlaneObj.scale.y = sprite->GetSize().y / sprite->GetSize().x;
	candlePlaneObj.scale.z = 0.0001f;
	candlePlaneModel = Model::Create("cube");
	candlePlaneModel->SetAnbient({ 1,1,1 });
	sprite->SetSize({ sprite->GetSize().x / (float)nextRoomNum ,sprite->GetSize().y });
	candlePlaneModel->SetSprite(std::move(sprite));
	candlePlaneModel->Update();
}

void RoomDoor::Update()
{
	// �ŏI�����̏ꍇ�͍X�V���Ȃ�
	if (roomNum == FINAL_ROOM_NUM) { return; }
	BaseDoor::Update();
	candlePlaneObj.Update();
}

void RoomDoor::Draw()
{
	if (!isCameraCapture) { return; }
	BaseDoor::Draw();
	candlePlaneModel->Draw(candlePlaneObj);
}

void RoomDoor::OnCollision(BoxCollider* boxCollider)
{
	// �ŏI�����̏ꍇ�͓����蔻������Ȃ�
	if (roomNum == FINAL_ROOM_NUM) { return; }

	SceneManager::GetInstance()->SetNextScene(Scene::Play);
	// �����̃h�A�������ꍇ�AroomNum���C���N�������g
	if (nextRoomNum == roomNum + 1) { roomNum++; }
	// �s�����̃h�A�������ꍇ�A�X�^�[�g�̕����ɖ߂�
	else { roomNum = 1; }
}
#pragma endregion

#pragma region KeyLock
void KeyLock::Initialize(const GimmickParam& param)
{
	// ���f���ǂݍ���
	model = Model::Create("key", true);
	// �p�����[�^�Z�b�g
	Gimmick::Initialize(param);
	// �X�V
	worldTransform.Update();
	// ��(����?)�̐��𑝂₷
	keyNum++;
}

void KeyLock::Update()
{
	// ����S�ďW�߂���t���O���I���ɂ���
	if (!isCollectAll && collectKeyNum == keyNum) {
		isCollectAll = true;
	}
}

void KeyLock::Draw()
{
	// �܂��擾����ĂȂ��Ȃ�`�悷��
	if (!isCollected) { model->Draw(worldTransform); }
}

void KeyLock::OnCollision(BoxCollider* boxCollider)
{
	// �擾�������Ђ̐��𑝂₷
	collectKeyNum++;
	// ���W�ς݃t���O���I���ɂ���
	isCollected = true;
	// �����蔻����Ȃ���
	collisionMask = CollisionMask::None;
}
#pragma endregion

#pragma region Candle
void Candle::Initialize(const GimmickParam& param)
{
	// �p�����[�^�Z�b�g
	Gimmick::Initialize(param);
	model = Model::Create("candle", true);
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
	healZone.Initialize(&worldTransform);
}

void Candle::Update()
{
	CheckIsCameraCapture();
	// �����蔻��𖳂���
	healZone.SetCollisionMask(CollisionMask::None);

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
	healZone.Update();
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
	std::random_device rnd;
	std::mt19937 rnddev(rnd());
	std::uniform_real_distribution<float> randRadius(0.0f, 0.5f);
	lightGroup->SetPointLightPos(lightIndex, lightPos+Vector3(randRadius(rnddev),0, randRadius(rnddev)));
	// �p�[�e�B�N���ǉ�
	ParticleManager::Add(particleProp);
	// �����Ă��鎞�̂ݓ����蔻������
	healZone.SetCollisionMask(CollisionMask::PlayerHeal);
}

void Candle::OnCollision(RayCollider* rayCollider)
{
	if (Length(rayCollider->GetWorldPosition() - worldTransform.GetWorldPosition()) >= 12.0f) { return; }
	ui->SetIsInvisible(Fire != &Candle::Dark);
	ui->SetPosition(To2DVector(worldTransform.GetWorldPosition() + Vector3(0, -3, 0)));
	if (!isExist) { return; }
	if (!Input::GetInstance()->IsTrigger(Mouse::Left)) { return; }
	if (Fire != &Candle::Dark) { return; }
	Fire = &Candle::PreLight;
	particleTimer = 60;
	playerPos = rayCollider->GetWorldPosition();
	lightedNum++; // ���������𑝂₷
	// �v���C���[��HP����
	Player* pPlayer = dynamic_cast<Player*>(rayCollider);
	if (pPlayer) { pPlayer->HPDecrease(40); }
}
#pragma endregion

#pragma region Block
void Block::Initialize(const GimmickParam& param)
{
	// �e�N�X�`���ǂݍ���
	std::unique_ptr<Sprite> sprite;
	switch (param.textureIndex)
	{
	case 0:	sprite = Sprite::Create("stages/wall.png");		break;
	case 1:	sprite = Sprite::Create("stages/floor.png");	break;
	}
	sprite->SetSize(sprite->GetSize() / max(max(param.scale.x, param.scale.y), param.scale.z) * 10.0f);
	// ���f���ǂݍ���
	model = Model::Create("cube");
	model->SetSprite(std::move(sprite));
	model->Update();

	// �p�����[�^�Z�b�g
	Gimmick::Initialize(param);
	if (param.vanishFlag == 1) { blockState |= (int)BlockStatus::VANISH_RED; }			// �ԉ��̎�������t���O
	else if (param.vanishFlag == 2) { blockState |= (int)BlockStatus::VANISH_BLUE; }	// ���̎�������t���O
	if (param.moveFlag == 1)
	{
		blockState |= (int)BlockStatus::MOVE; isMove = true;
	}	// �������ǂ���
	for (auto& pathPoint : param.pathPoints) { pathPoints.push_back(pathPoint); }		// �o�H�_�擾
	if (param.eventIndex != 0) { eventIndex = param.eventIndex; isMove = false; }
}

void Block::Update()
{
	CheckIsCameraCapture();
	// �����蔻��ݒ�
	if ((blockState & (int)BlockStatus::VANISH_RED) && player->IsRedFire()) { collisionMask = CollisionMask::None; }
	else if ((blockState & (int)BlockStatus::VANISH_BLUE) && player->IsBlueFire()) { collisionMask = CollisionMask::None; }
	else { collisionMask = CollisionMask::Block; }
	// �ړ�
	if (eventIndex != 0) { isMove = Switch::CheckEventFlag(eventIndex); }
	if (blockState & (int)BlockStatus::MOVE && isMove == true) { Move(); }
	// �X�V
	worldTransform.Update();
}

void Block::Draw()
{
	if (!isCameraCapture) { return; }
	// �����蔻�肪�Ȃ��Ȃ�`�悵�Ȃ�
	if (collisionMask == CollisionMask::None) { return; }
	// ����Ȃ�`��
	else { Gimmick::Draw(); }
}

void Block::Move()
{
	// �C���^�[�o�����Ȃ�X���[
	if (timeRate >= 1.0f)
	{
		if (!interval.CountDown()) { return; }
		pathIndex = NumberLoop(pathIndex + 1, pathPoints.size() - 1);
		timeRate = 0;
	}
	// �n�_����I�_�̋���
	Vector3 start = pathPoints[pathIndex];
	// NumberLoop�֐��Ŕz��O�A�N�Z�X��j�~
	Vector3 end = pathPoints[NumberLoop(pathIndex + 1, pathPoints.size() - 1)];
	Vector3 vec = start - end;
	timeRate += 0.1f / vec.Length();
	// �ړ�(���`���)
	worldTransform.translation = Lerp(start, end, min(timeRate, 1.0f));
}
#pragma endregion

#pragma region Switch
void Switch::Initialize(const GimmickParam& param)
{
	// �e�N�X�`���ǂݍ���
	std::unique_ptr<Sprite> sprite;
	switch (param.textureIndex)
	{
	case 0:	sprite = Sprite::Create("white1x1.png");		break;
	}
	sprite->SetSize(sprite->GetSize() / max(max(param.scale.x, param.scale.y), param.scale.z) * 10.0f);
	// ���f���ǂݍ���
	model = Model::Create("switch_table");
	model_lever = Model::Create("switch_lever");
	//model->SetSprite(std::move(sprite));
	model->Update();
	// �p�����[�^�Z�b�g
	Gimmick::Initialize(param);
	wo2.parent = &worldTransform;
	wo2.Initialize();
	SwitchParam sw;
	if (param.eventIndex != 0) { sw.eventIndex = param.eventIndex; }
	// �R���e�i�Ƀv�b�V��
	switches.push_back(sw);
	// �C�e���[�^���Z�b�g
	swItr = switchNum;
	// �C���N�������g
	switchNum++;
}

void Switch::Update()
{
	if (switches[swItr].isFlag == false)
	{
		wo2.rotation.z = 30 * PI / 180;
	}
	else
	{
		wo2.rotation.z = -30 * PI / 180;
	}

	// �X�V
	worldTransform.Update();
	wo2.Update();
}

void Switch::Draw()
{
	model_lever->Draw(wo2);
	Gimmick::Draw();
}

bool Switch::CheckEventFlag(const UINT16 index)
{
	for (auto& sw : switches)
	{
		if (index != sw.eventIndex) { continue; }
		if (!sw.isFlag) { return false; }
	}
	return true;
}

void Switch::OnCollision(RayCollider* rayCollider)
{
	if (Length(rayCollider->GetWorldPosition() - worldTransform.GetWorldPosition()) >= 8.0f) { return; }
	if (!Input::GetInstance()->IsTrigger(Mouse::Left)) { return; }
	switches[swItr].isFlag = true;
}
#pragma endregion
