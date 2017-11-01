/*==============================================================================

    SkinMeshModel.cpp - スキンメッシュモデル
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/10/09
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "SkinMeshModel.h"
#include "Material.h"
#include "GameObject.h"
#include "Transform.h"

#include "SkinMeshRenderer.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
SkinMeshModel::SkinMeshModel( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerDefault;
	m_pTransform = m_pGameObject->GetComponent<Transform>();

	vecAnimationClip.clear();
	m_CntFrame = 0;
	m_AnimationState = -1;
	m_bLoad = false;
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void SkinMeshModel::Uninit( void)
{
	
}

/*------------------------------------------------------------------------------
	ファイル読み込み
------------------------------------------------------------------------------*/
void SkinMeshModel::LoadModel( std::string FileName, float scaleRate)
{
	m_FileName = FileName;

	//ファイルオープン
	FILE* pFile;
	pFile = fopen( FileName.c_str(), "rb");
	if (!pFile)
	{
		MessageBox(NULL, "SkinMeshModel.cpp ファイルオープンに失敗しました", "エラー", MB_OK);
		return;
	}

	//モデルデータを読み込み
	//アニメーションクリップ
	int size;					//格納サイズ
	char fileName[MAX_PATH];	//ファイル名

	fread(&size, sizeof(int), 1, pFile);
	vecAnimationClip.resize(size);
	//for (auto anim : m_vecAnimation)
	for( int i = 0; i < vecAnimationClip.size(); i++)
	{
		AnimationClip& anim = vecAnimationClip[i];

		fread(&size, sizeof(int), 1, pFile);
		fread(&fileName, sizeof(char), size, pFile);
		anim.Name = fileName;
		fread(&anim.AllFrame, sizeof(int), 1, pFile);
	}
	m_AnimationState = 0;

	//メッシュ数
	int NumMesh = 0;
	fread(&NumMesh, sizeof(int), 1, pFile);

	//メッシュを作成
	GameObject* pObject = NULL;
	for( int nCnt = 0; nCnt < NumMesh; nCnt++)
	{
		pObject = new GameObject( m_pGameObject);
		auto mesh = pObject->AddComponent<SkinMeshRenderer>();

		mesh->LoadMeshData(pFile, this);
	}

	//ファイルクローズ
	fclose( pFile);

	m_bLoad = true;

	//スケールを更新
	if( scaleRate != 1.0f)
	{
		SetScale(scaleRate);
	}
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void SkinMeshModel::Update( void)
{
	if (!m_bLoad)
	{
		return;
	}

	m_CntFrame++;
}

/*------------------------------------------------------------------------------
	スケールの設定
------------------------------------------------------------------------------*/
void SkinMeshModel::SetScale( float rate)
{
	//メッシュデータを取得
	auto SkinMeshs = m_pGameObject->GetComponentListInChildren<SkinMeshRenderer>();

	//メッシュデータを更新
	for (auto mesh : SkinMeshs)
	{
		mesh->SetScale(rate);
	}
}