#pragma once
#include "Sprite.h"

class PostEffect : public Sprite
{
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

	ComPtr<ID3D12Resource> texBuff;
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;

public:
	void Initialize();
	void Draw();
};

