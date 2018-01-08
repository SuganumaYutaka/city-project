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

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class Shape
{
public:
	virtual ~Shape();
	virtual void Move( const Vector3& value) = 0;
	virtual void Rotate( float value) = 0;
	virtual void Scale( const Vector3& value) = 0;
	virtual void ConfirmShape( void){}

	void AddRoof( Roof* roof);
	void SubRoof(Roof* roof);
	void AddWall( Wall* wall);
	void SubWall( Wall* wall);

	std::list< Roof*>& GetRoofs( void) { return m_Roofs;}
	std::list< Wall*>& GetWalls( void) { return m_Walls;}

	GameObject* GetBuildingObject( void) { return m_BuildingObject;}

protected:
	void Uninit( void);
	
	void SetBuildingObject( GameObject* buildingObject) { m_BuildingObject = buildingObject;}
	
private:
	std::list< Roof*> m_Roofs;
	std::list< Wall*> m_Walls;
	GameObject* m_BuildingObject;
};

#endif

