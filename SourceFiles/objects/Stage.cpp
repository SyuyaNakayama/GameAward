#include "Stage.h"
#include <cassert>
#include <fstream>

void Stage::Initialize()
{
	model_ = Model::Create("cube");
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			worldTransform_[y][x].Initialize();
			worldTransform_[y][x].translation = { 0.0f, -5.0f, 0.0f };
		}
	}

	LoadMap(0);
}

void Stage::Update()
{

	model_->TextureUpdate();
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			worldTransform_[y][x].Update();
		}
	}
}

void Stage::Draw()
{
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			model_->Draw(worldTransform_[y][x]);
		}
	}
}


void Stage::LoadMap(UINT16 stage)
{
	// バッファをクリア
	stageCommands.str("");
	// 状態をクリア
	stageCommands.clear(std::stringstream::goodbit);

	LoadStageData(stage);

	LoadStageCommands();
}

void Stage::LoadStageData(UINT16 stage)
{
	// ファイル
	std::ifstream file;
	// パスを取得
	std::string stageNum[7] = { "", "1", "2", "3", "4", "5", "6" };
	const std::string stagefile = "stages/";
	const std::string filename = "stage" + stageNum[stage] + ".txt";
	const std::string directoryPath = "Resources/" + stagefile + "/" + filename;
	// ファイルを開く
	file.open(directoryPath);
	assert(file.is_open());
	// ファイルの内容を文字列ストリームにコピー
	stageCommands << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void Stage::LoadStageCommands()
{
	// 1行分の文字列を入れる変数
	std::string line;
	// マップチップ用
	int x = 0;
	int y = 0;

	// コマンド実行ループ
	while (getline(stageCommands, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);
		// 文字列
		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ',');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を読み飛ばす
			continue;
		}

		while (x != STAGE_WIDTH)
		{
			// コマンド読み込み
			if (word.find("0") == 0) {
			}
			else if (word.find("1") == 0) {
				Vector3 trans{};
				trans.x = -9.5f + (1.0f * x);
				trans.y = -5.0f;
				trans.z = 9.5f - (1.0f * y);

				worldTransform_[y][x].translation = trans;
			}
			// 次の内容へ
			getline(line_stream, word, ',');
			x++;
		}
		y++;
		x = 0;
	}
}
