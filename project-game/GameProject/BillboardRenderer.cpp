/*==============================================================================

    BillboardRenderer.cpp - 3D�|���S���`��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/25
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "BillboardRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Material.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
BillboardRenderer::BillboardRenderer( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerBillboard;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_nPass = 1;

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾

	//���_�o�b�t�@����
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_3D) * 4,				//�쐬���������_�o�b�t�@�̃T�C�Y
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

	//�F�̐ݒ�
	m_Color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);

	//���_�ݒ�
	SetVtxBuffer();
	
	//�}�e���A��
	m_pMaterial = new Material();
	m_pMaterial->SetShader( eShaderBillboard);
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void BillboardRenderer::Uninit( void)
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
void BillboardRenderer::Update( void)
{
	//���_�ݒ�
	//SetVtxBuffer();
}

/*------------------------------------------------------------------------------
	�`��
------------------------------------------------------------------------------*/
void BillboardRenderer::Draw( Camera* pCamera)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾

	//�r���{�[�h����
	SetBillboard( *pCamera->GetViewMatrix());

	//�}�e���A���i�V�F�[�_�[�j���Z�b�g
	m_pMaterial->Set( pCamera, this);

	//���_���ݒ�
	pDevice-> SetStreamSource( 0, m_pVtxBuff, 0, sizeof( VERTEX_3D));

	//�e�N�j�b�N�J�n
	m_pMaterial->Begin( m_nPass);

	//�|���S���̕`��
	pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0,	2);

	//�e�N�j�b�N�I��
	m_pMaterial->End();
}

/*------------------------------------------------------------------------------
	���_�o�b�t�@�ݒ�
------------------------------------------------------------------------------*/
void BillboardRenderer::SetVtxBuffer( void)
{
	//���b�N
	VERTEX_3D* pVtx;		//���z�A�h���X�p�|�C���^
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx, 0);

	//���_�ݒ�
	//���W�iZ���j
	pVtx[ 0].Pos = D3DXVECTOR3( -0.5f, +0.5f, 0.0f);
	pVtx[ 1].Pos = D3DXVECTOR3( +0.5f, +0.5f, 0.0f);
	pVtx[ 2].Pos = D3DXVECTOR3( -0.5f, -0.5f, 0.0f);
	pVtx[ 3].Pos = D3DXVECTOR3( +0.5f, -0.5f, 0.0f);
	
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
	pVtx[ 0].Tex = D3DXVECTOR2( m_TextureUV.UV[ 0].x, m_TextureUV.UV[ 0].y);
	pVtx[ 1].Tex = D3DXVECTOR2( m_TextureUV.UV[ 1].x, m_TextureUV.UV[ 1].y);
	pVtx[ 2].Tex = D3DXVECTOR2( m_TextureUV.UV[ 2].x, m_TextureUV.UV[ 2].y);
	pVtx[ 3].Tex = D3DXVECTOR2( m_TextureUV.UV[ 3].x, m_TextureUV.UV[ 3].y);

	//�A�����b�N
	m_pVtxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	�e�N�X�`���ݒ�
------------------------------------------------------------------------------*/
void BillboardRenderer::LoadTexture(std::string FileName)
{
	m_pMaterial->SetTexture( FileName);
}

/*------------------------------------------------------------------------------
	�r���{�[�h����
------------------------------------------------------------------------------*/
void BillboardRenderer::SetBillboard(D3DXMATRIX mtxView)
{
	//�]�n�s��
	D3DXMatrixTranspose( &mtxView, &mtxView);
	mtxView._14 = 0.0f;
	mtxView._24 = 0.0f;
	mtxView._34 = 0.0f;


}


