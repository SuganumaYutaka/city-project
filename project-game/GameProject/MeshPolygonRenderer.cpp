/*==============================================================================

    MeshPolygonRenderer.cpp - 3Dポリゴン描画
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/17
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "MeshPolygonRenderer.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "Material.h"
#include "Camera.h"

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* MeshPolygonRenderer::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<MeshPolygonRenderer>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
MeshPolygonRenderer::MeshPolygonRenderer( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerDefault;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_nPass = 1;

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//色の設定
	m_Color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);

	//デフォルトの頂点設定
	std::vector<Vector3> vertices;
	vertices.resize(4);
	vertices[0] = Vector3( -0.5f, 0.0f, +0.5f);
	vertices[1] = Vector3( +0.5f, 0.0f, +0.5f);
	vertices[2] = Vector3( +0.5f, 0.0f, -0.5f);
	vertices[3] = Vector3( -0.5f, 0.0f, -0.5f);
	m_pVtxBuff = NULL;
	SetVertices( vertices);
	
	//マテリアル
	m_pMaterial = new Material();

}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void MeshPolygonRenderer::Uninit( void)
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
void MeshPolygonRenderer::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	描画
------------------------------------------------------------------------------*/
void MeshPolygonRenderer::Draw( Camera* pCamera)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//マテリアル（シェーダー）をセット
	m_pMaterial->Set( pCamera, this);

	//頂点情報設定
	pDevice-> SetStreamSource( 0, m_pVtxBuff, 0, sizeof( VERTEX_3D));

	//テクニック開始
	m_pMaterial->Begin( m_nPass);

	//ポリゴンの描画
	pDevice->DrawPrimitive( D3DPT_TRIANGLEFAN, 0, m_Vertices.size() - 2);

	//テクニック終了
	m_pMaterial->End();
}

/*------------------------------------------------------------------------------
	頂点バッファ設定
------------------------------------------------------------------------------*/
void MeshPolygonRenderer::SetVertices( const std::vector<Vector3>& vertices)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//頂点バッファの解放
	SAFE_RELEASE( m_pVtxBuff);
	
	//頂点数の取得
	int size = vertices.size();

	//頂点データの取得
	m_Vertices.resize( size);
	for (int nCnt = 0; nCnt < size; nCnt++)
	{
		m_Vertices[ nCnt] = vertices[ nCnt].ConvertToDX();
	}
	
	//頂点バッファ生成
	if( FAILED( pDevice->CreateVertexBuffer( sizeof( VERTEX_3D) * size, D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &m_pVtxBuff, NULL)))
	{
		//エラー
		assert( false);
		return;
	}

	SetVtxBuffer();
}

/*------------------------------------------------------------------------------
	頂点バッファ設定
------------------------------------------------------------------------------*/
void MeshPolygonRenderer::SetVtxBuffer( void)
{
	int size = m_Vertices.size();

	//ロック
	VERTEX_3D* pVtx;		//仮想アドレス用ポインタ
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx, 0);

	for (int nCnt = 0; nCnt < size; nCnt++)
	{
		pVtx[nCnt].Pos = m_Vertices[ nCnt];
		pVtx[nCnt].Normal = D3DXVECTOR3( 0.0f, 1.0f, 0.0f);
		pVtx[nCnt].Color = m_Color;
		pVtx[nCnt].Tex = D3DXVECTOR2( 0.0f, 0.0f);
	}

	//アンロック
	m_pVtxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	シェーダー設定
------------------------------------------------------------------------------*/
void MeshPolygonRenderer::SetShader(EShaderType Type)
{
	m_pMaterial->SetShader( Type);
}

/*------------------------------------------------------------------------------
	ロード
------------------------------------------------------------------------------*/
void MeshPolygonRenderer::Load(Text& text)
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
		else if (text.GetWord() == "Vertices")
		{
			text.ForwardPositionToNextWord();
			
			int size = std::stoi( text.GetWord());
			m_Vertices.clear();
			m_Vertices.resize(size);
			
			for (int nCnt = 0; nCnt < size; nCnt++)
			{
				text.ForwardPositionToNextWord();
				m_Vertices[nCnt].x = std::stof( text.GetWord());
				text.ForwardPositionToNextWord();
				m_Vertices[nCnt].y = std::stof( text.GetWord());
				text.ForwardPositionToNextWord();
				m_Vertices[nCnt].z = std::stof( text.GetWord());
			}
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
void MeshPolygonRenderer::Save(Text& text)
{
	StartSave(text);

	text += "Pass " + std::to_string(m_nPass) + '\n';
	m_pMaterial->Save(text);
	text += "Color " 
		+ std::to_string(m_Color.r) + ' '
		+ std::to_string(m_Color.g) + ' '
		+ std::to_string(m_Color.b) + ' '
		+ std::to_string(m_Color.a) + '\n';
	
	text += "Vertices "
		+ std::to_string(m_Vertices.size()) + '\n';
	for (const D3DXVECTOR3& vertex : m_Vertices)
	{
		text += std::to_string( vertex.x) + ' '
			+ std::to_string( vertex.y) + ' '
			+ std::to_string( vertex.z) + '\n';
	}

	EndSave( text);
}

/*------------------------------------------------------------------------------
	視錐台カリング判定
------------------------------------------------------------------------------*/
bool MeshPolygonRenderer::CheckFrustumCulling(Camera* pCamera)
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
