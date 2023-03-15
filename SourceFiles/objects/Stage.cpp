#include "Stage.h"
#include <cassert>
#include <fstream>

void LoadVector3Stream(std::istringstream& stream, Vector3& vec);

void Stage::Initialize()
{
	floorModel_ = Model::Create("cube");
	std::unique_ptr<Sprite> sprite_ = Sprite::Create("stages/floor.png");
	sprite_->SetSize(sprite_->GetSize() / 5.0f);
	floorModel_->SetSprite(std::move(sprite_));
	floorWTrans_.Initialize();
	floorWTrans_.translation = { 0.0f,-2.0f,0.0f };

	LoadMap(1);
}

void Stage::Update()
{
	Sprite* sprite = floorModel_->GetSprite();
	sprite->SetColor({ 1,1,1,1 });
	floorModel_->Update();
	floorWTrans_.Update();
	for (auto& gimmick : gimmicks_) { gimmick->Update(); }
}

void Stage::Draw()
{
	floorModel_->Draw(floorWTrans_);
	for (auto& gimmick : gimmicks_) { gimmick->Draw(); }
}

void Stage::LoadMap(UINT16 stageNum)
{
	// バッファをクリア
	stageCommands_.str("");
	// 状態をクリア
	stageCommands_.clear(std::stringstream::goodbit);
	// ギミックコンテナの中身を空にする
	gimmicks_.clear();
	// ライト関連変数のリセット
	lightIndex = 1;
	Candle::ResetLightNum();
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

	// コマンド実行ループ
	while (getline(stageCommands_, line)) {
		// 1行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);
		// 文字列
		std::string word;
		// 空白区切りで行の先頭文字列を取得
		getline(line_stream, word, ' ');

		// "//"から始まる行はコメント
		if (word.find("//") == 0) {
			// コメント行を読み飛ばす
			continue;
		}

		// コマンド読み込み
		if (word.find("floor") == 0) {
			// コマンド読み込み
			LoadStreamCommands(line_stream, word);
			// スケールをセット
			floorWTrans_.scale = scale;
			stageSize_ = { scale.x, scale.z };
		}
		else if (word.find("door") == 0) {
			// コマンド読み込み
			LoadStreamCommands(line_stream, word);
			// 生成
			PopGimmick(GimmickNum::DOOR, pos);
			doorPos = pos;
		}
		else if (word.find("candle") == 0) {
			// コマンド読み込み
			LoadStreamCommands(line_stream, word);
			// 生成
			PopGimmick(GimmickNum::CANDLE, pos);
		}
		else if (word.find("wall") == 0) {
			// コマンド読み込み
			LoadStreamCommands(line_stream, word);
			// 生成
			PopGimmick(GimmickNum::WALL, pos, scale, flag);
		}
		else if (word.find("start") == 0) {
			// 座標取得
			LoadVector3Stream(line_stream, startPos);
		}
	}
}

void Stage::LoadStreamCommands(std::istringstream& stream, std::string& word)
{
	// (区切りで先頭文字列を取得
	while (getline(stream, word, '('))
	{
		// 座標取得
		if (word.find("pos") == 0) { LoadVector3Stream(stream, pos); }
		// スケール取得
		else if (word.find("scale") == 0) { LoadVector3Stream(stream, scale); }
		// 回転角取得
		else if (word.find("rot") == 0) { LoadVector3Stream(stream, rot); }
		// フラグ取得
		else if (word.find("flag") == 0) { stream >> flag; }
		// 空白まで飛ばす
		getline(stream, word, ' ');
	}
}

void Stage::PopGimmick(GimmickNum gimmickNum, Vector3 pos, Vector3 scale, Vector3 rot, bool flag)
{
	// 宣言、生成
	std::unique_ptr<Gimmick> gimmick;
	switch (gimmickNum)
	{
	case GimmickNum::DOOR:		gimmick = std::make_unique<Door>();					break;
	case GimmickNum::CANDLE:	gimmick = std::make_unique<Candle>(lightIndex++);	break;
	case GimmickNum::WALL:		gimmick = std::make_unique<Wall>(scale, flag);		break;
	}

	//初期設定
	gimmick->SetPosition(pos);
	gimmick->Initialize();
	// コンテナにプッシュ
	gimmicks_.push_back(std::move(gimmick));
}