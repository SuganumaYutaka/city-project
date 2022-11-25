/*==============================================================================

    SkinMeshRenderer.cpp - スキンメッシュの描画
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/9/21
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "SkinMeshRenderer.h"
#include "Material.h"
#include "GameObject.h"
#include "Transform.h"
#include "RenderManager.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define MAX_CLUSTER (58)		//クラスターの最大数（シェーダーに渡すため固定長）

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* SkinMeshRenderer::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<SkinMeshRenderer>();
}

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
SkinMeshRenderer::SkinMeshRenderer( GameObject *pGameObject)
{
	m_pGameObject = pGameObject;
	m_nLayer = eLayerDefault;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
	m_nPass = 0;

	m_NumPolygon = 0;
}

/*------------------------------------------------------------------------------
	終了処理
------------------------------------------------------------------------------*/
void SkinMeshRenderer::Uninit( void)
{
	Manager::GetRenderManager()->ReleaseRenderer( this);

	//頂点バッファの解放
	SAFE_RELEASE( m_pVtxBuff);
	
	//マテリアルの解放
	m_vecMaterial.clear();

	//アニメーションデータの解放
	m_vecAnimation.clear();
}

/*------------------------------------------------------------------------------
	更新
------------------------------------------------------------------------------*/
void SkinMeshRenderer::Update( void)
{
	
}

/*------------------------------------------------------------------------------
	描画
------------------------------------------------------------------------------*/
void SkinMeshRenderer::Draw( Camera* pCamera)
{
	if (m_NumPolygon == 0)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得
																	
	//モデルの描画
	int NumMaterial = m_vecMaterial.size();
	for( int nCntMaterial = 0; nCntMaterial < NumMaterial; nCntMaterial++)
	{
		auto Mat = m_vecMaterial[ nCntMaterial];

		//マテリアルの設定
		Mat.Set( pCamera, this);

		//頂点情報設定
		pDevice-> SetStreamSource( 0, m_pVtxBuff, 0, sizeof( VERTEX_SKIN_MESH));

		//テクニック開始
		Mat.Begin( m_nPass);

		//ポリゴンの描画
		pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, 0, m_NumPolygon);

		//テクニック終了
		Mat.End();	
	}
}

/*------------------------------------------------------------------------------
	頂点バッファ作成
------------------------------------------------------------------------------*/
void SkinMeshRenderer::CreateVtxBuffer()
{
	LPDIRECT3DDEVICE9 pDevice = Manager::GetDevice();		//デバイス取得

	//頂点バッファ生成
	if( FAILED( pDevice->CreateVertexBuffer(
		sizeof( VERTEX_SKIN_MESH) * m_vecPositionIndex.size(),			//作成したい頂点バッファのサイズ
		D3DUSAGE_WRITEONLY,						//頂点バッファの使用方法(速さに影響)
		0,										//FVF(頂点フォーマット)
		D3DPOOL_MANAGED,						//メモリの管理(MANAGEDはデバイスにおまかせ)
		&m_pVtxBuff,							//頂点バッファ管理インターフェイス
		NULL)))	
	{
		//エラー
		assert( false);
		return;
	}

	//頂点バッファ設定
	SetVtxBuffer();
}

/*------------------------------------------------------------------------------
	頂点バッファ設定
------------------------------------------------------------------------------*/
void SkinMeshRenderer::SetVtxBuffer()
{
	//ロック
	VERTEX_SKIN_MESH* pVtx;		//仮想アドレス用ポインタ
	m_pVtxBuff->Lock( 0, 0, (void**)&pVtx, 0);

	int NumVtx = m_vecPositionIndex.size();
	for( int nCnt = 0; nCnt < NumVtx; nCnt++)
	{
		//頂点座標
		pVtx[ nCnt].Pos = m_vecPoint[ m_vecPositionIndex[nCnt]].Position;

		//法線
		pVtx[ nCnt].Normal = m_vecNormal[ m_vecNormalIndex[ nCnt]];

		//テクスチャ座標
		pVtx[ nCnt].Tex = m_vecTexcoord[ m_vecTexcoordIndex[ nCnt]];
	
		//ボーンのインデックス番号とウェイト値
		for (int i = 0; i < 4; i++)
		{
			pVtx[ nCnt].Weight[i] = (float)m_vecPoint[ m_vecPositionIndex[ nCnt]].BornRefarence[ i].Weight;
			pVtx[ nCnt].BornIndex[i] = (unsigned char)m_vecPoint[ m_vecPositionIndex[ nCnt]].BornRefarence[ i].Index;
		}
	}

	//アンロック
	m_pVtxBuff->Unlock();
}

