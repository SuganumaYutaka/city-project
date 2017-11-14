/*==============================================================================

   MeshDomeRenderer.h - 平面描画
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/24
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "MeshDomeRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Material.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define P_ANGLE (D3DX_PI * 2.0f / NUM_DOME_X)		//ポリゴン一枚あたりの角度
#define P_HEIGHT (ALL_HEIGHT / NUM_DOME_Y)			//ポリゴン一枚あたりの高さ
#define NUM_POLYGON( NUM_DOME_X, NUM_DOME_Y) ( 2 * NUM_DOME_X * NUM_DOME_Y + ( NUM_DOME_Y - 1) * 4)		//ポリゴン数
#define NUM_VERTEX( NUM_DOME_X, NUM_DOME_Y)	( ( NUM_DOME_X + 1) * ( NUM_DOME_Y + 1) + 2)				//頂点数
#define NUM_INDEX( NUM_DOME_X, NUM_DOME_Y) ( ( (NUM_DOME_X + 1) * 2 + 2) * NUM_DOME_Y - 2)				//インデックス数

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* MeshDomeRenderer::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<MeshDomeRenderer>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
MeshDomeRenderer::MeshDomeRenderer( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerBack;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_nPass = 0;

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//色の設定
	m_Color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);

	//デフォルト値設定
	m_nNumBlockX = 8;			//横の分割数
	m_nNumBlockY = 8;			//縦の分割数
	m_fRadius = 50.0f;			//半径
	m_fHeight = 50.0f;			//高さ
	m_fBlockAngle = D3DX_PI * 2.0f / m_nNumBlockX;
	m_fBlockHeight = m_fHeight / m_nNumBlockY;
	
	//頂点バッファ生成
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_3D) * NUM_VERTEX( m_nNumBlockX, m_nNumBlockY),				//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,					//頂点バッファの使用方法(速さに影響)
		0,									//FVF(頂点フォーマット)
		D3DPOOL_MANAGED,					//メモリの管理(MANAGEDはデバイスにおまかせ)
		&m_pVtxBuff,						//頂点バッファ管理インターフェイス
		NULL)))
	{
		//エラー
		assert( false);
		return;
	}

	//頂点バッファ設定
	SetVtxBuffer();

	//インデックスバッファの作成
	HRESULT hr;
	hr = pDevice->CreateIndexBuffer(
		sizeof( WORD) * NUM_INDEX( m_nNumBlockX, m_nNumBlockY),		 //サイズ（WORD or DWORD）*インデックス数
		D3DUSAGE_WRITEONLY,				 //使用用途フラグ
		D3DFMT_INDEX16,					 //インデックスデータのフォーマット（16 or 32）
		D3DPOOL_MANAGED,				 //メモリの管理方法（おまかせ）
		&m_pIdxBuff,					 //インデックスバッファインターフェイスポインタのアドレス
		NULL);

	if( FAILED( hr))
	{
		//エラー
		assert( false);
		return;
	}

	//インデックスバッファ設定
	SetIdxBuffer();

	//マテリアル
	m_pMaterial = new Material();
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void MeshDomeRenderer::Uninit( void)
{
	Manager::GetRenderManager()->ReleaseRenderer( this);

	//頂点バッファの解放
	if( m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//インデックスバッファの解放
	if( m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
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
void MeshDomeRenderer::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	描画
------------------------------------------------------------------------------*/
void MeshDomeRenderer::Draw( Camera* pCamera)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//マテリアル（シェーダー）をセット
	m_pMaterial->Set( pCamera, this);

	//頂点情報設定
	pDevice-> SetStreamSource( 0, m_pVtxBuff, 0, sizeof( VERTEX_3D));

	//インデックスバッファの設定
	pDevice->SetIndices( m_pIdxBuff);

	//テクニック開始
	m_pMaterial->Begin( m_nPass);

	//プリミティブ（ポリゴン・図形）の描画
	pDevice->DrawIndexedPrimitive( D3DPT_TRIANGLESTRIP,	0, 0, 
		NUM_VERTEX( m_nNumBlockX, m_nNumBlockY), 0,	NUM_POLYGON( m_nNumBlockX, m_nNumBlockY));

	//テクニック終了
	m_pMaterial->End();
}

/*------------------------------------------------------------------------------
	頂点バッファ設定
------------------------------------------------------------------------------*/
void MeshDomeRenderer::SetVtxBuffer( void)
{
	//頂点バッファをロックして、仮想アドレスを取得する
	VERTEX_3D* pVtx;				//仮想アドレス用ポインタ
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx,0);

	int nCntHeight = 0;				//たてカウンタ
	int nCnt = 0;					//円周カウンタ
	D3DXVECTOR3 normal;				//原点からのベクトル
	float fAngle;					//角度
	float fRadius;					//半径
	float fHeight;					//高さ

	//ポリゴンの設定
	for( nCntHeight = 0; nCntHeight < m_nNumBlockY + 1; nCntHeight++)
	{
		//半径の更新
		fRadius = 0.0f + m_fRadius * cosf( D3DX_PI * 0.5f * (m_nNumBlockY - nCntHeight) / m_nNumBlockY);

		//高さの更新
		fHeight = m_fHeight * sinf( D3DX_PI * 0.5f * (m_nNumBlockY - nCntHeight) / m_nNumBlockY);

		for( nCnt = 0; nCnt < m_nNumBlockX + 1; nCnt++)
		{
			//頂点座標の設定
			fAngle = m_fBlockAngle * nCnt;
			//fAngle = P_ANGLE * ( NUM_DOME_X - nCnt);
			pVtx[ 0].Pos = D3DXVECTOR3( fRadius * -cosf( fAngle), fHeight, fRadius * sinf( fAngle));

			//法線の設定
			D3DXVec3Normalize( &normal, &pVtx[ 0].Pos);
			pVtx[ 0].Normal = -normal;
			
			//頂点カラーの設定（0～255の整数値）
			pVtx[ 0].Color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);
			
			//UVデータの設定
			pVtx[ 0].Tex = D3DXVECTOR2( 1.0f / m_nNumBlockX * nCnt, 1.0f / m_nNumBlockY * nCntHeight);

			//ポインタをずらす
			pVtx += 1;
		}
	}

	//アンロック
	m_pVtxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	インデックスバッファ設定
