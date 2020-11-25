//GeometryShader.hlsl
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

struct Attributes
{
	float4 position : POSITION0;
	float3 normal : NORMAL0;
};
// Vertex Shader �� Geometry Shader �ɓn�����߂̍\����
struct GSOutput
{
	float4 pos : SV_POSITION;
	float4 color : COLOR0;
};

static const float4 trianglePos[3] = {
  float4(0.0f, 0.5f, 0.0f, 0.0f),
  float4(0.5f, -0.5f, 0.0f, 0.0f),
  float4(-0.5f, -0.5f, 0.0f, 0.0f),
};

struct Varyings
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
};

// Geometry Shader�̏o�͗p�̍\���̂�p�ӂ��邽�߂̊֐�
Varyings VertexOutput(float3 wpos, half3 wnrm)
{
	matrix wvp;
	wvp = mul(World, View);
	wvp = mul(wvp, Projection);

	Varyings o;
	o.position = float4(wpos,1.0f);//mul(wpos,wvp);
	o.normal = wnrm;
	return o;
}

// 3�_����@���x�N�g�������߂邽�߂̊֐�
float3 ConstructNormal(float3 v1, float3 v2, float3 v3)
{
	return normalize(cross(v2 - v1, v3 - v1));	//cross�֐��i�O�ρj
}

float _LocalTime = 0.1f;
float PI = 3.14f;
//=============================================================================
// �W�I���g���V�F�[�_
//=============================================================================

//maxvertexcount�����F�ő�15�̒��_�𐶐����邱�Ƃ�錾���Ă���(�K���L�q)
[maxvertexcount(15)]
void main(triangle Attributes input[3] , uint pid : SV_PrimitiveID, inout TriangleStream<Varyings> outStream)
{
	// Vertex inputs
	float3 wp0 = input[0].position.xyz;
	float3 wp1 = input[1].position.xyz;
	float3 wp2 = input[2].position.xyz;

	// Extrusion amount
	float ext = saturate(0.4 - cos(_LocalTime * PI * 2) * 0.41);
	ext *= 1 + 0.3 * sin(pid * 832.37843 + _LocalTime * 88.76);

	// Extrusion points
	float3 offs = ConstructNormal(wp0, wp1, wp2) * ext;
	float3 wp3 = wp0 + offs;
	float3 wp4 = wp1 + offs;
	float3 wp5 = wp2 + offs;

	// Cap triangle
	float3 wn = ConstructNormal(wp3, wp4, wp5);
	float np = saturate(ext * 10);
	//�e���_�ł̖@�����Čv�Z
	float3 wn0 = lerp(input[0].normal, wn, np);
	float3 wn1 = lerp(input[1].normal, wn, np);
	float3 wn2 = lerp(input[2].normal, wn, np);
	outStream.Append(VertexOutput(wp3, wn0));
	outStream.Append(VertexOutput(wp4, wn1));
	outStream.Append(VertexOutput(wp5, wn2));
	outStream.RestartStrip();

	// Side faces
	wn = ConstructNormal(wp3, wp0, wp4);
	outStream.Append(VertexOutput(wp3, wn));
	outStream.Append(VertexOutput(wp0, wn));
	outStream.Append(VertexOutput(wp4, wn));
	outStream.Append(VertexOutput(wp1, wn));
	outStream.RestartStrip();

	wn = ConstructNormal(wp4, wp1, wp5);
	outStream.Append(VertexOutput(wp4, wn));
	outStream.Append(VertexOutput(wp1, wn));
	outStream.Append(VertexOutput(wp5, wn));
	outStream.Append(VertexOutput(wp2, wn));
	outStream.RestartStrip();

	wn = ConstructNormal(wp5, wp2, wp3);
	outStream.Append(VertexOutput(wp5, wn));
	outStream.Append(VertexOutput(wp2, wn));
	outStream.Append(VertexOutput(wp3, wn));
	outStream.Append(VertexOutput(wp0, wn));
	outStream.RestartStrip();
}
