#pragma once
#include <sstream>
#include "Gimmick.h"

class Stage
{
private:
	// 列挙クラス
	enum class GimmickNum {
		None,
		Door,
		Candle,
		Wall,
		GimmickKinds
	};

private:
	// 床
	std::unique_ptr<Model> floorModel_;
	WorldTransform floorWTrans_;
	size_t lightIndex = 1;
	size_t doorIndex = 1;

	// ギミック
	std::vector<std::unique_ptr<Gimmick>> gimmicks_;
	// ファイルコマンド
	std::stringstream stageCommands_;
	Vector3 doorPos;

	// ステージ横幅、縦幅
	Vector2 stageSize_;
	// スタート地点
	Vector3 startPos;
	/*
		現在ステージ
		0 ステージ選択(タイトルシーン)
		1 チュートリアル
		2~6 ステージ1~5
	*/
	static UINT16 stageNum;
	// ステージマップ読み込み
	void LoadMap(UINT16 stageNum);
	// ステージファイル読み込み
	void LoadStageFile(UINT16 stageNum);
	// コマンド読み込み
	void LoadStageCommands();
	// ギミック生成
	void PopGimmick(GimmickNum gimmickNum, Vector3 pos, Vector3 scale = { 1.0f, 1.0f, 1.0f }, Vector3 rot = { 1.0f, 1.0f, 1.0f }, bool flag = false);
	// ストリームコマンド読み込み
	void LoadStreamCommands(std::istringstream& stream, std::string& word, GimmickParam& gimmickParam);
	// ギミック生成
	void PopGimmick(GimmickNum gimmickNum, GimmickParam& gimmickParam);
public:
	void Initialize();
	void Update(bool isLight);
	void Draw();

	// アクセッサ
	Vector2 GetStageSize() { return stageSize_; }
	Vector3 GetStartPos() { return startPos; }
	Vector3 GetDoorPos() { return doorPos; }
	// デストラクタ
	~Stage() { floorModel_.release(); }
	static UINT16 GetStageNum() { return stageNum; }
	static void SetStageNum(UINT16 stageNum_) { stageNum = stageNum_; }
};