#pragma once
#include <basetsd.h>
#include <Model.h>
#include <sstream>


class Stage
{
public:
	static const int STAGE_WIDTH = 20;
	static const int STAGE_HEIGHT = 20;
	static const int STAGE_DEPTH = 10;

public:
	void Initialize();
	void Update();
	void Draw();
	// ステージマップ読み込み
	void LoadMap(UINT16 stage);

private:
	// ステージファイル読み込み
	void LoadStageFile(UINT16 stage);
	// コマンド読み込み
	void LoadStageCommands();

private:
	std::unique_ptr<Model> model_;
	std::vector<WorldTransform> worldTransform_;
	// ファイルコマンド
	std::stringstream stageCommands;
};

