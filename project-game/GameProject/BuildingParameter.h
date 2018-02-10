/*==============================================================================

    BuildingParameter.h - �����̎��������[�����̐����ɗp����p�����[�^�[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/5
==============================================================================*/
#ifndef _BUILDING_PARAMETER_H_
#define _BUILDING_PARAMETER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class BuildingSurfacePattern;

/*------------------------------------------------------------------------------
	�񋓌^��`
------------------------------------------------------------------------------*/
//�`��̎��
enum E_SHAPE_TYPE
{
	eShapeBox = 0,
	eShapeCylinder,
	eShapeTypeCount
};

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class ShapeParameter
{
public:
	ShapeParameter() : m_Type( eShapeBox), m_Position(Vector3()), m_Rotation(0.0f), m_Size(Vector3()) {}
	ShapeParameter( E_SHAPE_TYPE type, Vector3 position, Vector3 size) 
		: m_Type( type), m_Position( position), m_Size( size) {}

	E_SHAPE_TYPE m_Type;
	Vector3 m_Position;
	float m_Rotation;
	Vector3 m_Size;
};

class GeometryParameter
{
public:
	GeometryParameter() : m_GroundFloorHeight( 0.0f), m_FloorHeight( 0.0f), m_EntranceWidth( 0.0f), m_WindowWidth( 0.0f), m_SurfacePattern( NULL), m_RandomSeed(0){}
	GeometryParameter( float groundFloorHeight, float floorHeight, float entranceWidth, float windowWidth, BuildingSurfacePattern* surfacePattern, int randomSeed)
		: m_GroundFloorHeight( groundFloorHeight), m_FloorHeight( floorHeight), m_EntranceWidth( entranceWidth), m_WindowWidth( windowWidth)
		, m_SurfacePattern( surfacePattern), m_RandomSeed( randomSeed){}

	void AddShapeParameter( ShapeParameter* parameter){ m_ShapeParameters.push_back(parameter); }

	float m_GroundFloorHeight;
	float m_FloorHeight;
	float m_EntranceWidth;
	float m_WindowWidth;
	BuildingSurfacePattern* m_SurfacePattern;
	std::list<ShapeParameter*> m_ShapeParameters;
	int m_RandomSeed;
};

#endif

