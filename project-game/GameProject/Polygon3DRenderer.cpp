/*==============================================================================

    Polygon3DRenderer.cpp - 3Dポリゴン描画
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/17
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Polygon3DRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Material.h"
#include "Camera.h"

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* Polygon3DRenderer::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<Polygon3DRenderer>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
Polygon3DRenderer::Polygon3DRenderer( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerDefault;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_nPass = 1;

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	m_Vertices.resize(4);
	m_Vertices[0] = D3DXVECTOR3( -0.5f, 0.0f, +0.5f);
	m_Vertices[1] = D3DXVECTOR3( +0.5f, 0.0f, +0.5f);
	m_Vertices[2] = D3DXVECTOR3( -0.5f, 0.0f, -0.5f);
	m_Vertices[3] = D3DXVECTOR3( +0.5f, 0.0f, -0.5f);

	//頂点バッファ生成
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_3D) * 4,				//作成したい頂点バッファのサイズ
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

	//色の設定
	m_Color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);

	//頂点設定
	SetVtxBuffer();

	//マテリアル
	m_pMaterial = new Material();
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void Polygon3DRenderer::Uninit( void)
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

	m_Vertices.clear();
	m_Vertices.shrink_to_fit();
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void Polygon3DRenderer::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	描画
------------------------------------------------------------------------------*/
void Polygon3DRenderer::Draw( Camera* pCamera)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//マテリアル（シェーダー）をセット
	m_pMaterial->Set( pCamera, this);

	//頂点情報設定
	pDevice-> SetStreamSource( 0, m_pVtxBuff, 0, sizeof( VERTEX_3D));

	//テクニック開始
	m_pMaterial->Begin( m_nPass);

	//ポリゴンの描画
	pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0,	2);

	//テクニック終了
	m_pMaterial->End();
}

/*------------------------------------------------------------------------------
	頂点バッファ設定
------------------------------------------------------------------------------*/
void Polygon3DRenderer::SetVtxBuffer( void)
{
	//ロック
	VERTEX_3D* pVtx;		//仮想アドレス用ポインタ
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx, 0);

	//頂点設定
	//座標（Z字）
	pVtx[ 0].Pos = m_Vertices[0];
	pVtx[ 1].Pos = m_Vertices[1];
	pVtx[ 2].Pos = m_Vertices[2];
	pVtx[ 3].Pos = m_Vertices[3];
	
	//法線
	pVtx[ 0].Normal = 
	pVtx[ 1].Normal = 
	pVtx[ 2].Normal = 
	pVtx[ 3].Normal = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);

	//頂点カラー
	pVtx[ 0].Color = 
	pVtx[ 1].Color = 
	pVtx[ 2].Color = 
	pVtx[ 3].Color = m_Color;

	//UV座標（Z字）
	pVtx[ 0].Tex = D3DXVECTOR2( m_TextureUV.UV[ 0].x, m_TextureUV.UV[ 0].y);
	pVtx[ 1].Tex = D3DXVECTOR2( m_TextureUV.UV[ 1].x, m_TextureUV.UV[ 1].y);
	pVtx[ 2].Tex = D3DXVECTOR2( m_TextureUV.UV[ 2].x, m_TextureUV.UV[ 2].y);
	pVtx[ 3].Tex = D3DXVECTOR2( m_TextureUV.UV[ 3].x, m_TextureUV.UV[ 3].y);

	//アンロック
	m_pVtxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	テクスチャ設定
------------------------------------------------------------------------------*/
void Polygon3DRenderer::LoadTexture(std::string FileName)
{
	m_pMaterial->SetTexture( FileName);
}

/*------------------------------------------------------------------------------
	シェーダー設定
------------------------------------------------------------------------------*/
void Polygon3DRenderer::SetShader(EShaderType Type)
{
	m_pMaterial->SetShader( Type);
}

/*------------------------------------------------------------------------------
	ロード
------------------------------------------------------------------------------*/
void Polygon3DRenderer::Load(Text& text)
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
		else if (text.GetWord() == "TextureUV")
		{
			text.ForwardPositionToNextWord();
		
			m_TextureUV.ConvertFromString( text.GetAllText(), text.GetPosition());
		}

		//textを読み進める
		if (text.ForwardPositionToNextWord() == Text::EoF)
		{
			return;
		}
	}

}

/*------------------------------------------------------------------------------
	セーブ
------------------------------------------------------------------------------*/
void Polygon3DRenderer::Save(Text& text)
{
	StartSave(text);

	text += "Pass " + std::to_string(m_nPass) + '\n';
	m_pMaterial->Save(text);
	text += "Color " 
		+ std::to_string(m_Color.r) + ' '
		+ std::to_string(m_Color.g) + ' '
		+ std::to_string(m_Color.b) + ' '
		+ std::to_string(m_Color.a) + '\n';
	text += "TextureUV " + m_TextureUV.ConvertToString() + '\n';

	EndSave( text);
}

/*------------------------------------------------------------------------------
	視錐台カリング判定
------------------------------------------------------------------------------*/
bool Polygon3DRenderer::CheckFrustumCulling(Camera* pCamera)
{
	D3DXMATRIX wvp = m_pTransform->WorldMatrix() * *pCamera->GetViewMatrix() * *pCamera->GetProjectionMatrix();
	bool isLeft = false;
	bool isRight = false;

	for (D3DXVECTOR3 vertex : m_Vertices)
	{
		D3DXVec3TransformCoord( &vertex, &vertex, &wvp);
		if (vertex.x >= -1.0f && vertex.x <= 1.0f)
		{
			if (vertex.z >= 0 && vertex.z <= 1.0f)
			{
				return true;
			}
		}

		if (vertex.x < -1.0f)
		{
			isLeft = true;
		}
		else if (vertex.x > 1.0f)
		{
			isRight = true;
		}
	}
	
	return false;
}
