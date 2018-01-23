/*==============================================================================

    Shape.h - �����̎��������[�`��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/
#ifndef _SHAPE_H_
#define _SHAPE_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class GameObject;
class Roof;
class Wall;
class BuildingRule;
class Tile;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class Shape
{
public:
	virtual ~Shape();
	virtual void Move( const Vector3& value) = 0;
	virtual void Rotate( float value) = 0;
	virtual void ScaleUpDown( const Vector3& value) = 0;
	virtual void ScaleRate( float rate) = 0;
	virtual void ScaleRate( const Vector3& rate) = 0;
	virtual void ConfirmShape( void){}

	void AddRoof( Roof* roof);
	void SubRoof(Roof* roof);
	void AddWall( Wall* wall);
	void SubWall( Wall* wall);
	void ClearWall( void);
	void ClearWallRenderer( void);

	std::list< Roof*>& GetRoofs( void) { return m_Roofs;}
	std::list< Wall*>& GetWalls( void) { return m_Walls;}
	const Vector3& GetPosition( void){ return m_Position;}
	float GetRotation( void){ return m_Rotation;}
	GameObject* GetBuildingObject( void) { return m_BuildingObject;}
	
	D3DXMATRIX GetMatrix( void);
	void UpdateView( void);
	void Split( Shape* other);
	Tile* GetStartTile( int floorCount);

	virtual bool CollisionPoint( const Vector3& point) = 0;
	Tile* GetTopTile( void);
	int GetFloorCount( void);

protected:
	void Uninit( void);
	void SetBuildingObject( GameObject* buildingObject) { m_BuildingObject = buildingObject;}
	void SetPosition( const Vector3& position){ m_Position = position;}
	void SetRotation( float rotation){ m_Rotation = rotation;}
	void UpdatePosition( const Vector3& value){ m_Position += value;}
	void UpdateRotation( float value){ m_Rotation += value;}
	BuildingRule* m_Rule;

private:
	std::list< Roof*> m_Roofs;
	std::list< Wall*> m_Walls;
	Vector3 m_Position;
	float m_Rotation;
	GameObject* m_BuildingObject;
};

#endif

