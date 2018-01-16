/*==============================================================================

    RenderTexture.cpp - �����_�[�e�N�X�`��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/8
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "RenderTexture.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
//#define TEXTURE_WIDTH (1024)
//#define TEXTURE_HEIGHT (1024)
#define TEXTURE_WIDTH (2048)
#define TEXTURE_HEIGHT (2048)

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
RenderTexture::RenderTexture( bool isBuckBuffer)
{
	auto pDevice = Manager::GetDevice();

	//�o�b�N�o�b�t�@���ǂ���
	m_bBuckBuffer = isBuckBuffer;

	//�o�b�N�o�b�t�@�̏ꍇ
	if (isBuckBuffer == true)
	{
		m_pTexture = NULL;
		pDevice->GetRenderTarget( 0, &m_pTexSurface);
		pDevice->GetDepthStencilSurface( &m_pTexZ);
		return;
	}

	//�����_�[�e�N�X�`���̍쐬
	HRESULT hr;
	hr = pDevice->CreateTexture(
		SCREEN_WIDTH, SCREEN_HEIGHT,		//�e�N�X�`���T�C�Y
		1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT,
		&m_pTexture, NULL);
	if (hr == E_FAIL)
	{
		MessageBox( NULL, "�����_�[�e�N�X�`���̍쐬�Ɏ��s���܂���\n", "�G���[", MB_OK);
		return;
	}

	//�e�N�X�`���T�[�t�F�X�̎擾
	m_pTexture->GetSurfaceLevel( 0, &m_pTexSurface);
	if (hr == E_FAIL)
	{
		MessageBox( NULL, "�e�N�X�`���T�[�t�F�X�̎擾�Ɏ��s���܂���\n", "�G���[", MB_OK);
		return;
	}

	//�[�x�o�b�t�@�̍쐬
	hr = pDevice->CreateDepthStencilSurface(
		SCREEN_WIDTH, SCREEN_HEIGHT,
		D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE,
		&m_pTexZ, NULL);
	if (hr == E_FAIL)
	{
		MessageBox( NULL, "�[�x�o�b�t�@�̍쐬�Ɏ��s���܂���\n", "�G���[", MB_OK);
		return;
	}

	//�e�N�X�`���T�C�Y�̐ݒ�
	m_Size.x = SCREEN_WIDTH;
	m_Size.y = SCREEN_HEIGHT;
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
RenderTexture::~RenderTexture()
{
	if( m_bBuckBuffer == false)
	{
		SAFE_RELEASE( m_pTexture);
		SAFE_RELEASE( m_pTexSurface);
		SAFE_RELEASE( m_pTexZ);
	}
}

/*------------------------------------------------------------------------------
	�[�x�o�b�t�@�`��p�t�H�[�}�b�g�ɕύX
------------------------------------------------------------------------------*/
bool RenderTexture::ChangeDepthRenderFormat( void)
{
	//�e�N�X�`���̔j��
	SAFE_RELEASE( m_pTexture);
	SAFE_RELEASE( m_pTexSurface);
	SAFE_RELEASE( m_pTexZ);

	auto pDevice = Manager::GetDevice();

	//�����_�[�e�N�X�`���̍쐬
	HRESULT hr;
	hr = pDevice->CreateTexture(
		TEXTURE_WIDTH, TEXTURE_HEIGHT,		//�e�N�X�`���T�C�Y
		1, D3DUSAGE_RENDERTARGET, D3DFMT_R32F, D3DPOOL_DEFAULT,
		&m_pTexture, NULL);
	if (hr == E_FAIL)
	{
		MessageBox( NULL, "�����_�[�e�N�X�`���̍쐬�Ɏ��s���܂���\n", "�G���[", MB_OK);
		return false;
	}

	//�e�N�X�`���T�[�t�F�X�̎擾
	m_pTexture->GetSurfaceLevel( 0, &m_pTexSurface);
	if (hr == E_FAIL)
	{
		MessageBox( NULL, "�e�N�X�`���T�[�t�F�X�̎擾�Ɏ��s���܂���\n", "�G���[", MB_OK);
		return false;
	}

	//�[�x�o�b�t�@�̍쐬
	hr = pDevice->CreateDepthStencilSurface(
		TEXTURE_WIDTH, TEXTURE_HEIGHT,
		D3DFMT_D24S8, D3DMULTISAMPLE_NONE, 0, FALSE,
		&m_pTexZ, NULL);
	if (hr == E_FAIL)
	{
		MessageBox( NULL, "�[�x�o�b�t�@�̍쐬�Ɏ��s���܂���\n", "�G���[", MB_OK);
		return false;
	}

	//�e�N�X�`���T�C�Y�̐ݒ�
	m_Size.x = TEXTURE_WIDTH;
	m_Size.y = TEXTURE_HEIGHT;

	return true;
}