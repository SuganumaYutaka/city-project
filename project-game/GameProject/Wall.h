/*==============================================================================

    Wall.h - 建物の自動生成ー壁
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/
#ifndef _WALL_H_
#define _WALL_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class Floor;
class WallRenderer;
class GameObject;
class BuildingRule;
class Texture;
class Tile;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Wall
{
public:
	Wall( GameObject* buildingObject);
	~Wall();
	void InitDefault( D3DXMATRIX shapeMatrix, float height, float width, const Vector3& bottomLeftPosition, const Vector3& normal, BuildingRule* rule);
	void InitCurve( D3DXMATRIX shapeMatrix, float height, float width, const Vector3& bottomLeftPosition, BuildingRule* rule);

	bool UpdateView( D3DXMATRIX shapeMatrix);
	
	void AddFloor( Floor* floor);
	void SubFloor( Floor* floor);

	float GetHeight(void) { return m_Height;}
	float GetWidth( void) { return m_Width;}
	const Vector3& GetBottomLeftPosition( void) { return m_BottomLeftPosition;}
	const Vector3& GetNormal( void) { return m_Normal;}
	const std::list<Floor*>& GetFloors( void){ return m_Floors;}

	void LoadTexture( std::string fileName);
	void FusionSameShape( Wall* other);
	bool ChangeRingList( void);
	bool Split( Wall* other);
	Vector3 GetVector( void);
	Vector3 GetCenter( void);
	float GetRadius( void);
	Tile* GetStartTile( int floorCount);
	void ClearRenderer( void);
	Tile* GetTopTile( void);
	WallRenderer* GetWallRenderer( void){ return m_Renderer;}

private:
	float m_Height;
	float m_Width;
	Vector3 m_BottomLeftPosition;
	Vector3 m_Normal;
	std::list<Floor*> m_Floors;
	WallRenderer* m_Renderer;
	bool m_IsCurve;

	int CulcCountVertex( void);
	int CulcCountPolygon( void);

	void Transform(D3DXMATRIX shapeMatrix);

	static bool SplitPlanes( Wall* source, Wall* dest);
	static bool SplitCylinders( Wall* source, Wall* dest);
	static bool SplitPlaneCylinder( Wall* plane, Wall* cylinder);

	static bool CulcPositionSplitPlanes( Wall* source, Wall* dest, Vector3* out = NULL);
	
	static bool InsertSplit( const std::list<Floor*>& floors1, const std::list<Floor*>& floors2, float length1, float length2, Vector3 positionSplit);
	
};

#endif

