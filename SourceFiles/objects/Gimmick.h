#pragma once
#include "Collider.h"
#include "Model.h"
#include "Input.h"

class Gimmick : public SphereCollider
{
protected:
	bool isExist = false;
	std::unique_ptr<Model> model;
	static bool isStart_;//true‚É‚È‚Á‚½‚çƒJƒƒ‰‚ðˆø‚­
public:
	
	virtual ~Gimmick() = default;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() { model->Draw(worldTransform); }
	virtual void SetNum(UINT16 num) {}
	void SetIsExist(bool isExist_) { isExist = isExist_; }
	Vector3 GetPosition() { return worldTransform.translation; }
	void SetPosition(Vector3 pos) { worldTransform.translation = pos; }

	bool GetIsStart() { return isStart_; }
	void SetIsStart(bool isStart) { isStart_ = isStart; }
};

class Door : public Gimmick
{
private:
	bool isOpen = false;
	bool isClose = false;
	std::unique_ptr<Model> model_back;
	UINT16 num;
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
	void SetNum(UINT16 num_) { num = num_; }
	
};

class Candle : public Gimmick
{
private:
	bool isLight;

public:
	void Initialize();
	void Update();
};