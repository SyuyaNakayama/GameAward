#pragma once
#include <sstream>
#include "Gimmick.h"

class Stage
{
public:
	static const int STAGE_WIDTH = 30;
	static const int STAGE_HEIGHT = 16;

	enum class GimmickNum {
		NONE,
		DOOR,
		CANDLE,
	};

	void Initialize();
	void Update();
	void Draw();
	// �X�e�[�W�}�b�v�ǂݍ���
	void LoadMap(UINT16 stageNum);

	Vector3 GetDoorPos() { return doorPos; }
	~Stage() { modelFloor_.release(); }

private:
	// �X�e�[�W�t�@�C���ǂݍ���
	void LoadStageFile(UINT16 stageNum);
	// �R�}���h�ǂݍ���
	void LoadStageCommands();
	// �M�~�b�N����
	void PopGimmick(GimmickNum gimmickNum, Vector3 pos);

	std::unique_ptr<Model> modelFloor_;
	WorldTransform floorWTrans_;

	// �M�~�b�N
	std::vector<std::unique_ptr<Gimmick>> gimmicks_;
	// �t�@�C���R�}���h
	std::stringstream stageCommands_;

	Vector3 doorPos;
};