#include "Stage.h"
#include <cassert>
#include <fstream>
#include <imgui.h>

UINT16 Stage::stageNum = static_cast<UINT16>(StageNum::Alpha);

void LoadVector3Stream(std::istringstream& stream, Vector3& vec);

void Stage::Initialize()
{
	// ステージセット
	LoadMap(stageNum);
}

void Stage::Update()
{
	for (auto& gimmick : gimmicks_) { gimmick->Update(); }
}

void Stage::Draw()
{
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
	// ライト関連の変数リセット
	lightIndex = 1;
	Candle::ResetLightNum();
	// 鍵関連の変数リセット
	KeyLock::ResetKeyNum();
	// ドア関連の変数リセット
	doorIndex = 1;
	// マップ読み込み
	LoadStageFile(stageNum);
	LoadStageCommands();
}

void Stage::LoadStageFile(UINT16 stageNum)
{
	// ファイル
	std::ifstream file;
	// パスを取得
	std::string stage[static_cast<UINT16>(StageNum::StageNum)] = { "_select", "_tutorial","_alpha" ,"1", "2", "3", "4", "5" };
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
		if (word.find("door") == 0) { gimmickType = 1; gimmickNum = GimmickNum::GoalDoor; }
		else if (word.find("sldoor") == 0) { gimmickType = 5; gimmickNum = GimmickNum::SelectDoor; }
		else if (word.find("rmdoor") == 0) { gimmickType = 5; gimmickNum = GimmickNum::RoomDoor; }
		else if (word.find("key") == 0) { gimmickType = 3; gimmickNum = GimmickNum::Key; }
		else if (word.find("candle") == 0) { gimmickType = 2; gimmickNum = GimmickNum::Candle; }
		else if (word.find("floor") == 0 || word.find("wall") == 0 || word.find("block") == 0) { gimmickType = 3; gimmickNum = GimmickNum::Block; }
		else if (word.find("start") == 0) { gimmickType = 4; }
		else { continue; } // 何も読み込まれてなければ次へ

		// コマンド読み込み
		LoadStreamCommands(line_stream, word, gimmickParam);

		// 固有処理
		switch (gimmickType)
		{
		case 1: // ドア
			doorPos = gimmickParam.pos;
			break;
		case 4: // スタート地点
			// 座標セット
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
	gimmickParam.textureIndex = 0;
	gimmickParam.vanishFlag = 0;
	gimmickParam.moveFlag = false;
	gimmickParam.limits = { 0.0f, 0.0f };

	// (区切りで先頭文字列を取得
	while (getline(stream, word, '('))
	{
		// 座標取得
		if (word.find("pos") == 0) { LoadVector3Stream(stream, gimmickParam.pos); }
		// スケール取得
		else if (word.find("scale") == 0) { LoadVector3Stream(stream, gimmickParam.scale); }
		// 回転角取得
		else if (word.find("rot") == 0) { stream >> gimmickParam.rot.y; }
		// フラグ取得(texture)
		else if (word.find("texture") == 0) { stream >> gimmickParam.textureIndex; }
		// フラグ取得(vanish)
		else if (word.find("vflag") == 0) { stream >> gimmickParam.vanishFlag; }
		// フラグ取得(move)
		else if (word.find("mflag") == 0) { stream >> gimmickParam.moveFlag; }
		// 移動の下限上限値設定
		else if (word.find("limit") == 0) {
			stream >> gimmickParam.limits.x;
			stream >> gimmickParam.limits.y;
		}
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
	case GimmickNum::GoalDoor:		gimmick = std::make_unique<GoalDoor>();					break;
	case GimmickNum::SelectDoor:	gimmick = std::make_unique<SelectDoor>(doorIndex++);	break;
	case GimmickNum::RoomDoor:		gimmick = std::make_unique<RoomDoor>(doorIndex++);		break;
	case GimmickNum::Key:			gimmick = std::make_unique<KeyLock>();					break;
	case GimmickNum::Candle:		gimmick = std::make_unique<Candle>(lightIndex++);		break;
	case GimmickNum::Block:		gimmick = std::make_unique<Block>();					break;
	}

	//初期設定
	gimmick->Initialize(gimmickParam);
	// コンテナにプッシュ
	gimmicks_.push_back(std::move(gimmick));
}