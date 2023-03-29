#include "Stage.h"
#include <cassert>
#include <fstream>

UINT16 Stage::stageNum = static_cast<UINT16>(StageNum::Alpha);

void LoadVectorXZStream(std::istringstream& stream, Vector3& vec)
{
	stream >> vec.x;
	stream >> vec.z;
}

void Stage::Initialize()
{
	// ���̃��f���A�e�N�X�`���ݒ�
	floorModel_ = Model::Create("cube");
	std::unique_ptr<Sprite> sprite_ = Sprite::Create("stages/floor.png");
	sprite_->SetSize(sprite_->GetSize() / 5.0f);
	floorModel_->SetSprite(std::move(sprite_));
	// �͂��ǂ̃��f��
	wallModel_ = Model::Create("cube");
	// ���[���h�s�񏉊���
	floorWTrans_.Initialize();
	floorWTrans_.translation = { 0.0f,-2.0f,0.0f };
	for (auto& wallWTrans : wallAroundWTrans_) { wallWTrans.Initialize(); }
	// �X�e�[�W�Z�b�g
	LoadMap(stageNum);
}

void Stage::Update()
{
	Sprite* sprite = floorModel_->GetSprite();
	sprite->SetColor({ 1,1,1,1 });
	floorModel_->Update();
	floorWTrans_.Update();
	wallModel_->Update();
	for (auto& wallWTrans : wallAroundWTrans_) { wallWTrans.Update(); }
	for (auto& gimmick : gimmicks_) { gimmick->Update(); }
}

void Stage::Draw()
{
	floorModel_->Draw(floorWTrans_);
	for (auto& wallWTrans : wallAroundWTrans_) { wallModel_->Draw(wallWTrans); }
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
	std::string stage[static_cast<UINT16>(StageNum::StageNum)] = { "_select", "_tutorial","_alpha" ,"1", "2", "3", "4", "5" };
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

		// �ǂ̃M�~�b�N��ǂݍ��񂾂��̔���
		int gimmickType = -1;
		GimmickNum gimmickNum = GimmickNum::None;
		if (word.find("floor") == 0) { gimmickType = 0; }
		else if (word.find("door") == 0) { gimmickType = 1; gimmickNum = GimmickNum::Door; }
		else if (word.find("candle") == 0) { gimmickType = 2; gimmickNum = GimmickNum::Candle; }
		else if (word.find("wall") == 0) { gimmickType = 3; gimmickNum = GimmickNum::Wall; }
		else if (word.find("start") == 0) { gimmickType = 4; }
		else { continue; } // �����ǂݍ��܂�ĂȂ���Ύ���

		// �R�}���h�ǂݍ���
		LoadStreamCommands(line_stream, word, gimmickParam);

		// �ŗL����
		switch (gimmickType)
		{
		case 0: // ��
			// ���̃X�P�[�����Z�b�g
			floorWTrans_.scale = gimmickParam.scale;
			// �͂��ǂ̃X�P�[���A���W���Z�b�g
			// ��
			wallAroundWTrans_[0].scale.x = gimmickParam.scale.x + 2.0f;
			wallAroundWTrans_[0].translation.z = gimmickParam.scale.z + 1.0f;
			// ��
			wallAroundWTrans_[1].scale.x = gimmickParam.scale.x + 2.0f;
			wallAroundWTrans_[1].translation.z = -gimmickParam.scale.z - 1.0f;
			// �E
			wallAroundWTrans_[2].scale.z = gimmickParam.scale.z + 2.0f;
			wallAroundWTrans_[2].translation.x = gimmickParam.scale.x + 1.0f;
			// ��
			wallAroundWTrans_[3].scale.z = gimmickParam.scale.z + 2.0f;
			wallAroundWTrans_[3].translation.x = -gimmickParam.scale.x - 1.0f;
			// �X�e�[�W�T�C�Y���Z�b�g
			stageSize_ = { gimmickParam.scale.x, gimmickParam.scale.z };
			continue;
		case 1: // �h�A
			doorPos = gimmickParam.pos;
			break;
		case 4: // �X�^�[�g�n�_
			// ���W�Z�b�g
			startPos = gimmickParam.pos;
			continue;
		}

		// �M�~�b�N����
		PopGimmick(gimmickNum, gimmickParam);
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
		if (word.find("pos") == 0) { LoadVectorXZStream(stream, gimmickParam.pos); }
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

void Stage::PopGimmick(GimmickNum gimmickNum, const GimmickParam& gimmickParam)
{
	// �錾�A����
	std::unique_ptr<Gimmick> gimmick;
	switch (gimmickNum)
	{
	case GimmickNum::Door:		gimmick = std::make_unique<Door>(doorIndex++);		break;
	case GimmickNum::Candle:	gimmick = std::make_unique<Candle>(lightIndex++);	break;
	case GimmickNum::Wall:		gimmick = std::make_unique<Wall>();					break;
	}

	//�����ݒ�
	gimmick->Initialize(gimmickParam);
	// �R���e�i�Ƀv�b�V��
	gimmicks_.push_back(std::move(gimmick));
}