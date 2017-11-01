/*==============================================================================

    ParticleRenderer.cpp - 3Dポリゴン描画
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/17
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "ParticleRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Material.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
ParticleRenderer::ParticleRenderer( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerAddBlend;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_nPass = 0;

	//初期値代入
	m_bNeedUpdateVtx = false;
	m_nNumParticle = 1024;		//☆パーティクルの総数
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

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//頂点バッファ生成
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_PARTICLE) * m_nNumParticle, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
	{
		//エラー
		assert( false);
		return;
	}

	//頂点設定
	SetVtxBuffer();

	//マテリアル
	m_pMaterial = new Material();
	m_pMaterial->SetShader( eShaderParticle);
	
	//加算合成
	m_pMaterial->SetTexture("data/TEXTURE/particle00.jpg");
	m_nPass = 2;
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void ParticleRenderer::Uninit( void)
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
void ParticleRenderer::Update( void)
{
	//頂点バッファの更新
	if (m_bNeedUpdateVtx == true)
	{
		m_bNeedUpdateVtx = false;

		//頂点設定
		SetVtxBuffer();
	}
}

/*------------------------------------------------------------------------------
	描画
------------------------------------------------------------------------------*/
void ParticleRenderer::Draw( Camera* pCamera)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//マテリアル（シェーダー）をセット
	m_pMaterial->Set( pCamera, this);

	//頂点情報設定
	pDevice-> SetStreamSource( 0, m_pVtxBuff, 0, sizeof( VERTEX_PARTICLE));

	//テクニック開始
	m_pMaterial->Begin( m_nPass);

	//ポリゴンの描画
	pDevice->DrawPrimitive( D3DPT_POINTLIST, 0,	m_nNumParticle);

	//テクニック終了
	m_pMaterial->End();
}

/*------------------------------------------------------------------------------
	頂点バッファ設定
------------------------------------------------------------------------------*/
void ParticleRenderer::SetVtxBuffer( void)
{
	//ロック
	VERTEX_PARTICLE* pVtx;		//仮想アドレス用ポインタ
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

	//アンロック
	m_pVtxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	パーティクルの総数設定
------------------------------------------------------------------------------*/
void ParticleRenderer::SetNumParticle(int NumParticle)
{
	if (NumParticle <= 0)
	{
		assert( false);
		return;
	}

	//総数の設定
	m_nNumParticle = NumParticle;

	//バッファの再生成
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();
	SAFE_RELEASE( m_pVtxBuff);
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_PARTICLE) * m_nNumParticle, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
	{
		//エラー
		assert( false);
		return;
	}

	//頂点設定
	SetVtxBuffer();
	m_bNeedUpdateVtx = false;
}

/*------------------------------------------------------------------------------
	テクスチャ設定
------------------------------------------------------------------------------*/
void ParticleRenderer::LoadTexture(std::string FileName)
{
	m_pMaterial->SetTexture( FileName);
}

/*------------------------------------------------------------------------------
	シェーダー設定
------------------------------------------------------------------------------*/
void ParticleRenderer::SetShader(EShaderType Type)
{
	m_pMaterial->SetShader( Type);
}
