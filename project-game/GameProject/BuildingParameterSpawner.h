/*==============================================================================

    BuildingParameterSpawner.h - �����̎��������[�����̐����ɗp����p�����[�^�[�𐶐�
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/6
==============================================================================*/
#ifndef _BUILDING_PARAMETER_SPAWNER_H_
#define _BUILDING_PARAMETER_SPAWNER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class ShapeParameter;
class GeometryParameter;
class BuildingSurfacePattern;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class GeometryParameterSpawner
{
public:
	GeometryParameter* operator()( const std::vector< BuildingSurfacePattern*>& surfacePatterns);

private:
	Random m_Random;

};

class ShapeParameterSpawner
{
public:
	void operator()( const std::vector<Vector3>& vertices, GeometryParameter* geometryParameter);

private:
	Random m_Random;

};

#endif

