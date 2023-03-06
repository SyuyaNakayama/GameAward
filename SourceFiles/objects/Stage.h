#pragma once
#include <sstream>
#include "Gimmick.h"

class Stage
{
public:
	static const int STAGE_WIDTH = 25;
	static const int STAGE_HEIGHT = 25;
	static const int STAGE_DEPTH = 10;
	enum class GimmickNum {
		NONE,
		DOOR,
		KEY,
		CANDLE,
		GIMMICK_NUM
	};

public:
	void Initialize();
	void Update();
	void Draw();
	// �X�e�[�W�}�b�v�ǂݍ���
	void LoadMap(UINT16 stageNum);

	~Stage() { sprite_.release(); }
	Vector3 GetDoorPos() { return doorPos; }
private:
	// �X�e�[�W�t�@�C���ǂݍ���
	void LoadStageFile(UINT16 stageNum);
	// �R�}���h�ǂݍ���
	void LoadStageCommands();
	// �M�~�b�N����
	void PopGimmick(GimmickNum gimmickNum, Vector3 pos);

	std::unique_ptr<Model> modelFloor_;
	std::unique_ptr<Sprite> sprite_;
	WorldTransform floorWTrans_;

	// �M�~�b�N
	std::vector<std::unique_ptr<Gimmick>> gimmicks_;
	// �t�@�C���R�}���h
	std::stringstream stageCommands_;

	Vector3 doorPos;
};