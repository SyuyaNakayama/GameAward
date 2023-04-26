#include <imgui.h>
#include <random>
#include "Gimmick.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "Stage.h"
#include "SceneManager.h"
#include "UIDrawer.h"

#pragma region �ÓI�����o�ϐ��̏�����
// �M�~�b�N���N���X
LightGroup* Gimmick::lightGroup = nullptr;
std::vector<EventParam> Gimmick::events;
// �C��
size_t Candle::lightNum = 0;
size_t Candle::lightedNum = 0;
// ��
Player* Block::player = nullptr;
// ��
size_t KeyLock::keyNum = 0;
// �X�e�[�W2�̃h�A
UINT RoomDoor::roomNum = 1;
std::array<UINT, 3> RoomDoor::allNextRoomNums;
// �X�C�b�`
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
	// �X�P�[����farZ / 2.0f�ȏ�̂Ƃ��͔��肵�Ȃ�
	for (size_t i = 0; i < 3; i++)
	{
		if (worldTransform.scale[i] < vp->farZ / 2.0f) { continue; }
		return;
	}
	// �J�����ʒu����I�u�W�F�N�g�܂ł̋������v�Z
	float dis = Length(worldTransform.GetWorldPosition() - vp->eye);
	// ������farZ�ȏ�̂Ƃ��͕`�悵�Ȃ�
	if (dis >= vp->farZ) { isCameraCapture = false; }
}

bool Gimmick::CheckEventFlag(const UINT16 index)
{
	for (auto& event_ : events)
	{
		// �C�x���g�C���f�b�N�X���������R���e�B�j���[
		if (index != event_.eventIndex) { continue; }
		// �t���O�������ĂȂ�������
		if (!event_.isFlag) {
			// ���Aeither��false�Ȃ�false��Ԃ�
			if (!event_.isEither) { return false; }
			continue;
		}
	}
	return true;
}

