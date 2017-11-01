//�\���̒�`
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

//�O���[�o���ϐ���`
float4x4 g_mtxWorld;
float4x4 g_mtxWorldInv;
float4x4 g_mtxView;
float4x4 g_mtxProj;

float4 g_colAmb;
float4 g_colDif;
float4 g_DirLight;

texture g_texture;

float4x4 g_mtxClusters[58];

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

	//�N���X�^�[�̉e�����u�����h
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
	
	//�g�U���ˌ�
	float4 LocalLight = normalize( mul( g_DirLight, g_mtxWorldInv));
	LocalLight = -LocalLight;
	float4 normal = mul(float4(input.normal, 0.0f), rev * comb * rev);
	output.col = saturate( g_colAmb + g_colDif * max( 0, dot( LocalLight, normal)));

	//output.col = g_colAmb + g_colDif;

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