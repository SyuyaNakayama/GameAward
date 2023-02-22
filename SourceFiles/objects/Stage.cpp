#include "Stage.h"
#include <cassert>
#include <fstream>

void Stage::Initialize()
{
	model_ = Model::Create("cube");
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			worldTransform_[y][x].Initialize();
			worldTransform_[y][x].translation = { 0.0f, -5.0f, 0.0f };
		}
	}

	LoadMap(0);
}

void Stage::Update()
{

	model_->TextureUpdate();
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			worldTransform_[y][x].Update();
		}
	}
}

void Stage::Draw()
{
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < STAGE_WIDTH; x++)
		{
			model_->Draw(worldTransform_[y][x]);
		}
	}
}


void Stage::LoadMap(UINT16 stage)
{
	// �o�b�t�@���N���A
	stageCommands.str("");
	// ��Ԃ��N���A
	stageCommands.clear(std::stringstream::goodbit);

	LoadStageData(stage);

	LoadStageCommands();
}

void Stage::LoadStageData(UINT16 stage)
{
	// �t�@�C��
	std::ifstream file;
	// �p�X���擾
	std::string stageNum[7] = { "", "1", "2", "3", "4", "5", "6" };
	const std::string stagefile = "stages/";
	const std::string filename = "stage" + stageNum[stage] + ".txt";
	const std::string directoryPath = "Resources/" + stagefile + "/" + filename;
	// �t�@�C�����J��
	file.open(directoryPath);
	assert(file.is_open());
	// �t�@�C���̓��e�𕶎���X�g���[���ɃR�s�[
	stageCommands << file.rdbuf();

	// �t�@�C�������
	file.close();
}

void Stage::LoadStageCommands()
{
	// 1�s���̕����������ϐ�
	std::string line;
	// �}�b�v�`�b�v�p
	int x = 0;
	int y = 0;

	// �R�}���h���s���[�v
	while (getline(stageCommands, line)) {
		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);
		// ������
		std::string word;
		// ,��؂�ōs�̐擪��������擾
		getline(line_stream, word, ',');

		// "//"����n�܂�s�̓R�����g
		if (word.find("//") == 0) {
			// �R�����g�s��ǂݔ�΂�
			continue;
		}

		while (x != STAGE_WIDTH)
		{
			// �R�}���h�ǂݍ���
			if (word.find("0") == 0) {
			}
			else if (word.find("1") == 0) {
				Vector3 trans{};
				trans.x = -9.5f + (1.0f * x);
				trans.y = -5.0f;
				trans.z = 9.5f - (1.0f * y);

				worldTransform_[y][x].translation = trans;
			}
			// ���̓��e��
			getline(line_stream, word, ',');
			x++;
		}
		y++;
		x = 0;
	}
}
