//構造体定義
struct VS_INPUT
{
	float3 pos : POSITION0;
};

struct PS_INPUT
{
	float4 pos : SV_Position;
	float depthWV : TEXCOORD0;
};

struct OM_INPUT
{
	float4 col : COLOR;
};

//グローバル変数定義
float4x4 g_mtxWorld;
float4x4 g_mtxView;
float4x4 g_mtxProj;
float g_far;

//頂点シェーダー
PS_INPUT vs(VS_INPUT input)
{
	PS_INPUT output;
	output.pos = mul(float4( input.pos, 1.0f), g_mtxWorld);
	output.pos = mul( output.pos, g_mtxView);
	output.pos = mul( output.pos, g_mtxProj);
	float4 depth = mul(float4( input.pos, 1.0f), g_mtxWorld);
	depth = mul( depth, g_mtxView);
	output.depthWV = depth.z / g_far;
	return output;
}

//ピクセルシェーダー
float4 ps(PS_INPUT input) : COLOR0
{
	return (float4)input.depthWV;
}

//テクニック
technique TShader
{
	pass P0
	{
		VertexShader = compile vs_3_0 vs();
		PixelShader = compile ps_3_0 ps();

		ZFunc = LESSEQUAL;
	}

	pass P1		//よび
	{
		VertexShader = compile vs_3_0 vs();
		PixelShader = compile ps_3_0 ps();

		ZFunc = LESSEQUAL;
	}

	pass P2		//よび
	{
		VertexShader = compile vs_3_0 vs();
		PixelShader = compile ps_3_0 ps();

		ZFunc = LESSEQUAL;
	}
}


float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}

