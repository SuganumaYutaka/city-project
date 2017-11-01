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

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define NUM_POLYGON (12)	//�|���S����
#define NUM_VERTEX (24)		//���_��
#define NUM_INDEX (36)		//�C���f�b�N�X��

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

	//���C���[�t���[���ݒ�
	if (m_nLayer == eLayerWireFrame)
	{
		pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);	//���C���[�t���[���\��
	}

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

	//���C���[�t���[���ݒ�
	if (m_nLayer == eLayerWireFrame)
	{
		pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	//���S��߂�
	Vector3 CenterInv = m_Center * -1;
	m_pTransform->Move( CenterInv);
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
	pVtx[ 0].Pos = D3DXVECTOR3( -Size.x,	+Size.y,	-Size.z);
	pVtx[ 1].Pos = D3DXVECTOR3( +Size.x,	+Size.y,	-Size.z);
	pVtx[ 2].Pos = D3DXVECTOR3( -Size.x,	-Size.y,	-Size.z);
	pVtx[ 3].Pos = D3DXVECTOR3( +Size.x,	-Size.y,	-Size.z);
	
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
	pVtx[ 0].Pos = D3DXVECTOR3( +Size.x,	+Size.y,	Size.z);
	pVtx[ 1].Pos = D3DXVECTOR3( -Size.x,	+Size.y,	Size.z);
	pVtx[ 2].Pos = D3DXVECTOR3( +Size.x,	-Size.y,	Size.z);
	pVtx[ 3].Pos = D3DXVECTOR3( -Size.x,	-Size.y,	Size.z);
	
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
	pVtx[ 0].Pos = D3DXVECTOR3( -Size.x,	+Size.y,	+Size.z);
	pVtx[ 1].Pos = D3DXVECTOR3( -Size.x,	+Size.y,	-Size.z);
	pVtx[ 2].Pos = D3DXVECTOR3( -Size.x,	-Size.y,	+Size.z);
	pVtx[ 3].Pos = D3DXVECTOR3( -Size.x,	-Size.y,	-Size.z);
	
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
	pVtx[ 0].Pos = D3DXVECTOR3( +Size.x,	+Size.y,	-Size.z);
	pVtx[ 1].Pos = D3DXVECTOR3( +Size.x,	+Size.y,	+Size.z);
	pVtx[ 2].Pos = D3DXVECTOR3( +Size.x,	-Size.y,	-Size.z);
	pVtx[ 3].Pos = D3DXVECTOR3( +Size.x,	-Size.y,	+Size.z);
	
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
	pVtx[ 0].Pos = D3DXVECTOR3( -Size.x,	+Size.y,	+Size.z);
	pVtx[ 1].Pos = D3DXVECTOR3( +Size.x,	+Size.y,	+Size.z);
	pVtx[ 2].Pos = D3DXVECTOR3( -Size.x,	+Size.y,	-Size.z);
	pVtx[ 3].Pos = D3DXVECTOR3( +Size.x,	+Size.y,	-Size.z);
	
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
	pVtx[ 0].Pos = D3DXVECTOR3( +Size.x,	-Size.y,	+Size.z);
	pVtx[ 1].Pos = D3DXVECTOR3( -Size.x,	-Size.y,	+Size.z);
	pVtx[ 2].Pos = D3DXVECTOR3( +Size.x,	-Size.y,	-Size.z);
	pVtx[ 3].Pos = D3DXVECTOR3( -Size.x,	-Size.y,	-Size.z);
	
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
}

/*------------------------------------------------------------------------------
	�傫���ݒ�
------------------------------------------------------------------------------*/
void MeshBoxRenderer::SetSize(const Vector3& Size)
{
	m_Size = Size;

	//���_�o�b�t�@�ݒ�
	SetVtxBuffer();
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
