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
	float4 posW : TEXCOORD1;
	float4 normal : TEXCOORD2;
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
float4 g_LightSpe;
float4 g_MaterialAmb;
float4 g_MaterialDif;
float4 g_MaterialSpe;

float4 g_PosLight;
float4 g_DirLight;
float4 g_PosEye;

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
	float4 PosWorld;
	PosWorld = mul(float4( input.pos, 1.0), g_mtxWorld);
	output.pos = mul( PosWorld, g_mtxView);
	output.pos = mul( output.pos, g_mtxProj);

	output.tex = input.tex;
	
	output.posW = PosWorld;

	output.normal = mul( float4( input.normal, 1.0f), g_mtxWorld);

	//�g�U���ˌ�
	float4 LocalLight = normalize( mul( g_DirLight, g_mtxWorldInv));
	LocalLight = -LocalLight;
	output.col = saturate( ( g_LightAmb * g_MaterialAmb) + ( g_LightDif * g_MaterialDif) * max( 0, dot( LocalLight, input.normal)));

	return output;
}

//�s�N�Z���V�F�[�_�[
OM_INPUT ps(PS_INPUT input)
{
	OM_INPUT output;
	float4 spe;		//���ʔ��ˌ�
	float3 DirHalf;	//�n�[�t�x�N�g��

	//���C�g����
	float3 DirLight;
	DirLight = normalize( g_PosLight.xyz - input.posW.xyz);
	//DirLight = g_DirLight.xyz;

	//�����̕���
	float3 DirEye;
	DirEye = normalize( g_PosEye.xyz - input.posW.xyz);

	//���ʔ��ˌ�
	DirHalf = normalize( DirLight + DirEye);
	spe = (g_LightSpe * g_MaterialSpe) * pow( max( 0, dot( DirHalf, input.normal.xyz)), 10);

	//�J���[�̐ݒ�
	output.col = saturate( input.col * tex2D(TextureSampler, input.tex) + spe);

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

