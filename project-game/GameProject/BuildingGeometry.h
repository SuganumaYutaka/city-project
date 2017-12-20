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
class Land;
class Shape;
class BuildingRule;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class BuildingGeometry : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	BuildingGeometry( GameObject* pGameObject);
	void Uninit( void);

	bool Init( const std::vector<Vector3>& vertices, BuildingRule* rule);

	Land* GetLand( void) { return m_Land;}
	void AddShape( Shape* shape){ m_Shapes.push_back( shape); }

private:
	void Update(void);

	Land* m_Land;
	std::list< Shape*> m_Shapes;
	BuildingRule* m_Rule;
};

#endif

