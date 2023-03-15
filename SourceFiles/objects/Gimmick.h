#pragma once
#include "Collider.h"
#include "Model.h"
#include "ParticleManager.h"
#include "Input.h"
#include "Collider.h"

// �M�~�b�N�̃p�����[�^
struct GimmickParam {
	Vector3 pos;		// ���W
	Vector3 scale;	// �X�P�[��
	Vector3 rot;		// ��]
	bool flag = false;	// �t���O
};

class Gimmick : public BoxCollider
{
protected:
	std::unique_ptr<Model> model;
	static bool isStart_;	// true�ɂȂ�����J����������
	static bool isGoal_;		// true�ɂȂ�����S�[���ł���
	static LightGroup* lightGroup;
	GimmickParam gimmickParam;

public:
	virtual ~Gimmick() { model.release(); }
	virtual void Initialize() = 0;
	virtual void Update(bool isLight) = 0;
	virtual void Draw() { model->Draw(worldTransform); }

	// �A�N�Z�b�T
	Vector3 GetPosition() { return worldTransform.GetWorldPosition(); }
	void SetPosition(Vector3 pos) { worldTransform.translation = pos; }
	Vector3 GetScale() { return worldTransform.scale; }
	void SetScale(Vector3 scale) { worldTransform.scale = scale; }
	Vector3 GetRotation() { return worldTransform.rotation; }
	void SetRotation(Vector3 rot) { worldTransform.rotation = rot * (PI / 180); }

	void SetParameter(GimmickParam param) { SetPosition(param.pos); SetScale(param.scale); SetRotation(param.rot); }

	static bool GetIsStart() { return isStart_; }
	static void SetIsStart(bool isStart) { isStart_ = isStart; }

	static bool GetIsGoal() { return isGoal_; }
	static void SetIsGoal(bool isStart) { isGoal_ = isStart; }
};

class Door : public Gimmick
{
private:
	bool isOpen = false;	//�����J���t���O
	bool isOpened = false;	//�����󂢂Ă邩�ǂ���
	bool isClose = false;	//�����܂�t���O

	std::unique_ptr<Model> model_back;
	WorldTransform doorL;
	WorldTransform doorR;
	Input* input = nullptr;
	float rot = 90;

	void OnCollision(BoxCollider* boxCollider);
	void Open();
	void Close();
public:
	// �����t���R���X�g���N�^
	Door(GimmickParam gimmickParam) { this->gimmickParam = gimmickParam; }
	void Initialize();
	void Update(bool isLight);
	void Draw() override;

	//�����蔻��̑傫���𒲐�
	Vector3 GetRadius() { return { 1.8f,1.9f,1.0f }; }
};

class Candle : public Gimmick, public SphereCollider
{
private:
	bool isLight_ = false;
	size_t lightIndex = 0;
	AddParticleProp particleProp;
	Vector3 lightPos;
	static size_t lightNum;

public:
	// �����t���R���X�g���N�^
	Candle(GimmickParam gimmickParam, size_t index) { this->gimmickParam = gimmickParam; lightIndex = index; lightNum++; }
	void OnCollision(RayCollider* rayCollider);
	void Initialize();
	void Update(bool isLight);
	void Draw() override;
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
	Wall(GimmickParam gimmickParam) { this->gimmickParam = gimmickParam; }
	void Initialize();
	void Update(bool isLight);
	void Draw() override;
};