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
		Door,
		Key,
		Candle,
		Wall,
		GimmickKinds
	};

	/// <summary>
	/// �X�e�[�W�̔ԍ�
	/// </summary>
	enum class StageNum {
		Select,		// �Z���N�g
		Tutorial,	// �`���[�g���A��
		Alpha,		//�A���t�@��
		Stage1,		// �X�e�[�W1
		Stage2,		// �X�e�[�W2
		Stage3,		// �X�e�[�W3
		Stage4,		// �X�e�[�W4
		Stage5,		// �X�e�[�W5
		StageNum,	// �X�e�[�W��
	};

private:
	// ��
	std::unique_ptr<Model> floorModel_;
	WorldTransform floorWTrans_;
	// ����̕�
	std::unique_ptr<Model> wallModel_;
	WorldTransform wallAroundWTrans_[4];
	size_t lightIndex = 1;
	size_t doorIndex = 1;
	// �M�~�b�N
	std::vector<std::unique_ptr<Gimmick>> gimmicks_;
	// �t�@�C���R�}���h
	std::stringstream stageCommands_;
	// �X�e�[�W�����A�c��
	Vector2 stageSize_;
	// �h�A���W
	Vector3 doorPos;
	// �X�^�[�g�n�_
	Vector3 startPos;
	// �X�e�[�W�ԍ�
	static UINT16 stageNum;
	
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
	// �f�X�g���N�^
	~Stage() { floorModel_.release(); }
	void Initialize();
	void Update();
	void Draw();

	// �A�N�Z�b�T
	Vector2 GetStageSize() { return stageSize_; }
	Vector3 GetStartPos() { return startPos; }
	Vector3 GetDoorPos() { return doorPos; }
	
	static UINT16 GetStageNum() { return stageNum; }
	static void SetStageNum(UINT16 stageNum_) { stageNum = stageNum_; }
};