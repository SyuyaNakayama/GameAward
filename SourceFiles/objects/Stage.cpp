#include "Stage.h"
#include <cassert>
#include <fstream>

void LoadVector3Stream(std::istringstream& stream, Vector3& vec);

void Stage::Initialize()
{
	modelFloor_ = Model::Create("cube");
	sprite_ = Sprite::Create("stages/floor.png");
	sprite_->SetSize(sprite_->GetSize() / 5.0f);
	modelFloor_->SetSprite(sprite_.get());
	floorWTrans_.Initialize();
	floorWTrans_.translation = { 0.0f,-2.0f,0.0f };
	floorWTrans_.scale = { STAGE_WIDTH,1,STAGE_HEIGHT };

	LoadMap(0);
}

void Stage::Update()
{
	modelFloor_->TextureUpdate();
	floorWTrans_.Update();
	for (auto& gimmick : gimmicks_) { gimmick->Update(); }
}

void Stage::Draw()
{
	modelFloor_->Draw(floorWTrans_);
	for (auto& gimmick : gimmicks_) { gimmick->Draw(); }
}

void Stage::LoadMap(UINT16 stageNum)
{
	// �o�b�t�@���N���A
	stageCommands_.str("");
	// ��Ԃ��N���A
	stageCommands_.clear(std::stringstream::goodbit);
	// �}�b�v�ǂݍ���
	LoadStageFile(stageNum);
	LoadStageCommands();
}

void Stage::LoadStageFile(UINT16 stageNum)
{
	// �t�@�C��
	std::ifstream file;
	// �p�X���擾
	std::string stage[7] = { "", "1", "2", "3", "4", "5", "6" };
	const std::string stagefile = "stages/";
	const std::string filename = "stage" + stage[stageNum] + ".txt";
	const std::string directoryPath = "Resources/" + stagefile + "/" + filename;
	// �t�@�C�����J��
	file.open(directoryPath);
	assert(file.is_open());
	// �t�@�C���̓��e�𕶎���X�g���[���ɃR�s�[
	stageCommands_ << file.rdbuf();

	// �t�@�C�������
	file.close();
}

void Stage::LoadStageCommands()
{
	// 1�s���̕����������ϐ�
	std::string line;
	// ���W
	Vector3 pos;

	// �R�}���h���s���[�v
	while (getline(stageCommands_, line)) {
		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);
		// ������
		std::string word;
		// ,��؂�ōs�̐擪��������擾
		getline(line_stream, word, ' ');

		// "//"����n�܂�s�̓R�����g
		if (word.find("//") == 0) {
			// �R�����g�s��ǂݔ�΂�
			continue;
		}

		// �R�}���h�ǂݍ���
		if (word.find("door") == 0) {
			// ���W�擾
			LoadVector3Stream(line_stream, pos);
			// ����
			PopGimmick(GimmickNum::DOOR, pos);
		}
		else if (word.find("candle") == 0) {
			// ���W�擾
			LoadVector3Stream(line_stream, pos);
			// ����
			PopGimmick(GimmickNum::CANDLE, pos);
		}
	}
}

void Stage::PopGimmick(GimmickNum gimmickNum, Vector3 pos)
{
	// �錾�A����
	std::unique_ptr<Gimmick> gimmick;
	switch (gimmickNum)
	{
	case GimmickNum::DOOR:		gimmick = std::make_unique<Door>();		break;
	case GimmickNum::KEY:		break;
	case GimmickNum::CANDLE:	gimmick = std::make_unique<Candle>();	break;
	}

	//�����ݒ�
	gimmick->Initialize();
	gimmick->SetPosition(pos);
	// �R���e�i�Ƀv�b�V��
	gimmicks_.push_back(std::move(gimmick));
}
