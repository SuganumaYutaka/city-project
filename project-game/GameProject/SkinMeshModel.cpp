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
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* SkinMeshModel::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<SkinMeshModel>();
}

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
	ScaleRate = 1.0f;
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
	//�t�@�C���I�[�v��
	FILE* pFile;
	pFile = fopen( FileName.c_str(), "rb");
	if (!pFile)
	{
		MessageBox(NULL, "SkinMeshModel.cpp �t�@�C���I�[�v���Ɏ��s���܂���", "�G���[", MB_OK);
		return;
	}
	m_FileName = FileName;

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
		pObject->IsCreatedByOtherComponent = true;
		auto mesh = pObject->AddComponent<SkinMeshRenderer>();
		mesh->IsCreatedByOtherComponent = true;
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
	ScaleRate = scaleRate;
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

/*------------------------------------------------------------------------------
	���[�h
------------------------------------------------------------------------------*/
void SkinMeshModel::Load(Text& text)
{
	//text��ǂݐi�߂�
	if (text.ForwardPositionToNextWord() == Text::EoF)
	{
		return;
	}

	while ( text.GetWord() != "EndComponent")
	{
		if (text.GetWord() == "FileName")
		{
			text.ForwardPositionToNextWord();
			m_FileName = text.GetWord();
		}
		else if (text.GetWord() == "ScaleRate")
		{
			text.ForwardPositionToNextWord();
			ScaleRate = std::stof(text.GetWord());
		}

		//text��ǂݐi�߂�
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}
	LoadModel( m_FileName, ScaleRate);
}

/*------------------------------------------------------------------------------
	�Z�[�u
------------------------------------------------------------------------------*/
void SkinMeshModel::Save(Text& text)
{
	if (m_bLoad == false)
	{
		return;
	}

	StartSave(text);

	text += "FileName " + m_FileName + '\n';
	text += "ScaleRate " + std::to_string(ScaleRate) + '\n';

	EndSave( text);
}