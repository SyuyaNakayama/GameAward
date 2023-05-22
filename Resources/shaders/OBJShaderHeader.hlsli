cbuffer cbuff0 : register(b0)
{
	matrix world; // ワールド行列
};

cbuffer cbuff3 : register(b3)
{
	matrix viewproj; // ビュープロジェクション行列
	float3 cameraPos; // カメラ座標(ワールド座標)
};

cbuffer cbuff1 : register(b1)
{
	float3 m_ambient  : packoffset(c0); // アンビエント係数
	float3 m_diffuse  : packoffset(c1); // ディフューズ係数
	float3 m_specular : packoffset(c2); // スペキュラー係数
	float m_alpha : packoffset(c2.w);	// アルファ
}

struct DirLight
{
	float3 lightv; // ライトへの方向の単位ベクトル
	float3 lightcolor; // ライトの色(RGB)
	uint active;
};

struct PointLight
{
	float3 lightpos; // ライト座標
	float3 lightcolor; // ライトの色(RGB)
	float3 lightatten; // ライト距離減衰係数
	uint active;
};

static const uint DIRLIGHT_NUM = 3;
static const uint POINTLIGHT_NUM = 9;

cbuffer cbuff2 : register(b2)
{
	float3 ambientColor;
	DirLight dirLights[DIRLIGHT_NUM];
	PointLight pointLights[POINTLIGHT_NUM];
}

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する構造体
struct VSOutput
{
	float4 svpos : SV_POSITION; // システム用頂点座標
	float4 worldpos : POSITION; // ワールド座標
	float3 normal : NORMAL; // 法線
	float2 uv : TEXCOORD; // uv値
	float4 spriteColor : COLOR; // スプライト色
};