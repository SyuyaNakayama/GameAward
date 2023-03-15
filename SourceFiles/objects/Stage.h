#pragma once
#include <sstream>
#include "Gimmick.h"

class Stage
{
public:
	static const int STAGE_WIDTH = 60;
	static const int STAGE_HEIGHT = 32;

	enum class GimmickNum {
		None,
		Door,
		Candle,
		Wall,
		GimmickKinds
	};

private:
	std::unique_ptr<Model> modelFloor_;
	WorldTransform floorWTrans_;
	size_t lightIndex = 1;
	size_t doorIndex = 1;

	// �M�~�b�N
	std::vector<std::unique_ptr<Gimmick>> gimmicks_;
	// �t�@�C���R�}���h
	std::stringstream stageCommands_;
	Vector3 doorPos;

	// �X�e�[�W�}�b�v�ǂݍ���
	void LoadMap(UINT16 stageNum);
	// �X�e�[�W�t�@�C���ǂݍ���
	void LoadStageFile(UINT16 stageNum);
	// �R�}���h�ǂݍ���
	void LoadStageCommands();
	// �M�~�b�N����
	void PopGimmick(GimmickNum gimmickNum, Vector3 pos, Vector3 scale = { 1.0f, 1.0f, 1.0f });

	/*
	���݃X�e�[�W
	0 �X�e�[�W�I��(�^�C�g���V�[��)
	1 �`���[�g���A��
	2~6 �X�e�[�W1~5
	*/
	static UINT16 stageNum;

public:
	void Initialize();
	void Update();
	void Draw();

	Vector3 GetDoorPos() { return doorPos; }
	~Stage() { modelFloor_.release(); }
	static UINT16 GetStageNum() { return stageNum; }
	static void SetStageNum(UINT16 stageNum_) { stageNum = stageNum_; }
};