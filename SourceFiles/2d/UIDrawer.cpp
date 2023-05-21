#include "UIDrawer.h"
#include "SpriteCommon.h"
#include "SceneManager.h"

std::array<UIDrawer::SceneUI, (size_t)Scene::SceneNum - 1> UIDrawer::sceneUIs;
Scene UIDrawer::scene = Scene::Null;

void UIDrawer::SceneUI::LoadUI(const std::string& fileName, float sizeRate, Vector2 anchorPoint, bool isInvisible)
{
	std::unique_ptr<Sprite> sprite = Sprite::Create("ui/" + fileName);
	sprite->SetSize(sprite->GetSize() * sizeRate);
	sprite->SetAnchorPoint(anchorPoint);
	sprite->SetIsInvisible(isInvisible);
	uis.push_back(std::move(sprite));
}

void UIDrawer::SceneUI::Load()
{
	float objUISizeRate = 1.0f / 8.0f;

	SpriteAnimation* spriteAnime = nullptr;
	switch (scene)
	{
	case Scene::Title:
		LoadUI("stage/stageTutorial.png", 3.0f, { 0.5f,0.5f }); // �^�C�g��
		LoadUI("stage/stageTutorial.png", 1.5f, { 0.5f,0.5f }); // �Q�[���v���C
		LoadUI("stage/stageTutorial.png", 1.5f, { 0.5f,0.5f }); // �`���[�g���A��
		break;
	case Scene::Select:
		// �ړ�
		//LoadUI("keyboard/move.png", 1.0f, { 0.5f,0.0f }, true);
		//LoadUI("controller/move.png", 1.0f, { 0.5f,0.0f }, true);
		// ����
		LoadUI("keyboard/light.png", 0.6f, { 0.5f,0.5f }, true);
		LoadUI("controller/light.png", 0.5f, { 0.5f,0.5f }, true);
		// ���_�ړ�
		//LoadUI("keyboard/cameraMove.png", 1.0f, { 0.5f,0.0f }, true);
		//LoadUI("controller/cameraMove.png", 1.0f, { 0.5f,0.0f }, true);
		break;
	case Scene::Tutorial:
	case Scene::Play:
		// �v���C���[��HP�Q�[�W�̔w�i
		LoadUI("white1x1.png");
		LoadUI("white1x1.png");
		// �v���C���[��HP�Q�[�W�̘g
		LoadUI("hpUI.png", 1.0f, { 0.0f,0.0f }, true);
		// �C���UI
		for (size_t i = 0; i < 8; i++) { LoadUI("CandleUI.png", objUISizeRate, {}, true); }
		// ��
		LoadUI("key/key.png", objUISizeRate, {}, true);
		// ���p�[�c
		LoadUI("key/parts1.png", objUISizeRate * 2.0f, {}, true);
		LoadUI("key/parts2.png", objUISizeRate * 2.0f, {}, true);
		LoadUI("key/parts3.png", objUISizeRate * 2.0f, {}, true);
		LoadUI("key/parts4.png", objUISizeRate * 2.0f, {}, true);
		LoadUI("key/parts4.png", objUISizeRate * 2.0f, {}, true);
		LoadUI("key/parts5.png", objUISizeRate * 2.0f, {}, true);
		// ����
		LoadUI("keyboard/light.png", 0.6f, { 0.5f,0.5f }, true);
		LoadUI("controller/light.png", 0.5f, { 0.5f,0.5f }, true);
		// ���o�[�؂�ւ�
		LoadUI("keyboard/lever.png", 0.8f, { 0.5f,0.0f }, true);
		LoadUI("controller/lever.png", 0.8f, { 0.5f,0.0f }, true);
		// �����J����
		LoadUI("keyboard/keyOpen.png", 0.8f, { 0.5f,0.0f }, true);
		LoadUI("controller/keyOpen.png", 0.8f, { 0.5f,0.0f }, true);
		// ���Z�b�gUI
		LoadUI("keyboard/reset.png", 0.5f, { -5.5f, -0.5f });
		LoadUI("controller/reset.png", 0.8f, { -5.5f, -0.15f });
		if (scene == Scene::Play) { break; }
		/* -----�`���[�g���A���̐���UI----- */
		// �΂�ς���
		LoadUI("keyboard/lightChange.png", 1.0f, { 0.5f,0.5f }, true);
		LoadUI("controller/lightChange.png", 1.0f, { 0.5f,0.5f }, true);
		// �W�����v
		LoadUI("keyboard/jump.png", 0.8f, { 0.5f,0.0f }, true);
		LoadUI("controller/jump.png", 0.8f, { 0.5f,0.0f }, true);
		//�`���[�g���A���e�L�X�g
		LoadUI("tutorial/tutorial1.png", 1.0f, { 0.5f,0.5f }, true);	// �ړ������_�ړ�
		LoadUI("tutorial/tutorial2.png", 1.0f, { 0.5f,0.5f }, true);	// �߂��̐C��𓔂���
		LoadUI("tutorial/tutorial3.png", 1.0f, { 0.5f,0.5f }, true);	// �W�����v
		LoadUI("tutorial/tutorial4.png", 1.0f, { 0.5f,0.5f }, true);	// HP�������
		LoadUI("tutorial/tutorial5.png", 1.0f, { 0.5f,0.5f }, true);	// �V���ȐC��𓔂���
		LoadUI("tutorial/tutorial6.png", 1.0f, { 0.5f,0.5f }, true);	// �΂�؂�ւ���
		LoadUI("tutorial/tutorial7.png", 1.0f, { 0.5f,0.5f }, true);	// �΂̐F�ɂ���ĕς���
		LoadUI("tutorial/tutorial8.png", 1.0f, { 0.5f,0.5f }, true);	// �X�C�b�`���ړ����悤
		LoadUI("tutorial/tutorial9.png", 1.0f, { 0.5f,0.5f }, true);	// �X�C�b�`���͈��������Ȃ�
		LoadUI("tutorial/tutorial10.png", 1.0f, { 0.5f,0.5f }, true);	// ��֍s������
		LoadUI("tutorial/tutorial11.png", 1.0f, { 0.5f,0.5f }, true);	// �����擾���Ă݂悤
		LoadUI("tutorial/tutorial12.png", 1.0f, { 0.5f,0.5f }, true);	// ���̌��Ђ������
		LoadUI("tutorial/tutorial13.png", 1.0f, { 0.5f,0.5f }, true);	// �����g���Ă݂悤
		LoadUI("tutorial/tutorial14.png", 1.0f, { 0.5f,0.5f }, true);	// ���̃h�A���S�[����
		//�`���[�g���A���e�L�X�g(�L�[�{�[�h)
		LoadUI("keyboard/move.png", 1.0f, { 0.5f,0.5f }, true);
		LoadUI("keyboard/jump.png", 1.0f, { 0.5f,0.5f }, true);
		LoadUI("keyboard/lightChange.png", 1.0f, { 0.5f,0.5f }, true);
	}
}

