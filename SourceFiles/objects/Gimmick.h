#pragma once
#include "Collider.h"
#include "Model.h"
#include "ParticleManager.h"
#include "Input.h"

class Gimmick : public BoxCollider
{
protected:
	std::unique_ptr<Model> model;
	static bool isStart_;	// true‚É‚È‚Á‚½‚çƒJƒƒ‰‚ðˆø‚­
	static bool isGoal_;		// true‚É‚È‚Á‚½‚çƒS[ƒ‹‚Å‚«‚é
public:
	virtual ~Gimmick() { model.release(); }
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() { model->Draw(worldTransform); }
	Vector3 GetPosition() { return worldTransform.GetWorldPosition(); }
	void SetPosition(Vector3 pos) { worldTransform.translation = pos; }

	static bool GetIsStart() { return isStart_; }
	static void SetIsStart(bool isStart) { isStart_ = isStart; }
	Vector3 GetScale() { return worldTransform.scale; }
	void SetScale(Vector3 scale) { worldTransform.scale = scale; }

	static bool GetIsGoal() { return isGoal_; }
	static void SetIsGoal(bool isStart) { isGoal_ = isStart; }
};

class Door : public Gimmick
{
private:
	bool isOpen = false;
	bool isClose = false;
	
	std::unique_ptr<Model> model_back;
	WorldTransform flip;
	WorldTransform back;
	Input* input;
	float rot = 90;

	void Open();
	void Close();
public:
	void Initialize();
	void Update();
	void Draw() override;
};

class Candle : public Gimmick
{
private:
	bool isLight = true;
	size_t lightIndex = 0;
	LightGroup* lightGroup = nullptr;
	AddParticleProp particleProp;
	Vector3 lightPos;

public:
	Candle(size_t index) { lightIndex = index; }
	void Initialize();
	void Update();
	void Draw() override;
};

class Wall : public Gimmick
{
private:

public:
	Wall(Vector3 scale);
	void Initialize();
	void Update();
};