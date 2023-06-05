#pragma once
#include <unordered_map>
#include "Scene.h"
#include "Sprite.h"

namespace UIType
{
	// �X�e�[�W�I����UI
	enum class Select
	{
		Light, // ����
	};

	// �`���[�g���A����UI
	enum class Tutorial
	{
		Move = 26,
		CameraMove = 28,
		LightChange = 30, // ���C�g�؂�ւ�
		Jump = 32, // �W�����v
		TutorialText1 = 34,	// �߂��̐C��𓔂���
		TutorialText2,	   	// HP�������
		TutorialText3,	   	// �V���ȐC��𓔂���
		TutorialText4,	   	// �΂�؂�ւ���
		TutorialText5,	   	// �΂̐F�ɂ���ĕς���
		TutorialText6,	   	// �X�C�b�`���ړ����悤
		TutorialText7,	   	// �X�C�b�`�͈��������Ȃ�
		TutorialText8,	   	// ��֍s������
		TutorialText9,	   	// �����擾���Ă݂悤
		TutorialText10,	   	// ���̌��Ђ������
		TutorialText11,	   	// �����g���Ă݂悤
		TutorialText12,	   	// ���̃h�A���S�[����
	};

	// �v���C�V�[����UI(�`���[�g���A���V�[���ƌ��p)
	enum class Play
	{
		PlayerGuage = 0, // HP�Q�[�W
		PlayerGuageFrame = 2, // HP�Q�[�W�̘g
		Candle = 3, // �C��
		Key = 11, // ��
		KeyParts = 12, // ���p�[�c
		Light = 18,
		Lever = 20, // ���o�[
		KeyOpen = 22, // �����J����
		Reset = 24, // ���Z�b�g
	};
}

class UIDrawer
{
private:
	class SceneUI
	{
	private:
		std::vector<std::unique_ptr<Sprite>> uis;
		Scene scene = Scene::Null;

		void LoadUI(const std::string& fileName, float sizeRate = 1.0f, const Vector2& anchorPoint = {}, bool isInvisible = false, const Vector2& pos = {});

	public:
		void SetScene(Scene nowScene) { scene = nowScene; }
		void Load();
		void Update();
		void Draw();
		Sprite* GetUI(size_t index) { return uis[index].get(); }
	};

	static Scene scene;
	static std::unordered_map<Scene, SceneUI> sceneUIs;

public:
	UIDrawer() = delete;
	~UIDrawer() = delete;

	static void LoadAll();
	static void Update();
	static void Draw();
	static Sprite* GetUI(size_t index);
};

void UIReset(); // �C��ƌ���UI������