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
		LoadUI("keyboard/UI_WASD.png", 1.0f, { 0.5f,0.0f }, true);
		LoadUI("controller/UI_Lstick.png", 1.0f, { 0.5f,0.0f }, true);
		// ����
		LoadUI("keyboard/UI_Shift3.png", 0.6f, { 0.5f,0.5f }, true);
		LoadUI("controller/UI_Abutton.png", 0.5f, { 0.5f,0.5f }, true);
		// ���_�ړ�
		LoadUI("keyboard/UI_Arrow.png", 1.0f, { 0.5f,0.0f }, true);
		LoadUI("controller/UI_Rstick.png", 1.0f, { 0.5f,0.0f }, true);
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
		LoadUI("key/keyUI.png", objUISizeRate, {}, true);
		// ���p�[�c
		LoadUI("key/parts1.png", objUISizeRate * 2.0f, {}, true);
		LoadUI("key/parts2.png", objUISizeRate * 2.0f, {}, true);
		LoadUI("key/parts3.png", objUISizeRate * 2.0f, {}, true);
		LoadUI("key/parts4.png", objUISizeRate * 2.0f, {}, true);
		LoadUI("key/parts4.png", objUISizeRate * 2.0f, {}, true);
		LoadUI("key/parts5.png", objUISizeRate * 2.0f, {}, true);
		// ����
		LoadUI("keyboard/UI_Shift3.png", 0.6f, { 0.5f,0.5f }, true);
		LoadUI("controller/UI_Abutton.png", 0.5f, { 0.5f,0.5f }, true);
		// �A�N�V����(SHIFT�L�[)
		LoadUI("keyboard/UI_Shift1.png", 0.8f, { 0.5f,0.0f }, true);
		LoadUI("keyboard/UI_Shift2.png", 0.8f, { 0.5f,0.0f }, true);
		// ���Z�b�gUI
		LoadUI("keyboard/UI_R.png", 0.5f, { -5.5f, -0.5f });
		if (scene == Scene::Play) { break; }
		/* -----�`���[�g���A���̐���UI----- */
		// �΂�ς���
		LoadUI("keyboard/UI_SPACE.png", 1.0f, { 0.5f,0.5f }, true);
		LoadUI("controller/UI_LBtrigger.png", 1.0f, { 0.5f,0.5f }, true);
		// �W�����v
		LoadUI("keyboard/UI_Enter.png", 0.8f, { 0.5f,0.0f }, true);
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