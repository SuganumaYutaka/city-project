/*==============================================================================

    XModelData.cpp - Xモデル
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/7
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "XModelData.h"
#include "Material.h"

/*------------------------------------------------------------------------------
	コンストラクタ
	引数
		const char* pFileName
------------------------------------------------------------------------------*/
XModelData::XModelData( std::string fileName)
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//モデルの読み込み
	LPD3DXBUFFER pBuffMaterial = NULL;		//マテリアル情報格納
	HRESULT hr;
	hr = D3DXLoadMeshFromX( 
		fileName.c_str(), 
		D3DXMESH_MANAGED,
		pDevice,
		NULL,
		&pBuffMaterial,
		NULL,
		&m_nNumMaterial,
		&m_pMeshModel);
	if (hr == E_FAIL)
	{
		MessageBox( NULL, "Xモデルの読み込みに失敗しました", "エラー", MB_OK);
		return;
	}
	FileName = fileName;

	//マテリアル情報設定
	m_vecMaterial.resize( m_nNumMaterial);		//要素数をマテリアル数分に変更
	D3DXMATERIAL* pMat;							//マテリアル情報の先頭アドレス
	pMat = (D3DXMATERIAL*)pBuffMaterial->GetBufferPointer();
	for( int nCnt = 0; nCnt < (int)m_nNumMaterial; nCnt++)
	{
		//マテリアルの設定
		m_vecMaterial[ nCnt] = new Material( &pMat[ nCnt]);
		m_vecMaterial[ nCnt]->SetShader( eShaderXModel);
	}
	
	//マテリアル情報の解放
	if( pBuffMaterial != NULL)
	{
		pBuffMaterial->Release();
		pBuffMaterial = NULL;
	}
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
XModelData::~XModelData()
{
	if (m_pMeshModel != NULL)
	{
		m_pMeshModel->Release();
		m_pMeshModel = NULL;
	}

	for (auto mat : m_vecMaterial)
	{
		delete mat;
	}
}
