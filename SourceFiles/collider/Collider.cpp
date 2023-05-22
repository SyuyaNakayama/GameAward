#include "Collider.h"
#include "CollisionManager.h"
#include <cassert>

BoxCollider::BoxCollider() { CollisionManager::PushCollider(this); }
BoxCollider::~BoxCollider() { CollisionManager::PopCollider(this); }
SphereCollider::SphereCollider() { CollisionManager::PushCollider(this); }
SphereCollider::~SphereCollider() { CollisionManager::PopCollider(this); }
RayCollider::RayCollider() { CollisionManager::PushCollider(this); }
RayCollider::~RayCollider() { CollisionManager::PopCollider(this); }