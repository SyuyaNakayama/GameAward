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
class PlaneCollider;
class PolygonCollider;
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
	virtual void OnCollision(PlaneCollider* boxCollider) {}
	virtual void OnCollision(PolygonCollider* sphereCollider) {}
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

class PlaneCollider : public virtual BaseCollider
{
protected:
	// 基準法線
	Vector3 baseNormal = Vector3::MakeAxis(Axis::Y);
	float distance = 0;
	Vector3 inter;

public:
	PlaneCollider();
	virtual ~PlaneCollider();

	void SetInter(const Vector3& inter_) { inter = inter_; }
	void SetDistance(float distance_) { distance = distance_; }
	void SetRotation(const Vector3& rotation) { worldTransform.rotation = rotation; }
	void SetBaseNormal(const Vector3& baseNormal_) { baseNormal = baseNormal_; }
	virtual Vector3 GetNormal() { return baseNormal * Matrix4::Rotate(worldTransform.rotation); }
	virtual Vector3* GetInter() { return &inter; }
	virtual float GetDistance() { return distance; }
};

class PolygonCollider : public virtual BaseCollider
{
protected:
	// 基準法線
	Vector3 baseNormal = Vector3::MakeAxis(Axis::Y);
	// 頂点は時計回り
	std::vector<Vector3> vertices;
	float distance = 0;
	// メッシュコライダーで使う
	Vector3 normal;

public:
	PolygonCollider();
	virtual ~PolygonCollider();

	void UpdateVertices();
	void ComputeDistance() { distance = Dot(GetNormal(), vertices[0]); }
	void ComputeNormal();
	void ToPlaneCollider(PlaneCollider* planeCollider);
	void AddVertices(Vector3 pos) { vertices.push_back(pos); }
	void SetBaseNormal(Vector3 baseNormal_) { baseNormal = baseNormal_; }
	virtual Vector3 GetNormal() { return baseNormal * Matrix4::Rotate(worldTransform.rotation); }
	virtual void SetVertices();
	virtual std::vector<Vector3> GetVertices() { return vertices; }
};

class RayCollider : public virtual BaseCollider
{
public:
	// 基準レイ
	Vector3 baseRayDirection = Vector3::MakeAxis(Axis::Z);
	RayCollider();
	virtual ~RayCollider();

	virtual const Vector3 GetRayDirection() { return baseRayDirection * Matrix4::Rotate(worldTransform.rotation); }
};