/*==============================================================================

    TileDefault.cpp - 建物の自動生成ータイル（デフォルト）
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/1/8
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "TileDefault.h"

/*------------------------------------------------------------------------------
	初期化
------------------------------------------------------------------------------*/
void TileDefault::Init( float height, float width, const Vector3& bottomLeftPosition, const Vector3& normal, E_TILE_TYPE type, const TextureUV& texUV)
{
	m_Height = height;
	m_Width = width;
	m_BottomLeftPosition = bottomLeftPosition;
	m_Normal = normal;
	m_Type = type;
	m_TexUV = texUV;
}

/*------------------------------------------------------------------------------
	位置と法線の更新
------------------------------------------------------------------------------*/
void TileDefault::Transform(D3DXMATRIX shapeMatrix)
{
	auto position = m_BottomLeftPosition.ConvertToDX();
	auto normal = m_Normal.ConvertToDX();

	D3DXVec3TransformCoord( &position, &position, &shapeMatrix);
	D3DXVec3TransformNormal( &normal, &normal, &shapeMatrix);

	m_BottomLeftPosition = Vector3::ConvertFromDX( position);
	m_Normal = Vector3::ConvertFromDX( normal);
}

/*------------------------------------------------------------------------------
	頂点バッファの設定
------------------------------------------------------------------------------*/
void TileDefault::SetVertexBuffer(VERTEX_3D* pVtx)
{
	//縮退ポリゴンに注意して頂点を設定
	auto vector = Vector3::Cross( m_Normal, Vector3(0.0f, 1.0f, 0.0f)).Normalize();
	Vector3 nextPosition = m_BottomLeftPosition + vector * m_Width;

	/*pVtx[ 0].Pos = pVtx[ 1].Pos = D3DXVECTOR3( m_BottomLeftPosition.x, m_BottomLeftPosition.y, m_BottomLeftPosition.z);
	pVtx[ 2].Pos = D3DXVECTOR3( m_BottomLeftPosition.x, m_BottomLeftPosition.y + m_Height, m_BottomLeftPosition.z);
	pVtx[ 3].Pos = D3DXVECTOR3( nextPosition.x, nextPosition.y, nextPosition.z);
	pVtx[ 4].Pos = pVtx[ 5].Pos = D3DXVECTOR3( nextPosition.x, nextPosition.y + m_Height, nextPosition.z);*/

	pVtx[ 0].Pos = pVtx[ 1].Pos = D3DXVECTOR3( m_BottomLeftPosition.x, m_BottomLeftPosition.y + m_Height, m_BottomLeftPosition.z);
	pVtx[ 2].Pos = D3DXVECTOR3( m_BottomLeftPosition.x, m_BottomLeftPosition.y, m_BottomLeftPosition.z);
	pVtx[ 3].Pos = D3DXVECTOR3( nextPosition.x, nextPosition.y + m_Height, nextPosition.z);
	pVtx[ 4].Pos = pVtx[ 5].Pos = D3DXVECTOR3( nextPosition.x, nextPosition.y, nextPosition.z);

	for (int i = 0; i < 6; i++)
	{
		pVtx[ i].Normal = m_Normal.Normalize().ConvertToDX();
		pVtx[ i].Color = D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f);
	}

	pVtx[0].Tex = pVtx[5].Tex = D3DXVECTOR2( 0.0f, 0.0f); 
	
	//UV（N字）
	pVtx[1].Tex = m_TexUV.GetTopLeft();
	pVtx[2].Tex = m_TexUV.GetBottomLeft();
	pVtx[3].Tex = m_TexUV.GetTopRight();
	pVtx[4].Tex = m_TexUV.GetBottomRight();
}

/*------------------------------------------------------------------------------
	頂点数の算出
------------------------------------------------------------------------------*/
int TileDefault::CulcCountVertex()
{
	return 4 + 2;
}

/*------------------------------------------------------------------------------
	ポリゴン数の算出
------------------------------------------------------------------------------*/
int TileDefault::CulcCountPolygon()
{
	return 6;
}
