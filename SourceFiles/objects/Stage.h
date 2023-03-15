#pragma once
#include <sstream>
#include "Gimmick.h"

class Stage
{
private:
	// 列挙クラス
	enum class GimmickNum {
		NONE,
		DOOR,
		CANDLE,
		WALL,
		GIMMICK_NUM
	};

	// 床
	std::unique_ptr<Model> floorModel_;
	WorldTransform floorWTrans_;

	// ギミック
	std::vector<std::unique_ptr<Gimmick>> gimmicks_;
	// ファイルコマンド
	std::stringstream stageCommands_;

	// ステージ横幅、縦幅
	Vector2 stageSize_;
	// スタート地点
	Vector3 startPos;
	// ドアの座標
	Vector3 doorPos;
	// ライトインデックス
	size_t lightIndex = 1;

	// 各種引数用変数
	Vector3 pos{};
	Vector3 scale{};
	Vector3 rot{};
	bool flag = false;

	// ステージファイル読み込み
	void LoadStageFile(UINT16 stageNum);
	// ステージコマンド読み込み
	void LoadStageCommands();
	// ストリームコマンド読み込み
	void LoadStreamCommands(std::istringstream& stream, std::string& word);
	// ギミック生成
	void PopGimmick(GimmickNum gimmickNum, Vector3 pos, Vector3 scale = { 1.0f, 1.0f, 1.0f }, Vector3 rot = { 1.0f, 1.0f, 1.0f }, bool flag = false);
public:
	void Initialize();
	void Update();
	void Draw();
	// ステージマップ読み込み
	void LoadMap(UINT16 stageNum);

	// アクセッサ
	Vector2 GetStageSize() { return stageSize_; }
	Vector3 GetStartPos() { return startPos; }
	Vector3 GetDoorPos() { return doorPos; }
	// デストラクタ
	~Stage() { floorModel_.release(); }
};