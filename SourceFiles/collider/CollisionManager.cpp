#include "CollisionManager.h"
#include <algorithm>
using namespace std;

list<BoxCollider*> CollisionManager::boxColliders;
list<SphereCollider*> CollisionManager::sphereColliders;
list<PlaneCollider*> CollisionManager::planeColliders;
list<PolygonCollider*> CollisionManager::polygonColliders;
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

bool CollisionManager::CheckCollisionSpherePlane(SphereCollider* colliderA, PlaneCollider* colliderB, Vector3* inter)
{
	// 座標系の原点から球の中心座標への距離
	float dist = Dot(colliderA->GetWorldPosition(), colliderB->GetNormal());
	// 平面の原点座標を減算することで、平面と球の中心との距離が出る
	dist -= colliderB->GetDistance();
	// 距離の絶対値が半径より大きければ当たっていない
	if (fabsf(dist) > colliderA->GetRadius()) { return false; }
	// 疑似交点を計算
	if (inter) { *inter = -dist * colliderB->GetNormal() + colliderA->GetWorldPosition(); } // 平面上の最近接点を疑似交点とする

	return true;
}

void ClosestPtPoint2Triangle(const Vector3& point, PolygonCollider* triangle, Vector3* closest)
{
	vector<Vector3> p0_p; p0_p.push_back({});
	p0_p.push_back(triangle->GetVertices()[1] - triangle->GetVertices()[0]);
	p0_p.push_back(triangle->GetVertices()[2] - triangle->GetVertices()[0]);
	vector<float> d; d.push_back(0);
	vector<float> v;

	for (size_t i = 0; i < 3; i++)
	{
		Vector3 pi_pt = point - triangle->GetVertices()[i];
		d.push_back(Dot(p0_p[1], pi_pt));
		d.push_back(Dot(p0_p[2], pi_pt));

		switch (i)
		{
		case 0:
			// pointがp0の外側の頂点領域の中にあるかどうかチェック
			if (d[1] <= 0.0f && d[2] <= 0.0f)
			{
				// p0が最近傍
				*closest = triangle->GetVertices()[0];
				return;
			}
			break;
		default:
			bool roopNum = i == 2;
			if (d[3 * i] >= 0.0f && d[4 + roopNum] <= d[3 * i])
			{
				// p[i]が最近傍
				*closest = triangle->GetVertices()[i];
				return;
			}

			// pointがp0_p[i]の辺領域の中にあるかどうかチェックし、あればpointのp0_p[i]上に対する射影を返す
			v.push_back(d[NumberLoop(1 + 4 * roopNum, 1, 6)] * d[NumberLoop(4 + 4 * roopNum, 1, 6)] -
				d[NumberLoop(3 + 4 * roopNum, 1, 6)] * d[NumberLoop(2 + 4 * roopNum, 1, 6)]);
			if (v[0 + roopNum] <= 0.0f && d[1 + roopNum] >= 0.0f && d[3 * i] <= 0.0f)
			{
				float v = d[1 + roopNum] / (d[1 + roopNum] - d[3 * i]);
				*closest = triangle->GetVertices()[0] + v * p0_p[i];
				return;
			}

			if (!roopNum) { continue; }
			// pointがp1_p2の辺領域の中にあるかどうかチェックし、あればpointのp1_p2上に対する射影を返す
			float va = d[3] * d[6] - d[5] * d[4];
			if (va <= 0.0f && (d[4] - d[3]) >= 0.0f && (d[5] - d[6]) >= 0.0f)
			{
				float w = (d[4] - d[3]) / ((d[4] - d[3]) + (d[5] - d[6]));
				*closest = triangle->GetVertices()[1] + w * (triangle->GetVertices()[2] - triangle->GetVertices()[1]);
				return;
			}

			float denom = 1.0f / (va + v[1] + v[0]);
			float v1 = v[1] * denom;
			float w = v[0] * denom;
			*closest = triangle->GetVertices()[0] + p0_p[1] * v1 + p0_p[2] * w;
			break;
		}
	}
}

bool CollisionManager::CheckCollisionSpherePolygon(SphereCollider* colliderA, PolygonCollider* colliderB, Vector3* inter)
{
	Vector3 p;
	// 球の中心に対する最近接点である三角形上にある点pを見つける
	ClosestPtPoint2Triangle(colliderA->GetWorldPosition(), colliderB, &p);
	// 点pと球の中心の差分ベクトル
	Vector3 v = p - colliderA->GetWorldPosition();
	float vLenSq = Dot(v, v);
	// 球と三角形の距離が半径以下なら当たっていない
	if (vLenSq > colliderA->GetRadius() * colliderA->GetRadius()) { return false; } // 三角形上の最近接点pを疑似交点とする
	// 疑似交点を計算
	if (inter) { *inter = p; }
	return true;
}

