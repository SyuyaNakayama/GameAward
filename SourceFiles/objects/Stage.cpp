#include "Stage.h"
#include <cassert>
#include <fstream>

UINT16 Stage::stageNum = static_cast<UINT16>(StageNum::Tutorial);

void LoadVector3Stream(std::istringstream& stream, Vector3& vec);

void Stage::Initialize()
{
	// �X�e�[�W�Z�b�g
	LoadMap(stageNum);
}

void Stage::Update()
{
	for (auto& gimmick : gimmicks_) { gimmick->Update(); }
}

void Stage::Draw()
{
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
	Gimmick::ResetEventParam();
	// ���C�g�֘A�̕ϐ����Z�b�g
	lightIndex = 1;
	Candle::Reset();
	// ���֘A�̕ϐ����Z�b�g
	KeyLock::ResetKeyNum();
	// �X�C�b�`�֘A�̕ϐ����Z�b�g
	Switch::ResetSwitchNum();
	// �h�A�֘A�̕ϐ����Z�b�g
	doorIndex = 1;
	// �}�b�v�ǂݍ���
	LoadStageFile(stageNum);
	LoadStageCommands();
}

void Stage::LoadStageFile(UINT16 stageNum)
{
	// �t�@�C��
	std::ifstream file;
	// �p�X���擾
	std::string stage[static_cast<UINT16>(StageNum::StageNum)] = { "_select", "_tutorial", "1", "2", "3", "4" };
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
		GimmickNum gimmickNum = GimmickNum::None;
		if (word.find("door") == 0) { gimmickNum = GimmickNum::GoalDoor; }
		else if (word.find("sldoor") == 0) { gimmickNum = GimmickNum::SelectDoor; }
		else if (word.find("rmdoor") == 0) { gimmickNum = GimmickNum::RoomDoor; }
		else if (word.find("key") == 0) { gimmickNum = GimmickNum::Key; }
		else if (word.find("candle") == 0) { gimmickNum = GimmickNum::Candle; }
		else if (word.find("floor") == 0 || word.find("wall") == 0 || word.find("block") == 0) { gimmickNum = GimmickNum::Block; }
		else if (word.find("switch") == 0) { gimmickNum = GimmickNum::Switch; }
		else if (word.find("start") == 0) {}
		else { continue; } // �����ǂݍ��܂�ĂȂ���Ύ���

		// �R�}���h�ǂݍ���
		LoadStreamCommands(line_stream, word, gimmickParam);

		switch (gimmickNum)
		{
		case Stage::GimmickNum::None: // �X�^�[�g�ʒu
			startPos = gimmickParam.pos;
			startRot = gimmickParam.rot;
			continue;
		case Stage::GimmickNum::GoalDoor: // �h�A
			doorPos = gimmickParam.pos; 
			break;
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
	gimmickParam.pathPoints.clear();
	gimmickParam.vanishFlag = 0;
	gimmickParam.moveFlag = false;
	gimmickParam.textureIndex = 0;
	gimmickParam.eventIndex = 0;
	gimmickParam.isEither = false;

	// (��؂�Ő擪��������擾
	while (getline(stream, word, '('))
	{
		// ���W�擾
		if (word.find("pos") == 0) { LoadVector3Stream(stream, gimmickParam.pos); }
		// �X�P�[���擾
		else if (word.find("scale") == 0) { LoadVector3Stream(stream, gimmickParam.scale); }
		// ��]�p�擾
		else if (word.find("rot") == 0) { stream >> gimmickParam.rot.y; }
		// �t���O�擾(texture)
		else if (word.find("texture") == 0) { stream >> gimmickParam.textureIndex; }
		// �t���O�擾(vanish)
		else if (word.find("vflag") == 0) { stream >> gimmickParam.vanishFlag; }
		// �t���O�擾(move)
		else if (word.find("mflag") == 0) { stream >> gimmickParam.moveFlag; }
		// �o�H�_�̎擾
		else if (word.find("pathPos") == 0) {
			Vector3 pos;	// ���W
			LoadVector3Stream(stream, pos);	// ���W�擾
			gimmickParam.pathPoints.push_back(pos);	// �R���e�i�Ƀv�b�V��
		}
		// �C�x���g�C���f�b�N�X�擾
		else if (word.find("event") == 0) { stream >> gimmickParam.eventIndex; }
		// either�t���O�擾
		else if (word.find("either") == 0) { stream >> gimmickParam.isEither; }
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
	case GimmickNum::GoalDoor:		gimmick = std::make_unique<GoalDoor>();						break;
	case GimmickNum::SelectDoor:	gimmick = std::make_unique<SelectDoor>(doorIndex++);	break;
	case GimmickNum::RoomDoor:	gimmick = std::make_unique<RoomDoor>(doorIndex++);	break;
	case GimmickNum::Key:				gimmick = std::make_unique<KeyLock>();							break;
	case GimmickNum::Candle:		gimmick = std::make_unique<Candle>(lightIndex++);		break;
	case GimmickNum::Block:			gimmick = std::make_unique<Block>();								break;
	case GimmickNum::Switch:			gimmick = std::make_unique<Switch>();							break;
	}

	//�����ݒ�
	gimmick->Initialize(gimmickParam);
	// �R���e�i�Ƀv�b�V��
	gimmicks_.push_back(std::move(gimmick));
}