#pragma once
#include <basetsd.h>
#include <Model.h>
#include <sstream>


class Stage
{
public:
	static const int STAGE_WIDTH = 20;
	static const int STAGE_HEIGHT = 20;
	static const int STAGE_DEPTH = 10;

public:
	void Initialize();
	void Update();
	void Draw();
	// �X�e�[�W�}�b�v�ǂݍ���
	void LoadMap(UINT16 stage);

private:
	// �X�e�[�W�t�@�C���ǂݍ���
	void LoadStageFile(UINT16 stage);
	// �R�}���h�ǂݍ���
	void LoadStageCommands();

private:
	std::unique_ptr<Model> model_;
	std::vector<WorldTransform> worldTransform_;
	// �t�@�C���R�}���h
	std::stringstream stageCommands;
};

