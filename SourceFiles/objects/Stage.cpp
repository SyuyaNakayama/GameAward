#include "Stage.h"
#include <cassert>
#include <fstream>
#include "BaseScene.h"

UINT16 Stage::stageNum = 0;

void LoadVectorXZStream(std::istringstream& stream, Vector3& vec)
{
	stream >> vec.x;
	stream >> vec.z;
}

void Stage::Initialize(bool* isLight)
{
	floorModel_ = Model::Create("cube");
	std::unique_ptr<Sprite> sprite_ = Sprite::Create("stages/floor.png");
	sprite_->SetSize(sprite_->GetSize() / 5.0f);
	floorModel_->SetSprite(std::move(sprite_));
	floorWTrans_.Initialize();
	floorWTrans_.translation = { 0.0f,-2.0f,0.0f };
	isPlayerLight = isLight;
	LoadMap(stageNum);
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
	std::string stage[8] = { "_select","", "_tutorial", "1", "2", "3", "4", "5" };
	const std::string stagefile = "stages/";
	const std::string filename = "stage" + stage[stageNum] + ".txt";
	const std::string directoryPath = "Resources/" + stagefile + filename;
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
	// 引数用変数
	GimmickParam gimmickParam;

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

		// どのギミックを読み込んだかの判別
		int gimmickType = -1;
		GimmickNum gimmickNum = GimmickNum::None;
		if (word.find("floor") == 0) { gimmickType = 0; }
		else if (word.find("door") == 0) { gimmickType = 1; gimmickNum = GimmickNum::Door; }
		else if (word.find("candle") == 0) { gimmickType = 2; gimmickNum = GimmickNum::Candle; }
		else if (word.find("wall") == 0) { gimmickType = 3; gimmickNum = GimmickNum::Wall; }
		else if (word.find("start") == 0) { gimmickType = 4; }
		else { continue; } // 何も読み込まれてなければ次へ

		// コマンド読み込み
		LoadStreamCommands(line_stream, word, gimmickParam);

		// 固有処理
		switch (gimmickType)
		{
		case 0: // 床
			// 床のスケールをセット
			floorWTrans_.scale = gimmickParam.scale;
			stageSize_ = { gimmickParam.scale.x, gimmickParam.scale.z };
			continue;
		case 1: // ドア
			doorPos = gimmickParam.pos;
			break;
		case 4: // スタート地点
			// 座標セット
			gimmickParam.pos.y = -1.0f;
			startPos = gimmickParam.pos;
			continue;
		}

		// ギミック生成
		PopGimmick(gimmickNum, gimmickParam);
	}
}

void Stage::LoadStreamCommands(std::istringstream& stream, std::string& word, GimmickParam& gimmickParam)
{
	// パラメータリセット
	gimmickParam.pos = { 0.0f, 0.0f, 0.0f };
	gimmickParam.scale = { 1.0f, 1.0f, 1.0f };
	gimmickParam.rot = { 0.0f, 0.0f, 0.0f };
	gimmickParam.flag = false;

	// (区切りで先頭文字列を取得
	while (getline(stream, word, '('))
	{
		// 座標取得
		if (word.find("pos") == 0) { LoadVectorXZStream(stream, gimmickParam.pos); }
		// スケール取得
		else if (word.find("scale") == 0) { LoadVectorXZStream(stream, gimmickParam.scale); }
		// 回転角取得
		else if (word.find("rot") == 0) { stream >> gimmickParam.rot.y; }
		// フラグ取得
		else if (word.find("flag") == 0) { stream >> gimmickParam.flag; }
		// 空白まで飛ばす
		getline(stream, word, ' ');
	}
}

void Stage::PopGimmick(GimmickNum gimmickNum, const GimmickParam& gimmickParam)
{
	// 宣言、生成
	std::unique_ptr<Gimmick> gimmick;
	switch (gimmickNum)
	{
	case GimmickNum::Door:		gimmick = std::make_unique<Door>(doorIndex++);		break;
	case GimmickNum::Candle:	gimmick = std::make_unique<Candle>(lightIndex++);	break;
	case GimmickNum::Wall:		gimmick = std::make_unique<Wall>(isPlayerLight);	break;
	}

	//初期設定
	gimmick->Initialize(gimmickParam);
	// コンテナにプッシュ
	gimmicks_.push_back(std::move(gimmick));
}