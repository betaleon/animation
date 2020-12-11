
struct Varyings
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL0;
	float4 diffuse : COLOR0;
	float2 texCoord : TEXCOORD0;
};

//
// Pixel shader
//
float4 main(Varyings input) : SV_Target0
{
	float4 col = float4(1,0,0.5f,1);
	//col.rgb *= input.normal;
	return col;
}