/*==============================================================================

   MeshPlaneRenderer.cpp - ���ʕ`��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/24
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "MeshPlaneRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Material.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define P_POS_X(P_WIDTH, NUM_FIELD_X) (0.0f + P_WIDTH * NUM_FIELD_X * -0.5f)		//�J�n�n�_��X���W
#define P_POS_Y (0.0f)																//�J�n�n�_��Y���W
#define P_POS_Z(P_HEIGHT, NUM_FIELD_Z) (0.0f + P_HEIGHT * NUM_FIELD_Z * 0.5f)		//�J�n�n�_��Z���W
#define P_WIDTH( ALL_WIDTH, NUM_FIELD_X)			( ALL_WIDTH / NUM_FIELD_X)										//�|���S���ꖇ������̕�
#define P_HEIGHT( ALL_HEIGHT, NUM_FIELD_Z)			( ALL_HEIGHT / NUM_FIELD_Z)										//�|���S���ꖇ������̍���
#define NUM_POLYGON( NUM_FIELD_X, NUM_FIELD_Z)		( 2 * NUM_FIELD_X * NUM_FIELD_Z + ( NUM_FIELD_Z - 1) * 4)		//�|���S����
#define NUM_VERTEX( NUM_FIELD_X, NUM_FIELD_Z)		( ( NUM_FIELD_X + 1) * ( NUM_FIELD_Z + 1))						//���_��
#define NUM_INDEX( NUM_FIELD_X, NUM_FIELD_Z)		( ( (NUM_FIELD_X + 1) * 2 + 2) * NUM_FIELD_Z - 2)				//�C���f�b�N�X��

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
MeshPlaneRenderer::MeshPlaneRenderer( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerBack;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_nPass = 1;

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾

	//�F�̐ݒ�
	m_Color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);

	//�f�t�H���g�l�ݒ�
	m_nNumBlockX = 1;			//���̕�����
	m_nNumBlockZ = 1;			//�c�̕�����
	m_fWidth = 1.0f;			//��
	m_fHeight = 1.0f;			//����
	m_fBlockWidth = P_WIDTH( m_fWidth, m_nNumBlockX);		//�|���S��1��������̕�
	m_fBlockHeight = P_HEIGHT( m_fHeight, m_nNumBlockZ);	//�|���S��1��������̍���
	
	//���_�o�b�t�@����
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_3D) * NUM_VERTEX( m_nNumBlockX, m_nNumBlockZ),				//�쐬���������_�o�b�t�@�̃T�C�Y
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
		sizeof( WORD) * NUM_INDEX( m_nNumBlockX, m_nNumBlockZ),		 //�T�C�Y�iWORD or DWORD�j*�C���f�b�N�X��
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
void MeshPlaneRenderer::Uninit( void)
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
void MeshPlaneRenderer::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	�`��
------------------------------------------------------------------------------*/
void MeshPlaneRenderer::Draw( Camera* pCamera)
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

	//�v���~�e�B�u�i�|���S���E�}�`�j�̕`��
	pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP,	0, 0, 
		NUM_VERTEX( m_nNumBlockX, m_nNumBlockZ), 0,	NUM_POLYGON( m_nNumBlockX, m_nNumBlockZ));

	//�e�N�j�b�N�I��
	m_pMaterial->End();
}

/*------------------------------------------------------------------------------
	���_�o�b�t�@�ݒ�
------------------------------------------------------------------------------*/
void MeshPlaneRenderer::SetVtxBuffer( void)
{
	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	VERTEX_3D* pVtx;				//���z�A�h���X�p�|�C���^
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx,0);

	//�ϐ��錾
	int nCntHeight = 0;		//���ăJ�E���^
	int nCntWidth = 0;		//�悱�J�E���^

	//�|���S���̐ݒ�
	for( nCntHeight = 0; nCntHeight < m_nNumBlockZ + 1; nCntHeight++)
	{
		for( nCntWidth = 0; nCntWidth < m_nNumBlockX + 1; nCntWidth++)
		{
			//���_���W�̐ݒ�
			pVtx[ 0].Pos = D3DXVECTOR3(
				m_fWidth * -0.5f + m_fBlockWidth * nCntWidth,
				0.0f,
				m_fHeight * 0.5f - m_fBlockHeight * nCntHeight);
			
			//�@���̐ݒ�
			pVtx[ 0].Normal = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
			
			//���_�J���[�̐ݒ�
			pVtx[ 0].Color = m_Color;
			
			//UV�f�[�^�̐ݒ�
			pVtx[ 0].Tex = D3DXVECTOR2( 1.0f * nCntWidth, 1.0f * nCntHeight);
			
			//�|�C���^�����炷
			pVtx += 1;
		}
	}

	//�A�����b�N
	m_pVtxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	�C���f�b�N�X�o�b�t�@�ݒ�
