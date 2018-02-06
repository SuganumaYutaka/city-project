/*==============================================================================

    BuildingGeometry.h - �����̎��������[�����W�I���g��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/
#ifndef _BUILDING_GEOMETRY_H_
#define _BUILDING_GEOMETRY_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class Shape;
class Tile;
class GeometryParameter;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class BuildingGeometry : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	BuildingGeometry( GameObject* pGameObject);
	void Uninit( void);

	void Init( GeometryParameter* parameter);
	void AddShape( Shape* shape){ m_Shapes.push_back( shape); }
	const std::list< Shape*>& GetShape( void){ return m_Shapes;}
	void ConfirmGeometry(void);

private:
	void Update(void);

	GeometryParameter* m_Parameter;
	std::list< Shape*> m_Shapes;
	
	Tile* SearchStartTile( Shape* shape, int floorCount);
	bool CanLookTile( Tile* tile, Shape* owner);

};

#endif

