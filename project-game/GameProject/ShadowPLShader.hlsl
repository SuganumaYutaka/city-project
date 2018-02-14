//�\���̒�`
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

//�O���[�o���ϐ���`
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
	output.posH = mul( float4(input.pos, 1.0f), g_mtxWVP);
	output.posW = mul( float4(input.pos, 1.0f), g_mtxWorld);
	output.normalW = mul( float4(input.normal, 0.0f), g_mtxWIT);
	output.color = input.color;
	output.tex = input.tex;

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
	float4 tex = tex2D(TextureSampler, input.tex);
	
	//���C�e�B���O
	//�@���̐��K��
	input.normalW = normalize( input.normalW);

	//�J�����̃x�N�g���Z�o
	float3 toEyeW = normalize( g_posEyeW - input.posW);

	//���˃x�N�g���Z�o
	float3 reflectDirW = normalize( reflect( g_lightDirW, input.normalW));

	//�X�y�L�����̋��������߂�
	float spec = pow( max( dot( toEyeW, reflectDirW), 0.0f), 5);
	
	//���s����
	//float diff = max( dot( input.normalW, -g_lightDirW), 0.0f) * 0.6f;

	//���s�����i�n�[�t�����o�[�g�j
	float diff = max( (dot( input.normalW, -g_lightDirW) + 1) * 0.5f, 0.0f) * 0.6f;

	//���C�g�ƃ}�e���A���̃J���[����Z
	float3 diffColor = diff * g_LightDif * g_MaterialDif;
	float3 ambColor = 0.5f * g_LightAmb * g_MaterialAmb;
	float3 speColor = spec * g_LightSpe * g_MaterialSpe;

	//�������C�g( 0.0f �` 0.2f)
	float lim = max( 1.0f - abs( dot( toEyeW, input.normalW)) * 1.2f, -0.2f);
	float3 limColor = lim * g_LightSpe * g_MaterialSpe;

	//�J���[�̌���
	//float4 color = float4( tex.rgb * ( diffColor + ambColor + speColor) , tex.a);
	float4 color = float4( tex.xyz * ( diffColor + ambColor + speColor + limColor) , tex.a);

	//�e
	//�e�N�X�`�����W���Z�o
	input.lightPosH.xy /= input.lightPosH.w;
	input.lightPosH.x = input.lightPosH.x * +0.5f + 0.5f;
	input.lightPosH.y = input.lightPosH.y * -0.5f + 0.5f;

	//�[�x�o�b�t�@����[�x�𓾂�
	float lightDepthWV = tex2D(ShadowBufSampler, input.lightPosH.xy).r;

	//�J���[�̌���(�������邩���̂܂܂�)
	float s = lightDepthWV + SHADOW_EPSILON < input.depthWV ? 0.6f : 1.0f;
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

		AlphaBlendEnable = True;
		SrcBlend = SRCALPHA;
		DestBlend = INVSRCALPHA;
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

