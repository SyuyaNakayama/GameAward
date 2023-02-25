#pragma once
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
	void LoadMap(UINT16 stage);
	void LoadStageData(UINT16 stage);
	void LoadStageCommands();

	std::unique_ptr<Model> model_;
	std::vector<WorldTransform> worldTransform_;
	// ファイルコマンド
	std::stringstream stageCommands;
};

