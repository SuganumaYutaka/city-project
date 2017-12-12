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
	�񋓌^��`
------------------------------------------------------------------------------*/
//�傫���̒i�K
enum E_BUILDING_SIZE_LEVEL
{
	eBuildingSizeSmall = 0,
	eBuildingSizeMiddle,
	eBuildingSizeLarge,
	eBuildingSizeLevelCount
};

//�t���A�̎��
enum E_FLOOR_TYPE
{
	eFloorDefault = 0,
	eFloorGround,
	eFloorMargin,
	eFloorBorder,
	eFloorTypeCount
};

//�^�C���̎��
enum E_TILE_TYPE
{
	eTileWall = 0,
	eTileWindow,
	eTileEntrance,
	eTileBorder,
	eTileTypeCount
};

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
	std::mt19937  m_rand;
};

#endif

