#pragma once
#include <sstream>
#include "Gimmick.h"

class Stage
{
public: // 列挙クラス
	/// <summary>
	/// ギミックの番号
	/// </summary>
	enum class GimmickNum {
		None,
		GoalDoor,
		SelectDoor,
		RoomDoor,
		Key,
		Candle,
		Block,
		Switch,
		GimmickKinds
	};

	/// <summary>
	/// ステージの番号
	/// </summary>
	enum class StageNum {
		Select,		// セレクト
		Tutorial,	// チュートリアル
		Tutorial2,
		Stage1,		// ステージ1
		Stage2,		// ステージ2
		Stage3,		// ステージ3
		Stage4,		// ステージ4
		StageNum,	// ステージ数
	};

private:
	// ギミック
	std::vector<std::unique_ptr<Gimmick>> gimmicks_;
	// ギミックインデックス
	size_t lightIndex = 1;
	size_t doorIndex = 1;
	// ドア座標
	Vector3 doorPos;
	// スタート地点
	Vector3 startPos;
	Vector3 startRot;
	// ステージ番号
	static UINT16 stageNum;
	// ファイルコマンド
	std::stringstream stageCommands_;
	
	// ステージマップ読み込み
	void LoadMap(UINT16 stageNum);
	// ステージファイル読み込み
	void LoadStageFile(UINT16 stageNum);
	// コマンド読み込み
	void LoadStageCommands();
	// ギミック生成
	void PopGimmick(GimmickNum gimmickNum, const GimmickParam& gimmickParam);
	// ストリームコマンド読み込み
	void LoadStreamCommands(std::istringstream& stream, std::string& word, GimmickParam& gimmickParam);
public:
	void Initialize();
	void Update();
	void Draw();

	// アクセッサ
	Vector3 GetStartPos() { return startPos; }
	Vector3 GetStartRot() { return startRot; }
	Vector3 GetDoorPos() { return doorPos; }
	
	static UINT16 GetStageNum() { return stageNum; }
	static void SetStageNum(UINT16 stageNum_) { stageNum = stageNum_; }
};