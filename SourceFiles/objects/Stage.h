#pragma once
#include <sstream>
#include "Gimmick.h"

class Stage
{
public:
	static const int STAGE_WIDTH = 60;
	static const int STAGE_HEIGHT = 32;

	enum class GimmickNum {
		None,
		Door,
		Candle,
		Wall,
		GimmickKinds
	};

private:
	std::unique_ptr<Model> modelFloor_;
	WorldTransform floorWTrans_;
	size_t lightIndex = 1;
	size_t doorIndex = 1;

	// ギミック
	std::vector<std::unique_ptr<Gimmick>> gimmicks_;
	// ファイルコマンド
	std::stringstream stageCommands_;
	Vector3 doorPos;

	// ステージマップ読み込み
	void LoadMap(UINT16 stageNum);
	// ステージファイル読み込み
	void LoadStageFile(UINT16 stageNum);
	// コマンド読み込み
	void LoadStageCommands();
	// ギミック生成
	void PopGimmick(GimmickNum gimmickNum, Vector3 pos, Vector3 scale = { 1.0f, 1.0f, 1.0f });

	/*
	現在ステージ
	0 ステージ選択(タイトルシーン)
	1 チュートリアル
	2~6 ステージ1~5
	*/
	static UINT16 stageNum;

public:
	void Initialize();
	void Update();
	void Draw();

	Vector3 GetDoorPos() { return doorPos; }
	~Stage() { modelFloor_.release(); }
	static UINT16 GetStageNum() { return stageNum; }
	static void SetStageNum(UINT16 stageNum_) { stageNum = stageNum_; }
};