#pragma region Door
void BaseDoor::Initialize(const GimmickParam& param)
{
	// �p�����[�^�Z�b�g
	Gimmick::Initialize(param);
	// ���f���ǂݍ���
	model = Model::Create("door", true);
	model_back = Model::Create("door_back");
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

// �h�A�̍X�V����
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

void GoalDoor::Initialize(const GimmickParam& param)
{
	BaseDoor::Initialize(param);
	// �߂�
	door[(int)WTType::L].rotation.y = PI;
	door[(int)WTType::R].rotation.y = 0;
}

void GoalDoor::Update()
{
	assert(Move);
	(this->*Move)(); // ���𓮂���
	BaseDoor::Update();
}

// �h�A���J��
void GoalDoor::Open()
{
	if (++rot >= 90) { Move = &GoalDoor::Opened; }

	door[(int)WTType::L].rotation.y = (rot + 180) * PI / 180;
	door[(int)WTType::R].rotation.y = -rot * PI / 180;
}

void GoalDoor::Closed()
{
	// �h�A���J����
	if (Candle::GetLightNum() == Candle::GetLightedNum()) { Move = &GoalDoor::Open; }
}

// �h�A�ɓ���������
void GoalDoor::OnCollision(BoxCollider* boxCollider)
{
	if (Move != &GoalDoor::Opened) { return; } // �h�A���󂢂Ă��鎞�S�[��
	SceneManager::GetInstance()->SetNextScene(Scene::Title);
	Stage::SetStageNum(0);
	// �v���C���[�ȊO�̃��C�g���I�t��
	for (size_t i = 1; i < LightGroup::POINT_LIGHT_NUM; i++)
	{
		lightGroup->SetPointLightActive(i, false);
	}
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
	// �v���C���[�ȊO�̃��C�g���I�t��
	for (size_t i = 1; i < LightGroup::POINT_LIGHT_NUM; i++)
	{
		lightGroup->SetPointLightActive(i, false);
	}
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
	candlePlaneObj.translation.y += 7.5f;
	candlePlaneObj.translation.z -= 0.05f;
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
	// �v���C���[�ȊO�̃��C�g���I�t��
	for (size_t i = 1; i < LightGroup::POINT_LIGHT_NUM; i++)
	{
		lightGroup->SetPointLightActive(i, false);
	}
}
#pragma endregion

#pragma region KeyLock
void KeyLock::Initialize(const GimmickParam& param)
{
	// ���f���ǂݍ���
	model = Model::Create("key", true);
	// �p�����[�^�Z�b�g
	Gimmick::Initialize(param);
	EventParam key;
	if (param.eventIndex != 0) { key.eventIndex = param.eventIndex; }
	if (param.isEither) { key.isEither = param.isEither; }
	key.KorS = false;
	// �R���e�i�Ƀv�b�V��
	events.push_back(key);
	// �C�e���[�^���Z�b�g
	eventItr = keyNum + Switch::switchNum;
	// �C���N�������g
	keyNum++;
}

void KeyLock::Update()
{
	// �X�V
	worldTransform.Update();
}

void KeyLock::Draw()
{
	// �܂��擾����ĂȂ��Ȃ�`�悷��
	if (!events[eventItr].isFlag) { model->Draw(worldTransform); }
}

void KeyLock::OnCollision(BoxCollider* boxCollider)
{
	// �t���O���I����
	events[eventItr].isFlag = true;
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

	// �X�e�[�W1�̏ꍇ
	if (Stage::GetStageNum() == (UINT)Stage::StageNum::Stage1)
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
	lightGroup->SetPointLightPos(lightIndex, lightPos + Vector3(randRadius(rnddev), 0, randRadius(rnddev)));
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
	if (!Input::GetInstance()->IsTrigger(Key::Lshift) && !Input::GetInstance()->IsTrigger(Key::Rshift)) { return; }
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
	if(param.vanishFlag == 3){model = Model::Create("keyDoor");}

	// �p�����[�^�Z�b�g
	Gimmick::Initialize(param);
	if (param.vanishFlag == 1) { blockState |= (int)BlockStatus::VANISH_RED; }			// �ԉ��̎�������t���O
	else if (param.vanishFlag == 2) { blockState |= (int)BlockStatus::VANISH_BLUE; }	// ���̎�������t���O
	else if (param.vanishFlag == 3) { blockState |= (int)BlockStatus::VANISH_KEY; }	// ������������������t���O
	if (param.moveFlag) { blockState |= (int)BlockStatus::MOVE; isMove = true; }
	if (param.repeatFlag) { blockState |= (int)BlockStatus::REPEAT; }
	// �������ǂ���
	if (!param.pathPoints.empty())
	{
		pathPoints.push_back(param.pos); // �n�_�͏������W
		for (auto& pathPoint : param.pathPoints) { pathPoints.push_back(pathPoint); } // �o�H�_�擾
		interval = param.interval;
	}
	if (param.eventIndex != 0) { eventIndex = param.eventIndex; isMove = false; }
}

void Block::Update()
{
	CheckIsCameraCapture();
	// �����蔻��ݒ�
	if ((blockState & (int)BlockStatus::VANISH_RED) && player->IsRedFire()) { collisionMask = CollisionMask::None; }
	else if ((blockState & (int)BlockStatus::VANISH_BLUE) && player->IsBlueFire()) { collisionMask = CollisionMask::None; }
	else if (blockState & (int)BlockStatus::VANISH_KEY) {
		if (collisionMask != CollisionMask::None) { collisionMask = CollisionMask::Block; }
	}
	else { collisionMask = CollisionMask::Block; }
	// �ړ�
	if (blockState & (int)BlockStatus::MOVE) { isMove = CheckEventFlag(eventIndex); }
	if (blockState & (int)BlockStatus::MOVE && isMove == true) { Move(); }
	// �X�V
	worldTransform.Update();
	if (blockState & (int)BlockStatus::VANISH_KEY)
	{
		UIDrawer::GetUI(15)->SetIsInvisible(true);
	}
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
	// ���̃C���f�b�N�X��Ԃ��֐�
	auto nextPathIndex = [&] { return std::clamp(pathIndex + (isTurn ? -1 : 1), 0, (int)pathPoints.size() - 1); };

	// �C���^�[�o�����Ȃ�X���[
	if (timeRate >= 1.0f)
	{
		if (!interval.CountDown()) { return; }
		pathIndex = nextPathIndex();
		if (blockState & (int)BlockStatus::REPEAT)
		{
			if (pathIndex >= pathPoints.size() - 1) { isTurn = true; }
			if (pathIndex < 1) { isTurn = false; }
		}
		timeRate = 0;
	}
	// �n�_����I�_�̋���
	Vector3 start = pathPoints[pathIndex];
	Vector3 end = pathPoints[nextPathIndex()];
	Vector3 vec = start - end;
	timeRate += 0.15f / vec.Length();
	// �ړ�(���`���)
	worldTransform.translation = Lerp(start, end, min(timeRate, 1.0f));
}

void Block::OnCollision(BoxCollider* boxCollider)
{
	// ���h�A�̏���
	if (!(blockState & (int)BlockStatus::VANISH_KEY)) { return; } // ���h�A����Ȃ���
	if (!CheckEventFlag(eventIndex)) { return; }
	Sprite* ui=UIDrawer::GetUI(15);
	ui->SetIsInvisible(false);
	ui->SetPosition(To2DVector(worldTransform.GetWorldPosition() + Vector3(0, -6, 0)));
	if (!Input::GetInstance()->IsTrigger(Key::Lshift) && !Input::GetInstance()->IsTrigger(Key::Rshift)) { return; } // Shift�L�[�������ĂȂ���
	collisionMask = CollisionMask::None;
}
#pragma endregion

#pragma region Switch
void Switch::Initialize(const GimmickParam& param)
{
	// ���f���ǂݍ���
	model = Model::Create("switch_table");
	model_lever = Model::Create("switch_lever");
	// �p�����[�^�Z�b�g
	Gimmick::Initialize(param);
	wo2.parent = &worldTransform;
	wo2.Initialize();
	EventParam sw;
	if (param.eventIndex != 0) { sw.eventIndex = param.eventIndex; }
	if (param.isEither) { sw.isEither = param.isEither; }
	sw.KorS = true;
	// �R���e�i�Ƀv�b�V��
	events.push_back(sw);
	// �C�e���[�^���Z�b�g
	eventItr = switchNum + KeyLock::keyNum;
	// �C���N�������g
	switchNum++;
	// UI�擾
	ui = UIDrawer::GetUI((size_t)14 + Input::GetInstance()->IsConnectGamePad());
}

void Switch::Update()
{
	if (!events[eventItr].isFlag) { wo2.rotation.z = 30 * PI / 180; }
	else { wo2.rotation.z = -30 * PI / 180; }

	// �X�V
	worldTransform.Update();
	wo2.Update();
	ui->SetIsInvisible(true);
}

void Switch::Draw()
{
	model_lever->Draw(wo2);
	Gimmick::Draw();
}

void Switch::OnCollision(RayCollider* rayCollider)
{
	if (Length(rayCollider->GetWorldPosition() - worldTransform.GetWorldPosition()) >= 8.0f) { return; }
	if(!events[eventItr].isFlag)
	{
		ui->SetIsInvisible(false);
		ui->SetPosition(To2DVector(worldTransform.GetWorldPosition() + Vector3(0, -3, 0)));
	}
	if (!Input::GetInstance()->IsTrigger(Key::Lshift) && !Input::GetInstance()->IsTrigger(Key::Rshift)) { return; }
	events[eventItr].isFlag = true;
}
#pragma endregion
