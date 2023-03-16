#include "Stage.h"
#include <cassert>
#include <fstream>
#include "BaseScene.h"

UINT16 Stage::stageNum = 0;

void LoadVectorXZStream(std::istringstream& stream, Vector3& vec)
{
	stream >> vec.x;
	stream >> vec.z;
}

void Stage::Initialize()
{
	floorModel_ = Model::Create("cube");
	std::unique_ptr<Sprite> sprite_ = Sprite::Create("stages/floor.png");
	sprite_->SetSize(sprite_->GetSize() / 5.0f);
	floorModel_->SetSprite(std::move(sprite_));
	floorWTrans_.Initialize();
	floorWTrans_.translation = { 0.0f,-2.0f,0.0f };
	LoadMap(stageNum);
}

void Stage::Update(bool isLight)
{
	Sprite* sprite = floorModel_->GetSprite();
	sprite->SetColor({ 1,1,1,1 });
	floorModel_->Update();
	floorWTrans_.Update();
	for (auto& gimmick : gimmicks_) { gimmick->Update(isLight); }
}

void Stage::Draw()
{
	floorModel_->Draw(floorWTrans_);
	for (auto& gimmick : gimmicks_) { gimmick->Draw(); }
}

void Stage::LoadMap(UINT16 stageNum)
{
	// �o�b�t�@���N���A
	stageCommands_.str("");
	// ��Ԃ��N���A
	stageCommands_.clear(std::stringstream::goodbit);
	// �M�~�b�N�R���e�i�̒��g����ɂ���
	gimmicks_.clear();
	// ���C�g�֘A�ϐ��̃��Z�b�g
	lightIndex = 1;
	Candle::ResetLightNum();
	// �}�b�v�ǂݍ���
	LoadStageFile(stageNum);
	LoadStageCommands();
}

void Stage::LoadStageFile(UINT16 stageNum)
{
	// �t�@�C��
	std::ifstream file;
	// �p�X���擾
	std::string stage[8] = { "_select","", "_tutorial", "1", "2", "3", "4", "5" };
	const std::string stagefile = "stages/";
	const std::string filename = "stage" + stage[stageNum] + ".txt";
	const std::string directoryPath = "Resources/" + stagefile + filename;
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
	// �����p�ϐ�
	GimmickParam gimmickParam;

	// �R�}���h���s���[�v
	while (getline(stageCommands_, line)) {
		// 1�s���̕�������X�g���[���ɕϊ����ĉ�͂��₷������
		std::istringstream line_stream(line);
		// ������
		std::string word;
		// �󔒋�؂�ōs�̐擪��������擾
		getline(line_stream, word, ' ');

		// "//"����n�܂�s�̓R�����g
		if (word.find("//") == 0) {
			// �R�����g�s��ǂݔ�΂�
			continue;
		}

		// �R�}���h�ǂݍ���
		if (word.find("floor") == 0) {
			// �R�}���h�ǂݍ���
			LoadStreamCommands(line_stream, word, gimmickParam);
			// ���̃X�P�[�����Z�b�g
			floorWTrans_.scale = gimmickParam.scale;
			stageSize_ = { gimmickParam.scale.x, gimmickParam.scale.z };
		}
		else if (word.find("door") == 0) {
			// �R�}���h�ǂݍ���
			LoadStreamCommands(line_stream, word, gimmickParam);
			// ����
			PopGimmick(GimmickNum::Door, gimmickParam);
			doorPos = gimmickParam.pos;
		}
		else if (word.find("candle") == 0) {
			// �R�}���h�ǂݍ���
			LoadStreamCommands(line_stream, word, gimmickParam);
			// ����
			PopGimmick(GimmickNum::Candle, gimmickParam);
		}
		else if (word.find("wall") == 0) {
			// �R�}���h�ǂݍ���
			LoadStreamCommands(line_stream, word, gimmickParam);
			// ����
			PopGimmick(GimmickNum::Wall, gimmickParam);
		}
		else if (word.find("start") == 0) {
			// �R�}���h�ǂݍ���
			LoadStreamCommands(line_stream, word, gimmickParam);
			// ���W�Z�b�g
			gimmickParam.pos.y = -1.0f;
			startPos = gimmickParam.pos;
		}
	}
}

void Stage::LoadStreamCommands(std::istringstream& stream, std::string& word, GimmickParam& gimmickParam)
{
	// �p�����[�^���Z�b�g
	gimmickParam.pos = { 0.0f, 0.0f, 0.0f };
	gimmickParam.scale = { 1.0f, 1.0f, 1.0f };
	gimmickParam.rot = { 0.0f, 0.0f, 0.0f };
	gimmickParam.flag = false;

	// (��؂�Ő擪��������擾
	while (getline(stream, word, '('))
	{
		// ���W�擾
		if (word.find("pos") == 0)
		{
			LoadVectorXZStream(stream, gimmickParam.pos); 
		}
		// �X�P�[���擾
		else if (word.find("scale") == 0) { LoadVectorXZStream(stream, gimmickParam.scale); }
		// ��]�p�擾
		else if (word.find("rot") == 0) { stream >> gimmickParam.rot.y; }
		// �t���O�擾
		else if (word.find("flag") == 0) { stream >> gimmickParam.flag; }
		// �󔒂܂Ŕ�΂�
		getline(stream, word, ' ');
	}
}

void Stage::PopGimmick(GimmickNum gimmickNum, GimmickParam& gimmickParam)
{
	// �錾�A����
	std::unique_ptr<Gimmick> gimmick;
	switch (gimmickNum)
	{
	case GimmickNum::Door:		gimmick = std::make_unique<Door>(gimmickParam, doorIndex++);	break;
	case GimmickNum::Candle:	gimmick = std::make_unique<Candle>(gimmickParam, lightIndex++);	break;
	case GimmickNum::Wall:		gimmick = std::make_unique<Wall>(gimmickParam);					break;
	}

	//�����ݒ�
	gimmick->Initialize();
	// �R���e�i�Ƀv�b�V��
	gimmicks_.push_back(std::move(gimmick));
}