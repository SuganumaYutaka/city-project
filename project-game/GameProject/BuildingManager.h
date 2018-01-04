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
class BuildingController;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class BuildingManager
{
public:
	BuildingManager();

	void Register( BuildingController* buildingController);
	void Unregister( BuildingController* buildingController);

	const std::list<BuildingController*>& GetAllBuildings( void){ return m_Buildings;}

private:
	std::list<BuildingController*> m_Buildings;

};

#endif

