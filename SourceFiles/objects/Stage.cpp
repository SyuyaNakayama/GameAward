#include "Stage.h"
#include <cassert>
#include <fstream>

void Stage::Initialize()
{
	model_ = Model::Create("cube");
	sprite_ = Sprite::Create("stages/floor.png");
	sprite_->SetSize(sprite_->GetSize() / 5.0f);
	model_->SetSprite(sprite_.get());
	worldTransform_.Initialize();
	worldTransform_.translation = { 0.0f,-2.0f,0.0f };
	worldTransform_.scale = { STAGE_WIDTH,1,STAGE_HEIGHT };

	LoadMap(0);
}

void Stage::Update()
{
	model_->TextureUpdate();
	worldTransform_.Update();
	/*for (WorldTransform& worldTransform : worldTransform_)
	{
		worldTransform.Update();
	}*/
}

void Stage::Draw()
{
	model_->Draw(worldTransform_);
	/*for (WorldTransform& worldTransform : worldTransform_)
	{
		model_->Draw(worldTransform);
	}*/
}


void Stage::LoadMap(UINT16 stage)
{
	//// �o�b�t�@���N���A
	//stageCommands.str("");
	//// ��Ԃ��N���A
	//stageCommands.clear(std::stringstream::goodbit);

	//LoadStageFile(stage);

	//LoadStageCommands();
}

void Stage::LoadStageFile(UINT16 stage)
{
	//// �t�@�C��
	//std::ifstream file;
	//// �p�X���擾
	//std::string stageNum[7] = { "", "1", "2", "3", "4", "5", "6" };
	//const std::string stagefile = "stages/";
	//const std::string filename = "stage" + stageNum[stage] + ".txt";
	//const std::string directoryPath = "Resources/" + stagefile + "/" + filename;
	//// �t�@�C�����J��
	//file.open(directoryPath);
	//assert(file.is_open());
	//// �t�@�C���̓��e�𕶎���X�g���[���ɃR�s�[
	//stageCommands << file.rdbuf();

	//// �t�@�C�������
	//file.close();
}

void Stage::LoadStageCommands()
{
	//// 1�s���̕����������ϐ�
	//std::string line;
	//// �}�b�v�`�b�v�p
	//int x = 0;
	//int y = 0;

	//Vector3 trans{};
	//trans.y = -5.0f;

	//// �R�}���h���s���[�v
	//while (getline(stageCommands, line)) {
	//	// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
	//	std::istringstream line_stream(line);
	//	// ������
	//	std::string word;
	//	// ,��؂�ōs�̐擪��������擾
	//	getline(line_stream, word, ',');

	//	// "//"����n�܂�s�̓R�����g
	//	if (word.find("//") == 0) {
	//		// �R�����g�s��ǂݔ�΂�
	//		continue;
	//	}

	//	// �R�}���h�ǂݍ���
	//	if (word.find("map") == 0) {
	//		while (x != 20)
	//		{
	//			if (word.find("0") == 0) {
	//			}
	//			else if (word.find("1") == 0) {
	//				WorldTransform worldTransform;

	//				trans.x = -9.5f + (1.0f * x);
	//				trans.z = 9.5f - (1.0f * y);

	//				worldTransform.Initialize();
	//				worldTransform.translation = trans;

	//				worldTransform_.push_back(worldTransform);
	//			}
	//			// ���̓��e��
	//			getline(line_stream, word, ',');
	//			x++;
	//		}
	//	}
	//	else if (word.find("next") == 0) {
	//		trans.y += 1.5f;
	//		y = 0;
	//		x = 0;
	//		continue;
	//	}
	//	y++;
	//	x = 0;
	//}
}
