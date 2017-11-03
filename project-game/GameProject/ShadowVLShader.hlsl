//構造体定義
struct VS_INPUT
{
	float3 pos : POSITION0;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
};

struct PS_INPUT
{
	float4 pos : SV_Position;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
	float4 posL : TEXCOORD1;
};

struct OM_INPUT
{
	float4 col : SV_Target0;
};

//グローバル変数定義
float4x4 g_mtxWorld;
float4x4 g_mtxWorldInv;
float4x4 g_mtxView;
float4x4 g_mtxProj;

float4 g_colAmb;
float4 g_colDif;
float4 g_DirLight;

texture g_texture;

texture g_shadowBuf;
float4x4 g_mtxLightVP;

//テクスチャサンプラ
sampler TextureSampler = 
sampler_state
{
	Texture = <g_texture>;

	AddressU = WRAP;
	AddressV = WRAP;
	
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	MipFilter = LINEAR;
	
};

sampler ShadowBufSampler = 
sampler_state
{
	Texture = <g_shadowBuf>;

	AddressU = CLAMP;
	AddressV = CLAMP;

	MinFilter = POINT;
	MagFilter = POINT;
	MipFilter = NONE;
	
};

//頂点シェーダー
PS_INPUT vs(VS_INPUT input)
{
	PS_INPUT output;
	float4 PosWorld;
	PosWorld = mul(float4( input.pos, 1.0), g_mtxWorld);
	output.pos = mul( PosWorld, g_mtxView);
	output.pos = mul( output.pos, g_mtxProj);

	output.tex = input.tex;
	
	//拡散反射光
	float4 LocalLight = normalize( mul( g_DirLight, g_mtxWorldInv));
	LocalLight = -LocalLight;
	output.col = saturate( g_colAmb + g_colDif * max( 0, dot( LocalLight.xyz, input.normal)));

	//影
	float4 PosL = mul( PosWorld, g_mtxLightVP);
	output.posL = PosL / PosL.w;

	return output;
}

//ピクセルシェーダー
OM_INPUT ps(PS_INPUT input)
{
	OM_INPUT output;
	float4 color = tex2D(TextureSampler, input.tex) * input.col;
	output.col = color;

	//影
	float bias = 0.0053f;		//バイアス値（調整が必要）
	float ZValue = input.posL.z / input.posL.w;	//Z値
	
	//XY座標をテクスチャ座標に変換
	float2 shadowTex;
	shadowTex.x = (1.0f + input.posL.x / input.posL.w) * 0.5f;
	shadowTex.y = (1.0f - input.posL.y / input.posL.w) * 0.5f;

	//テクスチャからZ値を抽出
	float shadowZ = tex2D(ShadowBufSampler, shadowTex).x;

	//実際のZ値がテクスチャから抽出したZ値より大きい場合影と判定
	if (ZValue - shadowZ > bias)
	{
		output.col = color * 0.5f;
	}

	return output;
}

//テクニック
technique TShader
{
	pass P0
	{
		VertexShader = compile vs_3_0 vs();
		PixelShader = compile ps_3_0 ps();
	}

	pass P1		//半透明
	{
		VertexShader = compile vs_3_0 vs();
		PixelShader = compile ps_3_0 ps();

		/*AlphaBlendEnable = True;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;*/
	}

	pass P2		//加算合成
	{
		VertexShader = compile vs_3_0 vs();
		PixelShader = compile ps_3_0 ps();

		AlphaBlendEnable = True;
		SrcBlend = SRCALPHA;
		DestBlend = ONE;
	}
}


float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}

