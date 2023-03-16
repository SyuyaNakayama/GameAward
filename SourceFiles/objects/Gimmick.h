#pragma once
#include "Collider.h"
#include "Model.h"
#include "ParticleManager.h"
#include "Input.h"
#include "Collider.h"

// �M�~�b�N�̃p�����[�^
struct GimmickParam {
	Vector3 pos;		// ���W
	Vector3 scale;		// �X�P�[��
	Vector3 rot;		// ��]
	bool flag = false;	// �t���O
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
	virtual void Update(bool isLight) = 0;
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
	void Update(bool isLight);
	void Draw() override;

	//�����蔻��̑傫���𒲐�
	Vector3 GetRadius() { return { 1.8f,1.9f,1.0f }; }
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

	void Dark(); // �����Ă��Ȃ����̏���
	void PreLight(); // ����O
	void PostLight(); // ��������
public:
	// �����t���R���X�g���N�^
	Candle(size_t index) { lightIndex = index; lightNum++; }
	void OnCollision(RayCollider* rayCollider);
	void Initialize(const GimmickParam& param);
	void Update(bool isLight);
	static size_t GetLightNum() { return lightNum; }
	static void ResetLightNum() { lightNum = 0; }
};

class Wall : public Gimmick
{
private:
	// ���ʂ̕ǂ�������ǂ��̃t���O
	bool isVanish = false;
	// ���݂��邩�̃t���O
	bool isExist = true;
public:
	// �����t���R���X�g���N�^
	void Initialize(const GimmickParam& param);
	void Update(bool isLight);
	void Draw() override;
};