------------------------------------------------------------------------------*/
void MeshDomeRenderer::SetIdxBuffer(void)
{
	//頂点バッファをロックして、仮想アドレスを取得する
	WORD* pIdx;		//インデックス情報
	m_pIdxBuff->Lock( 0, 0, (void**)&pIdx, 0);

	//変数宣言
	int nCntHeight = 0;		//たてカウンタ
	int nCntWidth = 0;		//よこカウンタ

	//インデックス情報の設定
	for( nCntHeight = 0; nCntHeight < m_nNumBlockY; nCntHeight++)
	{
		//表示するポリゴンのインデックス情報の設定
		for( nCntWidth = 0; nCntWidth < m_nNumBlockX + 1; nCntWidth++)
		{
			pIdx[ 0] = ( nCntHeight + 1) * ( m_nNumBlockX + 1) + nCntWidth;
			pIdx[ 1] = ( nCntHeight + 0) * ( m_nNumBlockX + 1) + nCntWidth;

			pIdx += 2;
		}

		//最後は縮退ポリゴン不要
		if( nCntHeight == m_nNumBlockY - 1)
		{
			break;
		}

		//縮退ポリゴンのインデックス情報の設定
		pIdx[ 0] = nCntHeight * ( m_nNumBlockX + 1) + nCntWidth - 1;
		pIdx[ 1] = ( nCntHeight + 2) * ( m_nNumBlockX + 1);
		
		pIdx += 2;
	}

	//アンロック
	m_pIdxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	テクスチャ設定
------------------------------------------------------------------------------*/
void MeshDomeRenderer::LoadTexture(std::string FileName)
{
	m_pMaterial->SetTexture( FileName);
}

/*------------------------------------------------------------------------------
	シェーダー設定
------------------------------------------------------------------------------*/
void MeshDomeRenderer::SetShader(EShaderType Type)
{
	m_pMaterial->SetShader( Type);
}

/*------------------------------------------------------------------------------
	ドームの設定
------------------------------------------------------------------------------*/
void MeshDomeRenderer::SetDome(int X, int Y, float Radius, float Height)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得
	
	//デフォルト値設定
	m_nNumBlockX = X;			//横の分割数
	m_nNumBlockY = Y;			//縦の分割数
	m_fRadius = Radius;			//半径
	m_fHeight = Height;			//高さ
	m_fBlockAngle = D3DX_PI * 2.0f / m_nNumBlockX;
	m_fBlockHeight = m_fHeight / m_nNumBlockY;

	//頂点バッファの解放
	SAFE_RELEASE( m_pVtxBuff);

	//頂点バッファ生成
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_3D) * NUM_VERTEX( m_nNumBlockX, m_nNumBlockY),				//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,					//頂点バッファの使用方法(速さに影響)
		0,									//FVF(頂点フォーマット)
		D3DPOOL_MANAGED,					//メモリの管理(MANAGEDはデバイスにおまかせ)
		&m_pVtxBuff,						//頂点バッファ管理インターフェイス
		NULL)))
	{
		//エラー
		assert( false);
		return;
	}

	//頂点バッファ設定
	SetVtxBuffer();

	//インデックスバッファの解放
	SAFE_RELEASE( m_pIdxBuff);

	//インデックスバッファの作成
	HRESULT hr;
	hr = pDevice->CreateIndexBuffer(
		sizeof( WORD) * NUM_INDEX( m_nNumBlockX, m_nNumBlockY),		 //サイズ（WORD or DWORD）*インデックス数
		D3DUSAGE_WRITEONLY,				 //使用用途フラグ
		D3DFMT_INDEX16,					 //インデックスデータのフォーマット（16 or 32）
		D3DPOOL_MANAGED,				 //メモリの管理方法（おまかせ）
		&m_pIdxBuff,					 //インデックスバッファインターフェイスポインタのアドレス
		NULL);

	if( FAILED( hr))
	{
		//エラー
		assert( false);
		return;
	}

	//インデックスバッファ設定
	SetIdxBuffer();
}

/*------------------------------------------------------------------------------
	ポリゴン一枚あたりの幅・高さを設定
------------------------------------------------------------------------------*/
void MeshDomeRenderer::SetBlockSize(float BlockAngle, float BlockHeight)
{
	m_fBlockAngle = D3DX_PI * 2.0f / m_nNumBlockX;
	m_fBlockHeight = BlockHeight;
	m_fHeight = m_nNumBlockY * BlockHeight;
	
	//頂点バッファ設定
	SetVtxBuffer();
}

/*------------------------------------------------------------------------------
	幅・高さを設定
------------------------------------------------------------------------------*/
void MeshDomeRenderer::SetSize(float Radius, float Height)
{
	m_fRadius = Radius;
	m_fHeight = Height;
	m_fBlockHeight = Height / m_nNumBlockY;

	//頂点バッファ設定
	SetVtxBuffer();
}

/*------------------------------------------------------------------------------
	ロード
------------------------------------------------------------------------------*/
void MeshDomeRenderer::Load(Text& text)
{
	//textを読み進める
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

		//textを読み進める
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}
	SetVtxBuffer();
	SetIdxBuffer();
}

/*------------------------------------------------------------------------------
	セーブ
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
