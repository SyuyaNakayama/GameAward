#pragma once
#include <sstream>
#include "Gimmick.h"

class Stage
{
public:
	static const int STAGE_WIDTH = 60;
	static const int STAGE_HEIGHT = 32;

	enum class GimmickNum {
		NONE,
		DOOR,
		CANDLE,
		WALL,
		GIMMICK_NUM
	};

	void Initialize(UINT16 stageNum);
	void Update();
	void Draw();
	// ステージマップ読み込み
	void LoadMap(UINT16 stageNum);

	Vector3 GetDoorPos() { return doorPos; }
	~Stage() { modelFloor_.release(); }

private:
	// ステージファイル読み込み
	void LoadStageFile(UINT16 stageNum);
	// コマンド読み込み
	void LoadStageCommands();
	// ギミック生成
	void PopGimmick(GimmickNum gimmickNum, Vector3 pos, Vector3 scale = { 1.0f, 1.0f, 1.0f });
	// ギミックの当たり判定
	bool OnCollisionGimmicks();

	std::unique_ptr<Model> modelFloor_;
	WorldTransform floorWTrans_;
	size_t lightIndex = 1;
	size_t doorIndex = 1;

	// ギミック
	std::vector<std::unique_ptr<Gimmick>> gimmicks_;
	// ファイルコマンド
	std::stringstream stageCommands_;

	Vector3 doorPos;
};