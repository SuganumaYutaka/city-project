/*==============================================================================

    Tile.cpp - 建物の自動生成ータイル
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Tile.h"
#include "BuildingRule.h"
#include "BuildingSurfacePattern.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
Tile::Tile() : m_Next( NULL)
{
	m_Height = 1.0f;
	m_Width = 1.0f;
	m_Type = eTileWall;
}


/*------------------------------------------------------------------------------
	初期化
------------------------------------------------------------------------------*/
void Tile::Init( float height, float width, E_TILE_TYPE type, const TextureUV& texUV)
{
	m_Height = height;
	m_Width = width;
	m_Type = type;
	m_TexUV = texUV;
}

/*------------------------------------------------------------------------------
	頂点バッファの設定
------------------------------------------------------------------------------*/
Vector3 Tile::SetVertexBuffer(VERTEX_3D* pVtx, const Vector3& bottomLeftPosition, const Vector3& normal, const Vector3& vector)
{
	//縮退ポリゴンに注意して頂点を設定
	Vector3 nextPosition = bottomLeftPosition + vector * m_Width;

	pVtx[ 0].Pos = pVtx[ 1].Pos = D3DXVECTOR3( bottomLeftPosition.x, bottomLeftPosition.y, bottomLeftPosition.z);
	pVtx[ 2].Pos = D3DXVECTOR3( bottomLeftPosition.x, bottomLeftPosition.y + m_Height, bottomLeftPosition.z);
	pVtx[ 3].Pos = D3DXVECTOR3( nextPosition.x, nextPosition.y, nextPosition.z);
	pVtx[ 4].Pos = pVtx[ 5].Pos = D3DXVECTOR3( nextPosition.x, nextPosition.y + m_Height, nextPosition.z);

	for (int i = 0; i < 6; i++)
	{
		pVtx[ i].Normal = normal.ConvertToDX();
		pVtx[ i].Color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);
	}

	pVtx[0].Tex = pVtx[5].Tex = D3DXVECTOR2( 0.0f, 0.0f); 
	
	//UV（N字）
	pVtx[1].Tex = m_TexUV.GetBottomLeft();
	pVtx[2].Tex = m_TexUV.GetTopLeft();
	pVtx[3].Tex = m_TexUV.GetBottomRight();
	pVtx[4].Tex = m_TexUV.GetTopRight();

	return nextPosition;
}

/*------------------------------------------------------------------------------
	頂点バッファの設定（円に沿って曲げる）
------------------------------------------------------------------------------*/
Vector3 Tile::SetVertexBufferCurve(VERTEX_3D* pVtx, const Vector3& bottomLeftPosition,  const Vector3& center, float radius)
{
	//縮退ポリゴンに注意して頂点を設定
	
	//回転用の準備
	float angle = CulcAngle( radius);
	int countDivide = CulcCountDivide( radius);
	float deltaAngle = angle / (float)( countDivide + 1);
	D3DXMATRIX mtxRotate;
	D3DXMatrixIdentity( &mtxRotate);
	D3DXMatrixRotationY( &mtxRotate, deltaAngle);
	D3DXVECTOR3 positionFromCenter = (bottomLeftPosition - center).ConvertToDX();

	//UV値設定の準備
	D3DXVECTOR2 topTexUV = m_TexUV.GetTopLeft();
	D3DXVECTOR2 bottomTexUV = m_TexUV.GetBottomLeft();
	float deltaTexU = m_TexUV.GetSize().x / (float)( countDivide + 1);
	
	int countVertex = CulcCountVertexCurve( radius);
	for (int i = 1; i < countVertex - 2; i++)
	{
		//位置の設定
		D3DXVECTOR3 pos = center.ConvertToDX() + positionFromCenter;
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

	//returnする次のbottomLeftPositionを設定
	D3DXVECTOR3 nextPosition;
	D3DXMatrixRotationY( &mtxRotate, angle);
	D3DXVec3TransformCoord( &nextPosition, &( bottomLeftPosition - center).ConvertToDX(), &mtxRotate);
	nextPosition += center.ConvertToDX();

	//縮退ポリゴンの設定
	pVtx[ 0].Pos = bottomLeftPosition.ConvertToDX();
	pVtx[ 0].Normal = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	pVtx[ 0].Color = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 0.0f);
	pVtx[ 0].Tex = D3DXVECTOR2( 0.0f, 0.0f);

	D3DXVECTOR3 lastPosition = nextPosition;
	lastPosition.y += m_Height;
	pVtx[ countVertex - 1].Pos = lastPosition;
	pVtx[ countVertex - 1].Normal = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	pVtx[ countVertex - 1].Color = D3DXCOLOR( 0.0f, 0.0f, 0.0f, 0.0f);
	pVtx[ countVertex - 1].Tex = D3DXVECTOR2( 0.0f, 0.0f);

	return Vector3::ConvertFromDX( nextPosition);
}

/*------------------------------------------------------------------------------
	頂点数の算出
------------------------------------------------------------------------------*/
int Tile::CulcCountVertex()
{
	return 4 + 2;
}

/*------------------------------------------------------------------------------
	頂点数の算出（円に沿って曲げる場合）
------------------------------------------------------------------------------*/
int Tile::CulcCountVertexCurve( float radius)
{
	int countDivide = CulcCountDivide( radius);
	int countVertex = ( countDivide + 2) * 2 + 2;

	return countVertex;
}

/*------------------------------------------------------------------------------
	ポリゴン数の算出
------------------------------------------------------------------------------*/
int Tile::CulcCountPolygon()
{
	return 6;
}

/*------------------------------------------------------------------------------
	ポリゴン数の算出（円に沿って曲げる場合）
------------------------------------------------------------------------------*/
int Tile::CulcCountPolygonCurve( float radius)
{
	int countDivide = CulcCountDivide( radius);


	return 6 + countDivide * 2;
}

/*------------------------------------------------------------------------------
	分割数を算出
------------------------------------------------------------------------------*/
int Tile::CulcCountDivide(float radius)
{
	float angle = CulcAngle( radius);
	return (int)( angle / TILE_DIVIDE_PER_RADIAN);
}

