#include <imgui.h>
#include <random>
#include "Gimmick.h"
#include "ImGuiManager.h"
#include "Input.h"
#include "Stage.h"
#include "SceneManager.h"
#include "UIDrawer.h"

#pragma region 静的メンバ変数
bool Gimmick::isStart_;
LightGroup* Gimmick::lightGroup = nullptr;
size_t Candle::lightNum = 0;
Player* Block::player = nullptr;
// 鍵の静的メンバ変数の初期化
size_t KeyLock::keyNum = 0;
size_t KeyLock::collectKeyNum = 0;
bool KeyLock::isCollectAll = false;
UINT RoomDoor::roomNum = 1;
std::array<UINT, 3> RoomDoor::allNextRoomNums;
#pragma endregion

void Gimmick::Initialize(const GimmickParam& param)
{
	worldTransform.translation = param.pos;
	worldTransform.scale = param.scale;
	worldTransform.rotation = param.rot * (PI / 180);
}

#pragma region Door
void BaseDoor::Initialize(const GimmickParam& param)
{
	collisionAttribute = CollisionAttribute::Block;
	collisionMask = CollisionMask::Block;
	// パラメータセット
	Gimmick::Initialize(param);
	// モデル読み込み
	model = Model::Create("door", true);
	model_back = Model::Create("door_back");
	model->SetAnbient({ 1,1,1 });
	model->Update();
	// 各モデルのworldTransform初期化とモデルの位置調整
	worldTransform.Initialize();
	worldTransform.translation += { 0, 1.35f };
	worldTransform.scale = { 1.8f,1.9f,2.0f };
	for (auto& w : door)
	{
		// 親子関係
		w.parent = &worldTransform;
		w.Initialize();
		// 大きさを調整
		w.scale = { 1.1f,1.05f,1.0f };
		// 開ける
		w.rotation.y = -PI / 2.0f;
	}
	// 座標を調整
	door[(int)WTType::L].translation += {-1.4f, -1.3f, 0.0f};
	door[(int)WTType::R].translation += { 1.4f, -1.3f, 0.0f};
}

/// <summary>
/// ドアの更新処理
/// </summary>
void BaseDoor::Update()
{
	for (auto& w : door) { w.Update(); }
	worldTransform.Update();
}

void BaseDoor::Draw()
{
	for (auto& w : door) { model->Draw(w); }
	model_back->Draw(worldTransform);
}

void GoalDoor::Update()
{
	assert(Move);
	(this->*Move)(); // 扉を動かす
	BaseDoor::Update();
}

/// <summary>
/// ドアを開く
/// </summary>
void GoalDoor::Open()
{
	if (++rot >= 90) { Move = &GoalDoor::Opened; }

	door[(int)WTType::L].rotation.y = (rot + 180) * PI / 180;
	door[(int)WTType::R].rotation.y = -rot * PI / 180;
}

/// <summary>
/// ドアを閉じる
/// </summary>
void GoalDoor::Close()
{
	if (--rot <= 0)
	{
		isStart_ = true;
		Move = &GoalDoor::Closed;
	}

	door[(int)WTType::L].rotation.y = (rot + 180) * PI / 180;
	door[(int)WTType::R].rotation.y = -rot * PI / 180;
}

void GoalDoor::Opened()
{
	if (input->IsTrigger(Key::P)) { Move = &GoalDoor::Close; }	// 扉を閉める
}

void GoalDoor::Closed()
{
	if (input->IsTrigger(Key::O)) { Move = &GoalDoor::Open; }
	// ゴール判定
	// Playerのライトインデックスは飛ばす
	for (size_t i = 1; i <= Candle::GetLightNum(); i++)
	{
		if (lightGroup->GetPointLightActive(i))
		{
			// ライトがついている時
			Move = &GoalDoor::Open;
			continue;
		}
		// ライトがついていない時は関数を終了する
		Move = &GoalDoor::Closed;
		return;
	}
}

