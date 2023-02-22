#pragma once
#include <basetsd.h>
#include <Model.h>
#include <sstream>


class Stage
{
public:
	static const int STAGE_WIDTH = 20;
	static const int STAGE_HEIGHT = 20;

public:
	void Initialize();
	void Update();
	void Draw();

	void LoadMap(UINT16 stage);
	void LoadStageData(UINT16 stage);
	void LoadStageCommands();

private:
	std::unique_ptr<Model> model_;
	WorldTransform worldTransform_[STAGE_HEIGHT][STAGE_WIDTH];
	// ファイルコマンド
	std::stringstream stageCommands;
};

