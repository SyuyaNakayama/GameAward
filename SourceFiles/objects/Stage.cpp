#include "Stage.h"
#include <cassert>
#include <fstream>

void Stage::Initialize()
{
	model_ = Model::Create("cube");
	sprite_ = Sprite::Create("stages/floor.png");
	sprite_->SetSize(sprite_->GetSize() / 5.0f);
	model_->SetSprite(sprite_.get());
	worldTransform_.Initialize();
	worldTransform_.translation = { 0.0f,-2.0f,0.0f };
	worldTransform_.scale = { STAGE_WIDTH,1,STAGE_HEIGHT };

	LoadMap(0);
}

void Stage::Update()
{
	model_->TextureUpdate();
	worldTransform_.Update();
	/*for (WorldTransform& worldTransform : worldTransform_)
	{
		worldTransform.Update();
	}*/
}

void Stage::Draw()
{
	model_->Draw(worldTransform_);
	/*for (WorldTransform& worldTransform : worldTransform_)
	{
		model_->Draw(worldTransform);
	}*/
}


void Stage::LoadMap(UINT16 stage)
{
	//// バッファをクリア
	//stageCommands.str("");
	//// 状態をクリア
	//stageCommands.clear(std::stringstream::goodbit);

	//LoadStageFile(stage);

	//LoadStageCommands();
}

void Stage::LoadStageFile(UINT16 stage)
{
	//// ファイル
	//std::ifstream file;
	//// パスを取得
	//std::string stageNum[7] = { "", "1", "2", "3", "4", "5", "6" };
	//const std::string stagefile = "stages/";
	//const std::string filename = "stage" + stageNum[stage] + ".txt";
	//const std::string directoryPath = "Resources/" + stagefile + "/" + filename;
	//// ファイルを開く
	//file.open(directoryPath);
	//assert(file.is_open());
	//// ファイルの内容を文字列ストリームにコピー
	//stageCommands << file.rdbuf();

	//// ファイルを閉じる
	//file.close();
}

void Stage::LoadStageCommands()
{
	//// 1行分の文字列を入れる変数
	//std::string line;
	//// マップチップ用
	//int x = 0;
	//int y = 0;

	//Vector3 trans{};
	//trans.y = -5.0f;

	//// コマンド実行ループ
	//while (getline(stageCommands, line)) {
	//	// 1行分の文字列をストリームに変換して解析しやすくする
	//	std::istringstream line_stream(line);
	//	// 文字列
	//	std::string word;
	//	// ,区切りで行の先頭文字列を取得
	//	getline(line_stream, word, ',');

	//	// "//"から始まる行はコメント
	//	if (word.find("//") == 0) {
	//		// コメント行を読み飛ばす
	//		continue;
	//	}

	//	// コマンド読み込み
	//	if (word.find("map") == 0) {
	//		while (x != 20)
	//		{
	//			if (word.find("0") == 0) {
	//			}
	//			else if (word.find("1") == 0) {
	//				WorldTransform worldTransform;

	//				trans.x = -9.5f + (1.0f * x);
	//				trans.z = 9.5f - (1.0f * y);

	//				worldTransform.Initialize();
	//				worldTransform.translation = trans;

	//				worldTransform_.push_back(worldTransform);
	//			}
	//			// 次の内容へ
	//			getline(line_stream, word, ',');
	//			x++;
	//		}
	//	}
	//	else if (word.find("next") == 0) {
	//		trans.y += 1.5f;
	//		y = 0;
	//		x = 0;
	//		continue;
	//	}
	//	y++;
	//	x = 0;
	//}
}
