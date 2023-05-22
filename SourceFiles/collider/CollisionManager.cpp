#include "CollisionManager.h"
#include <algorithm>
using namespace std;

list<BoxCollider*> CollisionManager::boxColliders;
list<SphereCollider*> CollisionManager::sphereColliders;
list<RayCollider*> CollisionManager::rayColliders;

bool CollisionManager::CheckCollisionFiltering(BaseCollider* colliderA, BaseCollider* colliderB)
{
	return
		(UINT)colliderA->GetCollisionAttribute() & (UINT)colliderB->GetCollisionMask() &&
		(UINT)colliderB->GetCollisionAttribute() & (UINT)colliderA->GetCollisionMask();
}

bool CollisionManager::CheckCollision2Boxes(BoxCollider* colliderA, BoxCollider* colliderB)
{
	if (!CheckCollisionFiltering(colliderA, colliderB)) { return false; }

	Vector3 vecAB = colliderA->GetWorldPosition() - colliderB->GetWorldPosition();
	Vector3 radAB = colliderA->GetRadius3D() + colliderB->GetRadius3D();
	vecAB = vecAB.abs();

	return vecAB <= radAB;
}

bool CollisionManager::CheckCollision2Spheres(SphereCollider* colliderA, SphereCollider* colliderB)
{
	if (!CheckCollisionFiltering(colliderA, colliderB)) { return false; }

	Vector3 vecAB = colliderA->GetWorldPosition() - colliderB->GetWorldPosition();
	float radAB = colliderA->GetRadius() + colliderB->GetRadius();

	return vecAB.Length() <= radAB;
}

bool CollisionManager::CheckCollisionRaySphere(RayCollider* colliderA, SphereCollider* colliderB, float* distance, Vector3* inter)
{
	Vector3 m = colliderA->GetWorldPosition() - colliderB->GetWorldPosition();
	float b = Dot(m, colliderA->GetRayDirection());
	float c = Dot(m, m) - colliderB->GetRadius() * colliderB->GetRadius();
	// レイの始点がsphereの外側にあり(c > 0)、レイが離れていく方向を差している場合(b > 0)、当たらない
	if (c > 0.0f && b > 0.0f) { return false; }

	float discr = b * b - c; // 判別式
	// 負の判別式はレイが球から外れている
	if (discr < 0.0f) { return false; }

	// レイは球と交差している
	float t = -b - sqrtf(discr); // 交差する最小の値tを計算
	// tが負である場合、レイは球の内側から開始しているのでtを0にクランプ
	if (t < 0) { t = 0.0f; }

	if (distance) { *distance = t; }
	if (inter) { *inter = colliderA->GetWorldPosition() + t * colliderA->GetRayDirection(); }
	return true;
}

void CollisionManager::CheckBoxCollisions()
{
	auto itrA = boxColliders.begin();
	for (; itrA != boxColliders.end(); itrA++)
	{
		list<BoxCollider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != boxColliders.end(); itrB++)
		{
			if (!CheckCollision2Boxes(*itrA, *itrB)) { continue; }

			(*itrA)->OnCollision(*itrB);
			(*itrB)->OnCollision(*itrA);
		}
	}
}

void CollisionManager::CheckSphereCollisions()
{
	auto itrA = sphereColliders.begin();
	for (; itrA != sphereColliders.end(); itrA++)
	{
		list<SphereCollider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != sphereColliders.end(); itrB++)
		{
			if (!CheckCollision2Spheres(*itrA, *itrB)) { continue; }

			(*itrA)->OnCollision(*itrB);
			(*itrB)->OnCollision(*itrA);
		}
	}
}

void CollisionManager::CheckRaySphereCollisions()
{
	if (rayColliders.empty() || sphereColliders.empty()) { return; }
	for (RayCollider* rayCollider : rayColliders) {
		for (SphereCollider* sphereCollider : sphereColliders)
		{
			if (!CheckCollisionRaySphere(rayCollider, sphereCollider)) { continue; }

			rayCollider->OnCollision(sphereCollider);
			sphereCollider->OnCollision(rayCollider);
		}
	}
}

void CollisionManager::CheckAllCollisions()
{
	CheckBoxCollisions();
	CheckSphereCollisions();
	CheckRaySphereCollisions();
}