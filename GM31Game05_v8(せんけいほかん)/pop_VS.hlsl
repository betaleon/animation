//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************

// �}�g���N�X�o�b�t�@
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

// �}�e���A���o�b�t�@
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


// ���C�g�o�b�t�@
struct LIGHT
{
	bool		Enable;
	bool3		Dummy;//16byte���E�p
	float4		Direction;
	float4		Diffuse;
	float4		Ambient;
};

struct Attributes
{
	float4 position : POSITION0;
	float3 normal : NORMAL0;
};


cbuffer LightBuffer : register(b4)
{
	LIGHT		Light;
}

//=============================================================================
// ���_�V�F�[�_
//=============================================================================
// Vertex Shader �� Geometry Shader �ɓn�����߂̍\����

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
