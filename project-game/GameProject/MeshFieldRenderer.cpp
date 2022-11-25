/*==============================================================================

   MeshFieldRenderer.cpp - メッシュフィールド描画
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/24
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "MeshFieldRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Material.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define P_POS_X(P_WIDTH, NUM_FIELD_X) (0.0f + P_WIDTH * NUM_FIELD_X * -0.5f)		//開始地点のX座標
#define P_POS_Y (0.0f)																//開始地点のY座標
#define P_POS_Z(P_HEIGHT, NUM_FIELD_Z) (0.0f + P_HEIGHT * NUM_FIELD_Z * 0.5f)		//開始地点のZ座標
#define P_WIDTH( ALL_WIDTH, NUM_FIELD_X)			( ALL_WIDTH / NUM_FIELD_X)										//ポリゴン一枚あたりの幅
#define P_HEIGHT( ALL_HEIGHT, NUM_FIELD_Z)			( ALL_HEIGHT / NUM_FIELD_Z)										//ポリゴン一枚あたりの高さ
#define NUM_POLYGON( NUM_FIELD_X, NUM_FIELD_Z)		( 2 * NUM_FIELD_X * NUM_FIELD_Z + ( NUM_FIELD_Z - 1) * 4)		//ポリゴン数
#define NUM_VERTEX( NUM_FIELD_X, NUM_FIELD_Z)		( ( NUM_FIELD_X + 1) * ( NUM_FIELD_Z + 1))						//頂点数
#define NUM_INDEX( NUM_FIELD_X, NUM_FIELD_Z)		( ( (NUM_FIELD_X + 1) * 2 + 2) * NUM_FIELD_Z - 2)				//インデックス数

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* MeshFieldRenderer::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<MeshFieldRenderer>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
MeshFieldRenderer::MeshFieldRenderer( GameObject *pGameObject)
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
	m_nNumBlockZ = 8;			//縦の分割数
	m_fWidth = 8.0f;			//幅
	m_fHeight = 8.0f;			//高さ
	m_fBlockWidth = P_WIDTH( m_fWidth, m_nNumBlockX);		//ポリゴン1枚あたりの幅
	m_fBlockHeight = P_HEIGHT( m_fHeight, m_nNumBlockZ);	//ポリゴン1枚あたりの高さ

	//頂点ごとの高さ
	m_pVertexHeight = new float[ ( m_nNumBlockZ + 1) * ( m_nNumBlockX + 1)];
	for (int nCnt = 0; nCnt < ( m_nNumBlockZ + 1) * ( m_nNumBlockX + 1); nCnt++)
	{
		m_pVertexHeight[ nCnt] = 0.0f;
	}
	
	//頂点バッファ生成
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_3D) * NUM_VERTEX( m_nNumBlockX, m_nNumBlockZ),				//作成したい頂点バッファのサイズ
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
		sizeof( WORD) * NUM_INDEX( m_nNumBlockX, m_nNumBlockZ),		 //サイズ（WORD or DWORD）*インデックス数
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
void MeshFieldRenderer::Uninit( void)
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
	//頂点ごとの高さ解放
	delete[] m_pVertexHeight;

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
void MeshFieldRenderer::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	描画
------------------------------------------------------------------------------*/
void MeshFieldRenderer::Draw( Camera* pCamera)
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
		NUM_VERTEX( m_nNumBlockX, m_nNumBlockZ), 0,	NUM_POLYGON( m_nNumBlockX, m_nNumBlockZ));

	//テクニック終了
	m_pMaterial->End();
}

