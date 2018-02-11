/*==============================================================================

    BuildingManager.h - �����̎��������[�����I�u�U�[�o�[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/24
==============================================================================*/
#ifndef _BUILDING_MANAGER_H_
#define _BUILDING_MANAGER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class Building;
class GameObject;
class GeometryParameter;
class BuildingSurfacePattern;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class BuildingManager
{
public:
	BuildingManager( GameObject* parent);
	~BuildingManager();
	void DeleteGameObject( void);

	Building* CreateBuilding( GeometryParameter* parameter, BuildingSurfacePattern* surfacePattern);
	Building* GetBuilding( int id);
	int GetBuildingID( Building* building);
	int GetBuildingCount( void);
	const std::vector<Building*>& GetBuildings( void){ return m_Buildings;}
	void Register( Building* building){ m_Buildings.push_back( building); }
	bool Unregister( Building* building);

	
private:
	std::vector<Building*> m_Buildings;
	GameObject* m_GameObject;

};

#endif

