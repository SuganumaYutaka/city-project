/*==============================================================================

    BuildingRule.h - �����̎��������[�����𐶐����郋�[��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/12
==============================================================================*/
#ifndef _BUILDING_RULE_H_
#define _BUILDING_RULE_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class BuildingSurfacePattern; 
class BuildingGeometry;
class Wall;
class Floor;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class BuildingRule
{
public:
	BuildingRule( ) : m_SurfacePattern( NULL) {}
	virtual ~BuildingRule(){}

	virtual bool ProceduralShape( BuildingGeometry* geometry) = 0;
	virtual bool ProceduralFloor( Wall* wall) = 0;
	virtual bool ProceduralTile( Floor* floor) = 0;

	void SetSurfacePattern( BuildingSurfacePattern* surfacePattern) { m_SurfacePattern = surfacePattern;}
	BuildingSurfacePattern* GetSurfacePattern( void) { return m_SurfacePattern;}

private:
	BuildingSurfacePattern* m_SurfacePattern;
	
};

#endif

