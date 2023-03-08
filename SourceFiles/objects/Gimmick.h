#pragma once
#include "Collider.h"
#include "Model.h"

class Gimmick : public BoxCollider
{
protected:
	bool isExist = false;
	std::unique_ptr<Model> model;

public:
	virtual ~Gimmick() = default;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() { model->Draw(worldTransform); }
	virtual void SetNum(UINT16 num) {}
	void SetIsExist(bool isExist_) { isExist = isExist_; }
	Vector3 GetPosition() { return worldTransform.translation; }
	void SetPosition(Vector3 pos) { worldTransform.translation = pos; }
	Vector3 GetScale() { return worldTransform.scale; }
	void SetScale(Vector3 scale) { worldTransform.scale = scale; }
};

class Door : public Gimmick
{
private:
	bool isOpen = false;
	UINT16 num;

	WorldTransform flip;

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

class Wall : public Gimmick
{
private:

public:
	Wall(Vector3 scale);
	void Initialize();
	void Update();
};