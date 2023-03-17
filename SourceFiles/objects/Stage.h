#pragma once
#include <sstream>
#include "Gimmick.h"

class Stage
{
private:
	// �񋓃N���X
	enum class GimmickNum {
		None,
		Door,
		Candle,
		Wall,
		GimmickKinds
	};

private:
	// ��
	std::unique_ptr<Model> floorModel_;
	WorldTransform floorWTrans_;
	size_t lightIndex = 1;
	size_t doorIndex = 1;
	bool* isPlayerLight = nullptr;

	// �M�~�b�N
	std::vector<std::unique_ptr<Gimmick>> gimmicks_;
	// �t�@�C���R�}���h
	std::stringstream stageCommands_;
	Vector3 doorPos;

	// �X�e�[�W�����A�c��
	Vector2 stageSize_;
	// �X�^�[�g�n�_
	Vector3 startPos;
	/*
		���݃X�e�[�W
		0 �X�e�[�W�I��(�^�C�g���V�[��)
		1 �`���[�g���A��
		2~6 �X�e�[�W1~5
	*/
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
	void Initialize(bool* isLight);
	void Update();
	void Draw();

	// �A�N�Z�b�T
	Vector2 GetStageSize() { return stageSize_; }
	Vector3 GetStartPos() { return startPos; }
	Vector3 GetDoorPos() { return doorPos; }
	// �f�X�g���N�^
	~Stage() { floorModel_.release(); }
	static UINT16 GetStageNum() { return stageNum; }
	static void SetStageNum(UINT16 stageNum_) { stageNum = stageNum_; }
};