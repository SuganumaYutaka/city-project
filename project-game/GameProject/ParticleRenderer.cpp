/*==============================================================================

    ParticleRenderer.cpp - 3D�|���S���`��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/17
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "ParticleRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Material.h"

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* ParticleRenderer::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<ParticleRenderer>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
ParticleRenderer::ParticleRenderer( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerAddBlend;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_nPass = 0;

	//�����l���
	m_bNeedUpdateVtx = false;
	m_nNumParticle = 1024;		//���p�[�e�B�N���̑���
	m_Color = D3DXCOLOR( 0.8f, 0.8f, 0.8f, 1.0f);
	m_AlphaRange = 0.2f;
	m_PosRange = Vector3(0.2f, 2.0f, 0.2f);	
	m_SizeCenter = 20.0f;
	m_SizeRange = 0.8f;
	m_DirCenter = Vector3( 0.0f, 1.0f, 0.0f);
	m_DirRange = Vector3( 0.4f, 0.0f, 0.4f);
	m_SpeedCenter = 0.015f;
	m_SpeedRange = 0.0005f;
	m_LifeCenter = 15000.0f;
	m_LifeRenge = 3000.0f;
	m_StartRange = 2000.0f;

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾

	//���_�o�b�t�@����
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_PARTICLE) * m_nNumParticle, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
	{
		//�G���[
		assert( false);
		return;
	}

	//���_�ݒ�
	SetVtxBuffer();

	//�}�e���A��
	m_pMaterial = new Material();
	m_pMaterial->SetShader( eShaderParticle);
	
	//���Z����
	m_pMaterial->SetTexture("data/TEXTURE/particle00.jpg");
	m_nPass = 2;
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void ParticleRenderer::Uninit( void)
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
void ParticleRenderer::Update( void)
{
	//���_�o�b�t�@�̍X�V
	if (m_bNeedUpdateVtx == true)
	{
		m_bNeedUpdateVtx = false;

		//���_�ݒ�
		SetVtxBuffer();
	}
}

/*------------------------------------------------------------------------------
	�`��
------------------------------------------------------------------------------*/
void ParticleRenderer::Draw( Camera* pCamera)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾

	//�}�e���A���i�V�F�[�_�[�j���Z�b�g
	m_pMaterial->Set( pCamera, this);

	//���_���ݒ�
	pDevice-> SetStreamSource( 0, m_pVtxBuff, 0, sizeof( VERTEX_PARTICLE));

	//�e�N�j�b�N�J�n
	m_pMaterial->Begin( m_nPass);

	//�|���S���̕`��
	pDevice->DrawPrimitive( D3DPT_POINTLIST, 0,	m_nNumParticle);

	//�e�N�j�b�N�I��
	m_pMaterial->End();
}

/*------------------------------------------------------------------------------
	���_�o�b�t�@�ݒ�
------------------------------------------------------------------------------*/
void ParticleRenderer::SetVtxBuffer( void)
{
	//���b�N
	VERTEX_PARTICLE* pVtx;		//���z�A�h���X�p�|�C���^
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < m_nNumParticle; nCnt++)
	{
		pVtx->Pos = D3DXVECTOR3( RAND_FLORT( m_PosRange.x), RAND_FLORT( m_PosRange.y), RAND_FLORT( m_PosRange.z));
		pVtx->Color = D3DXCOLOR( m_Color.r, m_Color.g, m_Color.b, m_Color.a + RAND_FLORT( m_AlphaRange));
		pVtx->Size = m_SizeCenter + RAND_FLORT( m_SizeRange);
		D3DXVECTOR3 Dir;
		Dir.x = m_DirCenter.x + RAND_FLORT( m_DirRange.x);
		Dir.y = m_DirCenter.y + RAND_FLORT( m_DirRange.y);
		Dir.z = m_DirCenter.z + RAND_FLORT( m_DirRange.z);
		D3DXVec3Normalize( &Dir, &Dir);
		Dir = Dir * ( m_SpeedCenter + RAND_FLORT( m_SpeedRange));
		pVtx->Dir = Dir;
		pVtx->Life = m_LifeCenter + RAND_FLORT( m_LifeRenge);
		pVtx->Start = -(float)rand() / (float)RAND_MAX * m_StartRange;
		pVtx++;
	}

	//�A�����b�N
	m_pVtxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	�p�[�e�B�N���̑����ݒ�
------------------------------------------------------------------------------*/
void ParticleRenderer::SetNumParticle(int NumParticle)
{
	if (NumParticle <= 0)
	{
		assert( false);
		return;
	}

	//�����̐ݒ�
	m_nNumParticle = NumParticle;

	//�o�b�t�@�̍Đ���
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();
	SAFE_RELEASE( m_pVtxBuff);
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_PARTICLE) * m_nNumParticle, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
	{
		//�G���[
		assert( false);
		return;
	}

	//���_�ݒ�
	SetVtxBuffer();
	m_bNeedUpdateVtx = false;
}

