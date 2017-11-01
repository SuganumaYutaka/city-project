/*==============================================================================

    SkinMeshShader.cpp - �X�L�����b�V���V�F�[�_�[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/9/21
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "SkinMeshShader.h"
#include "Renderer.h"
#include "ComponentInclude.h"
#include "Material.h"
#include "Texture.h"
#include "Light.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define MAX_CLUSTER (58)		//�N���X�^�[�̍ő吔�i�V�F�[�_�[�ɓn�����ߌŒ蒷�j

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
SkinMeshShader::SkinMeshShader()
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();	//�f�o�C�X�̃|�C���^
	
	//���_�錾�i�[�C���^�[�t�F�C�X�쐬
	D3DVERTEXELEMENT9 g_Dec1[] =
	{
		{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
		{ 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 32, D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDWEIGHT,  0},
		{ 0, 48, D3DDECLTYPE_UBYTE4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_BLENDINDICES, 0},
		D3DDECL_END()
	};
	pDevice->CreateVertexDeclaration( g_Dec1, &m_pVertexDec);

	//�V�F�[�_�̓ǂݍ���
	HRESULT hr;
	ID3DXBuffer *pError;		//�R���p�C���G���[���i�[�o�b�t�@
	hr = D3DXCreateEffectFromFile(pDevice, "data/SHADER/SkinMeshShader.cso", NULL, NULL, D3DXSHADER_SKIPVALIDATION, NULL, &m_pEffect, &pError);
	if( FAILED( hr))
	{
		MessageBox( NULL, "�V�F�[�_�[�̓ǂݍ��݂Ɏ��s���܂���\n", "�G���[", MB_OK);
		return;
	}

	//�n���h���̎擾
	m_hTech = m_pEffect->GetTechnique(0);
	m_hMtxWorld = m_pEffect->GetParameterByName(0, "g_mtxWorld");
	m_hMtxWorldInv = m_pEffect->GetParameterByName(0, "g_mtxWorldInv");
	m_hMtxView = m_pEffect->GetParameterByName(0, "g_mtxView");
	m_hMtxProj = m_pEffect->GetParameterByName(0, "g_mtxProj");
	m_hTexture = m_pEffect->GetParameterByName( 0, "g_texture");
	m_hColAmb = m_pEffect->GetParameterByName( 0, "g_colAmb");
	m_hColDif = m_pEffect->GetParameterByName( 0, "g_colDif");
	m_hDirLight = m_pEffect->GetParameterByName( 0, "g_DirLight");
	m_hClusters = m_pEffect->GetParameterByName( 0, "g_mtxClusters");
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
SkinMeshShader::~SkinMeshShader()
{
	Uninit();


}

/*------------------------------------------------------------------------------
	�V�F�[�_�[���Z�b�g
------------------------------------------------------------------------------*/
void SkinMeshShader::Set(Camera* pCamera, Renderer* pRenderer, Material* pMaterial)
{
	D3DXMATRIX mtxWorldInv = pRenderer->m_pTransform->WorldMatrix();
	D3DXMatrixInverse( &mtxWorldInv, NULL, &mtxWorldInv);

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();	//�f�o�C�X�̃|�C���^
	
	//���_�錾
	pDevice->SetVertexDeclaration( m_pVertexDec);

	//�e�N�j�b�N�̐ݒ�
	m_pEffect->SetTechnique( m_hTech);

	//�萔���V�F�[�_�ɓ`����
	m_pEffect->SetMatrix( m_hMtxWorld, &pRenderer->m_pTransform->WorldMatrix());
	m_pEffect->SetMatrix( m_hMtxWorldInv, &mtxWorldInv);
	m_pEffect->SetMatrix( m_hMtxView, pCamera->GetViewMatrix());
	m_pEffect->SetMatrix( m_hMtxProj, pCamera->GetProjectionMatrix());
	m_pEffect->SetTexture( m_hTexture, pMaterial->GetTexture());
	m_pEffect->SetVector( m_hColAmb, pMaterial->GetAmbient());
	m_pEffect->SetVector( m_hColDif, pMaterial->GetDiffuse());
	
	//���C�g�̎擾
	auto list = Light::GetList();
	for (auto light : list)
	{
		m_pEffect->SetVector( m_hDirLight, light->GetDirection());
	}

	//�N���X�^�[���𑗂�
	SkinMeshRenderer* pMesh = pRenderer->m_pGameObject->GetComponent<SkinMeshRenderer>();
	if (pMesh == NULL)
	{
		MessageBox( NULL, "����FbxMeshRenderer�݂̂�SkinMeshShader�𗘗p�ł��܂�", "�G���[", MB_OK);
		return;
	}

	//�A�j���[�V������񂪂���Ƃ�
	int numAnim = pMesh->GetAnimation().size();
	if ( numAnim > 0)
	{
		int state = pMesh->GetSkinMeshModel()->GetAnimationState();	//���݂̃A�j���[�V����
		auto anim = pMesh->GetAnimation( state);
	
		//���݂̃A�j���[�V�����̌��݂̃t���[���̎p���s���n��
		HRESULT hr = m_pEffect->SetMatrixArray( m_hClusters, 
			anim.vecMatrix[pMesh->GetSkinMeshModel()->GetFrame() % anim.AllFrame].data(), 58);
		if (hr == D3DERR_INVALIDCALL)
		{
			MessageBox( NULL, "�N���X�^�[�f�[�^�𑗂�܂���", "�G���[", MB_OK);
		}
	}
	else
	{
		MessageBox( NULL, "�A�j���[�V������񂪂���܂���", "�G���[", MB_OK);
	}

	m_pEffect->CommitChanges();
}


