/*==============================================================================

    TextureManager.h - テクスチャ管理
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/4/1
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "TextureManager.h"
#include "Texture.h"
#include "RenderTexture.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
TextureManager::TextureManager()
{
	//デフォルトテクスチャ読み込み
	m_DefaultTexture = Load( DEFAULT_TEXTURE_FILE_NAME);
}

/*------------------------------------------------------------------------------
	デストラクタ
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
	テクスチャロード
	引数
	const char* pFileName
	戻り値
	Texture *
------------------------------------------------------------------------------*/
Texture *TextureManager::Load( std::string FileName)
{
	//すでに読み込まれているか
	auto iteTexture = m_mapTexture.find( FileName);
	if (iteTexture != m_mapTexture.end())
	{
		return m_mapTexture[FileName];
	}

	//ロード
	Texture *pTexture = new Texture( FileName);
	m_mapTexture[FileName] = pTexture;
	return pTexture;
}

/*------------------------------------------------------------------------------
	レンダーテクスチャを生成
------------------------------------------------------------------------------*/
RenderTexture *TextureManager::CreateRenderTexture( std::string TextureName, bool isBuckBuffer)
{
	//すでに作成されているか
	auto iteTexture = m_mapTexture.find( TextureName);
	if (iteTexture != m_mapTexture.end())
	{
		return (RenderTexture*)m_mapTexture[TextureName];
	}

	//レンダーテクスチャ生成
	RenderTexture* pRenderTexture = new RenderTexture( isBuckBuffer);
	m_mapTexture[ TextureName] = pRenderTexture;
	return pRenderTexture;
}
