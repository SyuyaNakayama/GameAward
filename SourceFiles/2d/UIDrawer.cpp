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
		// �ړ�
		LoadUI("keyboard/move.png", 1.0f, { 0.5f,0.0f }, true);
		LoadUI("controller/move.png", 1.0f, { 0.5f,0.0f }, true);
		// ����
		LoadUI("keyboard/light.png", 0.6f, { 0.5f,0.5f }, true);
		LoadUI("controller/light.png", 0.5f, { 0.5f,0.5f }, true);
		// ���_�ړ�
		LoadUI("keyboard/cameraMove.png", 1.0f, { 0.5f,0.0f }, true);
		LoadUI("controller/cameraMove.png", 1.0f, { 0.5f,0.0f }, true);
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
		// �����J����
		LoadUI("keyboard/keyOpen.png", 0.8f, { 0.5f,0.0f }, true);
		// ���Z�b�gUI
		LoadUI("keyboard/reset.png", 0.5f, { -5.5f, -0.5f });
		if (scene == Scene::Play) { break; }
		/* -----�`���[�g���A���̐���UI----- */
		// �΂�ς���
		LoadUI("keyboard/lightChange.png", 1.0f, { 0.5f,0.5f }, true);
		LoadUI("controller/lightChange.png", 1.0f, { 0.5f,0.5f }, true);
		// �W�����v
		LoadUI("keyboard/jump.png", 0.8f, { 0.5f,0.0f }, true);
		// �񕜃G���A
		LoadUI("tutorial/heal.png", 1.0f, { 0.5f,0.0f }, true);
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
	sceneUIs[0].SetScene(Scene::Title);
	sceneUIs[1].SetScene(Scene::Tutorial);
	sceneUIs[2].SetScene(Scene::Play);
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