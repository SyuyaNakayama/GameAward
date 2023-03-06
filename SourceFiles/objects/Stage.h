#pragma once
#include <sstream>
#include "Gimmick.h"

class Stage
{
public:
	static const int STAGE_WIDTH = 25;
	static const int STAGE_HEIGHT = 25;
	static const int STAGE_DEPTH = 10;
	enum class GimmickNum {
		NONE,
		DOOR,
		KEY,
		CANDLE,
		GIMMICK_NUM
	};

public:
	void Initialize();
	void Update();
	void Draw();
	// ステージマップ読み込み
	void LoadMap(UINT16 stageNum);

	~Stage() { sprite_.release(); }
	Vector3 GetDoorPos() { return doorPos; }
private:
	// ステージファイル読み込み
	void LoadStageFile(UINT16 stageNum);
	// コマンド読み込み
	void LoadStageCommands();
	// ギミック生成
	void PopGimmick(GimmickNum gimmickNum, Vector3 pos);

	std::unique_ptr<Model> modelFloor_;
	std::unique_ptr<Sprite> sprite_;
	WorldTransform floorWTrans_;

	// ギミック
	std::vector<std::unique_ptr<Gimmick>> gimmicks_;
	// ファイルコマンド
	std::stringstream stageCommands_;

	Vector3 doorPos;
};