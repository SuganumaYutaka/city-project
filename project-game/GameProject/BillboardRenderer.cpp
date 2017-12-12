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
#include "Camera.h"

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* BillboardRenderer::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<BillboardRenderer>();
}

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
	pVtx[ 0].Tex = m_TextureUV.GetTopLeft();
	pVtx[ 1].Tex = m_TextureUV.GetTopRight();
	pVtx[ 2].Tex = m_TextureUV.GetBottomLeft();
	pVtx[ 3].Tex = m_TextureUV.GetBottomRight();

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

/*------------------------------------------------------------------------------
	���[�h
------------------------------------------------------------------------------*/
void BillboardRenderer::Load(Text& text)
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
		else if (text.GetWord() == "TextureUV")
		{
			text.ForwardPositionToNextWord();
		
			m_TextureUV.ConvertFromString( text.GetAllText(), text.GetPosition());
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
void BillboardRenderer::Save(Text& text)
{
	StartSave(text);

	text += "Pass " + std::to_string(m_nPass) + '\n';
	m_pMaterial->Save(text);
	text += "Color " 
		+ std::to_string(m_Color.r) + ' '
		+ std::to_string(m_Color.g) + ' '
		+ std::to_string(m_Color.b) + ' '
		+ std::to_string(m_Color.a) + '\n';
	text += "TextureUV " + m_TextureUV.ConvertToString() + '\n';

	EndSave( text);
}
