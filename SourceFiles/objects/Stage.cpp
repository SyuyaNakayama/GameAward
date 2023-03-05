#include "Stage.h"
#include <cassert>
#include <fstream>

void LoadVector3Stream(std::istringstream& stream, Vector3& vec);

void Stage::Initialize()
{
	modelFloor_ = Model::Create("cube");
	sprite_ = Sprite::Create("stages/floor.png");
	sprite_->SetSize(sprite_->GetSize() / 5.0f);
	modelFloor_->SetSprite(sprite_.get());
	floorWTrans_.Initialize();
	floorWTrans_.translation = { 0.0f,-2.0f,0.0f };
	floorWTrans_.scale = { STAGE_WIDTH,1,STAGE_HEIGHT };

	LoadMap(0);
}

void Stage::Update()
{
	modelFloor_->TextureUpdate();
	floorWTrans_.Update();
	for (auto& gimmick : gimmicks_) { gimmick->Update(); }
}

void Stage::Draw()
{
	modelFloor_->Draw(floorWTrans_);
	for (auto& gimmick : gimmicks_) { gimmick->Draw(); }
}

void Stage::LoadMap(UINT16 stageNum)
{
	// バッファをクリア
	stageCommands_.str("");
	// 状態をクリア
	stageCommands_.clear(std::stringstream::goodbit);
	// マップ読み込み
	LoadStageFile(stageNum);
	LoadStageCommands();
}

void Stage::LoadStageFile(UINT16 stageNum)
{
	// ファイル
	std::ifstream file;
	// パスを取得
	std::string stage[7] = { "", "1", "2", "3", "4", "5", "6" };
	const std::string stagefile = "stages/";
	const std::string filename = "stage" + stage[stageNum] + ".txt";
	const std::string directoryPath = "Resources/" + stagefile + "/" + filename;
	// ファイルを開く
	file.open(directoryPath);
	assert(file.is_open());
	// ファイルの内容を文字列ストリームにコピー
	stageCommands_ << file.rdbuf();

	// ファイルを閉じる
	file.close();
}

void Stage::LoadStageCommands()
{
	// 1行分の文字列を入れる変数
	std::string line;
	// 座標
	Vector3 pos;

	// コマンド実行ループ
	while (getline(stageCommands_, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);
		// 文字列
		std::string word;
		// ,区切りで行の先頭文字列を取得
		getline(line_stream, word, ' ');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を読み飛ばす
			continue;
		}

		// コマンド読み込み
		if (word.find("door") == 0) {
			// 座標取得
			LoadVector3Stream(line_stream, pos);
			// 生成
			PopGimmick(GimmickNum::DOOR, pos);
		}
		else if (word.find("candle") == 0) {
			// 座標取得
			LoadVector3Stream(line_stream, pos);
			// 生成
			PopGimmick(GimmickNum::CANDLE, pos);
		}
	}
}

void Stage::PopGimmick(GimmickNum gimmickNum, Vector3 pos)
{
	// 宣言、生成
	std::unique_ptr<Gimmick> gimmick;
	switch (gimmickNum)
	{
	case GimmickNum::DOOR:		gimmick = std::make_unique<Door>();		break;
	case GimmickNum::KEY:		break;
	case GimmickNum::CANDLE:	gimmick = std::make_unique<Candle>();	break;
	}

	//初期設定
	gimmick->Initialize();
	gimmick->SetPosition(pos);
	// コンテナにプッシュ
	gimmicks_.push_back(std::move(gimmick));
}
