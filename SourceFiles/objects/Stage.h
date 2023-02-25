#pragma once
#include <Model.h>
#include <sstream>

class Stage
{
public:
	static const int STAGE_WIDTH = 25;
	static const int STAGE_HEIGHT = 25;
	static const int STAGE_DEPTH = 10;

public:
	void Initialize();
	void Update();
	void Draw();
	// ステージマップ読み込み
	void LoadMap(UINT16 stage);

	~Stage() { sprite_.release(); }

private:
	// ステージファイル読み込み
	void LoadStageFile(UINT16 stage);
	// コマンド読み込み
	void LoadStageCommands();

	std::unique_ptr<Model> model_;
	std::unique_ptr<Sprite> sprite_;
	//std::unique_ptr<WorldTransform> worldTransform_;

	std::vector<WorldTransform> worldTransform_;
	// ファイルコマンド
	std::stringstream stageCommands;
};

