/*==============================================================================

    ShaderManager.cpp - シェーダー管理
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/8/7
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "ShaderManager.h"
#include "Shader.h"
#include "DefaultShader.h"
#include "BillboardShader.h"
#include "PerPixelLightingShader.h"
#include "VertexLightingShader.h"
#include "SpriteShader.h"
#include "DepthShader.h"
#include "ShadowVLShader.h"
#include "ParticleShader.h"
#include "SkinMeshShader.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
ShaderManager::ShaderManager()
{
	//シェーダーのロード
	m_vecShaderSet.clear();
	m_vecShaderSet.resize( eNumShader);
	for (int nCnt = 0; nCnt < eNumShader; nCnt++)
	{
		m_vecShaderSet[ nCnt] = NULL;
	}

	//シェーダーの読み込み
	//デフォルト
	m_mapShaderLoad[ "def_Default"] = new DefaultShader();
	m_mapShaderLoad[ "def_Billboard"] = new BillboardShader();
	m_mapShaderLoad[ "def_XModel"] = new VertexLightingShader();
	m_mapShaderLoad[ "def_Sprite"] = new SpriteShader();
	m_mapShaderLoad[ "particle"] = new ParticleShader();
	m_mapShaderLoad[ "skinmesh"] = new SkinMeshShader();

	//影あり
	m_mapShaderLoad[ "shadowVL"] = new ShadowVLShader();

	//デプス情報設定
	m_mapShaderLoad[ "depth"] = new DepthShader();

	//シェーダーセット
	SetDefault();
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
ShaderManager::~ShaderManager()
{
	for (auto Shader : m_mapShaderLoad)
	{
		 delete Shader.second;
	}
	m_mapShaderLoad.clear();

	m_vecShaderSet.clear();
}

/*------------------------------------------------------------------------------
	シェーダーをロード
	引数
	EShaderType Type
	戻り値
	Shader *
------------------------------------------------------------------------------*/
Shader *ShaderManager::Load( EShaderType Type)
{
	//範囲をチェック
	if (Type < 0 || Type >= m_vecShaderSet.capacity())
	{
		assert(false);
		return NULL;
	}

	//ロードされているか
	if (m_vecShaderSet[Type] == NULL)
	{
		MessageBox( NULL, "そのシェーダーはロードされていません。\nShaderManager.cppのコンストラクタに追加してください\n", "エラー", MB_OK);
		return NULL;
	}

	return m_vecShaderSet[ Type];
}

/*------------------------------------------------------------------------------
	シェーダー切り替え（デフォルト）
------------------------------------------------------------------------------*/
void ShaderManager::SetDefault(void)
{
	//m_vecShaderSet[ eShaderDefault]		= m_mapShaderLoad[ "def_Default"];
	m_vecShaderSet[ eShaderDefault]		= m_mapShaderLoad[ "shadowVL"];
	//m_vecShaderSet[ eShaderXModel]		= m_mapShaderLoad[ "def_XModel"];
	m_vecShaderSet[ eShaderXModel]		= m_mapShaderLoad[ "shadowVL"];
	
	m_vecShaderSet[ eShaderBillboard]	= m_mapShaderLoad[ "def_Billboard"];
	m_vecShaderSet[ eShaderSprite]		= m_mapShaderLoad[ "def_Sprite"];
	m_vecShaderSet[ eShaderParticle]	= m_mapShaderLoad[ "particle"];
	m_vecShaderSet[ eShaderSky]			= m_mapShaderLoad[ "def_Default"];

	m_vecShaderSet[ eShaderSkinMesh]	= m_mapShaderLoad[ "skinmesh"];
}

/*------------------------------------------------------------------------------
	シェーダー切り替え（影あり）
------------------------------------------------------------------------------*/
void ShaderManager::SetOnShadow(void)
{
	/*m_vecShaderSet[ eShaderDefault]		= m_mapShaderLoad[ "shadow_Default"];
	m_vecShaderSet[ eShaderBillboard]	= m_mapShaderLoad[ "shadow_Billboard"];
	m_vecShaderSet[ eShaderXModel]		= m_mapShaderLoad[ "shadow_XModel"];
	m_vecShaderSet[ eShaderSprite]		= m_mapShaderLoad[ "shadow_Sprite"];*/
}

/*------------------------------------------------------------------------------
	シェーダー切り替え（Zバッファ設定）
------------------------------------------------------------------------------*/
void ShaderManager::SetDepth(void)
{
	m_vecShaderSet[ eShaderDefault]		= m_mapShaderLoad[ "depth"];
	m_vecShaderSet[ eShaderBillboard]	= m_mapShaderLoad[ "depth"];
	m_vecShaderSet[ eShaderXModel]		= m_mapShaderLoad[ "depth"];
	m_vecShaderSet[ eShaderSkinMesh]	= m_mapShaderLoad[ "depth"];
}

