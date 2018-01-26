/*==============================================================================

    WallRenderer.cpp - 3D�|���S���`��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/17
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "WallRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Material.h"


/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* WallRenderer::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<WallRenderer>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
WallRenderer::WallRenderer( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerDefault;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_nPass = 1;

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾

	m_pVtxBuff = NULL;

	//�}�e���A��
	m_pMaterial = new Material();

	m_CountVertex = 0;
	m_CountPolygon = 0;
	m_CountRenderPolygon = 0;
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void WallRenderer::Uninit( void)
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
void WallRenderer::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	�`��
------------------------------------------------------------------------------*/
void WallRenderer::Draw( Camera* pCamera)
{
	if (!m_pVtxBuff)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾

	//�}�e���A���i�V�F�[�_�[�j���Z�b�g
	m_pMaterial->Set( pCamera, this);

	//���_���ݒ�
	pDevice-> SetStreamSource( 0, m_pVtxBuff, 0, sizeof( VERTEX_3D));

	//�e�N�j�b�N�J�n
	m_pMaterial->Begin( m_nPass);

	//�|���S���̕`��
	//pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, m_CountPolygon);
	pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, m_CountRenderPolygon);

	//�e�N�j�b�N�I��
	m_pMaterial->End();
}

/*------------------------------------------------------------------------------
	�V�F�[�_�[�ݒ�
------------------------------------------------------------------------------*/
void WallRenderer::SetShader(EShaderType Type)
{
	m_pMaterial->SetShader( Type);
}

/*------------------------------------------------------------------------------
	�e�N�X�`���ݒ�
------------------------------------------------------------------------------*/
void WallRenderer::LoadTexture(std::string fileName)
{
	m_pMaterial->SetTexture( fileName);
}

/*------------------------------------------------------------------------------
	���_�o�b�t�@�̐ݒ���J�n
------------------------------------------------------------------------------*/
VERTEX_3D* WallRenderer::StartSetVertexBuffer( int countVertex, int countPolygon)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾

	SAFE_RELEASE( m_pVtxBuff);

	m_CountVertex = countVertex;
	m_CountPolygon = countPolygon;
	m_CountRenderPolygon = countPolygon;

	//���_�o�b�t�@�𐶐�
	if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_3D) * countVertex, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
	{
		//�G���[
		assert( false);
		return NULL;
	}

	//���b�N
	VERTEX_3D* pVtx;
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx, 0);

	return pVtx;
}

/*------------------------------------------------------------------------------
	���_�o�b�t�@�̐ݒ���I��
------------------------------------------------------------------------------*/
void WallRenderer::EndSetVertexBuffer(void)
{
	//�A�����b�N
	m_pVtxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	�`�悷��|���S����OnOff��ύX
	Max�Ȃ�true
------------------------------------------------------------------------------*/
bool WallRenderer::ChangeRenderPolygon()
{
	if (m_CountRenderPolygon > 0)
	{
		m_CountRenderPolygon = 0;
		return false;
	}
	
	m_CountRenderPolygon = m_CountPolygon;
	return true;
}

/*------------------------------------------------------------------------------
	�`�悷��|���S�������Z
	Max�Ȃ�true
------------------------------------------------------------------------------*/
bool WallRenderer::AddRenderPolygon()
{
	m_CountRenderPolygon += 6;

	if (m_CountRenderPolygon > m_CountPolygon)
	{
		m_CountRenderPolygon = m_CountPolygon;
		return true;
	}

	return false;
}


