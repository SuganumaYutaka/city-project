//構造体定義
struct VS_INPUT
{
	float3 pos : POSITION0;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
	float2 tex : TEXCOORD0;
};

struct PS_INPUT
{
	float4 posH : POSITION0;
	float4 color : COLOR0;
	float3 normalW : TEXCOORD0;
	float3 posW : TEXCOORD1;
	float2 tex : TEXCOORD2;
	float4 lightPosH : TEXCOORD3;
	float depthWV : TEXCOORD4;
};

struct OM_INPUT
{
	float4 col : SV_Target0;
};

//グローバル変数定義
float4x4 g_mtxWorld;
float4x4 g_mtxWVP;
float4x4 g_mtxWIT;

float4 g_lightDirW;
float4 g_posEyeW;
float g_specularPower;
float4 g_LightDif;
float4 g_LightAmb;
float4 g_LightSpe;
float4 g_MaterialDif;
float4 g_MaterialAmb;
float4 g_MaterialSpe;

texture g_texture;

texture g_shadowBuf;
float4x4 g_mtxLightWVP;
float4x4 g_mtxLightWV;
float g_far;

static const float SHADOW_EPSILON = 0.00094f;

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
	output.posH = mul( float4(input.pos, 1.0f), g_mtxWVP);
	output.posW = mul( float4(input.pos, 1.0f), g_mtxWorld);
	output.normalW = mul( float4(input.normal, 0.0f), g_mtxWIT);
	output.color = input.color;
	output.tex = input.tex;

	//影
	output.lightPosH = mul( float4(input.pos, 1.0f), g_mtxLightWVP);
	output.depthWV = mul( float4(input.pos, 1.0f), g_mtxLightWV).z / g_far;

	return output;
}

//ピクセルシェーダー
OM_INPUT ps(PS_INPUT input)
{
	//カラーの計算
	OM_INPUT output;
	float4 tex = tex2D(TextureSampler, input.tex);
	
	//ライティング
	//法線の正規化
	input.normalW = normalize( input.normalW);

	//カメラのベクトル算出
	float3 toEyeW = normalize( g_posEyeW - input.posW);

	//反射ベクトル算出
	float3 reflectDirW = normalize( reflect( g_lightDirW, input.normalW));

	//スペキュラの強さを求める
	float spec = pow( max( dot( toEyeW, reflectDirW), 0.0f), 5);
	
	//平行光源
	//float diff = max( dot( input.normalW, -g_lightDirW), 0.0f) * 0.6f;

	//平行光源（ハーフランバート）
	float diff = max( (dot( input.normalW, -g_lightDirW) + 1) * 0.5f, 0.0f) * 0.6f;

	//ライトとマテリアルのカラーを乗算
	float3 diffColor = diff * g_LightDif * g_MaterialDif;
	float3 ambColor = 0.5f * g_LightAmb * g_MaterialAmb;
	float3 speColor = spec * g_LightSpe * g_MaterialSpe;

	//リムライト( 0.0f ～ 0.2f)
	float lim = max( 1.0f - abs( dot( toEyeW, input.normalW)) * 1.2f, -0.2f);
	float3 limColor = lim * g_LightSpe * g_MaterialSpe;

	//カラーの決定
	//float4 color = float4( tex.rgb * ( diffColor + ambColor + speColor) , tex.a);
	float4 color = float4( tex.xyz * ( diffColor + ambColor + speColor + limColor) , tex.a);

	//影
	//テクスチャ座標を算出
	input.lightPosH.xy /= input.lightPosH.w;
	input.lightPosH.x = input.lightPosH.x * +0.5f + 0.5f;
	input.lightPosH.y = input.lightPosH.y * -0.5f + 0.5f;

	//深度バッファから深度を得る
	float lightDepthWV = tex2D(ShadowBufSampler, input.lightPosH.xy).r;

	//カラーの決定(黒くするかそのままか)
	float s = lightDepthWV + SHADOW_EPSILON < input.depthWV ? 0.6f : 1.0f;
	//output.col = float4( color.rgb * s, color.a);
	output.col = float4( color.r * s, color.g * s, color.b * s, color.a);

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

