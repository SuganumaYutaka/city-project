/*==============================================================================

    TextureManager.h - �e�N�X�`���Ǘ�
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/4/1
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "TextureManager.h"
#include "Texture.h"
#include "RenderTexture.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
TextureManager::TextureManager()
{
	//�f�t�H���g�e�N�X�`���ǂݍ���
	m_DefaultTexture = Load( DEFAULT_TEXTURE_FILE_NAME);
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
TextureManager::~TextureManager()
{
	for (auto itr = m_mapTexture.begin(); itr != m_mapTexture.end(); ++itr)
	{
		Texture *pTexture = itr->second;
		delete pTexture;
	}
	m_mapTexture.clear();
}

/*------------------------------------------------------------------------------
	�e�N�X�`�����[�h
	����
	const char* pFileName
	�߂�l
	Texture *
------------------------------------------------------------------------------*/
Texture *TextureManager::Load( std::string FileName)
{
	//���łɓǂݍ��܂�Ă��邩
	auto iteTexture = m_mapTexture.find( FileName);
	if (iteTexture != m_mapTexture.end())
	{
		return m_mapTexture[FileName];
	}

	//���[�h
	Texture *pTexture = new Texture( FileName);
	m_mapTexture[FileName] = pTexture;
	return pTexture;
}

/*------------------------------------------------------------------------------
	�����_�[�e�N�X�`���𐶐�
------------------------------------------------------------------------------*/
RenderTexture *TextureManager::CreateRenderTexture( std::string TextureName, bool isBuckBuffer)
{
	//���łɍ쐬����Ă��邩
	auto iteTexture = m_mapTexture.find( TextureName);
	if (iteTexture != m_mapTexture.end())
	{
		return (RenderTexture*)m_mapTexture[TextureName];
	}

	//�����_�[�e�N�X�`������
	RenderTexture* pRenderTexture = new RenderTexture( isBuckBuffer);
	m_mapTexture[ TextureName] = pRenderTexture;
	return pRenderTexture;
}