/// <summary>
/// ドアに当たった時
/// </summary>
void GoalDoor::OnCollision(BoxCollider* boxCollider)
{
	if (Move != &GoalDoor::Opened) { return; } // ドアが空いている時ゴール
	SceneManager::GetInstance()->SetNextScene(Scene::Clear);
}

void SelectDoor::Closed()
{
	// ライトがついている時
	if (lightGroup->GetPointLightActive(doorIndex)) { Move = &GoalDoor::Open; }
}

void SelectDoor::OnCollision(BoxCollider* boxCollider)
{
	if (Move != &GoalDoor::Opened) { return; } // ドアが空いている時
	Stage::SetStageNum(doorIndex);
	SceneManager::GetInstance()->SetNextScene(Scene::Play);
}

void RoomDoor::Initialize(const GimmickParam& param)
{
	BaseDoor::Initialize(param);
	// 乱数
	std::random_device rnd;
	std::mt19937 rnddev(rnd());
	std::uniform_int_distribution<UINT> rand(1, 5);

	// 乱数の重複を無くす
	bool isExistNextRoomNum = false;
	do
	{
		nextRoomNum = rand(rnddev);
		for (size_t i = 0; i < doorIndex - 1; i++)
		{
			// 乱数の値がリストに登録されていた場合
			if (allNextRoomNums[i] == nextRoomNum)
			{
				isExistNextRoomNum = true;
				break;
			}
			else { isExistNextRoomNum = false; }
		}
	} while (isExistNextRoomNum);
	
	// 3つ目のドアの時
	if (doorIndex >= allNextRoomNums.size())
	{
		// 次の部屋を示すドアがあるかチェック
		bool isExistRightNextRoomNum = false;
		for (UINT nextRoomNumTemp : allNextRoomNums)
		{
			if (nextRoomNumTemp == roomNum + 1)
			{
				isExistRightNextRoomNum = true;
				break;
			}
		}
		// なかった場合は次の部屋番号をセット
		if (!isExistRightNextRoomNum) { nextRoomNum = roomNum + 1; }
	}
	// 乱数を配列に登録
	allNextRoomNums[doorIndex - 1] = nextRoomNum;
}

void RoomDoor::Update()
{
	// 最終部屋の場合は更新しない
	if (roomNum == FINAL_ROOM_NUM) { return; }

	std::array<std::string, 3> strings = { "Left","Center","Right" };

	BaseDoor::Update();
	ImGui::Text("nextRoomNum(%s) : %d", strings[doorIndex - 1].c_str(), nextRoomNum);
}

void RoomDoor::OnCollision(BoxCollider* boxCollider)
{
	// 最終部屋の場合は当たり判定を取らない
	if (roomNum == FINAL_ROOM_NUM) { return; }

	SceneManager::GetInstance()->SetNextScene(Scene::Play);
	// 正解のドアだった場合
	if (nextRoomNum == roomNum + 1)
	{
		// roomNumをインクリメント
		roomNum++;
	}
	// 不正解のドアだった場合
	else
	{
		// スタートの部屋に戻す
		roomNum = 1;
	}
}
#pragma endregion

#pragma region KeyLock
void KeyLock::Initialize(const GimmickParam& param)
{
	// 当たり判定設定
	collisionAttribute = CollisionAttribute::Block;
	collisionMask = CollisionMask::Block;
	// モデル読み込み
	model = Model::Create("cube", true);
	// 初期化
	worldTransform.Initialize();
	// パラメータセット
	Gimmick::Initialize(param);
	// 更新
	worldTransform.Update();
	// 鍵(欠片?)の数を増やす
	keyNum++;
}

void KeyLock::Update()
{
	// 鍵を全て集めたらフラグをオンにする
	if (!isCollectAll && collectKeyNum == keyNum) {
		isCollectAll = true;
	}
}

void KeyLock::Draw()
{
	// まだ取得されてないなら描画する
	if (!isCollected) { model->Draw(worldTransform); }
}

void KeyLock::OnCollision(BoxCollider* boxCollider)
{
	// 取得した欠片の数を増やす
	collectKeyNum++;
	// 収集済みフラグをオンにする
	isCollected = true;
	// 当たり判定をなくす
	collisionMask = CollisionMask::None;
}
#pragma endregion

