//�\���̒�`
struct VS_INPUT
{
	float3 pos : POSITION0;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
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

//�O���[�o���ϐ���`
float4x4 g_mtxPos;
float4x4 g_mtxScale;
float4x4 g_mtxView;
float4x4 g_mtxViewInverse;
float4x4 g_mtxProj;

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

float4x4 Identity =
{
    { 1, 0, 0, 0 },
    { 0, 1, 0, 0 },
    { 0, 0, 1, 0 },
    { 0, 0, 0, 1 }
};

//���_�V�F�[�_�[
PS_INPUT vs(VS_INPUT input)
{
	float4x4 mtxWorld = Identity;
	mtxWorld = mul( mtxWorld, g_mtxViewInverse);
	mtxWorld = mul( mtxWorld, g_mtxScale);
	mtxWorld = mul( mtxWorld, g_mtxPos);

	PS_INPUT output;
	output.pos = mul(float4( input.pos, 1.0), mtxWorld);
	output.pos = mul( output.pos, g_mtxView);
	output.pos = mul( output.pos, g_mtxProj);
	output.col = input.col;
	output.tex = input.tex;
	return output;
}

//�s�N�Z���V�F�[�_�[
OM_INPUT ps(PS_INPUT input)
{
	OM_INPUT output;
	output.col = tex2D(TextureSampler, input.tex) * input.col;
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