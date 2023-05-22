#pragma once
#include "DirectionalLight.h"
#include "PointLight.h"
#include <wrl.h>
#include <d3d12.h>
#include <memory>

class LightGroup
{
public:
	static const int DIR_LIGHT_NUM = 3;
	static const int POINT_LIGHT_NUM = 9;

private:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	struct ConstBufferData
	{
		ColorRGB ambientColor;
		float pad1;
		DirectionalLight::ConstBufferData dirLights[DIR_LIGHT_NUM];
		PointLight::ConstBufferData pointLights[POINT_LIGHT_NUM];
	};

	ComPtr<ID3D12Resource> constBuff;
	ColorRGB ambientColor;
	DirectionalLight dirLights[DIR_LIGHT_NUM];
	PointLight pointLights[POINT_LIGHT_NUM];
	bool dirty = false;
	ConstBufferData* constMap = nullptr;

	void TransferConstBuffer();
public:
	static std::unique_ptr<LightGroup> Create();
	void Initialize();
	void Update();
	void Draw();
	void DefaultLightSetting();
	void SetAmbientColor(const ColorRGB& color) { ambientColor = color; dirty = true; }
#pragma region ïΩçsåıåπSetter
	void SetDirLightActive(size_t index, bool active);
	void SetDirLightDir(size_t index, const Vector3& lightDir);
	void SetDirLightColor(size_t index, const ColorRGB& lightcolor);
#pragma endregion
#pragma region ì_åıåπSetter
	void SetPointLightActive(size_t index, bool active);
	void SetPointLightPos(size_t index, const Vector3& lightpos);
	void SetPointLightColor(size_t index, const ColorRGB& lightcolor);
	void SetPointLightAtten(size_t index, const Vector3& lightAtten);
#pragma endregion
	bool GetPointLightActive(size_t index) { return pointLights[index].IsActive(); }
};