/*------------------------------------------------------------------------------
	頂点バッファ設定
------------------------------------------------------------------------------*/
void MeshFieldRenderer::SetVtxBuffer( void)
{
	//頂点バッファをロックして、仮想アドレスを取得する
	VERTEX_3D* pVtx;				//仮想アドレス用ポインタ
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx,0);

	//変数宣言
	int nCntHeight = 0;		//たてカウンタ
	int nCntWidth = 0;		//よこカウンタ
	VERTEX_3D* pVtxStock = pVtx;	//先頭アドレスを保存

	//ポリゴンの設定
	for( nCntHeight = 0; nCntHeight < m_nNumBlockZ + 1; nCntHeight++)
	{
		for( nCntWidth = 0; nCntWidth < m_nNumBlockX + 1; nCntWidth++)
		{
			//頂点座標の設定
			pVtx[ 0].Pos = D3DXVECTOR3(
				m_fWidth * -0.5f + m_fBlockWidth * nCntWidth,
				m_pVertexHeight[ nCntHeight * ( m_nNumBlockX + 1) + nCntWidth],
				m_fHeight * 0.5f - m_fBlockHeight * nCntHeight);
			
			//頂点カラーの設定（0〜255の整数値）
			pVtx[ 0].Color = m_Color;
			
			//UVデータの設定
			pVtx[ 0].Tex = D3DXVECTOR2( 1.0f * nCntWidth, 1.0f * nCntHeight);
			
			//ポインタをずらす
			pVtx += 1;
		}
	}

	//法線の設定
	pVtx = pVtxStock;				//先頭アドレスへ
	D3DXVECTOR3 DirX, DirZ;			//法線に垂直なベクトル
	D3DXVECTOR3 NormalX, NormalZ;	//各成分に対する法線
	D3DXVECTOR3 Normal;				//法線
	D3DXVECTOR3 NormalUp(0, 1, 0);	//上向きの法線
	
	for( nCntHeight = 0; nCntHeight < m_nNumBlockZ + 1; nCntHeight++)
	{
		for( nCntWidth = 0; nCntWidth < m_nNumBlockX + 1; nCntWidth++)
		{
			//端の例外処理
			if (nCntWidth == 0 || nCntWidth == m_nNumBlockX || nCntHeight == 0 || nCntHeight == m_nNumBlockZ)
			{
				pVtx[ nCntHeight * (m_nNumBlockX + 1) + nCntWidth].Normal = NormalUp;
			}
			else
			{
				//X方向
				DirX = pVtx[ nCntHeight * (m_nNumBlockX + 1) + nCntWidth + 1].Pos - pVtx[ nCntHeight * (m_nNumBlockX + 1) + nCntWidth - 1].Pos;
				NormalX.x = -DirX.y;
				NormalX.y = DirX.x;
				NormalX.z = 0.0f;

				//Z方向
				DirZ = pVtx[ ( nCntHeight - 1) * (m_nNumBlockX + 1) + nCntWidth].Pos - pVtx[ ( nCntHeight + 1) * (m_nNumBlockX + 1) + nCntWidth].Pos;
				NormalZ.x = 0.0f;
				NormalZ.y = DirZ.z;
				NormalZ.z = -DirX.y;

				//法線を作成
				Normal = NormalX + NormalZ;
				D3DXVec3Normalize( &Normal, &Normal);
				pVtx[ nCntHeight * (m_nNumBlockX + 1) + nCntWidth].Normal = Normal;
				//pVtx[ nCntHeight * m_nNumBlockX + nCntWidth].Normal = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
			}
		}
	}

	//アンロック
	m_pVtxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	インデックスバッファ設定
------------------------------------------------------------------------------*/
void MeshFieldRenderer::SetIdxBuffer(void)
{
	//頂点バッファをロックして、仮想アドレスを取得する
	WORD* pIdx;		//インデックス情報
	m_pIdxBuff->Lock( 0, 0, (void**)&pIdx, 0);

	//変数宣言
	int nCntHeight = 0;		//たてカウンタ
	int nCntWidth = 0;		//よこカウンタ

	//インデックス情報の設定
	for( nCntHeight = 0; nCntHeight < m_nNumBlockZ; nCntHeight++)
	{
		//表示するポリゴンのインデックス情報の設定
		for( nCntWidth = 0; nCntWidth < m_nNumBlockX + 1; nCntWidth++)
		{
			pIdx[ 0] = ( nCntHeight + 1) * ( m_nNumBlockX + 1) + nCntWidth;
			pIdx[ 1] = ( nCntHeight + 0) * ( m_nNumBlockX + 1) + nCntWidth;

			pIdx += 2;
		}

		//最後は縮退ポリゴン不要
		if( nCntHeight == m_nNumBlockZ - 1)
		{
			break;
		}

		//縮退ポリゴンのインデックス情報の設定
		pIdx[ 0] = ( nCntHeight + 0) * ( m_nNumBlockX + 1) + nCntWidth - 1;
		pIdx[ 1] = ( nCntHeight + 2) * ( m_nNumBlockX + 1) + 0;
		
		pIdx += 2;
	}

	//アンロック
	m_pIdxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	フィールドを設定
------------------------------------------------------------------------------*/
void MeshFieldRenderer::SetField( int X, int Z, float BlockWidth, float BlockHeight, float *pVertexHeight)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//設定
	m_nNumBlockX = X;
	m_nNumBlockZ = Z;
	m_fBlockWidth = BlockWidth;
	m_fBlockHeight = BlockHeight;
	m_fWidth = m_nNumBlockX * m_fBlockWidth;
	m_fHeight = m_nNumBlockZ * m_fBlockHeight;
	
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

	//頂点ごとの高さ解放
	delete[] m_pVertexHeight;

	//頂点ごとの高さ
	m_pVertexHeight = new float[ ( m_nNumBlockZ + 1) * ( m_nNumBlockX + 1)];
	if( pVertexHeight != NULL)
	{
		for (int nCnt = 0; nCnt < ( m_nNumBlockZ + 1) * ( m_nNumBlockX + 1); nCnt++)
		{
			m_pVertexHeight[ nCnt] = pVertexHeight[ nCnt];
		}
	}
	else
	{
		for (int nCnt = 0; nCnt < ( m_nNumBlockZ + 1) * ( m_nNumBlockX + 1); nCnt++)
		{
			m_pVertexHeight[ nCnt] = 0.0f;
		}
	}
	
	//頂点バッファ生成
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_3D) * NUM_VERTEX( m_nNumBlockX, m_nNumBlockZ),				//作成したい頂点バッファのサイズ
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
		sizeof( WORD) * NUM_INDEX( m_nNumBlockX, m_nNumBlockZ),		 //サイズ（WORD or DWORD）*インデックス数
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
void MeshFieldRenderer::SetBlockSize( float Width, float Height)
{
	//設定
	m_fBlockWidth = Width;
	m_fBlockHeight = Height;
	m_fWidth = m_nNumBlockX * m_fBlockWidth;
	m_fHeight = m_nNumBlockZ * m_fBlockHeight;
	
	//頂点バッファ設定
	SetVtxBuffer();
}

