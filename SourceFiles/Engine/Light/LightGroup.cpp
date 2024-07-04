#include "LightGroup.h"
#include "D3D12Common.h"

void LightGroup::TransferConstBuffer()
{
	constMap->ambientColor = ambientColor;
	// 平行光源
	for (int i = 0; i < DIR_LIGHT_NUM; i++)
	{
		// ライトが有効なら設定を転送	
		if (dirLights[i].IsActive())
		{
			constMap->dirLights[i].active = 1;
			constMap->dirLights[i].lightv = -dirLights[i].GetLightDir();
			constMap->dirLights[i].lightcolor = dirLights[i].GetLightColor();
		}
		// ライトが無効なら転送しない
		else { constMap->dirLights[i].active = 0; }
	}
	// 点光源
	for (int i = 0; i < POINT_LIGHT_NUM; i++)
	{
		if (pointLights[i].IsActive())
		{
			constMap->pointLights[i].active = 1;
			constMap->pointLights[i].lightpos = pointLights[i].GetLightPos();
			constMap->pointLights[i].lightcolor = pointLights[i].GetLightColor();
			constMap->pointLights[i].lightAtten = pointLights[i].GetLightAtten();
		}
		else { constMap->pointLights[i].active = 0; }
	}
}

std::unique_ptr<LightGroup> LightGroup::Create()
{
	std::unique_ptr<LightGroup> instance = std::make_unique<LightGroup>();
	instance->Initialize();
	return instance;
}

void LightGroup::Initialize()
{
	DefaultLightSetting();
	CreateBuffer(&constBuff, &constMap, (sizeof(ConstBufferData) + 0xff) & ~0xff);
	TransferConstBuffer();
}

void LightGroup::Update()
{
	if (!dirty) { return; }
	TransferConstBuffer();
	dirty = false;
}

void LightGroup::Draw()
{
	ID3D12GraphicsCommandList* cmdList = DirectXCommon::GetInstance()->GetCommandList();
	cmdList->SetGraphicsRootConstantBufferView(3, constBuff->GetGPUVirtualAddress());
}

void LightGroup::DefaultLightSetting()
{
	for (DirectionalLight& dirLight : dirLights)
	{
		dirLight.SetActive(true);
		dirLight.SetLightColor({ 1,1,1 });
	}
	dirLights[0].SetLightDir({ 0,-1,0 });
	dirLights[1].SetLightDir({ 0.5f,0.1f,0.2f });
	dirLights[2].SetLightDir({ -0.5f,0.1f,-0.2f });
}

#pragma region 平行光源Setter
void LightGroup::SetDirLightActive(size_t index, bool active)
{
	assert(0 <= index && index < DIR_LIGHT_NUM);
	dirLights[index].SetActive(active);
	dirty = true;
}

void LightGroup::SetDirLightDir(size_t index, const Vector3& lightdir)
{
	assert(0 <= index && index < DIR_LIGHT_NUM);
	dirLights[index].SetLightDir(lightdir);
	dirty = true;
}

void LightGroup::SetDirLightColor(size_t index, const ColorRGB& lightcolor)
{
	assert(0 <= index && index < DIR_LIGHT_NUM);
	dirLights[index].SetLightColor(lightcolor);
	dirty = true;
}
#pragma endregion
#pragma region 点光源Setter
void LightGroup::SetPointLightActive(size_t index, bool active)
{
	assert(0 <= index && index < POINT_LIGHT_NUM);
	pointLights[index].SetActive(active);
	dirty = true;
}

void LightGroup::SetPointLightPos(size_t index, const Vector3& lightpos)
{
	assert(0 <= index && index < POINT_LIGHT_NUM);
	pointLights[index].SetLightPos(lightpos);
	dirty = true;
}

void LightGroup::SetPointLightColor(size_t index, const ColorRGB& lightcolor)
{
	assert(0 <= index && index < POINT_LIGHT_NUM);
	pointLights[index].SetLightColor(lightcolor);
	dirty = true;
}

void LightGroup::SetPointLightAtten(size_t index, const Vector3& lightAtten)
{
	assert(0 <= index && index < POINT_LIGHT_NUM);
	pointLights[index].SetLightAtten(lightAtten);
	dirty = true;
}
#pragma endregion