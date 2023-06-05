#include "UIDrawer.h"
#include "SpriteCommon.h"
#include "SceneManager.h"

std::unordered_map<Scene, UIDrawer::SceneUI> UIDrawer::sceneUIs;
Scene UIDrawer::scene = Scene::Null;

void UIDrawer::SceneUI::LoadUI(const std::string& fileName, float sizeRate,
	const Vector2& anchorPoint, bool isInvisible, const Vector2& pos)
{
	std::unique_ptr<Sprite> sprite = Sprite::Create("ui/" + fileName);
	sprite->SetSize(sprite->GetSize() * sizeRate);
	sprite->SetPosition(pos);
	sprite->SetAnchorPoint(anchorPoint);
	sprite->SetIsInvisible(isInvisible);
	uis.push_back(std::move(sprite));
}

void UIDrawer::SceneUI::Load()
{
	float objUISizeRate = 1.0f / 8.0f;

	switch (scene)
	{
	case Scene::Title:
		LoadUI("title/title.png", 2.0f, { 0.5f,0.5f });	// �^�C�g��
		LoadUI("title/select.png", 1.0f, { 0.5f,0.5f });	// �Q�[���v���C
		LoadUI("title/tutorial.png", 0.85f, { 0.5f,0.5f });	// �`���[�g���A��
		// ���֐i�ނ��߂̃{�^��
		LoadUI("keyboard/jump.png", 1.0f, { 0.5f,0.5f }, true);
		LoadUI("controller/light.png", 0.8f, { 0.5f,0.5f }, true);
		break;
	case Scene::Select:
		// ����
		LoadUI("keyboard/light.png", 0.8f, { 0.5f,0.5f }, true);
		LoadUI("controller/light.png", 0.8f, { 0.5f,0.5f }, true);
		//�^�C�g����
		LoadUI("keyboard/title.png", 0.8f, { 0.5f,0.5f }, false, { 1600,100 });
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
		LoadUI("keyboard/light.png", 0.8f, { 0.5f,0.5f }, true);
		LoadUI("controller/light.png", 0.8f, { 0.5f,0.5f }, true);
		// ���o�[�؂�ւ�
		LoadUI("keyboard/lever.png", 0.8f, { 0.5f,0.0f }, true);
		LoadUI("controller/lever.png", 0.8f, { 0.5f,0.0f }, true);
		// �����J����
		LoadUI("keyboard/keyOpen.png", 0.8f, { 0.5f,0.0f }, true);
		LoadUI("controller/keyOpen.png", 0.8f, { 0.5f,0.0f }, true);
		// ���Z�b�gUI
		LoadUI("keyboard/reset.png", 0.9f, { 0.5f, 0.5f }, false, { 1600,100 });
		LoadUI("controller/reset.png", 0.9f, { 0.5f, 0.5f }, false, { 1600,100 });
		
		if (scene == Scene::Play) 
		{ 
			//�Z���N�g��
			LoadUI("keyboard/stageSelect.png", 0.7f, { 0.5f,0.5f }, false, { 1600,200 }); break; 
		}

		// ��������`���[�g���A���̐���UI
		// �ړ�
		LoadUI("keyboard/move.png", 1.0f, { 0.5f,0 }, true);
		LoadUI("controller/move.png", 1.0f, { 0.5f,0 }, true);
		// ���_�ړ�
		LoadUI("keyboard/cameraMove.png", 1.0f, { 0.5f,0 }, true);
		LoadUI("controller/cameraMove.png", 1.0f, { 0.5f,0 }, true);
		// �΂�ς���
		LoadUI("keyboard/lightChange.png", 1.0f, { 0.5f,0 }, true);
		LoadUI("controller/lightChange.png", 1.0f, { 0.5f,0 }, true);
		// �W�����v
		LoadUI("keyboard/jump.png", 1.0f, { 0.5f,0.0f }, true);
		LoadUI("controller/jump.png", 1.0f, { 0.5f,0.0f }, true);
		//�`���[�g���A���e�L�X�g
		for (size_t i = 0; i < 12; i++)
		{
			LoadUI("tutorial/tutorial" + std::to_string(i + 1) + ".png", 1.0f, { 0.5f,0 }, true);
		}
		break;
	case Scene::Clear:
		LoadUI("GameClear.png", 1.0f, { 0.5f,0.5f });
		// ���֐i�ނ��߂̃{�^��
		LoadUI("keyboard/jump.png", 1.0f, { 0.5f,0.5f }, true);
		LoadUI("controller/light.png", 0.8f, { 0.5f,0.5f }, true);
	}
}

void UIDrawer::SceneUI::Update()
{
	for (auto& ui : uis) { ui->Update(); }
}

void UIDrawer::SceneUI::Draw()
{
	for (auto& ui : uis) { ui->Draw(); }
}

void UIDrawer::LoadAll()
{
	sceneUIs[Scene::Title].SetScene(Scene::Title);
	sceneUIs[Scene::Select].SetScene(Scene::Select);
	sceneUIs[Scene::Tutorial].SetScene(Scene::Tutorial);
	sceneUIs[Scene::Play].SetScene(Scene::Play);
	sceneUIs[Scene::Clear].SetScene(Scene::Clear);
	for (auto& sceneUI : sceneUIs) { sceneUI.second.Load(); }
}

void UIDrawer::Update()
{
	scene = SceneManager::GetInstance()->GetNowScene();
	for (auto& sceneUI : sceneUIs) { if (sceneUI.first == scene) { sceneUI.second.Update(); } }
}

void UIDrawer::Draw()
{
	SpriteCommon::PreDraw();
	for (auto& sceneUI : sceneUIs) { if (sceneUI.first == scene) { sceneUI.second.Draw(); } }
	SpriteCommon::PostDraw();
}

Sprite* UIDrawer::GetUI(size_t index)
{
	scene = SceneManager::GetInstance()->GetNowScene();
	return sceneUIs[scene].GetUI(index);
}

void UIReset()
{
	// �C���UI
	// �`���ԏ�����
	size_t candleUIIndex = (size_t)UIType::Play::Candle;
	for (size_t i = candleUIIndex; i < candleUIIndex + 8; i++) { UIDrawer::GetUI(i)->SetIsInvisible(true); }
	for (size_t i = 0; i < Candle::GetLightNum(); i++)
	{
		Sprite* candleUI = nullptr;
		candleUI = UIDrawer::GetUI(candleUIIndex + i);
		candleUI->SetIsInvisible(false);
		candleUI->SetPosition({ candleUI->GetSize().x * i + 32,85 });
		candleUI->SetColor({ 1,1,1,0.5f });
	}
	// ����UI
	// �`���ԏ�����
	size_t keyUIIndex = (size_t)UIType::Play::Key;
	size_t keyPartsUIIndex = (size_t)UIType::Play::KeyParts;
	UIDrawer::GetUI(keyUIIndex)->SetIsInvisible(true);
	for (size_t i = keyPartsUIIndex; i < keyPartsUIIndex + 6; i++) { UIDrawer::GetUI(i)->SetIsInvisible(true); }
	for (size_t i = 0; i < KeyLock::GetKeyNum(); i++)
	{
		Sprite* keyUI = nullptr;
		if (KeyLock::GetKeyNum() == 1) { keyUI = UIDrawer::GetUI(keyUIIndex); }
		else { keyUI = UIDrawer::GetUI(keyPartsUIIndex + i); }
		keyUI->SetIsInvisible(false);
		keyUI->SetPosition({ keyUI->GetSize().x * i + 32, 165 });
		keyUI->SetColor({ 1,1,1,0.5f });
	}
}