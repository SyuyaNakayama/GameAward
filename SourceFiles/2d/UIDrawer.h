#pragma once
#include <vector>
#include "SpriteAnimation.h"
#include "Scene.h"

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
		LightChange = 26, // ���C�g�؂�ւ�
		Jump = 28, // �W�����v
		tutorialText1 = 30,
		tutorialText2,
		tutorialText3,
		tutorialText4,
		tutorialText5,
		tutorialText6,
		tutorialText7,
		tutorialText8,
		tutorialText9,
		tutorialText10,
		tutorialText11,
		tutorialText12,
		tutorialText13,
		tutorialText14,
		tutorialText1_2,
		tutorialText3_2,
		tutorialText6_2
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
		std::vector<SpriteAnimation> animationUIs;
		Scene scene = Scene::Null;

		void LoadUI(const std::string& fileName, float sizeRate = 1.0f, Vector2 anchorPoint = {}, bool isInvisible = false);

	public:
		void SetScene(Scene nowScene) { scene = nowScene; }
		Scene GetScene() { return scene; }
		void Load();
		void Update();
		void Draw();
		Sprite* GetUI(size_t index) { return uis[index].get(); }
	};

	static Scene scene;
	static std::array<SceneUI, (size_t)Scene::SceneNum - 1> sceneUIs;

public:
	UIDrawer() = delete;
	~UIDrawer() = delete;

	static void LoadAll();
	static void Update();
	static void Draw();
	static Sprite* GetUI(size_t index);
};