/*==============================================================================

    XModelRenderer.cpp - モデル（Xファイル）の描画
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/7
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "XModelRenderer.h"
#include "Material.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"
#include "XModelManager.h"
#include "XModelData.h"

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* XModelRenderer::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<XModelRenderer>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
XModelRenderer::XModelRenderer( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerDefault;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_nPass = 0;

	m_pData = NULL;
	m_vecMaterial.clear();
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void XModelRenderer::Uninit( void)
{
	Manager::GetRenderManager()->ReleaseRenderer( this);

	//マテリアルの解放
	m_vecMaterial.clear();
}

/*------------------------------------------------------------------------------
	Xファイル読み込み
------------------------------------------------------------------------------*/
void XModelRenderer::LoadXModel( std::string FileName)
{
	//Xファイルを読み込み
	m_pData = Manager::GetXModelManager()->Load( FileName);
	
	//マテリアルをコピー
	m_vecMaterial.clear();
	for (auto pMat : m_pData->m_vecMaterial)
	{
		m_vecMaterial.push_back( *pMat);

	}
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void XModelRenderer::Update( void)
{
	if (m_pData == NULL)
	{
		return;
	}

	//レンダーマネージャーに登録
	//Manager::GetRenderManager()->AddRenderer( this);
}

/*------------------------------------------------------------------------------
	描画
------------------------------------------------------------------------------*/
void XModelRenderer::Draw( Camera* pCamera)
{
	if (m_pData == NULL)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//ワールド座標変換
	D3DXMATRIX mtxWorld = m_pTransform->WorldMatrix();	//ワールド座標変換行列
	pDevice->SetTransform( D3DTS_WORLD, &mtxWorld);

	//モデルの描画
	for( int nCnt = 0; nCnt < (int)m_pData->m_nNumMaterial; nCnt++)
	{
		auto Mat = m_vecMaterial[ nCnt];

		//マテリアルの設定
		Mat.Set( pCamera, this);

		//テクニック開始
		Mat.Begin( m_nPass);

		//メッシュの描画
		m_pData->m_pMeshModel->DrawSubset( nCnt);

		//テクニック終了
		Mat.End();
	}
}

/*------------------------------------------------------------------------------
	マテリアルの取得
------------------------------------------------------------------------------*/
std::vector<Material>& XModelRenderer::GetAllMaterial()
{
	return m_vecMaterial;
}

/*------------------------------------------------------------------------------
	ロード
------------------------------------------------------------------------------*/
void XModelRenderer::Load(Text& text)
{
	//textを読み進める
	if (text.ForwardPositionToNextWord() == Text::EoF)
	{
		return;
	}

	while ( text.GetWord() != "EndComponent")
	{
		if (text.GetWord() == "FileName")
		{
			text.ForwardPositionToNextWord();
			
			LoadXModel( text.GetWord());
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
void XModelRenderer::Save(Text& text)
{
	StartSave(text);

	text += "FileName " + m_pData->FileName + '\n';

	EndSave( text);
}
