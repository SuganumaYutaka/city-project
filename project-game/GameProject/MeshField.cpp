/*==============================================================================

   MeshField.h - ���b�V���t�B�[���h
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/31
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "GameObject.h"
#include "MeshField.h"
#include "ComponentInclude.h"
#include "TextureManager.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define MESH_FIELD_NUM_VERTEX_X (4)		//�t�B�[���h�̕�����
#define MESH_FIELD_NUM_VERTEX_Z (4)		//�t�B�[���h�̕�����
#define P_WIDTH( ALL_WIDTH, NUM_FIELD_X)			( ALL_WIDTH / NUM_FIELD_X)				//�|���S���ꖇ������̕�
#define P_HEIGHT( ALL_HEIGHT, NUM_FIELD_Z)			( ALL_HEIGHT / NUM_FIELD_Z)				//�|���S���ꖇ������̍���

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* MeshField::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<MeshField>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
MeshField::MeshField( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerBack;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	//�����_���[�ǉ�
	m_pMeshFieldRenderer = pGameObject->AddComponent<MeshFieldRenderer>();
	m_pMeshFieldRenderer->IsCreatedByOtherComponent = true;
	
	//�R���C�_�[�ǉ�
	m_pMeshFieldCollider = pGameObject->AddComponent<MeshFieldCollider>();
	m_pMeshFieldCollider->IsCreatedByOtherComponent = true;

	//���b�V���t�B�[���h�̐ݒ�
	m_nNumBlockX = MESH_FIELD_NUM_VERTEX_X;
	m_nNumBlockZ = MESH_FIELD_NUM_VERTEX_Z;
	m_fWidth = 15.0f;
	m_fHeight = 15.0f;
	m_fBlockWidth = P_WIDTH( m_fWidth, m_nNumBlockX);		//�|���S��1��������̕�
	m_fBlockHeight = P_HEIGHT( m_fHeight, m_nNumBlockZ);	//�|���S��1��������̍���

	m_pVertexHeight = new float[ ( m_nNumBlockZ + 1) * ( m_nNumBlockX + 1)];
	for (int nCnt = 0; nCnt < ( m_nNumBlockZ + 1) * ( m_nNumBlockX + 1); nCnt++)
	{
		m_pVertexHeight[ nCnt] = 0.0f;
	}

	//���̃R���|�[�l���g�ɐݒ����`����
	m_pMeshFieldCollider->SetField( m_nNumBlockX, m_nNumBlockZ, m_fBlockWidth, m_fBlockHeight, m_pVertexHeight);
	m_pMeshFieldRenderer->SetField( m_nNumBlockX, m_nNumBlockZ, m_fBlockWidth, m_fBlockHeight, m_pVertexHeight);
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void MeshField::Uninit( void)
{
	//���_���Ƃ̍������
	delete[] m_pVertexHeight;
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void MeshField::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	�`��
------------------------------------------------------------------------------*/
void MeshField::Draw( void)
{
	
}

/*------------------------------------------------------------------------------
	�t�B�[���h��ݒ�
------------------------------------------------------------------------------*/
void MeshField::SetField( int X, int Z, float BlockWidth, float BlockHeight, float *pVertexHeight)
{
	m_nNumBlockX = X;
	m_nNumBlockZ = Z;
	m_fBlockWidth = BlockWidth;
	m_fBlockHeight = BlockHeight;
	m_fWidth = m_nNumBlockX * m_fBlockWidth;
	m_fHeight = m_nNumBlockZ * m_fBlockHeight;

	m_pMeshFieldRenderer->SetField( X, Z, BlockWidth, BlockHeight, pVertexHeight);
	m_pMeshFieldCollider->SetField( X, Z, BlockWidth, BlockHeight, pVertexHeight);
}

/*------------------------------------------------------------------------------
	�|���S���ꖇ������̕��E������ݒ�
------------------------------------------------------------------------------*/
void MeshField::SetBlockSize( float Width, float Height)
{
	m_fBlockWidth = Width;
	m_fBlockHeight = Height;
	m_fWidth = m_nNumBlockX * m_fBlockWidth;
	m_fHeight = m_nNumBlockZ * m_fBlockHeight;

	m_pMeshFieldRenderer->SetBlockSize( Width, Height);
	m_pMeshFieldCollider->SetBlockSize( Width, Height);
}

