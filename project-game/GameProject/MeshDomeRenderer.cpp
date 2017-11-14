/*==============================================================================

   MeshDomeRenderer.h - ���ʕ`��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/24
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "MeshDomeRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Material.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define P_ANGLE (D3DX_PI * 2.0f / NUM_DOME_X)		//�|���S���ꖇ������̊p�x
#define P_HEIGHT (ALL_HEIGHT / NUM_DOME_Y)			//�|���S���ꖇ������̍���
#define NUM_POLYGON( NUM_DOME_X, NUM_DOME_Y) ( 2 * NUM_DOME_X * NUM_DOME_Y + ( NUM_DOME_Y - 1) * 4)		//�|���S����
#define NUM_VERTEX( NUM_DOME_X, NUM_DOME_Y)	( ( NUM_DOME_X + 1) * ( NUM_DOME_Y + 1) + 2)				//���_��
#define NUM_INDEX( NUM_DOME_X, NUM_DOME_Y) ( ( (NUM_DOME_X + 1) * 2 + 2) * NUM_DOME_Y - 2)				//�C���f�b�N�X��

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* MeshDomeRenderer::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<MeshDomeRenderer>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
MeshDomeRenderer::MeshDomeRenderer( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerBack;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_nPass = 0;

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾

	//�F�̐ݒ�
	m_Color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);

	//�f�t�H���g�l�ݒ�
	m_nNumBlockX = 8;			//���̕�����
	m_nNumBlockY = 8;			//�c�̕�����
	m_fRadius = 50.0f;			//���a
	m_fHeight = 50.0f;			//����
	m_fBlockAngle = D3DX_PI * 2.0f / m_nNumBlockX;
	m_fBlockHeight = m_fHeight / m_nNumBlockY;
	
	//���_�o�b�t�@����
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_3D) * NUM_VERTEX( m_nNumBlockX, m_nNumBlockY),				//�쐬���������_�o�b�t�@�̃T�C�Y
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
		sizeof( WORD) * NUM_INDEX( m_nNumBlockX, m_nNumBlockY),		 //�T�C�Y�iWORD or DWORD�j*�C���f�b�N�X��
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
void MeshDomeRenderer::Uninit( void)
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
void MeshDomeRenderer::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	�`��
------------------------------------------------------------------------------*/
void MeshDomeRenderer::Draw( Camera* pCamera)
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
		NUM_VERTEX( m_nNumBlockX, m_nNumBlockY), 0,	NUM_POLYGON( m_nNumBlockX, m_nNumBlockY));

	//�e�N�j�b�N�I��
	m_pMaterial->End();
}