/*------------------------------------------------------------------------------
	�e�N�X�`���ݒ�
------------------------------------------------------------------------------*/
void ParticleRenderer::LoadTexture(std::string FileName)
{
	m_pMaterial->SetTexture( FileName);
}

/*------------------------------------------------------------------------------
	�V�F�[�_�[�ݒ�
------------------------------------------------------------------------------*/
void ParticleRenderer::SetShader(EShaderType Type)
{
	m_pMaterial->SetShader( Type);
}

/*------------------------------------------------------------------------------
	���[�h
------------------------------------------------------------------------------*/
void ParticleRenderer::Load(Text& text)
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
		else if (text.GetWord() == "AlphaRange")
		{
			text.ForwardPositionToNextWord();
			m_AlphaRange = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "PosRange")
		{
			text.ForwardPositionToNextWord();
			text.SetPosition( m_PosRange.ConvertFromString(text.GetAllText(), text.GetPosition()));
		}
		else if (text.GetWord() == "SizeCenter")
		{
			text.ForwardPositionToNextWord();
			m_SizeCenter = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "SizeRange")
		{
			text.ForwardPositionToNextWord();
			m_SizeRange = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "DirCenter")
		{
			text.ForwardPositionToNextWord();
			text.SetPosition( m_DirCenter.ConvertFromString(text.GetAllText(), text.GetPosition()));
		}
		else if (text.GetWord() == "DirRange")
		{
			text.ForwardPositionToNextWord();
			text.SetPosition( m_DirRange.ConvertFromString(text.GetAllText(), text.GetPosition()));
		}
		else if (text.GetWord() == "SpeedCenter")
		{
			text.ForwardPositionToNextWord();
			m_SpeedCenter = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "SpeedRange")
		{
			text.ForwardPositionToNextWord();
			m_SpeedRange = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "LifeCenter")
		{
			text.ForwardPositionToNextWord();
			m_LifeCenter = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "LifeRange")
		{
			text.ForwardPositionToNextWord();
			m_LifeRenge = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "StartRange")
		{
			text.ForwardPositionToNextWord();
			m_StartRange = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "NumParticle")
		{
			text.ForwardPositionToNextWord();
			m_nNumParticle = std::stoi(text.GetWord());
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
void ParticleRenderer::Save(Text& text)
{
	StartSave(text);

	text += "Pass " + std::to_string(m_nPass) + '\n';
	m_pMaterial->Save(text);
	text += "Color " 
		+ std::to_string(m_Color.r) + ' '
		+ std::to_string(m_Color.g) + ' '
		+ std::to_string(m_Color.b) + ' '
		+ std::to_string(m_Color.a) + '\n';
	text += "AlphaRange " + std::to_string(m_AlphaRange) + "\n";
	text += "PosRange " + m_PosRange.ConvertToString() + "\n";
	text += "SizeCenter " + std::to_string( m_SizeCenter) + "\n";
	text += "SizeRange " + std::to_string( m_SizeRange) + "\n";
	text += "DirCenter " + m_DirCenter.ConvertToString() + "\n";
	text += "DirRange " + m_DirRange.ConvertToString() + "\n";
	text += "SpeedCenter " + std::to_string( m_SpeedCenter) + "\n";
	text += "SpeedRange " + std::to_string( m_SpeedRange) + "\n";
	text += "LifeCenter " + std::to_string( m_LifeCenter) + "\n";
	text += "LifeRange " + std::to_string( m_LifeRenge) + "\n";
	text += "StartRange " + std::to_string( m_StartRange) + "\n";
	text += "NumParticle " + std::to_string( m_nNumParticle) + "\n";
	
	EndSave( text);
}
