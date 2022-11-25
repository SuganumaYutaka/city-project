/*==============================================================================

    XModelManager.cpp - Xモデル管理
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/7/5
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "XModelManager.h"
#include "XModelData.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
XModelManager::XModelManager()
{
	
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
XModelManager::~XModelManager()
{
	for (auto itr = m_Map.begin(); itr != m_Map.end(); ++itr)
	{
		auto pXModelData = itr->second;
		delete pXModelData;
	}
	m_Map.clear();
}

/*------------------------------------------------------------------------------
	テクスチャロード
	引数
	const char* pFileName
	戻り値
	XModelData *
------------------------------------------------------------------------------*/
XModelData *XModelManager::Load( std::string FileName)
{
	//すでに読み込まれているか
	auto iteTexture = m_Map.find( FileName);
	if (iteTexture != m_Map.end())
	{
		return m_Map[FileName];
	}

	//ロード
	XModelData *pXModelData = new XModelData( FileName);
	m_Map[FileName] = pXModelData;
	return pXModelData;
}

