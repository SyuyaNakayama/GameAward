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
	Vector3 scale;		// �X�P�[��
	Vector3 rot;		// ��]
	int flag = false;	// �t���O
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

class Door : public Gimmick
{
private:
	enum class WTType { L, R };

	// �h�A�����Ă��鎞��nullptr�ɂȂ�
	void (Door::* Move)() = &Door::Opened;

	UINT16 doorIndex = 0;
	std::unique_ptr<Model> model_back;
	std::array<WorldTransform, 2> door;
	Input* input = Input::GetInstance();
	float rot = 90;

	void Open();	// �h�A���J�����ɌĂяo�����֐�
	void Close();	// �h�A�����鎞�ɌĂяo�����֐�
	void Opened();	// �h�A���J���Ă��鎞�ɌĂяo�����֐�
	void Closed();	// �h�A�����Ă��鎞�ɌĂяo�����֐�
	void OnCollision(BoxCollider* boxCollider);
public:
	// �����t���R���X�g���N�^
	Door(UINT16 doorIndex_) { doorIndex = doorIndex_; }
	void Initialize(const GimmickParam& param);
	void Update();
	void Draw() override;

	//�����蔻��̑傫���𒲐�
	Vector3 GetRadius() { return { 1.8f,1.9f,1.0f }; }
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
	bool isCollect = false;

	// �����蔻��
	void OnCollision(BoxCollider* boxCollider);
public:
	void Initialize(const GimmickParam& param);
	void Update();
	void Draw() override;
	static bool GetIsCollectAll() { return isCollectAll; }
	static void ResetKeyNum() { keyNum = 0; collectKeyNum = 0; }
};

class Candle : public Gimmick, public SphereCollider
{
private:
	void (Candle::* Fire)() = &Candle::Dark;

	bool isLight = false;
	size_t lightIndex = 0;
	DiffuseParticle::AddProp particleProp;
	Vector3 lightPos;
	static size_t lightNum;
	int particleTimer = 60; // �����p�[�e�B�N����������
	Vector3 playerPos;
	Sprite* ui = nullptr;

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

class Wall : public Gimmick
{
public: // �񋓃N���X
	// �ǂ̃X�e�[�^�X
	enum class WallStatus {
		NORMAL	= 0b000,
		MOVE		= 0b001,
		VANISH_RED		= 0b010,
		VANISH_BLUE	= 0b100,
	};
private:
	// �ǂ̏��
	int wallState;
	// �t���O
	bool isMove;
	// �v���C���[
	static Player* player;

public:
	static void SetPlayerAddress(Player* pPlayer) { player = pPlayer; }
	void Initialize(const GimmickParam& param);
	void Update();
	void Draw() override;
	void Move();
};