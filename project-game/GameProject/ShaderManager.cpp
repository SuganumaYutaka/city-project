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
#include "ShadowPLShader.h"
#include "ParticleShader.h"
#include "SkinMeshShader.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
ShaderManager::ShaderManager() : m_CurrentShader( NULL)
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
	m_mapShaderLoad[ "default"] = new DefaultShader();
	m_mapShaderLoad[ "vertexLighting"] = new VertexLightingShader();
	m_mapShaderLoad[ "perpixel"] = new PerPixelLightingShader();
	m_mapShaderLoad[ "billboard"] = new BillboardShader();
	m_mapShaderLoad[ "sprite"] = new SpriteShader();
	m_mapShaderLoad[ "particle"] = new ParticleShader();
	m_mapShaderLoad[ "skinmesh"] = new SkinMeshShader();
	m_mapShaderLoad[ "shadowVL"] = new ShadowVLShader();
	m_mapShaderLoad[ "shadowPL"] = new ShadowPLShader();
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
	シェーダーをセット
------------------------------------------------------------------------------*/
void ShaderManager::SetShader( Camera* pCamera, Renderer* pRenderer, Material* pMaterial, EShaderType Type)
{
	auto shader = Load( Type);
	if (m_CurrentShader == shader)
	{
		shader->Set( pCamera, pRenderer, pMaterial, true);
	}
	else
	{
		shader->Set( pCamera, pRenderer, pMaterial, false);
		m_CurrentShader = shader;
	}
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
	if (Type < 0 || Type >= (int)m_vecShaderSet.capacity())
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
	m_vecShaderSet[ eShaderDefault]		= m_mapShaderLoad[ "vertexLighting"];
	m_vecShaderSet[ eShaderXModel]		= m_mapShaderLoad[ "perpixel"];
	m_vecShaderSet[ eShaderBillboard]	= m_mapShaderLoad[ "billboard"];
	m_vecShaderSet[ eShaderSkinMesh]	= m_mapShaderLoad[ "skinmesh"];
	m_vecShaderSet[ eShaderSprite]		= m_mapShaderLoad[ "sprite"];
	m_vecShaderSet[ eShaderParticle]	= m_mapShaderLoad[ "particle"];
	m_vecShaderSet[ eShaderSky]			= m_mapShaderLoad[ "default"];
	
}

/*------------------------------------------------------------------------------
	シェーダー切り替え（低品質）
------------------------------------------------------------------------------*/
void ShaderManager::SetLow(void)
{
	m_vecShaderSet[ eShaderDefault]		= m_mapShaderLoad[ "default"];
	m_vecShaderSet[ eShaderXModel]		= m_mapShaderLoad[ "default"];
	m_vecShaderSet[ eShaderBillboard]	= m_mapShaderLoad[ "billboard"];
	m_vecShaderSet[ eShaderSkinMesh]	= m_mapShaderLoad[ "skinmesh"];
}

/*------------------------------------------------------------------------------
	シェーダー切り替え（高品質）
------------------------------------------------------------------------------*/
void ShaderManager::SetHigh(void)
{
	m_vecShaderSet[ eShaderDefault]		= m_mapShaderLoad[ "shadowPL"];
	m_vecShaderSet[ eShaderXModel]		= m_mapShaderLoad[ "shadowPL"];
	m_vecShaderSet[ eShaderBillboard]	= m_mapShaderLoad[ "billboard"];
	m_vecShaderSet[ eShaderSkinMesh]	= m_mapShaderLoad[ "skinmesh"];
}

/*------------------------------------------------------------------------------
	シェーダー切り替え（Zバッファ設定）
------------------------------------------------------------------------------*/
void ShaderManager::SetDepth(void)
{
	m_vecShaderSet[ eShaderDefault]		= m_mapShaderLoad[ "depth"];
	m_vecShaderSet[ eShaderXModel]		= m_mapShaderLoad[ "depth"];
	m_vecShaderSet[ eShaderBillboard]	= m_mapShaderLoad[ "depth"];
	m_vecShaderSet[ eShaderSkinMesh]	= m_mapShaderLoad[ "depth"];
}

