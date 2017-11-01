/*==============================================================================

    XModelData.cpp - X���f��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/7
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "XModelData.h"
#include "Material.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
	����
		const char* pFileName
------------------------------------------------------------------------------*/
XModelData::XModelData( std::string FileName)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾

	//���f���̓ǂݍ���
	LPD3DXBUFFER pBuffMaterial = NULL;		//�}�e���A�����i�[
	HRESULT hr;
	hr = D3DXLoadMeshFromX( 
		FileName.c_str(), 
		D3DXMESH_MANAGED,
		pDevice,
		NULL,
		&pBuffMaterial,
		NULL,
		&m_nNumMaterial,
		&m_pMeshModel);
	if (hr == E_FAIL)
	{
		MessageBox( NULL, "X���f���̓ǂݍ��݂Ɏ��s���܂���", "�G���[", MB_OK);
		return;
	}

	//�}�e���A�����ݒ�
	m_vecMaterial.resize( m_nNumMaterial);		//�v�f�����}�e���A�������ɕύX
	D3DXMATERIAL* pMat;							//�}�e���A�����̐擪�A�h���X
	pMat = (D3DXMATERIAL*)pBuffMaterial->GetBufferPointer();
	for( int nCnt = 0; nCnt < (int)m_nNumMaterial; nCnt++)
	{
		//�}�e���A���̐ݒ�
		m_vecMaterial[ nCnt] = new Material( &pMat[ nCnt]);
		m_vecMaterial[ nCnt]->SetShader( eShaderXModel);
	}
	
	//�}�e���A�����̉��
	if( pBuffMaterial != NULL)
	{
		pBuffMaterial->Release();
		pBuffMaterial = NULL;
	}
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
XModelData::~XModelData()
{
	if (m_pMeshModel != NULL)
	{
		m_pMeshModel->Release();
		m_pMeshModel = NULL;
	}

	for (auto mat : m_vecMaterial)
	{
		delete mat;
	}
}
