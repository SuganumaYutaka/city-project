/*==============================================================================

    XModelRenderer.cpp - ���f���iX�t�@�C���j�̕`��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/7
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "XModelRenderer.h"
#include "Material.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "XModelManager.h"
#include "XModelData.h"

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* XModelRenderer::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<XModelRenderer>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
XModelRenderer::XModelRenderer( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerDefault;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_nPass = 0;

	m_pData = NULL;
	m_vecMaterial.clear();
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void XModelRenderer::Uninit( void)
{
	Manager::GetRenderManager()->ReleaseRenderer( this);

	//�}�e���A���̉��
	m_vecMaterial.clear();
}

/*------------------------------------------------------------------------------
	X�t�@�C���ǂݍ���
------------------------------------------------------------------------------*/
void XModelRenderer::LoadXModel( std::string FileName)
{
	//X�t�@�C����ǂݍ���
	m_pData = Manager::GetXModelManager()->Load( FileName);
	
	//�}�e���A�����R�s�[
	m_vecMaterial.clear();
	for (auto pMat : m_pData->m_vecMaterial)
	{
		m_vecMaterial.push_back( *pMat);

	}
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void XModelRenderer::Update( void)
{
	if (m_pData == NULL)
	{
		return;
	}

	//�����_�[�}�l�[�W���[�ɓo�^
	//Manager::GetRenderManager()->AddRenderer( this);
}

/*------------------------------------------------------------------------------
	�`��
------------------------------------------------------------------------------*/
void XModelRenderer::Draw( Camera* pCamera)
{
	if (m_pData == NULL)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾

	//���[���h���W�ϊ�
	D3DXMATRIX mtxWorld = m_pTransform->WorldMatrix();	//���[���h���W�ϊ��s��
	pDevice->SetTransform( D3DTS_WORLD, &mtxWorld);

	//���f���̕`��
	for( int nCnt = 0; nCnt < (int)m_pData->m_nNumMaterial; nCnt++)
	{
		auto Mat = m_vecMaterial[ nCnt];

		//�}�e���A���̐ݒ�
		Mat.Set( pCamera, this);

		//�e�N�j�b�N�J�n
		Mat.Begin( m_nPass);

		//���b�V���̕`��
		m_pData->m_pMeshModel->DrawSubset( nCnt);

		//�e�N�j�b�N�I��
		Mat.End();
	}
}

/*------------------------------------------------------------------------------
	�}�e���A���̎擾
------------------------------------------------------------------------------*/
std::vector<Material>& XModelRenderer::GetAllMaterial()
{
	return m_vecMaterial;
}

/*------------------------------------------------------------------------------
	���[�h
------------------------------------------------------------------------------*/
void XModelRenderer::Load(Text& text)
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
			
			LoadXModel( text.GetWord());
		}

		//text��ǂݐi�߂�
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}

}

/*------------------------------------------------------------------------------
	�Z�[�u
------------------------------------------------------------------------------*/
void XModelRenderer::Save(Text& text)
{
	StartSave(text);

	text += "FileName " + m_pData->FileName + '\n';

	EndSave( text);
}
