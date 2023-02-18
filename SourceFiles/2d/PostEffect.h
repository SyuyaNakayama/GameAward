#pragma once
#include "Sprite.h"

class PostEffect : public Sprite
{
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Resource> texBuff;
	ComPtr<ID3D12Resource> depthBuff;
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	static const float CLEAR_COLOR[4];

public:
	void Initialize();
	void Draw();
	void PreDrawScene();
	void PostDrawScene();
};