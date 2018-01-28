/*==============================================================================

   MeshBoxRenderer.cpp - �{�b�N�X�`��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/7/14
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "MeshBoxRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Material.h"
#include "Camera.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define NUM_POLYGON (12)	//�|���S����
#define NUM_VERTEX (24)		//���_��
#define NUM_INDEX (36)		//�C���f�b�N�X��

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* MeshBoxRenderer::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<MeshBoxRenderer>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
MeshBoxRenderer::MeshBoxRenderer( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerDefault;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_nPass = 0;

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾

	//�F�̐ݒ�
	m_Color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);

	//���S�ʒu�E�傫���̐ݒ�
	m_Center = Vector3( 0.0f, 0.0f, 0.0f);
	m_Size = Vector3( 1.0f, 1.0f, 1.0f);

	SetVertices();
	
	//���_�o�b�t�@����
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_3D) * NUM_VERTEX,	//�쐬���������_�o�b�t�@�̃T�C�Y
		D3DUSAGE_WRITEONLY,					//���_�o�b�t�@�̎g�p���@(�����ɉe��)
		0,									//FVF(���_�t�H�[�}�b�g)
		D3DPOOL_MANAGED,					//�������̊Ǘ�(MANAGED�̓f�o�C�X�ɂ��܂���)
		&m_pVtxBuff,						//���_�o�b�t�@�Ǘ��C���^�[�t�F�C�X
		NULL)))
	{
		//�G���[
		assert( false);
		return;
	}

	//���_�o�b�t�@�ݒ�
	SetVtxBuffer();

	//�C���f�b�N�X�o�b�t�@�̍쐬
	HRESULT hr;
	hr = pDevice->CreateIndexBuffer(
		sizeof( WORD) * NUM_INDEX,		 //�T�C�Y�iWORD or DWORD�j*�C���f�b�N�X��
		D3DUSAGE_WRITEONLY,				 //�g�p�p�r�t���O
		D3DFMT_INDEX16,					 //�C���f�b�N�X�f�[�^�̃t�H�[�}�b�g�i16 or 32�j
		D3DPOOL_MANAGED,				 //�������̊Ǘ����@�i���܂����j
		&m_pIdxBuff,					 //�C���f�b�N�X�o�b�t�@�C���^�[�t�F�C�X�|�C���^�̃A�h���X
		NULL);

	if( FAILED( hr))
	{
		//�G���[
		assert( false);
		return;
	}

	//�C���f�b�N�X�o�b�t�@�ݒ�
	SetIdxBuffer();

	//�}�e���A��
	m_pMaterial = new Material();
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void MeshBoxRenderer::Uninit( void)
{
	Manager::GetRenderManager()->ReleaseRenderer( this);

	//���_�o�b�t�@�̉��
	if( m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//�C���f�b�N�X�o�b�t�@�̉��
	if( m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}

	//�}�e���A���̉��
	if (m_pMaterial != NULL)
	{
		delete m_pMaterial;
		m_pMaterial = NULL;
	}

	m_Vertices.clear();
	m_Vertices.shrink_to_fit();
}

/*------------------------------------------------------------------------------
	�X�V
------------------------------------------------------------------------------*/
void MeshBoxRenderer::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	�`��
------------------------------------------------------------------------------*/
void MeshBoxRenderer::Draw( Camera* pCamera)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾

	//�}�e���A���i�V�F�[�_�[�j���Z�b�g
	m_pMaterial->Set( pCamera, this);

	//���_���ݒ�
	pDevice-> SetStreamSource( 0, m_pVtxBuff, 0, sizeof( VERTEX_3D));

	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	pDevice->SetIndices( m_pIdxBuff);

	//�e�N�j�b�N�J�n
	m_pMaterial->Begin( m_nPass);

	//�v���~�e�B�u�i�|���S���E�}�`�j�̕`��i�C���f�b�N�X�j
	pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, NUM_VERTEX, 0,	NUM_POLYGON);

	//�e�N�j�b�N�I��
	m_pMaterial->End();
}

/*------------------------------------------------------------------------------
	���_�o�b�t�@�ݒ�
------------------------------------------------------------------------------*/
void MeshBoxRenderer::SetVtxBuffer( void)
{
	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	VERTEX_3D* pVtx;				//���z�A�h���X�p�|�C���^
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx,0);

	Vector3 Size = m_Size * 0.5f;

	//��O
	//���W�iZ���j
	pVtx[ 0].Pos = m_Vertices[2];
	pVtx[ 1].Pos = m_Vertices[3];
	pVtx[ 2].Pos = m_Vertices[6];
	pVtx[ 3].Pos = m_Vertices[7];
	
	//�@��
	pVtx[ 0].Normal = 
	pVtx[ 1].Normal = 
	pVtx[ 2].Normal = 
	pVtx[ 3].Normal = D3DXVECTOR3( 0.0f, 0.0f, -1.0f);

	//���_�J���[
	pVtx[ 0].Color = 
	pVtx[ 1].Color = 
	pVtx[ 2].Color = 
	pVtx[ 3].Color = m_Color;

	//UV���W�iZ���j
	pVtx[ 0].Tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[ 1].Tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[ 2].Tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[ 3].Tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	//��
	//���W�iZ���j
	pVtx[ 0].Pos = m_Vertices[1];
	pVtx[ 1].Pos = m_Vertices[0];
	pVtx[ 2].Pos = m_Vertices[5];
	pVtx[ 3].Pos = m_Vertices[4];
	
	//�@��
	pVtx[ 0].Normal = 
	pVtx[ 1].Normal = 
	pVtx[ 2].Normal = 
	pVtx[ 3].Normal = D3DXVECTOR3( 0.0f, 0.0f, 1.0f);

	//���_�J���[
	pVtx[ 0].Color = 
	pVtx[ 1].Color = 
	pVtx[ 2].Color = 
	pVtx[ 3].Color = m_Color;

	//UV���W�iZ���j
	pVtx[ 0].Tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[ 1].Tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[ 2].Tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[ 3].Tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	//��
	//���W�iZ���j
	pVtx[ 0].Pos = m_Vertices[0];
	pVtx[ 1].Pos = m_Vertices[2];
	pVtx[ 2].Pos = m_Vertices[4];
	pVtx[ 3].Pos = m_Vertices[6];
	
	//�@��
	pVtx[ 0].Normal = 
	pVtx[ 1].Normal = 
	pVtx[ 2].Normal = 
	pVtx[ 3].Normal = D3DXVECTOR3( -1.0f, 0.0f, 0.0f);

	//���_�J���[
	pVtx[ 0].Color = 
	pVtx[ 1].Color = 
	pVtx[ 2].Color = 
	pVtx[ 3].Color = m_Color;

	//UV���W�iZ���j
	pVtx[ 0].Tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[ 1].Tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[ 2].Tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[ 3].Tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	//�E
	//���W�iZ���j
	pVtx[ 0].Pos = m_Vertices[3];
	pVtx[ 1].Pos = m_Vertices[1];
	pVtx[ 2].Pos = m_Vertices[7];
	pVtx[ 3].Pos = m_Vertices[5];
	
	//�@��
	pVtx[ 0].Normal = 
	pVtx[ 1].Normal = 
	pVtx[ 2].Normal = 
	pVtx[ 3].Normal = D3DXVECTOR3( 1.0f, 0.0f, 0.0f);

	//���_�J���[
	pVtx[ 0].Color = 
	pVtx[ 1].Color = 
	pVtx[ 2].Color = 
	pVtx[ 3].Color = m_Color;

	//UV���W�iZ���j
	pVtx[ 0].Tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[ 1].Tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[ 2].Tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[ 3].Tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	//��
	//���W�iZ���j
	pVtx[ 0].Pos = m_Vertices[0];
	pVtx[ 1].Pos = m_Vertices[1];
	pVtx[ 2].Pos = m_Vertices[2];
	pVtx[ 3].Pos = m_Vertices[3];
	
	//�@��
	pVtx[ 0].Normal = 
	pVtx[ 1].Normal = 
	pVtx[ 2].Normal = 
	pVtx[ 3].Normal = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);

	//���_�J���[
	pVtx[ 0].Color = 
	pVtx[ 1].Color = 
	pVtx[ 2].Color = 
	pVtx[ 3].Color = m_Color;

	//UV���W�iZ���j
	pVtx[ 0].Tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[ 1].Tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[ 2].Tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[ 3].Tex = D3DXVECTOR2(1.0f, 1.0f);

	pVtx += 4;

	//��
	//���W�iZ���j
	pVtx[ 0].Pos = m_Vertices[4];
	pVtx[ 1].Pos = m_Vertices[5];
	pVtx[ 2].Pos = m_Vertices[6];
	pVtx[ 3].Pos = m_Vertices[7];
	
	//�@��
	pVtx[ 0].Normal = 
	pVtx[ 1].Normal = 
	pVtx[ 2].Normal = 
	pVtx[ 3].Normal = D3DXVECTOR3( 0.0f, -1.0f, 0.0f);

	//���_�J���[
	pVtx[ 0].Color = 
	pVtx[ 1].Color = 
	pVtx[ 2].Color = 
	pVtx[ 3].Color = m_Color;

	//UV���W�iZ���j
	pVtx[ 0].Tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[ 1].Tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[ 2].Tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[ 3].Tex = D3DXVECTOR2(1.0f, 1.0f);

	//�A�����b�N
	m_pVtxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	�C���f�b�N�X�o�b�t�@�ݒ�
------------------------------------------------------------------------------*/
void MeshBoxRenderer::SetIdxBuffer(void)
{
	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	WORD* pIdx;		//�C���f�b�N�X���
	m_pIdxBuff->Lock( 0, 0, (void**)&pIdx, 0);

	for (int nCnt = 0; nCnt < 6; nCnt++)
	{
		pIdx[ 0] = nCnt * 4 + 0;
		pIdx[ 1] = nCnt * 4 + 1;
		pIdx[ 2] = nCnt * 4 + 2;

		pIdx[ 3] = nCnt * 4 + 1;
		pIdx[ 4] = nCnt * 4 + 3;
		pIdx[ 5] = nCnt * 4 + 2;

		if (nCnt >= 5)
		{
			return;
		}

		pIdx += 6;
	}

	//�A�����b�N
	m_pIdxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	���S�ʒu�ݒ�
------------------------------------------------------------------------------*/
void MeshBoxRenderer::SetCenter(const Vector3& Center)
{
	m_Center = Center;
	SetVertices();

	//���_�o�b�t�@�ݒ�
	SetVtxBuffer();
}

/*------------------------------------------------------------------------------
	�傫���ݒ�
------------------------------------------------------------------------------*/
void MeshBoxRenderer::SetSize(const Vector3& Size)
{
	m_Size = Size;
	SetVertices();

	//���_�o�b�t�@�ݒ�
	SetVtxBuffer();
}

/*------------------------------------------------------------------------------
	���_��ݒ�
------------------------------------------------------------------------------*/
void MeshBoxRenderer::SetVertices(void)
{
	std::vector<D3DXVECTOR3>().swap( m_Vertices);
	Vector3 size = m_Size * 0.5f;

	m_Vertices.resize(8);

	//�㉜��
	m_Vertices[0] = D3DXVECTOR3( m_Center.x - size.x, m_Center.y + size.y, m_Center.z + size.z);

	//�㉜�E
	m_Vertices[1] = D3DXVECTOR3( m_Center.x + size.x, m_Center.y + size.y, m_Center.z + size.z);

	//���O��
	m_Vertices[2] = D3DXVECTOR3( m_Center.x - size.x, m_Center.y + size.y, m_Center.z - size.z);

	//���O�E
	m_Vertices[3] = D3DXVECTOR3( m_Center.x + size.x, m_Center.y + size.y, m_Center.z - size.z);

	//������
	m_Vertices[4] = D3DXVECTOR3( m_Center.x - size.x, m_Center.y - size.y, m_Center.z + size.z);

	//�����E
	m_Vertices[5] = D3DXVECTOR3( m_Center.x + size.x, m_Center.y - size.y, m_Center.z + size.z);

	//����O��
	m_Vertices[6] = D3DXVECTOR3( m_Center.x - size.x, m_Center.y - size.y, m_Center.z - size.z);

	//����O�E
	m_Vertices[7] = D3DXVECTOR3( m_Center.x + size.x, m_Center.y - size.y, m_Center.z - size.z);
}

/*------------------------------------------------------------------------------
	�e�N�X�`���ݒ�
