#pragma once
#include <sstream>
#include "Gimmick.h"

class Stage
{
public:
	static const int STAGE_WIDTH = 30;
	static const int STAGE_HEIGHT = 16;

	enum class GimmickNum {
		NONE,
		DOOR,
		CANDLE,
	};

	void Initialize();
	void Update();
	void Draw();
	// ステージマップ読み込み
	void LoadMap(UINT16 stageNum);

	~Stage() { modelFloor_.release(); }

private:
	// ステージファイル読み込み
	void LoadStageFile(UINT16 stageNum);
	// コマンド読み込み
	void LoadStageCommands();
	// ギミック生成
	void PopGimmick(GimmickNum gimmickNum, Vector3 pos);

	std::unique_ptr<Model> modelFloor_;
	WorldTransform floorWTrans_;

	// ギミック
	std::vector<std::unique_ptr<Gimmick>> gimmicks_;
	// ファイルコマンド
	std::stringstream stageCommands_;
};