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
class TileSplit;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Floor
{
public:
	Floor();
	~Floor();
	void InitDefault( float height, float width, const Vector3& bottomLeftPosition, const Vector3& normal, E_FLOOR_TYPE type, BuildingRule* rule);
	void InitCurve( float height, float width, const Vector3& bottomLeftPosition, E_FLOOR_TYPE type, BuildingRule* rule);
	void Transform(D3DXMATRIX shapeMatrix);
	void SetTile( Tile* tile){ m_Tile = tile;}
	Tile* GetTile( void) { return m_Tile;}
	float GetHeight( void) { return m_Height;}
	float GetWidth( void) { return m_Width;}
	const Vector3& GetBottomLeftPosition( void) { return m_BottomLeftPosition;}
	const Vector3& GetNormal( void) { return m_Normal;}
	E_FLOOR_TYPE GetType( void) { return m_Type;}

	bool SetVertexBuffer( VERTEX_3D* pVtx);
	int CulcCountVertex( void);
	int CulcCountPolygon( void);

	void FusionSameShape( Floor* other);
	Tile* GetBackTile( void);
	bool ChangeRingList( void);
	bool InsertSplit( TileSplit* split);

private:
	Tile* m_Tile;
	float m_Height;
	float m_Width;
	Vector3 m_BottomLeftPosition;
	Vector3 m_Normal;
	E_FLOOR_TYPE m_Type;
};

#endif

