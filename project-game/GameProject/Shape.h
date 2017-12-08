/*==============================================================================

    Shape.h - 建物の自動生成ー形状
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/
#ifndef _SHAPE_H_
#define _SHAPE_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class GameObject;
class Roof;
class Wall;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Shape
{
public:
	virtual void Move( const Vector3& value) = 0;
	virtual void Rotate( float value) = 0;
	virtual void Scale( const Vector3& value) = 0;

protected:
	void Uninit( void);
	void AddRoof( Roof* roof);
	void AddWall( Wall* wall);
	void SubRoof(Roof* roof);
	void SubWall( Wall* wall);

	std::list< Roof*>& GetRoofs( void) { return m_Roofs;}
	std::list< Wall*>& GetWalls( void) { return m_Walls;}

	void SetBuildingObject( GameObject* buildingObject) { m_BuildingObject = buildingObject;}
	GameObject* GetBuildingObject( void) { return m_BuildingObject;}
	
private:
	std::list< Roof*> m_Roofs;
	std::list< Wall*> m_Walls;
	GameObject* m_BuildingObject;
};

#endif

