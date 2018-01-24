/*==============================================================================

    WallRenderer.cpp - 3Dポリゴン描画
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/17
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "WallRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Material.h"


/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* WallRenderer::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<WallRenderer>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
WallRenderer::WallRenderer( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerDefault;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_nPass = 1;

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	m_pVtxBuff = NULL;

	//マテリアル
	m_pMaterial = new Material();

	m_CountVertex = 0;
	m_CountPolygon = 0;
	m_CountRenderPolygon = 0;
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void WallRenderer::Uninit( void)
{
	Manager::GetRenderManager()->ReleaseRenderer( this);

	//頂点バッファの解放
	if( m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//マテリアルの解放
	if (m_pMaterial != NULL)
	{
		delete m_pMaterial;
		m_pMaterial = NULL;
	}
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void WallRenderer::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	描画
------------------------------------------------------------------------------*/
void WallRenderer::Draw( Camera* pCamera)
{
	if (!m_pVtxBuff)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//マテリアル（シェーダー）をセット
	m_pMaterial->Set( pCamera, this);

	//頂点情報設定
	pDevice-> SetStreamSource( 0, m_pVtxBuff, 0, sizeof( VERTEX_3D));

	//テクニック開始
	m_pMaterial->Begin( m_nPass);

	//ポリゴンの描画
	//pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, m_CountPolygon);
	pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, m_CountRenderPolygon);

	//テクニック終了
	m_pMaterial->End();
}

/*------------------------------------------------------------------------------
	シェーダー設定
------------------------------------------------------------------------------*/
void WallRenderer::SetShader(EShaderType Type)
{
	m_pMaterial->SetShader( Type);
}

/*------------------------------------------------------------------------------
	テクスチャ設定
------------------------------------------------------------------------------*/
void WallRenderer::LoadTexture(std::string fileName)
{
	m_pMaterial->SetTexture( fileName);
}

/*------------------------------------------------------------------------------
	頂点バッファの設定を開始
------------------------------------------------------------------------------*/
VERTEX_3D* WallRenderer::StartSetVertexBuffer( int countVertex, int countPolygon)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	SAFE_RELEASE( m_pVtxBuff);

	m_CountVertex = countVertex;
	m_CountPolygon = countPolygon;
	m_CountRenderPolygon = countPolygon;

	//頂点バッファを生成
	if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_3D) * countVertex, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
	{
		//エラー
		assert( false);
		return NULL;
	}

	//ロック
	VERTEX_3D* pVtx;
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx, 0);

	return pVtx;
}

/*------------------------------------------------------------------------------
	頂点バッファの設定を終了
------------------------------------------------------------------------------*/
void WallRenderer::EndSetVertexBuffer(void)
{
	//アンロック
	m_pVtxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	描画するポリゴンのOnOffを変更
	Maxならtrue
------------------------------------------------------------------------------*/
bool WallRenderer::ChangeRenderPolygon()
{
	if (m_CountRenderPolygon > 0)
	{
		m_CountRenderPolygon = 0;
		return false;
	}
	
	m_CountRenderPolygon = m_CountPolygon;
	return true;
}

/*------------------------------------------------------------------------------
	描画するポリゴンを加算
	Maxならtrue
------------------------------------------------------------------------------*/
bool WallRenderer::AddRenderPolygon()
{
	m_CountRenderPolygon += 6;

	if (m_CountRenderPolygon > m_CountPolygon)
	{
		m_CountRenderPolygon = m_CountPolygon;
		return true;
	}

	return false;
}