#pragma region Candle
void Candle::Initialize(const GimmickParam& param)
{
	// パラメータセット
	Gimmick::Initialize(param);
	model = Model::Create("candle", true);
	worldTransform.Initialize();
	worldTransform.translation.y -= 1.0f;
	worldTransform.scale = { 2.0f,2.0f,2.0f };
	lightGroup = Model::GetLightGroup();
	lightPos = worldTransform.translation + Vector3(0, worldTransform.scale.y + 1.2f);
	particleProp =
	{
		lightPos,{0,0.01f,0},{0,0.0005f,0},
		0.025f,0.001f,0,40,0.4f
	};
	lightGroup->SetPointLightAtten(lightIndex, { 0.2f, 0.01f });
	lightGroup->SetPointLightColor(lightIndex, { 1,0.5f,0.5f });
	ui = UIDrawer::GetUI((size_t)2 + Input::GetInstance()->IsConnectGamePad());
}

void Candle::Update()
{
	// ステージ2の場合
	if (Stage::GetStageNum() == (UINT)Stage::StageNum::Stage2)
	{
		// 現在の部屋番号以下のインデックスの場合出現する
		isExist = lightIndex <= RoomDoor::GetRoomNumber();
	}
	if (!isExist) { return; }
	worldTransform.Update();
	(this->*Fire)();
	model->Update();
	ui->SetIsInvisible(true);
}

void Candle::Dark()
{
	lightGroup->SetPointLightActive(lightIndex, false);
	model->SetAnbient({ 0.1f,0.1f,0.1f });
}

void Candle::PreLight()
{
	// 状態変更
	if (--particleTimer <= 0)
	{
		Fire = &Candle::PostLight;
		lightGroup->SetPointLightActive(lightIndex, true); // 点灯
		model->SetAnbient({ 0.7f,0.3f,0.3f }); // マテリアル調整
		// パーティクル調整
		lightPos = worldTransform.translation + Vector3(0, worldTransform.scale.y + 1.2f);
		particleProp.posOffset = lightPos;
		lightGroup->SetPointLightPos(lightIndex, lightPos);
	}
	// 乱数生成
	std::random_device rnd;
	std::mt19937 rnddev(rnd());
	std::uniform_real_distribution<float> randRadius(0, 2.0f);
	std::uniform_real_distribution<float> randAngle(-PI / 2.0f, PI / 2.0f);
	// パーティクル設定
	DirectionalParticle::AddProp particleProp =
	{
		playerPos,worldTransform.translation + Vector3(0, worldTransform.scale.y + 1.2f),
		0.5f,2,randAngle(rnddev),randRadius(rnddev),60
	};
	// パーティクル追加
	ParticleManager::Add(particleProp);
}

void Candle::PostLight()
{
	// パーティクル追加
	ParticleManager::Add(particleProp);
}

void Candle::OnCollision(RayCollider* rayCollider)
{
	if (Length(rayCollider->GetWorldPosition() - worldTransform.GetWorldPosition()) >= 8.0f) { return; }
	if (Stage::GetStageNum() == (UINT16)Stage::StageNum::Tutorial)
	{
		ui->SetIsInvisible(Fire != &Candle::Dark);
		ui->SetPosition(To2DVector(worldTransform.GetWorldPosition() + Vector3(0, 1, 0)));
	}
	if (!Input::GetInstance()->IsTrigger(Mouse::Left)) { return; }
	Fire = &Candle::PreLight;
	particleTimer = 60;
	playerPos = rayCollider->GetWorldPosition();
}
#pragma endregion

