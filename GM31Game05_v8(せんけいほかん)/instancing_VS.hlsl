

//*****************************************************************************
// �萔�o�b�t�@
//*****************************************************************************

// �}�g���N�X�o�b�t�@
cbuffer WorldBuffer : register( b0 )
{
	matrix World;
}
cbuffer ViewBuffer : register( b1 )
{
	matrix View;
}
cbuffer ProjectionBuffer : register( b2 )
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
	float3		Dummy;//16bit���E�p
};

cbuffer MaterialBuffer : register( b3 )
{
	MATERIAL	Material;
}


// ���C�g�o�b�t�@
struct LIGHT
{
	bool		Enable;
	bool3		Dummy;//16bit���E�p
	float4		Direction;
	float4		Diffuse;
	float4		Ambient;
};

cbuffer LightBuffer : register( b4 )
{
	LIGHT		Light;
}

StructuredBuffer<float3> Position : register(t2);

//=============================================================================
// ���_�V�F�[�_
//=============================================================================
void main( in  float4 inPosition		: POSITION0,
						  in  float4 inNormal		: NORMAL0,
						  in  float4 inDiffuse		: COLOR0,
						  in  float2 inTexCoord		: TEXCOORD0,
						  in  uint	 inInstanceId	: SV_InstanceID,

						  out float4 outPosition	: SV_POSITION,
						  out float4 outNormal		: NORMAL0,
						  out float2 outTexCoord	: TEXCOORD0,
						  out float4 outDiffuse		: COLOR0 )
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	inPosition.xyz += Position[inInstanceId];

	outPosition = mul( inPosition, wvp);
	outNormal = inNormal;
	outTexCoord = inTexCoord;
	
	float4 worldNormal, normal;
	normal = float4(inNormal.xyz, 0.0);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);

	if (Light.Enable)
	{
		float light = 0.5 - 0.5 * dot(Light.Direction.xyz, worldNormal.xyz);

		outDiffuse = inDiffuse * Material.Diffuse * light * Light.Diffuse;
		outDiffuse += inDiffuse * Material.Ambient * Light.Ambient;
		outDiffuse += Material.Emission;
	}
	else
	{
		outDiffuse = inDiffuse * Material.Diffuse;
	}

	outDiffuse.a = inDiffuse.a * Material.Diffuse.a;
}

//VertexShaderOutput VertexShaderFunction(VertexShaderInput input)
//{
//	VertexShaderOutput output = (VertexShaderOutput)0;
//
//	float4 worldPosition = mul(input.Position, World);
//	float4 viewPosition = mul(worldPosition, View);
//	output.Position = mul(viewPosition, Projection);
//
//	// TODO: add your vertex shader code here.
//	output.Texcoord = input.Texcoord;
//	output.Normal = input.Normal;
//	output.ObjPosition = input.Position;
//
//	//�@�t�H�O�W���Z�o
//	output.Fog = FogCoord.x + output.Position.w * FogCoord.y;
//
//	return output;
//}