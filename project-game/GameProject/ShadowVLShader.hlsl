//�\���̒�`
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
	float4 lightPosH : TEXCOORD1;
	float depthWV : TEXCOORD2;
};

struct OM_INPUT
{
	float4 col : SV_Target0;
};

//�O���[�o���ϐ���`
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

texture g_shadowBuf;
float4x4 g_mtxLightWVP;
float4x4 g_mtxLightWV;
float g_far;

static const float SHADOW_EPSILON = 0.00094f;

//�e�N�X�`���T���v��
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

//���_�V�F�[�_�[
PS_INPUT vs(VS_INPUT input)
{
	PS_INPUT output;
	float4 PosWorld;
	PosWorld = mul(float4( input.pos, 1.0), g_mtxWorld);
	output.pos = mul( PosWorld, g_mtxView);
	output.pos = mul( output.pos, g_mtxProj);

	output.tex = input.tex;
	
	//�g�U���ˌ�
	float4 LocalLight = normalize( mul( g_DirLight, g_mtxWorldInv));
	LocalLight = -LocalLight;
	output.col = saturate( ( g_LightAmb * g_MaterialAmb) + ( g_LightDif * g_MaterialDif) * max( 0, dot( LocalLight.xyz, input.normal)));

	//�e
	output.lightPosH = mul( float4(input.pos, 1.0f), g_mtxLightWVP);
	output.depthWV = mul( float4(input.pos, 1.0f), g_mtxLightWV).z / g_far;

	return output;
}

//�s�N�Z���V�F�[�_�[
OM_INPUT ps(PS_INPUT input)
{
	//�J���[�̌v�Z
	OM_INPUT output;
	float4 color = tex2D(TextureSampler, input.tex) * input.col;
	output.col = color;

	//�e
	//�e�N�X�`�����W���Z�o
	input.lightPosH.xy /= input.lightPosH.w;
	input.lightPosH.x = input.lightPosH.x * +0.5f + 0.5f;
	input.lightPosH.y = input.lightPosH.y * -0.5f + 0.5f;

	//�[�x�o�b�t�@����[�x�𓾂�
	float lightDepthWV = tex2D(ShadowBufSampler, input.lightPosH.xy).r;

	//�J���[�̌���(�������邩���̂܂܂�)
	float s = lightDepthWV + SHADOW_EPSILON < input.depthWV ? 0.2f : 1.0f;
	//output.col = float4( color.rgb * s, color.a);
	output.col = float4( color.r * s, color.g * s, color.b * s, color.a);

	return output;
}

//�e�N�j�b�N
technique TShader
{
	pass P0
	{
		VertexShader = compile vs_3_0 vs();
		PixelShader = compile ps_3_0 ps();
	}

	pass P1		//������
	{
		VertexShader = compile vs_3_0 vs();
		PixelShader = compile ps_3_0 ps();

		/*AlphaBlendEnable = True;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;*/
	}

	pass P2		//���Z����
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

