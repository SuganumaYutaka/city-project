/*==============================================================================

    BuildingRuleFactory.h - �����̎��������[�����𐶐����郋�[���𐶐�����
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/8
==============================================================================*/
#ifndef _BUILDING_RULE_FACTORY_H_
#define _BUILDING_RULE_FACTORY_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class BuildingSurfacePattern;
class BuildingRule;
class Land;
class BlockAttribute;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class BuildingRuleFactory
{
public:
	BuildingRuleFactory();
	~BuildingRuleFactory();

	BuildingRule* CreateBuildingRule( const std::vector<Vector3>& vertices, BlockAttribute* attribute);


private:
	std::vector<BuildingSurfacePattern*> m_SurfacePatterns;
	Random* m_Random;
};

#endif

