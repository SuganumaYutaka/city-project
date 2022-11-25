//構造体定義
struct VS_INPUT
{
	float3 pos : POSITION0;
	float3 normal : NORMAL;
	float2 tex : TEXCOORD0;
	float4 weight : BLENDWEIGHT;
	float4 bornIndex : BLENDINDICES;
};

struct PS_INPUT
{
	float4 pos : SV_Position;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
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

float4 g_LightAmb;
float4 g_LightDif;
float4 g_MaterialAmb;
float4 g_MaterialDif;
float4 g_DirLight;

texture g_texture;

float4x4 g_mtxClusters[58];

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
PS_INPUT vs(VS_INPUT input)
{
	PS_INPUT output;

	//クラスターの影響をブレンド
	float4x4 comb = 
	{
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};

	float4x4 rev = 
	{
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, -1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};
	
	int index[4] = (int[4])input.bornIndex;
	comb =			g_mtxClusters[ index[0]] * input.weight[0];
	comb = comb + ( g_mtxClusters[ index[1]] * input.weight[1]);
	comb = comb + ( g_mtxClusters[ index[2]] * input.weight[2]);
	comb = comb + ( g_mtxClusters[ index[3]] * input.weight[3]);

	output.pos = mul( float4( input.pos, 1.0), rev);
	output.pos = mul( output.pos, comb);
	output.pos = mul( output.pos, rev);
	
	output.pos = mul( output.pos, g_mtxWorld);
	output.pos = mul( output.pos, g_mtxView);
	output.pos = mul( output.pos, g_mtxProj);

	output.tex = input.tex;
	
	//拡散反射光
	float4 LocalLight = normalize( mul( g_DirLight, g_mtxWorldInv));
	LocalLight = -LocalLight;
	float4 normal = mul(float4(input.normal, 0.0f), rev * comb * rev);
	output.col = saturate( ( g_LightAmb * g_MaterialAmb) + ( g_LightDif * g_MaterialDif) * max( 0, dot( LocalLight, normal)));

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
	}

	pass P1		//半透明
	{
		VertexShader = compile vs_3_0 vs();
		PixelShader = compile ps_3_0 ps();

		AlphaBlendEnable = True;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
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