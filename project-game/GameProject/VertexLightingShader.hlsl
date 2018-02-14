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
	float3 normalW : TEXCOORD0;
	float2 tex : TEXCOORD1;
};

struct OM_INPUT
{
	float4 col : SV_Target0;
};

//�O���[�o���ϐ���`
float4x4 g_mtxWVP;
float4x4 g_mtxWIT;

float4 g_lightDirW;
float4 g_LightDif;
float4 g_LightAmb;
float4 g_MaterialDif;
float4 g_MaterialAmb;

texture g_texture;

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

//���_�V�F�[�_�[
PS_INPUT vs(VS_INPUT input)
{
	PS_INPUT output;
	
	output.posH = mul( float4(input.pos, 1.0f), g_mtxWVP);
	output.normalW = mul( float4(input.normal, 0.0f), g_mtxWIT);
	output.tex = input.tex;
	
	return output;
}

//�s�N�Z���V�F�[�_�[
OM_INPUT ps(PS_INPUT input)
{
	OM_INPUT output;

	float4 tex = tex2D(TextureSampler, input.tex);

	//�@���̐��K��
	input.normalW = normalize( input.normalW);

	//���s����
	//float diff = max( dot( input.normalW, -g_lightDirW), 0.0f) * 0.6f;

	//���s�����i�n�[�t�����o�[�g�j
	float diff = max( (dot( input.normalW, -g_lightDirW) + 1) * 0.5f, 0.0f) * 0.8f;

	//����
	float amb = 0.7f;

	//���C�g�ƃ}�e���A���̃J���[����Z
	float3 diffColor = diff * g_LightDif * g_MaterialDif;
	float3 ambColor = amb * g_LightAmb * g_MaterialAmb;
	
	//�J���[�̌���
	output.col = float4( tex.rgb * ( diffColor + ambColor) , tex.a);
	//output.col = float4( tex.rgb * ( diff + amb) , tex.a);

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

