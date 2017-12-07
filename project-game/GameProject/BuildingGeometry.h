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

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class BuildingGeometry : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	BuildingGeometry( GameObject* pGameObject);
	void Uninit( void);

	bool Init( Land* land);

private:
	void Update(void);

	Land* m_land;
	std::list< Shape*> m_Shapes;
	
};

#endif

