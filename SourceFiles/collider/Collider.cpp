#include "Collider.h"
#include "CollisionManager.h"
#include <cassert>

BoxCollider::BoxCollider() { CollisionManager::PushCollider(this); }
BoxCollider::~BoxCollider() { CollisionManager::PopCollider(this); }
SphereCollider::SphereCollider() { CollisionManager::PushCollider(this); }
SphereCollider::~SphereCollider() { CollisionManager::PopCollider(this); }
RayCollider::RayCollider() { CollisionManager::PushCollider(this); }
RayCollider::~RayCollider() { CollisionManager::PopCollider(this); }
PolygonCollider::PolygonCollider() { CollisionManager::PushCollider(this); }
PolygonCollider::~PolygonCollider() { CollisionManager::PopCollider(this); }
PlaneCollider::PlaneCollider() { CollisionManager::PushCollider(this); }
PlaneCollider::~PlaneCollider() { CollisionManager::PopCollider(this); }

void PolygonCollider::SetVertices()
{
	Vector3 objPos = worldTransform.translation;
	Vector3 objRad = worldTransform.scale;
	vertices.clear();
	vertices.push_back(objPos + Vector3(-objRad.x, objRad.y, -objRad.z));
	vertices.push_back(objPos + Vector3(objRad.x, objRad.y, -objRad.z));
	vertices.push_back(objPos + Vector3(objRad.x, -objRad.y, -objRad.z));
	vertices.push_back(objPos - objRad);
}

void PolygonCollider::ComputeNormal()
{
	assert(vertices.size() >= 3);
	// –@ü‚ÌŒvŽZ
	Vector3 vec1 = vertices[2] - vertices[0];
	Vector3 vec2 = vertices[1] - vertices[0];
	normal = Normalize(Cross(vec1, vec2));
}

void PolygonCollider::ToPlaneCollider(PlaneCollider* planeCollider)
{
	planeCollider->SetDistance(distance);
	planeCollider->SetRotation(worldTransform.rotation);
	planeCollider->SetBaseNormal(baseNormal);
}

void PolygonCollider::UpdateVertices()
{
	for (Vector3& vertex : vertices) { vertex *= worldTransform.matWorld; }
}