#pragma region Block
void Block::Initialize(const GimmickParam& param)
{
	// 当たり判定設定
	collisionAttribute = CollisionAttribute::Block;
	collisionMask = CollisionMask::Block;

	// テクスチャ読み込み
	std::unique_ptr<Sprite> sprite;
	switch (param.textureIndex)
	{
	case 0:	sprite = Sprite::Create("white1x1.png");		break;
	case 1:	sprite = Sprite::Create("stages/floor.png");	break;
	}
	sprite->SetSize(sprite->GetSize() / max(max(param.scale.x, param.scale.y), param.scale.z) * 10.0f);
	// モデル読み込み
	model = Model::Create("cube");
	model->SetSprite(std::move(sprite));
	model->Update();

	// 初期化
	worldTransform.Initialize();
	// パラメータセット
	Gimmick::Initialize(param);
	if (param.vanishFlag == 1) { blockState |= (int)BlockStatus::VANISH_RED; }			// 赤炎の時消えるフラグ
	else if (param.vanishFlag == 2) { blockState |= (int)BlockStatus::VANISH_BLUE; }	// 青炎の時消えるフラグ
	if (param.moveFlag == 1) { blockState |= (int)BlockStatus::MOVE; }						// 動くかどうか
	for (auto& pathPoint : param.pathPoints) { pathPoints.push_back(pathPoint); }			// 経路点取得
}

void Block::Update()
{
	// 当たり判定設定
	if ((blockState & (int)BlockStatus::VANISH_RED) && !player->IsBlueFire()) { collisionMask = CollisionMask::None; }
	//else if (blockState & (int)WallStatus::VANISH_RED && player->IsRedFire()) { collisionMask = CollisionMask::None; }
	else { collisionMask = CollisionMask::Block; }
	// 移動
	if (blockState & (int)BlockStatus::MOVE) { Move(); }
	// 更新
	worldTransform.Update();
}

void Block::Draw()
{
	// 当たり判定がないなら描画しない
	if (collisionMask == CollisionMask::None) { return; }
	// あるなら描画
	else { Gimmick::Draw(); }
}

void Block::Move()
{
	ImGui::Text("bPos : %f %f %f", worldTransform.translation.x, worldTransform.translation.y, worldTransform.translation.z);
	ImGui::Text("bIndex : %d", pathIndex);
	ImGui::Text("interval : %d", interval);
	// インターバル中なら戻る
	if (interval > 0) { interval--; return; }

	// 経路点に到着したならintervalと次の経路点をセットする
	if (worldTransform.translation == pathPoints[pathIndex]) {
		// 要素数を越したら先頭に戻す
		if (pathIndex == (pathPoints.size() - 1)) { pathIndex = 0; }
		else { pathIndex++; }
		interval = 200;
		return;
	}

	// プレイヤー座標
	Vector3 pPos = worldTransform.translation;

	// 経路点に到着してないなら移動する処理
	if (pPos.x != pathPoints[pathIndex].x) {
		// 経路点より小さいなら正のspeedを返し、大きいならなら負のspeedを足す
		pPos.x += pathPoints[pathIndex].x > pPos.x ? speed : -speed;
		// 経路点の近似値でfloatの誤差を補正する
		if (pathPoints[pathIndex].x - speed < pPos.x && pathPoints[pathIndex].x + speed > pPos.x) {
			pPos.x = pathPoints[pathIndex].x;
		}
	}
	// 経路点に到着してないなら移動する処理
	if (pPos.y != pathPoints[pathIndex].y) {
		// 経路点より小さいなら正のspeedを返し、大きいならなら負のspeedを返す
		pPos.y += pathPoints[pathIndex].y > pPos.y ? speed : -speed;
		// 経路点の近似値でfloatの誤差を補正する
		if (pathPoints[pathIndex].y - speed < pPos.y && pathPoints[pathIndex].y + speed > pPos.y) {
			pPos.y = pathPoints[pathIndex].y;
		}
	}
	// 経路点に到着してないなら移動する処理
	if (pPos.z != pathPoints[pathIndex].z) {
		// 経路点より小さいなら正のspeedを返し、大きいならなら負のspeedを返す
		pPos.z += pathPoints[pathIndex].z > pPos.z ? speed : -speed;
		// 経路点の近似値でfloatの誤差を補正する
		if (pathPoints[pathIndex].z - speed < pPos.z && pathPoints[pathIndex].z + speed > pPos.z) {
			pPos.z = pathPoints[pathIndex].z;
		}
	}
	// 代入
	worldTransform.translation = pPos;
}
#pragma endregion