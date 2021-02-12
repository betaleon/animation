
Texture2D g_Texture : register(t0);
Texture2D g_TextureShadowDepth : register(t1);//----シャドウマップ
SamplerState g_SamplerState : register(s0);
SamplerState g_ShadowSamplerState : register(s1);

void main(//in PS_IN In, out float4 outDiffuse : SV_Target
	in  float4 inPosition		: SV_POSITION,
	in  float4 inShadowPosition	: POSITION1,
	in  float4 inNormal : NORMAL0,
	in  float2 inTexCoord : TEXCOORD0,
	in  float4 inDiffuse : COLOR0,
	in  float  depth	: DEPTH,

	out float4 outDiffuse : SV_Target0
) 
{
	//普通のテクスチャの色を出力
	outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord);
	outDiffuse *= inDiffuse;	//色=普通にテクスチャ*明るさの処理

	outDiffuse.rgb = lerp(float3(1, 1, 1), outDiffuse.rgb, 1 - depth);

	//In.ShadowPosition.xyzをIn.ShadowPosition.wで割る→結果はそのままIn.ShadowPosition.xyzへ
	//遠いものは小さくするための調整処理
	inShadowPosition.xyz /= inShadowPosition.w;	//座標値-1.0~1.0の値になる
	//変換したIn.ShadowPosition.xyをテクスチャ座標(0.0~1.0)へ変換する
	inShadowPosition.x = inShadowPosition.x *0.5f + 0.5f;
	inShadowPosition.y = -inShadowPosition.y *0.5f + 0.5f;//Yは上下を反転するので符号を-にする

	//シャドウバッファ(テクスチャ)からこのピクセルの見た目の光源からの距離を取得する
	depth = g_TextureShadowDepth.Sample(g_ShadowSamplerState, inShadowPosition.xy)-0.01;
	//float depth = g_TextureShadowDepth.Sample(g_SamplerState, inShadowPosition.xy);

	//変換したピクセルの距離(In.ShadowPosition.z)とシャドウバッファの見た目の距離(depth)を比較する
	//見た目の距離が計算上の距離()より小さければ、このピクセルは影の中にある=色(Diffuse,rgb)が暗くなる
	//<ここを作ろう>
	//if文で普通に作ればOK

			//fog
	//outDiffuse = g_Texture.Sample(g_SamplerState, inTexCoord);
	//outDiffuse *= inDiffuse;

	//outDiffuse.rgb = lerp(float3(1, 1, 1), outDiffuse.rgb, 1 - depth);
	//outDiffuse.rgb = lerp(float3(1, 1, 1), outDiffuse.rgb, 1 - depth);

	if (inShadowPosition.z -0.01f < depth || (inShadowPosition.x < 0 || inShadowPosition.x > 1)
		||(inShadowPosition.y < 0 || inShadowPosition.y > 1 ))
	{

	}
	else
	{
		outDiffuse.rgb *= 0.5f;
	}




	//ピクセルが影になっていたら色(Diffuse.rgb)を暗くする 0.5倍くらい？

	outDiffuse.a = 1.0f;	//お約束

}