/*------------------------------------------------------------------------------
	メッシュデータを読み込み
------------------------------------------------------------------------------*/
void SkinMeshRenderer::LoadMeshData(FILE *pFile, SkinMeshModel* pModel)
{
	//ファイルチェック
	if (pFile == NULL)
	{
		return;
	}

	//モデルポインタ設定
	m_pSkinMeshModel = pModel;

	int size;					//格納サイズ
	char fileName[MAX_PATH];	//ファイル名
	D3DXMATRIX mtxIdentitiy;
	D3DXMatrixIdentity(&mtxIdentitiy);

	//ポリゴン数
	fread( &size, sizeof(int), 1, pFile);
	m_NumPolygon = size;

	//位置座標とボーン情報
	fread( &size, sizeof( int), 1, pFile);
	m_vecPoint.resize(size);
	fread( m_vecPoint.data(), sizeof(Point), size, pFile);
	
	//頂点インデックス
	fread( &size, sizeof( int), 1, pFile);
	m_vecPositionIndex.resize(size);
	fread( m_vecPositionIndex.data(), sizeof(unsigned short), size, pFile);

	//法線
	fread( &size, sizeof( int), 1, pFile);
	m_vecNormal.resize(size);
	fread( m_vecNormal.data(), sizeof( D3DXVECTOR3), size, pFile);

	//法線インデックス
	fread( &size, sizeof(int), 1, pFile);
	m_vecNormalIndex.resize(size);
	fread( m_vecNormalIndex.data(), sizeof(unsigned short), size, pFile);

	//UV座標
	fread( &size, sizeof(int), 1, pFile);
	m_vecTexcoord.resize(size);
	fread( m_vecTexcoord.data(), sizeof(D3DXVECTOR2), size, pFile);

	//UV座標インデックス
	fread( &size, sizeof(int), 1, pFile);
	m_vecTexcoordIndex.resize(size);
	fread( m_vecTexcoordIndex.data(), sizeof(unsigned short), size, pFile);

	//マテリアル
	fread(&size, sizeof(int), 1, pFile);
	m_vecMaterial.resize(size);
	int sizeMat = m_vecMaterial.size();
	for( int i = 0; i < sizeMat; i++)
	{
		Material& mat = m_vecMaterial[i];

		//マテリアルの設定
		mat.SetShader( eShaderSkinMesh);

		//テクスチャ名
		fread(&size, sizeof(int), 1, pFile);
		fread(&fileName, sizeof(char), size, pFile);
		mat.SetTexture(fileName);

		//カラーデータ
		D3DXVECTOR4 color;
		fread(&color, sizeof(D3DXVECTOR4), 1, pFile);
		mat.SetAmbient(color.x, color.y, color.z, color.w);
		fread(&color, sizeof(D3DXVECTOR4), 1, pFile);
		mat.SetDiffuse(color.x, color.y, color.z, color.w);
		fread(&color, sizeof(D3DXVECTOR4), 1, pFile);
		mat.SetSpecular(color.x, color.y, color.z, color.w);
		fread(&color, sizeof(D3DXVECTOR4), 1, pFile);
		mat.SetEmissive(color.x, color.y, color.z, color.w);
	}

	//アニメーション
	fread(&size, sizeof(int), 1, pFile);
	m_vecAnimation.resize(size);
	int sizeAnim = m_vecAnimation.size();
	for( int i = 0; i < sizeAnim; i++)
	{
		Animation& anim = m_vecAnimation[i];

		fread(&size, sizeof(int), 1, pFile);
		fread(&fileName, sizeof(char), size, pFile);
		anim.Name = fileName;
		
		//fread(&anim.StartTime, sizeof(FbxTime), 1, pFile);
		//fread(&anim.EndTime, sizeof(FbxTime), 1, pFile);
		fread(&anim.AllFrame, sizeof(int), 1, pFile);
		anim.vecMatrix = 
			std::vector<std::vector<D3DXMATRIX>>( anim.AllFrame, std::vector<D3DXMATRIX>(MAX_CLUSTER, mtxIdentitiy));
		for( int nCnt = 0; nCnt < anim.AllFrame; nCnt++)
		{
			for( int nCntCluster = 0; nCntCluster < MAX_CLUSTER; nCntCluster++)
			{
				fread( &anim.vecMatrix[nCnt][nCntCluster], sizeof(D3DXMATRIX), 1, pFile);
			}
		}
	}

	//頂点バッファ設定
	CreateVtxBuffer();
}

/*------------------------------------------------------------------------------
	スケールの設定
------------------------------------------------------------------------------*/
void SkinMeshRenderer::SetScale(float rate)
{
	//スケール行列の設定
	D3DXMATRIX mtxScale;
	D3DXMatrixIdentity( &mtxScale);
	D3DXMatrixScaling( &mtxScale, rate, rate, rate);

	//位置座標
	int sizePoint = m_vecPoint.size();
	for (int nCnt = 0; nCnt < sizePoint; nCnt++)
	{
		Point& point = m_vecPoint[nCnt];
		D3DXVec3Scale( &point.Position, &point.Position, rate);
	}

	//アニメーション情報
	int sizeAnim = m_vecAnimation.size();
	for (int nCntAnim = 0; nCntAnim < sizeAnim; nCntAnim++)
	{
		for (int nCntFrame = 0; nCntFrame < m_vecAnimation[nCntAnim].AllFrame; nCntFrame++)
		{
			for (int nCntCluster = 0; nCntCluster < MAX_CLUSTER; nCntCluster++)
			{
				m_vecAnimation[nCntAnim].vecMatrix[nCntFrame][nCntCluster] *= mtxScale;
			}
		}
	}
}