/*------------------------------------------------------------------------------
	���E������ݒ�
------------------------------------------------------------------------------*/
void MeshField::SetSize( float Width, float Height)
{
	m_fWidth = Width;
	m_fHeight = Height;
	m_fBlockWidth = P_WIDTH(m_fWidth, m_nNumBlockX);
	m_fBlockHeight = P_WIDTH(m_fHeight, m_nNumBlockZ);

	m_pMeshFieldRenderer->SetSize( Width, Height);
	m_pMeshFieldCollider->SetSize( Width, Height);
}

/*------------------------------------------------------------------------------
	�e�N�X�`���ݒ�
------------------------------------------------------------------------------*/
void MeshField::LoadTexture(std::string fileName)
{
	m_pMeshFieldRenderer->LoadTexture( fileName);
}

/*------------------------------------------------------------------------------
	�e�N�X�`�����擾
------------------------------------------------------------------------------*/
std::string MeshField::GetTextureName()
{
	return m_pMeshFieldRenderer->GetTextureName();
}

/*------------------------------------------------------------------------------
	���[�h
------------------------------------------------------------------------------*/
void MeshField::Load(Text& text)
{
	//text��ǂݐi�߂�
	if (text.ForwardPositionToNextWord() == Text::EoF)
	{
		return;
	}

	while ( text.GetWord() != "EndComponent")
	{
		if (text.GetWord() == "NumBlockX")
		{
			text.ForwardPositionToNextWord();
			m_nNumBlockX = std::stoi(text.GetWord());
		}
		else if (text.GetWord() == "NumBlockZ")
		{
			text.ForwardPositionToNextWord();
			m_nNumBlockZ = std::stoi(text.GetWord());
		}
		else if (text.GetWord() == "Width")
		{
			text.ForwardPositionToNextWord();
			m_fWidth = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "Height")
		{
			text.ForwardPositionToNextWord();
			m_fHeight = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "BlockWidth")
		{
			text.ForwardPositionToNextWord();
			m_fBlockWidth = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "BlockHeight")
		{
			text.ForwardPositionToNextWord();
			m_fBlockHeight = std::stof(text.GetWord());
		}

		else if (text.GetWord() == "VertexHeight")
		{
			delete[] m_pVertexHeight;
			m_pVertexHeight = NULL;
			m_pVertexHeight = new float[( m_nNumBlockZ + 1) * ( m_nNumBlockX + 1)];
			for (int nCnt = 0; nCnt < ( m_nNumBlockZ + 1) * ( m_nNumBlockX + 1); nCnt++)
			{
				text.ForwardPositionToNextWord();
				m_pVertexHeight[ nCnt] = std::stof(text.GetWord());
			}
		}

		else if (text.GetWord() == "TextureFileName")
		{
			text.ForwardPositionToNextWord();
			LoadTexture(text.GetWord());
		}

		//text��ǂݐi�߂�
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}

	//���̃R���|�[�l���g�ɐݒ����`����
	m_pMeshFieldCollider->SetField( m_nNumBlockX, m_nNumBlockZ, m_fBlockWidth, m_fBlockHeight, m_pVertexHeight);
	m_pMeshFieldRenderer->SetField( m_nNumBlockX, m_nNumBlockZ, m_fBlockWidth, m_fBlockHeight, m_pVertexHeight);
}

/*------------------------------------------------------------------------------
	�Z�[�u
------------------------------------------------------------------------------*/
void MeshField::Save(Text& text)
{
	StartSave(text);

	text += "NumBlockX " + std::to_string(m_nNumBlockX) + ' ';
	text += "NumBlockZ " + std::to_string(m_nNumBlockZ) + ' ';
	text += "Width " + std::to_string(m_fWidth) + ' ';
	text += "Height " + std::to_string(m_fHeight) + ' ';
	text += "BlockWidth " + std::to_string(m_fBlockWidth) + ' ';
	text += "BlockHeight " + std::to_string(m_fBlockHeight) + '\n';
	text += "VertexHeight ";
	for (int nCnt = 0; nCnt < ( m_nNumBlockZ + 1) * ( m_nNumBlockX + 1); nCnt++)
	{
		text += std::to_string(m_pVertexHeight[ nCnt]) + ' ';
	}
	text += "\n";
	text += "TextureFileName " + GetTextureName() + '\n';

	EndSave( text);
}