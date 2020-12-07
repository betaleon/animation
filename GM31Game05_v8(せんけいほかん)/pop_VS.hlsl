//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// マトリクスバッファ
cbuffer WorldBuffer : register(b0)
{
	matrix World;
}
cbuffer ViewBuffer : register(b1)
{
	matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}

// マテリアルバッファ
struct MATERIAL
{
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4		Emission;
	float		Shininess;
};

cbuffer MaterialBuffer : register(b3)
{
	MATERIAL	Material;
}


// ライトバッファ
struct LIGHT
{
	bool		Enable;
	bool3		Dummy;//16byte境界用
	float4		Direction;
	float4		Diffuse;
	float4		Ambient;
};

struct Attributes
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL0;
	//float4 diffuse : COLOR0;
	//float2 texCoord : TEXCOORD0;
};


cbuffer LightBuffer : register(b4)
{
	LIGHT		Light;
}

//=============================================================================
// 頂点シェーダ
//=============================================================================

//
// Vertex shader
//
Attributes main(Attributes input)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	input.position = mul(input.position,wvp);
	input.normal = mul(input.normal, World);

	return input;
}
