/*==============================================================================

    SpriteRenderer.cpp - 2D�|���S���`��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/5
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Material.h"

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* SpriteRenderer::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<SpriteRenderer>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
SpriteRenderer::SpriteRenderer( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerUI;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_nPass = 1;

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾

	//���_�o�b�t�@����
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_2D) * 4,				//�쐬���������_�o�b�t�@�̃T�C�Y
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

	//�ʒu�̐ݒ�
	//m_pTransform->SetLocalPosition( Vector3( SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f));

	//�傫���̐ݒ�
	//m_pTransform->SetLocalScale( Vector3( 100.0f, 100.0f, 1.0f));

	//��]�ʂ̐ݒ�
	m_fRotation = 0.0f;

	//�F�̐ݒ�
	m_Color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);

	//���_�ݒ�
	SetVtxBuffer();

	//�}�e���A��
	m_pMaterial = new Material();

	//�V�F�[�_�[
	m_pMaterial->SetShader( eShaderSprite);
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void SpriteRenderer::Uninit( void)
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
void SpriteRenderer::Update( void)
{
	//���_�ݒ�
	SetVtxBuffer();
}

/*------------------------------------------------------------------------------
	�`��
------------------------------------------------------------------------------*/
void SpriteRenderer::Draw( Camera* pCamera)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾

	//�}�e���A���i�V�F�[�_�[�j���Z�b�g
	m_pMaterial->Set( pCamera, this);

	//���_���ݒ�
	pDevice-> SetStreamSource( 0, m_pVtxBuff, 0, sizeof( VERTEX_2D));

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
void SpriteRenderer::SetVtxBuffer( void)
{
	//���a�E�p�x���Z�o
	Vector3 pos = m_pTransform->GetLocalPosition();
	Vector3 scale = m_pTransform->GetLocalScale();
	float fRadius = sqrt( ( scale.x * 0.5f) * (  scale.x * 0.5f) + (  scale.y * 0.5f) * (  scale.y * 0.5f));
	float fAngle = atan2( scale.y,  scale.x);

	//���b�N
	VERTEX_2D* pVtx;		//���z�A�h���X�p�|�C���^
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx, 0);

	//���_�ݒ�
	//���W�iZ���j
	pVtx[ 0].Pos = D3DXVECTOR3( pos.x + fRadius * cosf( float( m_fRotation +fAngle + D3DX_PI)),	pos.y + fRadius * sinf( float( m_fRotation +fAngle + D3DX_PI)),	0.0f);
	pVtx[ 1].Pos = D3DXVECTOR3( pos.x + fRadius * cosf( float( m_fRotation -fAngle)),			pos.y + fRadius * sinf( float( m_fRotation -fAngle)),			0.0f);
	pVtx[ 2].Pos = D3DXVECTOR3( pos.x + fRadius * cosf( float( m_fRotation -fAngle + D3DX_PI)),	pos.y + fRadius * sinf( float( m_fRotation -fAngle + D3DX_PI)),	0.0f);
	pVtx[ 3].Pos = D3DXVECTOR3( pos.x + fRadius * cosf( float( m_fRotation +fAngle)),			pos.y + fRadius * sinf( float( m_fRotation +fAngle)),			0.0f);
	
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
void SpriteRenderer::LoadTexture(std::string FileName)
{
	m_pMaterial->SetTexture( FileName);
}

/*------------------------------------------------------------------------------
	�V�F�[�_�[�ݒ�
------------------------------------------------------------------------------*/
void SpriteRenderer::SetShader(EShaderType Type)
{
	m_pMaterial->SetShader( Type);
}

/*------------------------------------------------------------------------------
	���[�h
------------------------------------------------------------------------------*/
void SpriteRenderer::Load(Text& text)
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
		else if (text.GetWord() == "Rotation")
		{
			text.ForwardPositionToNextWord();
		
			m_fRotation = std::stof( text.GetWord());
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
void SpriteRenderer::Save(Text& text)
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
	text += "Rotation " + std::to_string( m_fRotation) + '\n';

	EndSave( text);
}
