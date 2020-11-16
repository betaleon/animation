

//*****************************************************************************
// 定数バッファ
//*****************************************************************************



//*****************************************************************************
// グローバル変数
//*****************************************************************************
Texture2D		g_Texture	   : register( t0 );
SamplerState	g_SamplerState : register( s0 );


//=============================================================================
// ピクセルシェーダ
//=============================================================================
float4 main(	in  float2 inTexCoord		: TEXCOORD0,
			in  float4 inDiffuse        : COLOR0,
			in  float4 inPosition		: POSITION0,
			in  float4 inNormal		    : NORMAL0,
			in  float  depth			: DEPTH)	: SV_Target0
{
	float4 outDiffuse;

	outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord);
	outDiffuse *= inDiffuse;

	outDiffuse.rgb = lerp(float3(1,1,1), outDiffuse.rgb, 1 - depth);

	return outDiffuse;
}
