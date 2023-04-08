#pragma once
#include "Collider.h"
#include "Model.h"
#include "ParticleManager.h"
#include "Input.h"
#include "Collider.h"
#include "Player.h"

// ギミックのパラメータ
struct GimmickParam {
	Vector3 pos;		// 座標
	Vector3 scale;	// スケール
	Vector3 rot;		// 回転
	std::vector<Vector3> pathPoints;	// 経路点
	UINT16 vanishFlag = 0;		// 消えるかフラグ
	bool moveFlag = false;		// 移動フラグ
	UINT16 textureIndex = 0;	// テクスチャインデックス
};

class Gimmick : public BoxCollider
{
protected:
	std::unique_ptr<Model> model;
	static bool isStart_;	// trueになったらカメラを引く
	static LightGroup* lightGroup;

public:
	virtual ~Gimmick() { model.release(); }
	virtual void Initialize(const GimmickParam& param);
	virtual void Update() = 0;
	virtual void Draw() { model->Draw(worldTransform); }

	// アクセッサ
	Vector3 GetRotation() { return worldTransform.rotation; }

	static bool GetIsStart() { return isStart_; }
	static void SetIsStart(bool isStart) { isStart_ = isStart; }
};

// ドアクラスの基底クラス
class BaseDoor : public Gimmick
{
protected:
	enum class WTType { L, R };

	// モデル
	std::unique_ptr<Model> model_back;
	std::array<WorldTransform, 2> door;
public:
	virtual void Initialize(const GimmickParam& param);
	virtual void Update();
	virtual void Draw();

	//当たり判定の大きさを調整
	Vector3 GetRadius() { return { 1.8f,1.9f,1.0f }; }
};

class GoalDoor : public BaseDoor
{
protected:
	// ドアが閉じている時にnullptrになる
	void (GoalDoor::* Move)() = &GoalDoor::Opened;

	UINT16 doorIndex = 0;
	Input* input = Input::GetInstance();
	float rot = 90;

public:
	void Open();	// ドアが開く時に呼び出される関数
	void Close();	// ドアが閉じる時に呼び出される関数
	void Opened();	// ドアが開いている時に呼び出される関数
	virtual void Closed();	// ドアが閉じている時に呼び出される関数
	void Update();
	void OnCollision(BoxCollider* boxCollider);
};

// ステージセレクト画面のドア
class SelectDoor : public GoalDoor
{
private:
	UINT16 doorIndex = 0;

	void Closed();	// ドアが閉じている時に呼び出される関数

public:
	SelectDoor(UINT16 doorIndex_) { doorIndex = doorIndex_; }
	void OnCollision(BoxCollider* boxCollider);
};

// ステージ2のドア
class RoomDoor : public BaseDoor
{
private:
	UINT16 doorIndex = 0;
	static UINT roomNum; // 現在の部屋番号(部屋にある燭台の数)
	UINT nextRoomNum = 0; // ドアが示す部屋番号
	static std::array<UINT, 3> allNextRoomNums; // 全てのドアが示す部屋番号
	static const UINT FINAL_ROOM_NUM = 5;

public:
	RoomDoor(UINT16 doorIndex_) { doorIndex = doorIndex_; }
	static UINT GetRoomNumber() { return roomNum; }
	void Initialize(const GimmickParam& param);
	void Update();
	void OnCollision(BoxCollider* boxCollider);
};

class KeyLock : public Gimmick
{
private:
	// 全ての鍵の数
	static size_t keyNum;
	// 収集した鍵の数
	static size_t collectKeyNum;
	// 鍵を全て集めたか
	static bool isCollectAll;

	// 収集済みかどうか
	bool isCollected = false;

	// 当たり判定
	void OnCollision(BoxCollider* boxCollider);
public:
	void Initialize(const GimmickParam& param);
	void Update();
	void Draw() override;
	// 鍵が一個もなければtrueで返し、そうじゃなければフラグを返す
	static const bool GetIsCollectAll() { if (keyNum == 0) { return true; } return isCollectAll; }
	// リセット関数
	static void ResetKeyNum() { keyNum = 0; collectKeyNum = 0; }
};

class Candle : public Gimmick, public SphereCollider
{
private:
	void (Candle::* Fire)() = &Candle::Dark;

	size_t lightIndex = 0;
	DiffuseParticle::AddProp particleProp;
	Vector3 lightPos;
	static size_t lightNum;
	int particleTimer = 60; // 方向パーティクル発生時間
	Vector3 playerPos;
	Sprite* ui = nullptr;
	bool isExist = true;

	void Dark(); // 光っていない時の処理
	void PreLight(); // 光る前
	void PostLight(); // 光った後
public:
	// 引数付きコンストラクタ
	Candle(size_t index) { lightIndex = index; lightNum++; }
	void OnCollision(RayCollider* rayCollider);
	void Initialize(const GimmickParam& param);
	void Update();
	static size_t GetLightNum() { return lightNum; }
	static void ResetLightNum() { lightNum = 0; }
};

class Block : public Gimmick
{
public: // 列挙クラス
	// ブロックのステータス
	enum class BlockStatus {
		NORMAL = 0b000,
		MOVE = 0b001,
		VANISH_RED = 0b010,
		VANISH_BLUE = 0b100,
	};
private:
	// プレイヤー
	static Player* player;

	// ブロックの状態
	int blockState = (int)BlockStatus::NORMAL;
	// 移動関連
	bool isMove = false;
	float speed = 0.1f;
	int interval = 0;
	std::vector<Vector3> pathPoints;
	UINT16 pathIndex = 0;

public:
	static void SetPlayerAddress(Player* pPlayer) { player = pPlayer; }
	void Initialize(const GimmickParam& param);
	void Update();
	void Draw() override;
	void Move();
};