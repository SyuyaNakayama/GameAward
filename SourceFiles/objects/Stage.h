#pragma once
#include <sstream>
#include "Gimmick.h"

class Stage
{
public: // �񋓃N���X
	/// <summary>
	/// �M�~�b�N�̔ԍ�
	/// </summary>
	enum class GimmickNum {
		None,
		GoalDoor,
		SelectDoor,
		RoomDoor,
		Key,
		Candle,
		Block,
		Switch,
		GimmickKinds
	};

	/// <summary>
	/// �X�e�[�W�̔ԍ�
	/// </summary>
	enum class StageNum {
		Select,		// �Z���N�g
		Tutorial,	// �`���[�g���A��
		Stage1,		// �X�e�[�W1
		Stage2,		// �X�e�[�W2
		Stage3,		// �X�e�[�W3
		Stage4,		// �X�e�[�W4
		StageNum,	// �X�e�[�W��
	};

	enum class ClearFlagBit
	{
		None = 0,
		Stage1 = 1,
		Stage2 = 0b1 << 1,
		Stage3 = 0b1 << 2,
		Stage4 = 0b1 << 3,
		All = (0b1 << 4) - 1
	};

private:
	// �M�~�b�N
	std::vector<std::unique_ptr<Gimmick>> gimmicks_;
	// �M�~�b�N�C���f�b�N�X
	UINT16 lightIndex = 1;
	UINT16 doorIndex = 1;
	// �h�A���W
	Vector3 doorPos;
	// �X�e�[�W�ԍ�
	static UINT16 stageNum;
	// �X�e�[�W�̃N���A�t���O
	static UINT16 clearFlag;
	// �t�@�C���R�}���h
	std::stringstream stageCommands_;
	Player player;

	// �X�e�[�W�}�b�v�ǂݍ���
	void LoadMap(UINT16 stageNum);
	// �X�e�[�W�t�@�C���ǂݍ���
	void LoadStageFile(UINT16 stageNum);
	// �R�}���h�ǂݍ���
	void LoadStageCommands();
	// �M�~�b�N����
	void PopGimmick(GimmickNum gimmickNum, const GimmickParam& gimmickParam);
	// �X�g���[���R�}���h�ǂݍ���
	void LoadStreamCommands(std::istringstream& stream, std::string& word, GimmickParam& gimmickParam);
public:
	void Initialize();
	void Update();
	// �Ă΂ꂽ���̃X�e�[�W�̃N���A�t���O�𗧂Ă�
	static void ClearFlagUpdate();
	void Draw();

	// �A�N�Z�b�T
	Player* GetPlayer() { return &player; }
	static UINT16 GetStageNum() { return stageNum; }
	static void SetStageNum(UINT16 stageNum_) { stageNum = stageNum_; }
	static UINT16 GetClearFlag() { return clearFlag; }
	static bool IsClear() { return clearFlag == (int)ClearFlagBit::All; }
};