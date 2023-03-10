#pragma once
#include <Input.h>
#include <Model.h>
#include "LightGroup.h"
#include "Camera.h"
#include "Collider.h"

class Player : public BoxCollider
{
private:
	Input* input_;
	std::unique_ptr<Model> model_;
	std::unique_ptr<Sprite> sprite_;
	// カメラ
	Camera eyeCamera;
	bool isCameraChange = false;
	// ライト
	LightGroup* lightGroup_;
	bool useLight;
	// 前フレーム座標
	Vector3 prePos;

	// 移動処理
	void Move();
	// ライト切り替え
	void ChangeLight();

public:
	void Initialize();
	void Update();
	void Draw();

	// アクセッサ
	WorldTransform GetWorldTransform() { return worldTransform; }
	bool IsCameraChange() { return isCameraChange; }
	// デストラクタ
	~Player() { sprite_.release(); }

	// 当たり判定の処理
	void OnCollision(BoxCollider* boxCollider) override;
	//void OnCollision(SphereCollider* sphereCollider) override;
	//void OnCollision(PlaneCollider* boxCollider) override;
	//void OnCollision(PolygonCollider* sphereCollider) override;
	//void OnCollision(RayCollider* sphereCollider) override;
	//void OnCollision(IncludeCollider* sphereCollider) override;
};

