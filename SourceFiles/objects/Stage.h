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
		Stage1,		// ステージ1
		Stage2,		// ステージ2
		Stage3,		// ステージ3
		Stage4,		// ステージ4
		StageNum,	// ステージ数
	};

	enum class ClearFlagBit
	{
		None = 0,
		Stage1 = 1,
		Stage2 = 0b1 << 1,
		Stage3 = 0b1 << 2,
		Stage4 = 0b1 << 3,
		All = (0b1 << 4) - 1
	};

private:
	// ギミック
	std::vector<std::unique_ptr<Gimmick>> gimmicks_;
	// ギミックインデックス
	UINT16 lightIndex = 1;
	UINT16 doorIndex = 1;
	// ドア座標
	Vector3 doorPos;
	// ステージ番号
	static UINT16 stageNum;
	// ステージのクリアフラグ
	static UINT16 clearFlag;
	// ファイルコマンド
	std::stringstream stageCommands_;
	Player player;

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
	// 呼ばれた時のステージのクリアフラグを立てる
	static void ClearFlagUpdate();
	void Draw();

	// アクセッサ
	Player* GetPlayer() { return &player; }
	static UINT16 GetStageNum() { return stageNum; }
	static void SetStageNum(UINT16 stageNum_) { stageNum = stageNum_; }
	static UINT16 GetClearFlag() { return clearFlag; }
	static bool IsClear() { return clearFlag == (int)ClearFlagBit::All; }
};