------------------------------------------------------------------------------*/
void MeshPlaneRenderer::SetIdxBuffer(void)
{
	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	WORD* pIdx;		//�C���f�b�N�X���
	m_pIdxBuff->Lock( 0, 0, (void**)&pIdx, 0);

	//�ϐ��錾
	int nCntHeight = 0;		//���ăJ�E���^
	int nCntWidth = 0;		//�悱�J�E���^

	//�C���f�b�N�X���̐ݒ�
	for( nCntHeight = 0; nCntHeight < m_nNumBlockZ; nCntHeight++)
	{
		//�\������|���S���̃C���f�b�N�X���̐ݒ�
		for( nCntWidth = 0; nCntWidth < m_nNumBlockX + 1; nCntWidth++)
		{
			pIdx[ 0] = ( nCntHeight + 1) * ( m_nNumBlockX + 1) + nCntWidth;
			pIdx[ 1] = ( nCntHeight + 0) * ( m_nNumBlockX + 1) + nCntWidth;

			pIdx += 2;
		}

		//�Ō�͏k�ރ|���S���s�v
		if( nCntHeight == m_nNumBlockZ - 1)
		{
			break;
		}

		//�k�ރ|���S���̃C���f�b�N�X���̐ݒ�
		pIdx[ 0] = nCntHeight * ( m_nNumBlockX + 1) + nCntWidth - 1;
		pIdx[ 1] = ( nCntHeight + 2) * ( m_nNumBlockX + 1);
		
		pIdx += 2;
	}

	//�A�����b�N
	m_pIdxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	��������ݒ�i1��������̑傫���ς��Ȃ��j
------------------------------------------------------------------------------*/
void MeshPlaneRenderer::SetBlockNum( int X, int Z)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾

	//�ݒ�
	m_nNumBlockX = X;
	m_nNumBlockZ = Z;
	m_fWidth = m_nNumBlockX * m_fBlockWidth;
	m_fHeight = m_nNumBlockZ * m_fBlockHeight;
	

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
	
	//���_�o�b�t�@����
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_3D) * NUM_VERTEX( m_nNumBlockX, m_nNumBlockZ),				//�쐬���������_�o�b�t�@�̃T�C�Y
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
		sizeof( WORD) * NUM_INDEX( m_nNumBlockX, m_nNumBlockZ),		 //�T�C�Y�iWORD or DWORD�j*�C���f�b�N�X��
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
}

/*------------------------------------------------------------------------------
	�|���S���ꖇ������̕��E������ݒ�
------------------------------------------------------------------------------*/
void MeshPlaneRenderer::SetBlockSize( float Width, float Height)
{
	//�ݒ�
	m_fBlockWidth = Width;
	m_fBlockHeight = Height;
	m_fWidth = m_nNumBlockX * m_fBlockWidth;
	m_fHeight = m_nNumBlockZ * m_fBlockHeight;
	
	//���_�o�b�t�@�ݒ�
	SetVtxBuffer();
}

/*------------------------------------------------------------------------------
	��������ݒ�
------------------------------------------------------------------------------*/
void MeshPlaneRenderer::SetNum( int X, int Z)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾

	//�ݒ�
	m_nNumBlockX = X;			//���̕�����
	m_nNumBlockZ = Z;			//�c�̕�����
	m_fBlockWidth = P_WIDTH( m_fWidth, m_nNumBlockX);		//�|���S��1��������̕�
	m_fBlockHeight = P_HEIGHT( m_fHeight, m_nNumBlockZ);	//�|���S��1��������̍���

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
	
	//���_�o�b�t�@����
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_3D) * NUM_VERTEX( m_nNumBlockX, m_nNumBlockZ),				//�쐬���������_�o�b�t�@�̃T�C�Y
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
		sizeof( WORD) * NUM_INDEX( m_nNumBlockX, m_nNumBlockZ),		 //�T�C�Y�iWORD or DWORD�j*�C���f�b�N�X��
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
}

/*------------------------------------------------------------------------------
	���E������ݒ�
------------------------------------------------------------------------------*/
void MeshPlaneRenderer::SetSize( float Width, float Height)
{
	//�ݒ�
	m_fWidth = Width;
	m_fHeight = Height;
	m_fBlockWidth = P_WIDTH( m_fWidth, m_nNumBlockX);		//�|���S��1��������̕�
	m_fBlockHeight = P_HEIGHT( m_fHeight, m_nNumBlockZ);	//�|���S��1��������̍���
	
	//���_�o�b�t�@�ݒ�
	SetVtxBuffer();
}

/*------------------------------------------------------------------------------
	�e�N�X�`���ݒ�
------------------------------------------------------------------------------*/
void MeshPlaneRenderer::LoadTexture(std::string FileName)
{
	m_pMaterial->SetTexture( FileName);
}

/*------------------------------------------------------------------------------
	�V�F�[�_�[�ݒ�
------------------------------------------------------------------------------*/
void MeshPlaneRenderer::SetShader(EShaderType Type)
{
	m_pMaterial->SetShader( Type);
}
