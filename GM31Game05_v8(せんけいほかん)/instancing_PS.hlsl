

//*****************************************************************************
// 定数バッファ
//*****************************************************************************

// マテリアルバッファ
cbuffer MaterialBuffer : register( b1 )
{
	float4		Ambient;
	float4		Diffuse;
	float4		Specular;
	float4		Emission;
	float		Shininess;
	float3		Dummy;//16bit境界用
}





//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		g_Texture : register( t0 );
SamplerState	g_SamplerState : register( s0 );


//=============================================================================
// ピクセルシェーダ
//=============================================================================
void main(	in  float4 inPosition		: POSITION0,
						 in  float4 inNormal		: NORMAL0,
						 in  float2 inTexCoord		: TEXCOORD0,
						 in  float4 inDiffuse		: COLOR0,
						 in  float  depth			: DEPTH, 
						 out float4 outDiffuse		: SV_Target )
{

    outDiffuse = g_Texture.Sample( g_SamplerState, inTexCoord );

	outDiffuse *= inDiffuse;

	outDiffuse.rgb = lerp(float3(1, 1, 1), outDiffuse.rgb, 1 - depth);

}

//float4 PixelShaderFunction(VertexShaderOutput input) : COLOR0
//{
//	// TODO: add your pixel shader code here.
//	float3 N = normalize(input.Normal);
//	float3 L = normalize(LightPosition - input.ObjPosition.xyz);
//	float3 E = normalize(CameraPosition - input.ObjPosition.xyz);
//	float3 H = normalize(L + E);
//	
//	float4 output = HalfLambert(N, L, H);
//	output *= tex2D(DecalSmp, input.Texcoord);
//	
//	//　フォグ色とオブジェクト色と線形合成
//	output = lerp(FogColor, output, input.Fog);
//	
//	return output;
//	
//}