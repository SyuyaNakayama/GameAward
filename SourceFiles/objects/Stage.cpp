#include "Stage.h"
#include <cassert>
#include <fstream>

UINT16 Stage::stageNum = static_cast<UINT16>(StageNum::Tutorial);

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
	Gimmick::ResetEventParam();
	// ライト関連の変数リセット
	lightIndex = 1;
	Candle::Reset();
	// 鍵関連の変数リセット
	KeyLock::ResetKeyNum();
	// スイッチ関連の変数リセット
	Switch::ResetSwitchNum();
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
	std::string stage[static_cast<UINT16>(StageNum::StageNum)] = { "_select", "_tutorial", "1", "2", "3", "4" };
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
		GimmickNum gimmickNum = GimmickNum::None;
		if (word.find("door") == 0) { gimmickNum = GimmickNum::GoalDoor; }
		else if (word.find("sldoor") == 0) { gimmickNum = GimmickNum::SelectDoor; }
		else if (word.find("rmdoor") == 0) { gimmickNum = GimmickNum::RoomDoor; }
		else if (word.find("key") == 0) { gimmickNum = GimmickNum::Key; }
		else if (word.find("candle") == 0) { gimmickNum = GimmickNum::Candle; }
		else if (word.find("floor") == 0 || word.find("wall") == 0 || word.find("block") == 0) { gimmickNum = GimmickNum::Block; }
		else if (word.find("switch") == 0) { gimmickNum = GimmickNum::Switch; }
		else if (word.find("start") == 0) {}
		else { continue; } // 何も読み込まれてなければ次へ

		// コマンド読み込み
		LoadStreamCommands(line_stream, word, gimmickParam);

		switch (gimmickNum)
		{
		case Stage::GimmickNum::None: // スタート位置
			startPos = gimmickParam.pos;
			startRot = gimmickParam.rot;
			continue;
		case Stage::GimmickNum::GoalDoor: // ドア
			doorPos = gimmickParam.pos; 
			break;
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
	gimmickParam.pathPoints.clear();
	gimmickParam.vanishFlag = 0;
	gimmickParam.moveFlag = false;
	gimmickParam.textureIndex = 0;
	gimmickParam.eventIndex = 0;
	gimmickParam.isEither = false;

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
		// 経路点の取得
		else if (word.find("pathPos") == 0) {
			Vector3 pos;	// 座標
			LoadVector3Stream(stream, pos);	// 座標取得
			gimmickParam.pathPoints.push_back(pos);	// コンテナにプッシュ
		}
		// イベントインデックス取得
		else if (word.find("event") == 0) { stream >> gimmickParam.eventIndex; }
		// eitherフラグ取得
		else if (word.find("either") == 0) { stream >> gimmickParam.isEither; }
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
	case GimmickNum::GoalDoor:		gimmick = std::make_unique<GoalDoor>();						break;
	case GimmickNum::SelectDoor:	gimmick = std::make_unique<SelectDoor>(doorIndex++);	break;
	case GimmickNum::RoomDoor:	gimmick = std::make_unique<RoomDoor>(doorIndex++);	break;
	case GimmickNum::Key:				gimmick = std::make_unique<KeyLock>();							break;
	case GimmickNum::Candle:		gimmick = std::make_unique<Candle>(lightIndex++);		break;
	case GimmickNum::Block:			gimmick = std::make_unique<Block>();								break;
	case GimmickNum::Switch:			gimmick = std::make_unique<Switch>();							break;
	}

	//初期設定
	gimmick->Initialize(gimmickParam);
	// コンテナにプッシュ
	gimmicks_.push_back(std::move(gimmick));
}