/*------------------------------------------------------------------------------
	幅・高さを設定
------------------------------------------------------------------------------*/
void MeshFieldRenderer::SetSize( float Width, float Height)
{
	//設定
	m_fWidth = Width;
	m_fHeight = Height;
	m_fBlockWidth = P_WIDTH( m_fWidth, m_nNumBlockX);		//ポリゴン1枚あたりの幅
	m_fBlockHeight = P_HEIGHT( m_fHeight, m_nNumBlockZ);	//ポリゴン1枚あたりの高さ
	
	//頂点バッファ設定
	SetVtxBuffer();
}

/*------------------------------------------------------------------------------
	テクスチャ設定
------------------------------------------------------------------------------*/
void MeshFieldRenderer::LoadTexture(std::string FileName)
{
	m_pMaterial->SetTexture( FileName);
}

/*------------------------------------------------------------------------------
	テクスチャ名取得
------------------------------------------------------------------------------*/
std::string MeshFieldRenderer::GetTextureName()
{
	return m_pMaterial->GetTextureName();
}

/*------------------------------------------------------------------------------
	シェーダー設定
------------------------------------------------------------------------------*/
void MeshFieldRenderer::SetShader(EShaderType Type)
{
	m_pMaterial->SetShader( Type);
}

/*------------------------------------------------------------------------------
	ロード
------------------------------------------------------------------------------*/
void MeshFieldRenderer::Load(Text& text)
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
		else if (text.GetWord() == "NumBlockZ")
		{
			text.ForwardPositionToNextWord();
			m_nNumBlockZ = std::stoi(text.GetWord());
		}
		else if (text.GetWord() == "Width")
		{
			text.ForwardPositionToNextWord();
			m_fWidth = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "Height")
		{
			text.ForwardPositionToNextWord();
			m_fHeight = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "BlockWidth")
		{
			text.ForwardPositionToNextWord();
			m_fBlockWidth = std::stof(text.GetWord());
		}
		else if (text.GetWord() == "BlockHeight")
		{
			text.ForwardPositionToNextWord();
			m_fBlockHeight = std::stof(text.GetWord());
		}

		else if (text.GetWord() == "VertexHeight")
		{
			delete[] m_pVertexHeight;
			m_pVertexHeight = NULL;
			m_pVertexHeight = new float[( m_nNumBlockZ + 1) * ( m_nNumBlockX + 1)];
			for (int nCnt = 0; nCnt < ( m_nNumBlockZ + 1) * ( m_nNumBlockX + 1); nCnt++)
			{
				text.ForwardPositionToNextWord();
				m_pVertexHeight[ nCnt] = std::stof(text.GetWord());
			}
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
void MeshFieldRenderer::Save(Text& text)
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
	text += "NumBlockZ " + std::to_string(m_nNumBlockZ) + ' ';
	text += "Width " + std::to_string(m_fWidth) + ' ';
	text += "Height " + std::to_string(m_fHeight) + ' ';
	text += "BlockWidth " + std::to_string(m_fBlockWidth) + ' ';
	text += "BlockHeight " + std::to_string(m_fBlockHeight) + '\n';
	text += "VertexHeight ";
	for (int nCnt = 0; nCnt < ( m_nNumBlockZ + 1) * ( m_nNumBlockX + 1); nCnt++)
	{
		text += std::to_string(m_pVertexHeight[ nCnt]) + ' ';
	}
	text += "\n";

	EndSave( text);
}
