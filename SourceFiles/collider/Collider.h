#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include <vector>
#include <array>

enum class CollisionAttribute
{
	Player = 0b1,
	Block = 0b1 << 1,
	Door = 0b1 << 2,
	PlayerHeal = 0b1 << 3,
	UI = 0b1 << 4,
	All = -1
};

enum class CollisionMask
{
	None = 0,
	Player = (int)CollisionAttribute::Block | (int)CollisionAttribute::UI| (int)CollisionAttribute::Door,
	PlayerJump = (int)CollisionAttribute::Block,
	PlayerHeal = (int)CollisionAttribute::PlayerHeal,
	Block = (int)CollisionAttribute::Player,
	UI = (int)CollisionAttribute::Player,
	All = -1
};

class BoxCollider;
class SphereCollider;
class RayCollider;

class BaseCollider
{
protected:
	CollisionAttribute collisionAttribute = CollisionAttribute::All;
	CollisionMask collisionMask = CollisionMask::All;
	WorldTransform worldTransform;

public:
	virtual ~BaseCollider() = default;

	virtual void OnCollision(BoxCollider* boxCollider) {}
	virtual void OnCollision(SphereCollider* sphereCollider) {}
	virtual void OnCollision(RayCollider* sphereCollider) {}

	CollisionAttribute GetCollisionAttribute() { return collisionAttribute; }
	CollisionMask GetCollisionMask() { return collisionMask; }
	void SetWorldTransform(const WorldTransform& worldTransform_) { worldTransform = worldTransform_; }
	virtual Vector3 GetWorldPosition() { return worldTransform.GetWorldPosition(); }
};

class BoxCollider : public virtual BaseCollider
{
public:
	BoxCollider();
	virtual ~BoxCollider();

	virtual Vector3 GetRadius3D() { return worldTransform.scale; }
};

class SphereCollider : public virtual BaseCollider
{
public:
	SphereCollider();
	virtual ~SphereCollider();

	virtual float GetRadius() { return worldTransform.scale.x; }
};

class RayCollider : public virtual BaseCollider
{
public:
	// Šî€ƒŒƒC
	Vector3 baseRayDirection = Vector3::MakeAxis(Axis::Z);
	RayCollider();
	virtual ~RayCollider();

	virtual const Vector3 GetRayDirection() { return baseRayDirection * Matrix4::Rotate(worldTransform.rotation); }
};