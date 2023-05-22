#pragma once
#include "Collider.h"
#include <list>

class CollisionManager final
{
private:
	static std::list<BoxCollider*> boxColliders;
	static std::list<SphereCollider*> sphereColliders;
	static std::list<RayCollider*> rayColliders;

public:
	// 個別当たり判定
	static bool CheckCollisionFiltering(BaseCollider* colliderA, BaseCollider* colliderB);
	static bool CheckCollision2Boxes(BoxCollider* colliderA, BoxCollider* colliderB);
	static bool CheckCollision2Spheres(SphereCollider* colliderA, SphereCollider* colliderB);
	static bool CheckCollisionRaySphere(RayCollider* colliderA, SphereCollider* colliderB, float* distance = nullptr, Vector3* inter = nullptr);

	CollisionManager() = delete;
	// コライダー登録関数
	static void PushCollider(BoxCollider* collider) { boxColliders.push_back(collider); }
	static void PushCollider(SphereCollider* collider) { sphereColliders.push_back(collider); }
	static void PushCollider(RayCollider* collider) { rayColliders.push_back(collider); }
	static void PopCollider(BoxCollider* collider) { boxColliders.remove(collider); }
	static void PopCollider(SphereCollider* collider) { sphereColliders.remove(collider); }
	static void PopCollider(RayCollider* collider) { rayColliders.remove(collider); }
	// 種類別当たり判定
	static void CheckBoxCollisions();
	static void CheckSphereCollisions();
	static void CheckRaySphereCollisions();
	// 全当たり判定
	static void CheckAllCollisions();
};