/*------------------------------------------------------------------------------
	���_�o�b�t�@�ݒ�
------------------------------------------------------------------------------*/
void MeshDomeRenderer::SetVtxBuffer( void)
{
	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	VERTEX_3D* pVtx;				//���z�A�h���X�p�|�C���^
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx,0);

	int nCntHeight = 0;				//���ăJ�E���^
	int nCnt = 0;					//�~���J�E���^
	D3DXVECTOR3 normal;				//���_����̃x�N�g��
	float fAngle;					//�p�x
	float fRadius;					//���a
	float fHeight;					//����

	//�|���S���̐ݒ�
	for( nCntHeight = 0; nCntHeight < m_nNumBlockY + 1; nCntHeight++)
	{
		//���a�̍X�V
		fRadius = 0.0f + m_fRadius * cosf( D3DX_PI * 0.5f * (m_nNumBlockY - nCntHeight) / m_nNumBlockY);

		//�����̍X�V
		fHeight = m_fHeight * sinf( D3DX_PI * 0.5f * (m_nNumBlockY - nCntHeight) / m_nNumBlockY);

		for( nCnt = 0; nCnt < m_nNumBlockX + 1; nCnt++)
		{
			//���_���W�̐ݒ�
			fAngle = m_fBlockAngle * nCnt;
			//fAngle = P_ANGLE * ( NUM_DOME_X - nCnt);
			pVtx[ 0].Pos = D3DXVECTOR3( fRadius * -cosf( fAngle), fHeight, fRadius * sinf( fAngle));

			//�@���̐ݒ�
			D3DXVec3Normalize( &normal, &pVtx[ 0].Pos);
			pVtx[ 0].Normal = -normal;
			
			//���_�J���[�̐ݒ�i0�`255�̐����l�j
			pVtx[ 0].Color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);
			
			//UV�f�[�^�̐ݒ�
			pVtx[ 0].Tex = D3DXVECTOR2( 1.0f / m_nNumBlockX * nCnt, 1.0f / m_nNumBlockY * nCntHeight);

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
void MeshDomeRenderer::SetIdxBuffer(void)
{
	//���_�o�b�t�@�����b�N���āA���z�A�h���X���擾����
	WORD* pIdx;		//�C���f�b�N�X���
	m_pIdxBuff->Lock( 0, 0, (void**)&pIdx, 0);

	//�ϐ��錾
	int nCntHeight = 0;		//���ăJ�E���^
	int nCntWidth = 0;		//�悱�J�E���^

	//�C���f�b�N�X���̐ݒ�
	for( nCntHeight = 0; nCntHeight < m_nNumBlockY; nCntHeight++)
	{
		//�\������|���S���̃C���f�b�N�X���̐ݒ�
		for( nCntWidth = 0; nCntWidth < m_nNumBlockX + 1; nCntWidth++)
		{
			pIdx[ 0] = ( nCntHeight + 1) * ( m_nNumBlockX + 1) + nCntWidth;
			pIdx[ 1] = ( nCntHeight + 0) * ( m_nNumBlockX + 1) + nCntWidth;

			pIdx += 2;
		}

		//�Ō�͏k�ރ|���S���s�v
		if( nCntHeight == m_nNumBlockY - 1)
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
	�e�N�X�`���ݒ�
------------------------------------------------------------------------------*/
void MeshDomeRenderer::LoadTexture(std::string FileName)
{
	m_pMaterial->SetTexture( FileName);
}

/*------------------------------------------------------------------------------
	�V�F�[�_�[�ݒ�
------------------------------------------------------------------------------*/
void MeshDomeRenderer::SetShader(EShaderType Type)
{
	m_pMaterial->SetShader( Type);
}

/*------------------------------------------------------------------------------
	�h�[���̐ݒ�
------------------------------------------------------------------------------*/
void MeshDomeRenderer::SetDome(int X, int Y, float Radius, float Height)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//�f�o�C�X�擾
	
	//�f�t�H���g�l�ݒ�
	m_nNumBlockX = X;			//���̕�����
	m_nNumBlockY = Y;			//�c�̕�����
	m_fRadius = Radius;			//���a
	m_fHeight = Height;			//����
	m_fBlockAngle = D3DX_PI * 2.0f / m_nNumBlockX;
	m_fBlockHeight = m_fHeight / m_nNumBlockY;

	//���_�o�b�t�@�̉��
	SAFE_RELEASE( m_pVtxBuff);

	//���_�o�b�t�@����
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_3D) * NUM_VERTEX( m_nNumBlockX, m_nNumBlockY),				//�쐬���������_�o�b�t�@�̃T�C�Y
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

	//�C���f�b�N�X�o�b�t�@�̉��
	SAFE_RELEASE( m_pIdxBuff);

	//�C���f�b�N�X�o�b�t�@�̍쐬
	HRESULT hr;
	hr = pDevice->CreateIndexBuffer(
		sizeof( WORD) * NUM_INDEX( m_nNumBlockX, m_nNumBlockY),		 //�T�C�Y�iWORD or DWORD�j*�C���f�b�N�X��
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
void MeshDomeRenderer::SetBlockSize(float BlockAngle, float BlockHeight)
{
	m_fBlockAngle = D3DX_PI * 2.0f / m_nNumBlockX;
	m_fBlockHeight = BlockHeight;
	m_fHeight = m_nNumBlockY * BlockHeight;
	
	//���_�o�b�t�@�ݒ�
	SetVtxBuffer();
}

/*------------------------------------------------------------------------------
	���E������ݒ�
------------------------------------------------------------------------------*/
void MeshDomeRenderer::SetSize(float Radius, float Height)
{
	m_fRadius = Radius;
	m_fHeight = Height;
	m_fBlockHeight = Height / m_nNumBlockY;

	//���_�o�b�t�@�ݒ�
	SetVtxBuffer();
}

/*------------------------------------------------------------------------------
	���[�h
------------------------------------------------------------------------------*/
void MeshDomeRenderer::Load(Text& text)
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
		else if (text.GetWord() == "NumBlockX")
		{
			text.ForwardPositionToNextWord();
			m_nNumBlockX = std::stoi(text.GetWord());
		}
		else if (text.GetWord() == "NumBlockY")
		{
			text.ForwardPositionToNextWord();
			m_nNumBlockY = std::stoi(text.GetWord());
		}
		else if (text.GetWord() == "Radius")
		{
			text.ForwardPositionToNextWord();
			m_fRadius = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "Height")
		{
			text.ForwardPositionToNextWord();
			m_fHeight = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "BlockAngle")
		{
			text.ForwardPositionToNextWord();
			m_fBlockAngle = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "BlockHeight")
		{
			text.ForwardPositionToNextWord();
			m_fBlockHeight = std::stof(text.GetWord());
		}

		//text��ǂݐi�߂�
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}
	SetVtxBuffer();
	SetIdxBuffer();
}

/*------------------------------------------------------------------------------
	�Z�[�u
------------------------------------------------------------------------------*/
void MeshDomeRenderer::Save(Text& text)
{
	StartSave(text);

	text += "Pass " + std::to_string(m_nPass) + '\n';
	m_pMaterial->Save(text);
	text += "Color " 
		+ std::to_string(m_Color.r) + ' '
		+ std::to_string(m_Color.g) + ' '
		+ std::to_string(m_Color.b) + ' '
		+ std::to_string(m_Color.a) + '\n';
	text += "NumBlockX " + std::to_string(m_nNumBlockX) + ' ';
	text += "NumBlockY " + std::to_string(m_nNumBlockY) + ' ';
	text += "Radius " + std::to_string(m_fRadius) + ' ';
	text += "Height " + std::to_string(m_fHeight) + ' ';
	text += "BlockAngle " + std::to_string(m_fBlockAngle) + ' ';
	text += "BlockHeight " + std::to_string(m_fBlockHeight) + '\n';
	EndSave( text);
}
