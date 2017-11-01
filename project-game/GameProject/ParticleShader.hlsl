//構造体定義
struct VS_INPUT
{
	float3 pos : POSITION;
	float4 col : COLOR;
	float size : PSIZE;
	float3 dir : TEXCOORD0;
	float life : TEXCOORD1;
	float start : TEXCOORD2;
};

struct VS_OUTPUT
{
	float4 pos : POSITION;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
	float size : PSIZE;
};

struct PS_INPUT
{
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
};

struct OM_INPUT
{
	float4 col : SV_Target0;
};

//グローバル変数定義
float4x4 g_mtxWVP;
float g_time;
float g_gravity;

texture g_texture;

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

//頂点シェーダー
VS_OUTPUT vs(VS_INPUT input)
{
	VS_OUTPUT output;

	output.size = input.size;
	output.tex = 0.0;

	//時間の設定
	float time = g_time - input.start;	//スタート時間のずらし
	time = fmod( time, input.life);		//ループ

	//位置の計算
	float3 gravity = float3( 0.0, g_gravity, 0.0);	//重力
	float3 pos = input.pos + input.dir * time + 0.5 * gravity * time * time;

	//座標変換
	output.pos = mul( float4( pos, 1.0), g_mtxWVP);
	
	//だんだん透明に
	float alpha = input.col.a;
	alpha = alpha - ( alpha * time / input.life);
	output.col = input.col * alpha;

	return output;
}

//ピクセルシェーダー
OM_INPUT ps(PS_INPUT input)
{
	OM_INPUT output;
	output.col = tex2D(TextureSampler, input.tex) * input.col;
	return output;
}

//テクニック
technique TShader
{
	pass P0
	{
		VertexShader = compile vs_3_0 vs();
		PixelShader = compile ps_3_0 ps();

		PointSpriteEnable = TRUE;
		PointSize_MIN = 0.0;
		PointScale_A = 0.0;
		PointScale_B = 0.0;
		PointScale_C = 1.0;

	}

	pass P1		//半透明
	{
		VertexShader = compile vs_3_0 vs();
		PixelShader = compile ps_3_0 ps();

		PointSpriteEnable = TRUE;
		PointSize_MIN = 0.0;
		PointScale_A = 0.0;
		PointScale_B = 0.0;
		PointScale_C = 1.0;
		AlphaBlendEnable = True;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
	}

	pass P2		//加算合成
	{
		VertexShader = compile vs_3_0 vs();
		PixelShader = compile ps_3_0 ps();

		PointSpriteEnable = TRUE;
		PointSize_MIN = 0.0;
		PointScale_A = 0.0;
		PointScale_B = 0.0;
		PointScale_C = 1.0;
		AlphaBlendEnable = True;
		SrcBlend = ONE;
		DestBlend = ONE;
		ZWriteEnable = False;
	}
}


float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}

