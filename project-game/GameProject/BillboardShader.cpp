/*==============================================================================

    BillboardShader.h - �r���{�[�h�V�F�[�_�[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/09
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "BillboardShader.h"
#include "Renderer.h"
#include "ComponentInclude.h"
#include "Material.h"
#include "Texture.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
BillboardShader::BillboardShader()
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();	//�f�o�C�X�̃|�C���^
	
	//���_�錾�i�[�C���^�[�t�F�C�X�쐬
	D3DVERTEXELEMENT9 g_Dec1[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },
		{ 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		D3DDECL_END()
	};
	pDevice->CreateVertexDeclaration( g_Dec1, &m_pVertexDec);

	//�V�F�[�_�̓ǂݍ���
	HRESULT hr;
	ID3DXBuffer *pError;		//�R���p�C���G���[���i�[�o�b�t�@
	hr = D3DXCreateEffectFromFile(pDevice, "data/SHADER/BillboardShader.cso", NULL, NULL, D3DXSHADER_SKIPVALIDATION, NULL, &m_pEffect, &pError);
	if( FAILED( hr))
	{
		MessageBox( NULL, "�V�F�[�_�[�̓ǂݍ��݂Ɏ��s���܂���\n", "�G���[", MB_OK);
		return;
	}

	//�n���h���̎擾
	m_hTech = m_pEffect->GetTechnique(0);
	m_hMtxPos = m_pEffect->GetParameterByName(0, "g_mtxPos");
	m_hMtxScale = m_pEffect->GetParameterByName(0, "g_mtxScale");
	m_hMtxView = m_pEffect->GetParameterByName(0, "g_mtxView");
	m_hMtxViewInverse = m_pEffect->GetParameterByName(0, "g_mtxViewInverse");
	m_hMtxProj = m_pEffect->GetParameterByName(0, "g_mtxProj");
	m_hTexture = m_pEffect->GetParameterByName( 0, "g_texture");
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
BillboardShader::~BillboardShader()
{
	Uninit();


}

/*------------------------------------------------------------------------------
	�V�F�[�_�[���Z�b�g
------------------------------------------------------------------------------*/
void BillboardShader::Set(Camera* pCamera, Renderer* pRenderer, Material* pMaterial)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();	//�f�o�C�X�̃|�C���^

	//�r���{�[�h����
	D3DXMATRIX mtxPos, mtxScale, mtxViewInverse;
	Vector3 Pos = pRenderer->m_pTransform->GetWorldPosition();
	Vector3 Scale = pRenderer->m_pTransform->GetWorldScale();
	D3DXMatrixTranslation( &mtxPos, Pos.x, Pos.y, Pos.z);
	D3DXMatrixScaling( &mtxScale, Scale.x, Scale.y, Scale.z);
	D3DXMatrixTranspose( &mtxViewInverse, pCamera->GetViewMatrix());		//�]�n
	mtxViewInverse._14 = 0.0f;
	mtxViewInverse._24 = 0.0f;
	mtxViewInverse._34 = 0.0f;

	//���_�錾
	pDevice->SetVertexDeclaration( m_pVertexDec);

	//�e�N�j�b�N�̐ݒ�
	m_pEffect->SetTechnique( m_hTech);

	//�萔���V�F�[�_�ɓ`����
	m_pEffect->SetMatrix( m_hMtxPos, &mtxPos);
	m_pEffect->SetMatrix( m_hMtxScale, &mtxScale);
	m_pEffect->SetMatrix( m_hMtxView, pCamera->GetViewMatrix());
	m_pEffect->SetMatrix( m_hMtxViewInverse, &mtxViewInverse);
	m_pEffect->SetMatrix( m_hMtxProj, pCamera->GetProjectionMatrix());
	m_pEffect->SetTexture( m_hTexture, pMaterial->GetTexture());
	m_pEffect->CommitChanges();
}


