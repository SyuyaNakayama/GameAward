#pragma once
#include <sstream>
#include "Gimmick.h"

class Stage
{
private:
	// �񋓃N���X
	enum class GimmickNum {
		NONE,
		DOOR,
		CANDLE,
		WALL,
		GIMMICK_NUM
	};

	// ��
	std::unique_ptr<Model> floorModel_;
	WorldTransform floorWTrans_;

	// �M�~�b�N
	std::vector<std::unique_ptr<Gimmick>> gimmicks_;
	// �t�@�C���R�}���h
	std::stringstream stageCommands_;

	// �X�e�[�W�����A�c��
	Vector2 stageSize_;
	// �X�^�[�g�n�_
	Vector3 startPos;
	// �h�A�̍��W
	Vector3 doorPos;
	// ���C�g�C���f�b�N�X
	size_t lightIndex = 1;

	// �e������p�ϐ�
	Vector3 pos{};
	Vector3 scale{};
	Vector3 rot{};
	bool flag = false;

	// �X�e�[�W�t�@�C���ǂݍ���
	void LoadStageFile(UINT16 stageNum);
	// �X�e�[�W�R�}���h�ǂݍ���
	void LoadStageCommands();
	// �X�g���[���R�}���h�ǂݍ���
	void LoadStreamCommands(std::istringstream& stream, std::string& word);
	// �M�~�b�N����
	void PopGimmick(GimmickNum gimmickNum, Vector3 pos, Vector3 scale = { 1.0f, 1.0f, 1.0f }, Vector3 rot = { 1.0f, 1.0f, 1.0f }, bool flag = false);
public:
	void Initialize();
	void Update();
	void Draw();
	// �X�e�[�W�}�b�v�ǂݍ���
	void LoadMap(UINT16 stageNum);

	// �A�N�Z�b�T
	Vector2 GetStageSize() { return stageSize_; }
	Vector3 GetStartPos() { return startPos; }
	Vector3 GetDoorPos() { return doorPos; }
	// �f�X�g���N�^
	~Stage() { floorModel_.release(); }
};