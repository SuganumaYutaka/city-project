/*==============================================================================

    Floor.h - 建物の自動生成ーフロア（階層）
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/
#ifndef _FLOOR_H_
#define _FLOOR_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "BuildingRuleFactory.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class Tile;
class BuildingRule;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Floor
{
public:
	Floor();
	~Floor();
	void Init( float height, float width, E_FLOOR_TYPE type, BuildingRule* rule);
	
	void SetTile( Tile* tile){ m_Tile = tile;}
	Tile* GetTile( void) { return m_Tile;}

	bool SetVertexBuffer( VERTEX_3D* pVtx, const Vector3& bottomLeftPosition, const Vector3& normal, const Vector3& vector);
	bool SetVertexBufferCurve( VERTEX_3D* pVtx, const Vector3& bottomLeftPosition, const Vector3& center, float radius);

	int CulcCountVertex( void);
	int CulcCountVertexCurve( float radius);

	int CulcCountPolygon( void);
	int CulcCountPolygonCurve( float radius);

	float GetHeight( void) { return m_Height;}
	float GetWidth( void) { return m_Width;}
	E_FLOOR_TYPE GetType( void) { return m_Type;}

private:
	Tile* m_Tile;
	float m_Height;
	float m_Width;
	E_FLOOR_TYPE m_Type;
};

#endif