bool CollisionManager::CheckCollisionRayPlane(RayCollider* colliderA, PlaneCollider* colliderB, float* distance)
{
	if (!CheckCollisionFiltering(colliderA, colliderB)) { return false; }
	const float epsilon = 1.0e-5f; // 誤差吸収用の微小な値
	// 面法線とレイの方向ベクトルの内積
	float d1 = Dot(colliderB->GetNormal(), colliderA->GetRayDirection());
	// 裏面には当たらない
	if (d1 > -epsilon) { return false; }
	// 始点と原点の距離(平面の法線方向)
	// 面法線とレイの始点座標(位置ベクトル)の内積
	float d2 = Dot(colliderB->GetNormal(), colliderA->GetWorldPosition());
	// 始点と平面の距離(平面の法線方向)
	float dist = d2 - colliderB->GetDistance();
	// 始点と平面の距離(レイ方向)
	float t = dist / -d1;
	// 交点が始点より後ろにあるので当たらない
	if (t < 0) { return false; }
	// 距離を書き込む
	if (distance) { *distance = t; }
	// 交点を計算
	colliderB->SetInter(colliderA->GetWorldPosition() + t * colliderA->GetRayDirection());
	return true;
}

bool CollisionManager::CheckCollisionRayPolygon(RayCollider* colliderA, PolygonCollider* colliderB, float* distance)
{
	if (!CheckCollisionFiltering(colliderA, colliderB)) { return false; }
	// 三角形が乗っている平面を算出
	colliderB->ComputeDistance();
	// レイと平面が当たっていなければ当っていない
	PlaneCollider tempColliderB;
	colliderB->ToPlaneCollider(&tempColliderB);

	if (!CheckCollisionRayPlane(colliderA, &tempColliderB, distance)) { return false; }
	// レイと平面が当たっていたので、距離と座標が書き込まれた
	// レイと平面の交点が三角形の内側にあるか判定
	const float epsilon = 1.0e-5f; // 誤差吸収用の微小な値

	size_t vertexSize = colliderB->GetVertices().size();
	// 頂点数が2つ以下なら判定を打ち切る
	if (vertexSize <= 2) { return false; }

	for (size_t i = 0; i < vertexSize; i++)
	{
		// 辺pi_p(i+1)について
		Vector3 pt_px = colliderB->GetVertices()[i] - *tempColliderB.GetInter();
		Vector3 px_py = colliderB->GetVertices()[(i + 1) % vertexSize] - colliderB->GetVertices()[i];
		Vector3 m = Cross(pt_px, px_py);
		// 辺の外側であれば当たっていないので判定を打ち切る
		if (Dot(m, colliderB->GetNormal()) < -epsilon) { return false; }
	}

	// 内側なので当たっている
	return true;
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

bool CollisionManager::CheckCollisionRayBox(RayCollider* colliderA, BoxCollider* colliderB)
{
	if (!CheckCollisionFiltering(colliderA, colliderB)) { return false; }
	PolygonCollider pCollider;
	Vector3 boxPos = colliderB->GetWorldPosition();
	Vector3 boxRad = colliderB->GetRadius3D();
	// 上底左奥
	pCollider.AddVertices({ boxPos.x - boxRad.x,boxPos.y + boxRad.y,boxPos.z + boxRad.z });
	// 上底右奥
	pCollider.AddVertices({ boxPos.x + boxRad.x,boxPos.y + boxRad.y,boxPos.z + boxRad.z });
	// 上底右前
	pCollider.AddVertices({ boxPos.x + boxRad.x,boxPos.y + boxRad.y,boxPos.z - boxRad.z });
	// 上底左前
	pCollider.AddVertices({ boxPos.x - boxRad.x,boxPos.y + boxRad.y,boxPos.z - boxRad.z });

	pCollider.SetBaseNormal({ Vector3::MakeAxis(Axis::Y) });

	return CheckCollisionRayPolygon(colliderA, &pCollider);
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

void CollisionManager::CheckSpherePlaneCollisions()
{
	for (SphereCollider* sphereCollider : sphereColliders) {
		for (PlaneCollider* planeCollider : planeColliders)
		{
			if (!CheckCollisionSpherePlane(sphereCollider, planeCollider)) { continue; }

			sphereCollider->OnCollision(planeCollider);
			planeCollider->OnCollision(sphereCollider);
		}
	}
}

void CollisionManager::CheckSpherePolygonCollisions()
{
	for (SphereCollider* sphereCollider : sphereColliders) {
		for (PolygonCollider* polygonCollider : polygonColliders)
		{
			if (!CheckCollisionSpherePolygon(sphereCollider, polygonCollider)) { continue; }

			sphereCollider->OnCollision(polygonCollider);
			polygonCollider->OnCollision(sphereCollider);
		}
	}
}

void CollisionManager::CheckRayPlaneCollisions()
{
	if (rayColliders.empty() || planeColliders.empty()) { return; }
	for (RayCollider* rayCollider : rayColliders) {
		for (PlaneCollider* planeCollider : planeColliders)
		{
			if (!CheckCollisionRayPlane(rayCollider, planeCollider)) { continue; }

			rayCollider->OnCollision(planeCollider);
			planeCollider->OnCollision(rayCollider);
		}
	}
}

void CollisionManager::CheckRayPolygonCollisions()
{
	if (rayColliders.empty() || polygonColliders.empty()) { return; }
	for (RayCollider* rayCollider : rayColliders) {
		for (PolygonCollider* polygonCollider : polygonColliders)
		{
			if (!CheckCollisionRayPolygon(rayCollider, polygonCollider)) { continue; }

			rayCollider->OnCollision(polygonCollider);
			polygonCollider->OnCollision(rayCollider);
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

void CollisionManager::CheckRayBoxCollisions()
{
	for (RayCollider* rayCollider : rayColliders) {
		for (BoxCollider* boxCollider : boxColliders)
		{
			if (!CheckCollisionRayBox(rayCollider, boxCollider)) { continue; }

			rayCollider->OnCollision(boxCollider);
			boxCollider->OnCollision(rayCollider);
		}
	}
}

void CollisionManager::CheckRayCastCollision(RayCollider* collider)
{
	if (rayColliders.empty()) { return; }

	struct RayCastHit
	{
		float distance = 0;
		PlaneCollider* planeCollider = nullptr;
		PolygonCollider* polygonCollider = nullptr;
		SphereCollider* sphereCollider = nullptr;

		void OnCollision(RayCollider* collider)
		{
			if (this->planeCollider)
			{
				collider->OnCollision(this->planeCollider);
				this->planeCollider->OnCollision(collider);
			}
			else if (this->sphereCollider)
			{
				collider->OnCollision(this->sphereCollider);
				this->sphereCollider->OnCollision(collider);
			}
			else if (this->polygonCollider)
			{
				collider->OnCollision(this->polygonCollider);
				this->sphereCollider->OnCollision(collider);
			}
		}
	};

	float distance = 0;
	vector<RayCastHit> collisionInfo;

	for (PlaneCollider* planeCollider : planeColliders)
	{
		if (!CheckCollisionRayPlane(collider, planeCollider, &distance)) { continue; }
		RayCastHit newInfo;
		newInfo.distance = distance;
		newInfo.planeCollider = planeCollider;
		collisionInfo.push_back(newInfo);
	}
	for (PolygonCollider* polygonCollider : polygonColliders)
	{
		if (!CheckCollisionRayPolygon(collider, polygonCollider, &distance)) { continue; }
		RayCastHit newInfo;
		newInfo.distance = distance;
		newInfo.polygonCollider = polygonCollider;
		collisionInfo.push_back(newInfo);
	}
	for (SphereCollider* sphereCollider : sphereColliders)
	{
		if (!CheckCollisionRaySphere(collider, sphereCollider, &distance)) { continue; }
		RayCastHit newInfo;
		newInfo.distance = distance;
		newInfo.sphereCollider = sphereCollider;
		collisionInfo.push_back(newInfo);
	}

	// 何とも当っていない
	if (collisionInfo.empty()) { return; }
	if (collisionInfo.size() == 1)
	{
		collisionInfo[0].OnCollision(collider);
		return;
	}

	RayCastHit nearly = collisionInfo[0];
	for (size_t i = 1; i < collisionInfo.size(); i++)
	{
		if (nearly.distance > collisionInfo[i].distance) { nearly = collisionInfo[i]; }
	}
	nearly.OnCollision(collider);
}

void CollisionManager::CheckAllCollisions()
{
	CheckBoxCollisions();
	CheckSphereCollisions();
	CheckSpherePlaneCollisions();
	CheckSpherePolygonCollisions();
	CheckRayPlaneCollisions();
	CheckRayPolygonCollisions();
	CheckRaySphereCollisions();
	CheckRayBoxCollisions();
}