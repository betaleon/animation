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

//ライトオブジェクト構造体とコンスタントバッファ
struct LIGHT
{
	bool Enable;
	bool3 Dummy;		//4の倍数にすると効率がいいので調整用、実はC言語でも同じだがVS2017がやってくれている
	float4 Direction;
	float4 Diffuse;
	float4 Ambient;

	matrix View;		//ライトの行列
	matrix Projection;	//同上
};

cbuffer LightBuffer : register(b4)	//コンスタントバッファ4番とする
{
	LIGHT Light;			//ライト構造体
}


void main(
	in  float4 inPosition		: POSITION0,
	in  float4 inNormal : NORMAL0,
	in  float4 inDiffuse : COLOR0,
	in  float2 inTexCoord : TEXCOORD0,

	out float4 outPosition : SV_POSITION,
	out float4 outShadowPosition : POSITON1,
	out float4 outNormal : NORMAL0,
	out float2 outTexCoord : TEXCOORD0,
	out float4 outDiffuse : COLOR0
)
{
	matrix wvp;
	wvp = mul(World, View);					//1path目はView=Light行列 2path目View=Camera行列
	wvp = mul(wvp, Projection);				//変換行列作成
	//--ここまで定石--

	float4 worldNormal, normal;				//法線をワールド行列で回転
	normal = float4(inNormal.xyz, 0.0f);
	worldNormal = mul(normal, World);
	worldNormal = normalize(worldNormal);	//変換した法線を正規化

	//--ここまでセット--
	//サンプルではピクセルシェーダー内を単純にするため
	//光源計算を頂点でやる
	float light = -dot(Light.Direction.xyz, worldNormal.xyz);
	light = saturate(light);	//明るさを0~1にする
	outDiffuse = light;
	outDiffuse.a = inDiffuse.a;

	outPosition = mul(inPosition, wvp);		//頂点をワールドビュープロジェクション変換して出力
	outNormal = worldNormal;				//ワールド変換した法線を出力
	outTexCoord = inTexCoord;				//テクスチャ座標を出力

	//--ここまで普通の頂点変換--
	//1path目　シャドウマップ作成
	//カメラ&プロジェクション行列がライトの物になっている。
	//ライトから見た状態をレンダリングすることになる(深度バッファ作成のみ)
	//このpathではShadowPositionへの出力は意味がない
	//2path目　本描画　影つけ
	//通常のカメラから見た状態をレンダリングする
	//このpathでのShadowPositionはライトから見た目の座標を表す情報として
	//ピクセルシェーダーで利用される
	matrix lightwvp;//-----VSではここが大事　ライト=カメラとした変換行列を作成
	lightwvp = mul(World, Light.View);//
	lightwvp = mul(lightwvp, Light.Projection);

	outShadowPosition = mul(inPosition, lightwvp);//--ライトから見た頂点の座標を出力

}