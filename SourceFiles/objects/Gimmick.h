#pragma once
#include "Collider.h"
#include "Model.h"
#include "ParticleManager.h"
#include "Input.h"
#include "Collider.h"
#include "Player.h"

// �M�~�b�N�̃p�����[�^
struct GimmickParam {
	Vector3 pos;		// ���W
	Vector3 scale;	// �X�P�[��
	Vector3 rot;		// ��]
	std::vector<Vector3> pathPoints;	// �o�H�_
	UINT16 vanishFlag = 0;		// �����邩�t���O
	bool moveFlag = false;		// �ړ��t���O
	UINT16 textureIndex = 0;	// �e�N�X�`���C���f�b�N�X
};

class Gimmick : public BoxCollider
{
protected:
	std::unique_ptr<Model> model;
	static bool isStart_;	// true�ɂȂ�����J����������
	static LightGroup* lightGroup;

public:
	virtual ~Gimmick() { model.release(); }
	virtual void Initialize(const GimmickParam& param);
	virtual void Update() = 0;
	virtual void Draw() { model->Draw(worldTransform); }

	// �A�N�Z�b�T
	Vector3 GetRotation() { return worldTransform.rotation; }

	static bool GetIsStart() { return isStart_; }
	static void SetIsStart(bool isStart) { isStart_ = isStart; }
};

// �h�A�N���X�̊��N���X
class BaseDoor : public Gimmick
{
protected:
	enum class WTType { L, R };

	// ���f��
	std::unique_ptr<Model> model_back;
	std::array<WorldTransform, 2> door;
public:
	virtual void Initialize(const GimmickParam& param);
	virtual void Update();
	virtual void Draw();

	//�����蔻��̑傫���𒲐�
	Vector3 GetRadius() { return { 1.8f,1.9f,1.0f }; }
};

class GoalDoor : public BaseDoor
{
protected:
	// �h�A�����Ă��鎞��nullptr�ɂȂ�
	void (GoalDoor::* Move)() = &GoalDoor::Opened;

	UINT16 doorIndex = 0;
	Input* input = Input::GetInstance();
	float rot = 90;

public:
	void Open();	// �h�A���J�����ɌĂяo�����֐�
	void Close();	// �h�A�����鎞�ɌĂяo�����֐�
	void Opened();	// �h�A���J���Ă��鎞�ɌĂяo�����֐�
	virtual void Closed();	// �h�A�����Ă��鎞�ɌĂяo�����֐�
	void Update();
	void OnCollision(BoxCollider* boxCollider);
};

// �X�e�[�W�Z���N�g��ʂ̃h�A
class SelectDoor : public GoalDoor
{
private:
	UINT16 doorIndex = 0;

	void Closed();	// �h�A�����Ă��鎞�ɌĂяo�����֐�

public:
	SelectDoor(UINT16 doorIndex_) { doorIndex = doorIndex_; }
	void OnCollision(BoxCollider* boxCollider);
};

// �X�e�[�W2�̃h�A
class RoomDoor : public BaseDoor
{
private:
	UINT16 doorIndex = 0;
	static UINT roomNum; // ���݂̕����ԍ�(�����ɂ���C��̐�)
	UINT nextRoomNum = 0; // �h�A�����������ԍ�
	static std::array<UINT, 3> allNextRoomNums; // �S�Ẵh�A�����������ԍ�
	static const UINT FINAL_ROOM_NUM = 5;

public:
	RoomDoor(UINT16 doorIndex_) { doorIndex = doorIndex_; }
	static UINT GetRoomNumber() { return roomNum; }
	void Initialize(const GimmickParam& param);
	void Update();
	void OnCollision(BoxCollider* boxCollider);
};

class KeyLock : public Gimmick
{
private:
	// �S�Ă̌��̐�
	static size_t keyNum;
	// ���W�������̐�
	static size_t collectKeyNum;
	// ����S�ďW�߂���
	static bool isCollectAll;

	// ���W�ς݂��ǂ���
	bool isCollected = false;

	// �����蔻��
	void OnCollision(BoxCollider* boxCollider);
public:
	void Initialize(const GimmickParam& param);
	void Update();
	void Draw() override;
	// ��������Ȃ����true�ŕԂ��A��������Ȃ���΃t���O��Ԃ�
	static const bool GetIsCollectAll() { if (keyNum == 0) { return true; } return isCollectAll; }
	// ���Z�b�g�֐�
	static void ResetKeyNum() { keyNum = 0; collectKeyNum = 0; }
};

class Candle : public Gimmick, public SphereCollider
{
private:
	void (Candle::* Fire)() = &Candle::Dark;

	size_t lightIndex = 0;
	DiffuseParticle::AddProp particleProp;
	Vector3 lightPos;
	static size_t lightNum;
	int particleTimer = 60; // �����p�[�e�B�N����������
	Vector3 playerPos;
	Sprite* ui = nullptr;
	bool isExist = true;

	void Dark(); // �����Ă��Ȃ����̏���
	void PreLight(); // ����O
	void PostLight(); // ��������
public:
	// �����t���R���X�g���N�^
	Candle(size_t index) { lightIndex = index; lightNum++; }
	void OnCollision(RayCollider* rayCollider);
	void Initialize(const GimmickParam& param);
	void Update();
	static size_t GetLightNum() { return lightNum; }
	static void ResetLightNum() { lightNum = 0; }
};

class Block : public Gimmick
{
public: // �񋓃N���X
	// �u���b�N�̃X�e�[�^�X
	enum class BlockStatus {
		NORMAL = 0b000,
		MOVE = 0b001,
		VANISH_RED = 0b010,
		VANISH_BLUE = 0b100,
	};
private:
	// �v���C���[
	static Player* player;

	// �u���b�N�̏��
	int blockState = (int)BlockStatus::NORMAL;
	// �ړ��֘A
	bool isMove = false;
	float speed = 0.1f;
	int interval = 0;
	std::vector<Vector3> pathPoints;
	UINT16 pathIndex = 0;

public:
	static void SetPlayerAddress(Player* pPlayer) { player = pPlayer; }
	void Initialize(const GimmickParam& param);
	void Update();
	void Draw() override;
	void Move();
};