/*==============================================================================

    MeshPolygonRenderer.cpp - 3D�|���S���`��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/17
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "MeshPolygonRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Material.h"
#include "Camera.h"

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* MeshPolygonRenderer::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<MeshPolygonRenderer>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
MeshPolygonRenderer::MeshPolygonRenderer( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerDefault;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_nPass = 1;

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾

	//�F�̐ݒ�
	m_Color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);

	//�f�t�H���g�̒��_�ݒ�
	std::vector<Vector3> vertices;
	vertices.resize(4);
	vertices[0] = Vector3( -0.5f, 0.0f, +0.5f);
	vertices[1] = Vector3( +0.5f, 0.0f, +0.5f);
	vertices[2] = Vector3( +0.5f, 0.0f, -0.5f);
	vertices[3] = Vector3( -0.5f, 0.0f, -0.5f);
	m_pVtxBuff = NULL;
	SetVertices( vertices);
	
	//�}�e���A��
	m_pMaterial = new Material();

}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void MeshPolygonRenderer::Uninit( void)
{
	Manager::GetRenderManager()->ReleaseRenderer( this);

	//���_�o�b�t�@�̉��
	if( m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�}�e���A���̉��
	if (m_pMaterial != NULL)
	{
		delete m_pMaterial;
		m_pMaterial = NULL;
	}
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void MeshPolygonRenderer::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	�`��
------------------------------------------------------------------------------*/
void MeshPolygonRenderer::Draw( Camera* pCamera)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾

	//�}�e���A���i�V�F�[�_�[�j���Z�b�g
	m_pMaterial->Set( pCamera, this);

	//���_���ݒ�
	pDevice-> SetStreamSource( 0, m_pVtxBuff, 0, sizeof( VERTEX_3D));

	//�e�N�j�b�N�J�n
	m_pMaterial->Begin( m_nPass);

	//�|���S���̕`��
	pDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, 0, m_Vertices.size() - 2);

	//�e�N�j�b�N�I��
	m_pMaterial->End();
}

/*------------------------------------------------------------------------------
	���_�o�b�t�@�ݒ�
------------------------------------------------------------------------------*/
void MeshPolygonRenderer::SetVertices( const std::vector<Vector3>& vertices)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾

	//���_�o�b�t�@�̉��
	SAFE_RELEASE( m_pVtxBuff);
	
	//���_���̎擾
	int size = vertices.size();

	//���_�f�[�^�̎擾
	m_Vertices.resize( size);
	for (int nCnt = 0; nCnt < size; nCnt++)
	{
		m_Vertices[ nCnt] = vertices[ nCnt].ConvertToDX();
	}
	
	//���_�o�b�t�@����
	if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_3D) * size, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
	{
		//�G���[
		assert( false);
		return;
	}

	SetVtxBuffer();
}

/*------------------------------------------------------------------------------
	���_�o�b�t�@�ݒ�
------------------------------------------------------------------------------*/
void MeshPolygonRenderer::SetVtxBuffer( void)
{
	int size = m_Vertices.size();

	//���b�N
	VERTEX_3D* pVtx;		//���z�A�h���X�p�|�C���^
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < size; nCnt++)
	{
		pVtx[nCnt].Pos = m_Vertices[ nCnt];
		pVtx[nCnt].Normal = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
		pVtx[nCnt].Color = m_Color;
		pVtx[nCnt].Tex = D3DXVECTOR2( 0.0f, 0.0f);
	}

	//�A�����b�N
	m_pVtxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	�V�F�[�_�[�ݒ�
------------------------------------------------------------------------------*/
void MeshPolygonRenderer::SetShader(EShaderType Type)
{
	m_pMaterial->SetShader( Type);
}

/*------------------------------------------------------------------------------
	���[�h
------------------------------------------------------------------------------*/
void MeshPolygonRenderer::Load(Text& text)
{
	//text��ǂݐi�߂�
	if (text.ForwardPositionToNextWord() == Text::EoF)
	{
		return;
	}

	while ( text.GetWord() != "EndComponent")
	{
		if (text.GetWord() == "Pass")
		{
			text.ForwardPositionToNextWord();
			m_nPass = std::stoi( text.GetWord());
		}

		else if (text.GetWord() == "Material")
		{
			text.ForwardPositionToNextWord();
			m_pMaterial->Load(text);
		}
		else if (text.GetWord() == "Color")
		{
			text.ForwardPositionToNextWord();
		
			m_Color.r = std::stof(text.GetWord());
			text.ForwardPositionToNextWord();
			m_Color.g = std::stof(text.GetWord());
			text.ForwardPositionToNextWord();
			m_Color.b = std::stof(text.GetWord());
			text.ForwardPositionToNextWord();
			m_Color.a = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "Vertices")
		{
			text.ForwardPositionToNextWord();
			
			int size = std::stoi( text.GetWord());
			m_Vertices.clear();
			m_Vertices.resize(size);
			
			for (int nCnt = 0; nCnt < size; nCnt++)
			{
				text.ForwardPositionToNextWord();
				m_Vertices[nCnt].x = std::stof( text.GetWord());
				text.ForwardPositionToNextWord();
				m_Vertices[nCnt].y = std::stof( text.GetWord());
				text.ForwardPositionToNextWord();
				m_Vertices[nCnt].z = std::stof( text.GetWord());
			}
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
void MeshPolygonRenderer::Save(Text& text)
{
	StartSave(text);

	text += "Pass " + std::to_string(m_nPass) + '\n';
	m_pMaterial->Save(text);
	text += "Color " 
		+ std::to_string(m_Color.r) + ' '
		+ std::to_string(m_Color.g) + ' '
		+ std::to_string(m_Color.b) + ' '
		+ std::to_string(m_Color.a) + '\n';
	
	text += "Vertices "
		+ std::to_string(m_Vertices.size()) + '\n';
	for (const D3DXVECTOR3& vertex : m_Vertices)
	{
		text += std::to_string( vertex.x) + ' '
			+ std::to_string( vertex.y) + ' '
			+ std::to_string( vertex.z) + '\n';
	}

	EndSave( text);
}

/*------------------------------------------------------------------------------
	������J�����O����
------------------------------------------------------------------------------*/
bool MeshPolygonRenderer::CheckFrustumCulling(Camera* pCamera)
{
	D3DXMATRIX wvp = m_pTransform->WorldMatrix() * *pCamera->GetViewMatrix() * *pCamera->GetProjectionMatrix();
	bool isLeft = false;
	bool isRight = false;

	for (D3DXVECTOR3 vertex : m_Vertices)
	{
		D3DXVec3TransformCoord( &vertex, &vertex, &wvp);
		if (vertex.x >= -1.0f && vertex.x <= 1.0f)
		{
			if (vertex.z >= 0 && vertex.z <= 1.0f)
			{
				return true;
			}
		}

		if (vertex.x < -1.0f)
		{
			isLeft = true;
		}
		else if (vertex.x > 1.0f)
		{
			isRight = true;
		}
	}
	
	return false;
}