------------------------------------------------------------------------------*/
void MeshBoxRenderer::LoadTexture(std::string FileName)
{
	m_pMaterial->SetTexture( FileName);
}

/*------------------------------------------------------------------------------
	�V�F�[�_�[�ݒ�
------------------------------------------------------------------------------*/
void MeshBoxRenderer::SetShader(EShaderType Type)
{
	m_pMaterial->SetShader( Type);
}

/*------------------------------------------------------------------------------
	���[�h
------------------------------------------------------------------------------*/
void MeshBoxRenderer::Load(Text& text)
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
		else if (text.GetWord() == "Center")
		{
			text.ForwardPositionToNextWord();
			text.SetPosition( m_Center.ConvertFromString(text.GetAllText(), text.GetPosition()));
		}
		else if (text.GetWord() == "Size")
		{
			text.ForwardPositionToNextWord();
			text.SetPosition( m_Size.ConvertFromString(text.GetAllText(), text.GetPosition()));
		}

		//text��ǂݐi�߂�
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}
	SetVtxBuffer();
}

/*------------------------------------------------------------------------------
	�Z�[�u
------------------------------------------------------------------------------*/
void MeshBoxRenderer::Save(Text& text)
{
	StartSave(text);

	text += "Pass " + std::to_string(m_nPass) + '\n';
	m_pMaterial->Save(text);
	text += "Color " 
		+ std::to_string(m_Color.r) + ' '
		+ std::to_string(m_Color.g) + ' '
		+ std::to_string(m_Color.b) + ' '
		+ std::to_string(m_Color.a) + '\n';
	text += "Center " + m_Center.ConvertToString() + '\n';
	text += "Size " + m_Size.ConvertToString() + '\n';

	EndSave( text);
}

/*------------------------------------------------------------------------------
	������J�����O����
------------------------------------------------------------------------------*/
bool MeshBoxRenderer::CheckFrustumCulling(Camera* pCamera)
{
	D3DXMATRIX wvp = m_pTransform->WorldMatrix() * *pCamera->GetViewMatrix() * *pCamera->GetProjectionMatrix();
	bool isLeft = false;
	bool isRight = false;
	bool isInDepth = false;

	for (D3DXVECTOR3 vertex : m_Vertices)
	{
		D3DXVec3TransformCoord( &vertex, &vertex, &wvp);
		
		if (vertex.z >= 0 && vertex.z <= 1.0f)
		{
			isInDepth = true;

			if (vertex.x >= -1.0f && vertex.x <= 1.0f)
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

	if (isLeft && isRight && isInDepth)
	{
		return true;
	}
	
	return false;
}