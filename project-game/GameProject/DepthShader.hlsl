//�\���̒�`
struct VS_INPUT
{
	float3 pos : POSITION0;
};

struct PS_INPUT
{
	float4 pos : SV_Position;
	float4 depth : TEXCOORD0;
};

struct OM_INPUT
{
	float4 col : COLOR;
};

//�O���[�o���ϐ���`
float4x4 g_mtxWorld;
float4x4 g_mtxView;
float4x4 g_mtxProj;

//���_�V�F�[�_�[
PS_INPUT vs(VS_INPUT input)
{
	PS_INPUT output;
	output.pos = mul(float4( input.pos, 1.0f), g_mtxWorld);
	output.pos = mul( output.pos, g_mtxView);
	output.pos = mul( output.pos, g_mtxProj);
	output.depth = output.pos;
	return output;
}

//�s�N�Z���V�F�[�_�[
OM_INPUT ps(PS_INPUT input)
{
	OM_INPUT output;
	output.col = input.depth.z / input.depth.w;
	return output;
}

//�e�N�j�b�N
technique TShader
{
	pass P0
	{
		VertexShader = compile vs_3_0 vs();
		PixelShader = compile ps_3_0 ps();

		ZFunc = LESSEQUAL;
	}

	pass P1		//���
	{
		VertexShader = compile vs_3_0 vs();
		PixelShader = compile ps_3_0 ps();

		ZFunc = LESSEQUAL;
	}

	pass P2		//���
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