void UIDrawer::SceneUI::Update()
{
	for (auto& ui : uis) { ui->Update(); }
	for (auto& animationUI : animationUIs) { animationUI.Update(); }
}

void UIDrawer::SceneUI::Draw()
{
	for (auto& ui : uis) { ui->Draw(); }
	for (auto& animationUI : animationUIs) { animationUI.Draw(); }
}

void UIDrawer::LoadAll()
{
	sceneUIs[(size_t)Scene::Title - 1].SetScene(Scene::Title);
	sceneUIs[(size_t)Scene::Select - 1].SetScene(Scene::Select);
	sceneUIs[(size_t)Scene::Tutorial - 1].SetScene(Scene::Tutorial);
	sceneUIs[(size_t)Scene::Play - 1].SetScene(Scene::Play);
	for (auto& sceneUI : sceneUIs) { sceneUI.Load(); }
}

void UIDrawer::Update()
{
	scene = SceneManager::GetInstance()->GetNowScene();
	for (auto& sceneUI : sceneUIs) { if (sceneUI.GetScene() == scene) { sceneUI.Update(); } }
}

void UIDrawer::Draw()
{
	SpriteCommon::PreDraw();
	for (auto& sceneUI : sceneUIs) { if (sceneUI.GetScene() == scene) { sceneUI.Draw(); } }
	SpriteCommon::PostDraw();
}

Sprite* UIDrawer::GetUI(size_t index)
{
	scene = SceneManager::GetInstance()->GetNowScene();
	return sceneUIs[(size_t)scene - 1].GetUI(index);
}