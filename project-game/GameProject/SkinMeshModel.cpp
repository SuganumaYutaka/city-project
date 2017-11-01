/*==============================================================================

    SkinMeshModel.cpp - �X�L�����b�V�����f��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/10/09
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "SkinMeshModel.h"
#include "Material.h"
#include "GameObject.h"
#include "Transform.h"

#include "SkinMeshRenderer.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
SkinMeshModel::SkinMeshModel( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerDefault;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	vecAnimationClip.clear();
	m_CntFrame = 0;
	m_AnimationState = -1;
	m_bLoad = false;
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void SkinMeshModel::Uninit( void)
{
	
}

/*------------------------------------------------------------------------------
	�t�@�C���ǂݍ���
------------------------------------------------------------------------------*/
void SkinMeshModel::LoadModel( std::string FileName, float scaleRate)
{
	m_FileName = FileName;

	//�t�@�C���I�[�v��
	FILE* pFile;
	pFile = fopen( FileName.c_str(), "rb");
	if (!pFile)
	{
		MessageBox(NULL, "SkinMeshModel.cpp �t�@�C���I�[�v���Ɏ��s���܂���", "�G���[", MB_OK);
		return;
	}

	//���f���f�[�^��ǂݍ���
	//�A�j���[�V�����N���b�v
	int size;					//�i�[�T�C�Y
	char fileName[MAX_PATH];	//�t�@�C����

	fread(&size, sizeof(int), 1, pFile);
	vecAnimationClip.resize(size);
	//for (auto anim : m_vecAnimation)
	for( int i = 0; i < vecAnimationClip.size(); i++)
	{
		AnimationClip& anim = vecAnimationClip[i];

		fread(&size, sizeof(int), 1, pFile);
		fread(&fileName, sizeof(char), size, pFile);
		anim.Name = fileName;
		fread(&anim.AllFrame, sizeof(int), 1, pFile);
	}
	m_AnimationState = 0;

	//���b�V����
	int NumMesh = 0;
	fread(&NumMesh, sizeof(int), 1, pFile);

	//���b�V�����쐬
	GameObject* pObject = NULL;
	for( int nCnt = 0; nCnt < NumMesh; nCnt++)
	{
		pObject = new GameObject( m_pGameObject);
		auto mesh = pObject->AddComponent<SkinMeshRenderer>();

		mesh->LoadMeshData(pFile, this);
	}

	//�t�@�C���N���[�Y
	fclose( pFile);

	m_bLoad = true;

	//�X�P�[�����X�V
	if( scaleRate != 1.0f)
	{
		SetScale(scaleRate);
	}
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void SkinMeshModel::Update( void)
{
	if (!m_bLoad)
	{
		return;
	}

	m_CntFrame++;
}

/*------------------------------------------------------------------------------
	�X�P�[���̐ݒ�
------------------------------------------------------------------------------*/
void SkinMeshModel::SetScale( float rate)
{
	//���b�V���f�[�^���擾
	auto SkinMeshs = m_pGameObject->GetComponentListInChildren<SkinMeshRenderer>();

	//���b�V���f�[�^���X�V
	for (auto mesh : SkinMeshs)
	{
		mesh->SetScale(rate);
	}
}