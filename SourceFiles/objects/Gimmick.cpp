#include <imgui.h>
#include <random>
#include "Gimmick.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "Stage.h"
#include "SceneManager.h"
#include "UIDrawer.h"

bool Gimmick::isStart_;
LightGroup* Gimmick::lightGroup = nullptr;
size_t Candle::lightNum = 0;
Player* Block::player = nullptr;
// ���̐ÓI�����o�ϐ��̏�����
size_t KeyLock::keyNum = 0;
size_t KeyLock::collectKeyNum = 0;
bool KeyLock::isCollectAll = false;

void Gimmick::Initialize(const GimmickParam& param)
{
	worldTransform.translation = param.pos;
	worldTransform.scale = param.scale;
	worldTransform.rotation = param.rot * (PI / 180);
}

/// <summary>
/// Door�N���X�̏���������
/// </summary>
void Door::Initialize(const GimmickParam& param)
{
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
void Door::Update()
{
	assert(Move);
	(this->*Move)(); // ���𓮂���

	for (auto& w : door) { w.Update(); }
	worldTransform.Update();
}

/// <summary>
/// �h�A���J��
/// </summary>
void Door::Open()
{
	if (++rot >= 90) { Move = &Door::Opened; }

	door[(int)WTType::L].rotation.y = (rot + 180) * PI / 180;
	door[(int)WTType::R].rotation.y = -rot * PI / 180;
}

/// <summary>
/// �h�A�����
/// </summary>
void Door::Close()
{
	if (--rot <= 0)
	{
		isStart_ = true;
		Move = &Door::Closed;
	}

	door[(int)WTType::L].rotation.y = (rot + 180) * PI / 180;
	door[(int)WTType::R].rotation.y = -rot * PI / 180;
}

void Door::Opened()
{
	if (input->IsTrigger(Key::P)) { Move = &Door::Close; }	// ����߂�
}

void Door::Closed()
{
	if (input->IsTrigger(Key::O)) { Move = &Door::Open; }
	// �S�[������
	// Player�̃��C�g�C���f�b�N�X�͔�΂�
	switch (SceneManager::GetInstance()->GetNowScene())
	{
	case Scene::Title:
		if (lightGroup->GetPointLightActive(doorIndex))
		{
			// ���C�g�����Ă��鎞
			Move = &Door::Open;
		}
		return;
	case Scene::Play:
		for (size_t i = 1; i <= Candle::GetLightNum(); i++)
		{
			if (lightGroup->GetPointLightActive(i) && KeyLock::GetIsCollectAll())
			{
				// ���C�g�����Ă��鎞
				Move = &Door::Open;
				continue;
			}
			// ���C�g�����Ă��Ȃ����͊֐����I������
			Move = &Door::Closed;
			return;
		}
	}
}

/// <summary>
/// �h�A�ɓ���������
/// </summary>
void Door::OnCollision(BoxCollider* boxCollider)
{
	if (Move != &Door::Opened) { return; } // �h�A���󂢂Ă��鎞�S�[��

	SceneManager* sceneManager = SceneManager::GetInstance();

	switch (sceneManager->GetNowScene())
	{
	case Scene::Title:
		Stage::SetStageNum(doorIndex);
		sceneManager->SetNextScene(Scene::Play);
		return;
	case Scene::Play:
		//sceneManager->SetNextScene(Scene::Clear);
		return;
	}
}

/// <summary>
/// �h�A�̕`�揈��
/// </summary>
void Door::Draw()
{
	for (auto& w : door) { model->Draw(w); }
	model_back->Draw(worldTransform);
}

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
}

void Candle::Update()
{
	worldTransform.Update();
	(this->*Fire)();
	model->Update();
	isLight = false;
	ui->SetIsInvisible(true);
}

void Candle::Dark()
{
	lightGroup->SetPointLightActive(lightIndex, false);
	model->SetAnbient({ 0.1f,0.1f,0.1f });
	if (isLight)
	{
		Fire = &Candle::PreLight;
		particleTimer = 60;
	}
}

void Candle::PreLight()
{
	if (--particleTimer <= 0) { Fire = &Candle::PostLight; }
	std::random_device rnd;
	std::mt19937 rnddev(rnd());
	std::uniform_real_distribution<float> randRadius(0, 2.0f);
	std::uniform_real_distribution<float> randAngle(-PI / 2.0f, PI / 2.0f);
	DirectionalParticle::AddProp particleProp =
	{
		playerPos,worldTransform.translation + Vector3(0, worldTransform.scale.y + 1.2f),
		0.5f,2,randAngle(rnddev),randRadius(rnddev),60
	};

	ParticleManager::Add(particleProp);
}

void Candle::PostLight()
{
	lightGroup->SetPointLightActive(lightIndex, true);
	model->SetAnbient({ 0.7f,0.3f,0.3f });
	lightPos = worldTransform.translation + Vector3(0, worldTransform.scale.y + 1.2f);

	particleProp.posOffset = lightPos;

	lightGroup->SetPointLightPos(lightIndex, lightPos);
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
	isLight = true;
	playerPos = rayCollider->GetWorldPosition();
}

