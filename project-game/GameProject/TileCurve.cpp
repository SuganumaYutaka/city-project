/*==============================================================================

    Tile.cpp - 建物の自動生成ータイル
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "TileCurve.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define TILE_DIVIDE_PER_RADIAN (0.1f)	//ラジアン角に応じてタイルを分割する基準

/*------------------------------------------------------------------------------
	初期化
------------------------------------------------------------------------------*/
void TileCurve::Init( float height, float width, const Vector3& bottomLeftPosition, E_TILE_TYPE type, const TextureUV& texUV)
{
	m_Height = height;
	m_Width = width;
	m_BottomLeftPosition = bottomLeftPosition;
	m_Center = Vector3( 0.0f, bottomLeftPosition.y, 0.0f);
	m_Type = type;
	m_TexUV = texUV;
}

/*------------------------------------------------------------------------------
	位置と半径の更新
------------------------------------------------------------------------------*/
void TileCurve::Transform(D3DXMATRIX shapeMatrix)
{
	auto position = m_BottomLeftPosition.ConvertToDX();
	auto center = m_Center.ConvertToDX();

	D3DXVec3TransformCoord( &position, &position, &shapeMatrix);
	D3DXVec3TransformCoord( &center, &center, &shapeMatrix);
	m_BottomLeftPosition = Vector3::ConvertFromDX( position);
	m_Center = Vector3::ConvertFromDX( center);
}

/*------------------------------------------------------------------------------
	頂点バッファの設定（円に沿って曲げる）
------------------------------------------------------------------------------*/
void TileCurve::SetVertexBuffer(VERTEX_3D* pVtx)
{
	//縮退ポリゴンに注意して頂点を設定
	//回転用の準備
	float angle = CulcAngle();
	int countDivide = CulcCountDivide();
	float deltaAngle = angle / (float)( countDivide + 1);
	D3DXMATRIX mtxRotate;
	D3DXMatrixIdentity( &mtxRotate);
	D3DXMatrixRotationY( &mtxRotate, deltaAngle);
	D3DXVECTOR3 positionFromCenter = (m_BottomLeftPosition - m_Center).ConvertToDX();

	//UV値設定の準備
	D3DXVECTOR2 topTexUV = m_TexUV.GetTopLeft();
	D3DXVECTOR2 bottomTexUV = m_TexUV.GetBottomLeft();
	float deltaTexU = m_TexUV.GetSize().x / (float)( countDivide + 1);
	
	int countVertex = CulcCountVertex();
	for (int i = 1; i < countVertex - 2; i += 2)
	{
		//位置の設定
		D3DXVECTOR3 pos = m_Center.ConvertToDX() + positionFromCenter;
		pVtx[ i].Pos = pos;
		pos.y += m_Height;
		pVtx[ i + 1].Pos = pos;

		//法線の設定
		D3DXVECTOR3 normal;
		D3DXVec3Normalize( &normal, &positionFromCenter);
		pVtx[ i].Normal = pVtx[ i + 1].Normal = normal;

		//色の設定
		pVtx[ i].Color = pVtx[ i + 1].Color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);

		//UVの設定
		pVtx[ i].Tex = bottomTexUV;
		pVtx[ i + 1].Tex = topTexUV;
		
		//次の設定に向けて更新
		D3DXVec3TransformCoord( &positionFromCenter, &positionFromCenter, &mtxRotate);
		topTexUV.x += deltaTexU;
		bottomTexUV.x += deltaTexU;
	}

	//縮退ポリゴンの設定
	pVtx[ 0].Pos = m_BottomLeftPosition.ConvertToDX();
	pVtx[ 0].Normal = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	pVtx[ 0].Color = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 0.0f);
	pVtx[ 0].Tex = D3DXVECTOR2( 0.0f, 0.0f);

	D3DXVECTOR3 lastPosition;
	D3DXMatrixRotationY( &mtxRotate, angle);
	D3DXVec3TransformCoord( &lastPosition, &( m_BottomLeftPosition - m_Center).ConvertToDX(), &mtxRotate);
	lastPosition += m_Center.ConvertToDX();
	lastPosition.y += m_Height;
	pVtx[ countVertex - 1].Pos = lastPosition;
	pVtx[ countVertex - 1].Normal = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	pVtx[ countVertex - 1].Color = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 0.0f);
	pVtx[ countVertex - 1].Tex = D3DXVECTOR2( 0.0f, 0.0f);
}

/*------------------------------------------------------------------------------
	頂点数の算出
------------------------------------------------------------------------------*/
int TileCurve::CulcCountVertex( void)
{
	int countDivide = CulcCountDivide();
	int countVertex = ( countDivide + 2) * 2 + 2;

	return countVertex;
}

/*------------------------------------------------------------------------------
	ポリゴン数の算出
------------------------------------------------------------------------------*/
int TileCurve::CulcCountPolygon(void)
{
	int countDivide = CulcCountDivide();

	return 6 + countDivide * 2;
}

/*------------------------------------------------------------------------------
	半径から角度を算出
------------------------------------------------------------------------------*/
float TileCurve::CulcAngle( void)
{
	float radius = Vector3::Distance( m_BottomLeftPosition, m_Center);
	return m_Width / radius;
}

/*------------------------------------------------------------------------------
	分割数を算出
------------------------------------------------------------------------------*/
int TileCurve::CulcCountDivide(void)
{
	float angle = CulcAngle();
	return (int)( angle / TILE_DIVIDE_PER_RADIAN);
}