void Block::Initialize(const GimmickParam& param)
{
	// �����蔻��ݒ�
	collisionAttribute = CollisionAttribute::Block;
	collisionMask = CollisionMask::Block;

	// �e�N�X�`���ǂݍ���
	std::unique_ptr<Sprite> sprite;
	switch (param.textureIndex)
	{
	case 0:	sprite = Sprite::Create("white1x1.png");		break;
	case 1:	sprite = Sprite::Create("stages/floor.png");	break;
	}
	sprite->SetSize(sprite->GetSize() / max(max(param.scale.x, param.scale.y), param.scale.z) * 10.0f);
	// ���f���ǂݍ���
	model = Model::Create("cube");
	model->SetSprite(std::move(sprite));
	model->Update();

	// ������
	worldTransform.Initialize();
	// �p�����[�^�Z�b�g
	Gimmick::Initialize(param);
	if (param.vanishFlag == 1) { blockState |= (int)BlockStatus::VANISH_RED; }			// �ԉ��̎�������t���O
	else if (param.vanishFlag == 2) { blockState |= (int)BlockStatus::VANISH_BLUE; }	// ���̎�������t���O
	if (param.moveFlag == 1) { blockState |= (int)BlockStatus::MOVE; }						// �������ǂ���
	for (auto& pathPoint : param.pathPoints) { pathPoints.push_back(pathPoint); }			// �o�H�_�擾
}

void Block::Update()
{
	// �����蔻��ݒ�
	if ((blockState & (int)BlockStatus::VANISH_RED) && !player->IsBlueFire()) { collisionMask = CollisionMask::None; }
	//else if (blockState & (int)WallStatus::VANISH_RED && player->IsRedFire()) { collisionMask = CollisionMask::None; }
	else { collisionMask = CollisionMask::Block; }
	// �ړ�
	if (blockState & (int)BlockStatus::MOVE) { Move(); }
	// �X�V
	worldTransform.Update();
}

void Block::Draw()
{
	// �����蔻�肪�Ȃ��Ȃ�`�悵�Ȃ�
	if (collisionMask == CollisionMask::None) { return; }
	// ����Ȃ�`��
	else { Gimmick::Draw(); }
}

void Block::Move()
{
	ImGui::Text("bPos : %f %f %f", worldTransform.translation.x, worldTransform.translation.y, worldTransform.translation.z);
	ImGui::Text("bIndex : %d", pathIndex);
	ImGui::Text("interval : %d", interval);
	// �C���^�[�o�����Ȃ�߂�
	if (interval > 0) { interval--; return; }

	// �o�H�_�ɓ��������Ȃ�interval�Ǝ��̌o�H�_���Z�b�g����
	if (worldTransform.translation == pathPoints[pathIndex]) {
		// �v�f�����z������擪�ɖ߂�
		if (pathIndex == (pathPoints.size() - 1)) { pathIndex = 0; }
		else { pathIndex++; }
		interval = 200;
		return;
	}

	// �v���C���[���W
	Vector3 pPos = worldTransform.translation;

	// �o�H�_�ɓ������ĂȂ��Ȃ�ړ����鏈��
	if (pPos.x != pathPoints[pathIndex].x) {
		// �o�H�_��菬�����Ȃ琳��speed��Ԃ��A�傫���Ȃ�Ȃ畉��speed�𑫂�
		pPos.x += pathPoints[pathIndex].x > pPos.x ? speed : -speed;
		// �o�H�_�̋ߎ��l��float�̌덷��␳����
		if (pathPoints[pathIndex].x - speed < pPos.x && pathPoints[pathIndex].x + speed > pPos.x) {
			pPos.x = pathPoints[pathIndex].x;
		}
	}
	// �o�H�_�ɓ������ĂȂ��Ȃ�ړ����鏈��
	if (pPos.y != pathPoints[pathIndex].y) {
		// �o�H�_��菬�����Ȃ琳��speed��Ԃ��A�傫���Ȃ�Ȃ畉��speed��Ԃ�
		pPos.y += pathPoints[pathIndex].y > pPos.y ? speed : -speed;
		// �o�H�_�̋ߎ��l��float�̌덷��␳����
		if (pathPoints[pathIndex].y - speed < pPos.y && pathPoints[pathIndex].y + speed > pPos.y) {
			pPos.y = pathPoints[pathIndex].y;
		}
	}
	// �o�H�_�ɓ������ĂȂ��Ȃ�ړ����鏈��
	if (pPos.z != pathPoints[pathIndex].z) {
		// �o�H�_��菬�����Ȃ琳��speed��Ԃ��A�傫���Ȃ�Ȃ畉��speed��Ԃ�
		pPos.z += pathPoints[pathIndex].z > pPos.z ? speed : -speed;
		// �o�H�_�̋ߎ��l��float�̌덷��␳����
		if (pathPoints[pathIndex].z - speed < pPos.z && pathPoints[pathIndex].z + speed > pPos.z) {
			pPos.z = pathPoints[pathIndex].z;
		}
	}
	// ���
	worldTransform.translation = pPos;
}
