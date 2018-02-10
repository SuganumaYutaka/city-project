/*==============================================================================
	
	CityController.h - ���̎��������[�R���g���[��
														Author : Yutaka Suganuma
														Date   : 2017/12/1
==============================================================================*/
#ifndef _CITY_CONTROLLER_H_
#define _CITY_CONTROLLER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class GameObject;
class CityAttributeManager;
class LandManager;
class BuildingManager;
class CarManager;
class BuildingSurfacePattern;
class LandParameter;
class GeometryParameter;

namespace HalfEdgeDataStructure
{
	class Model;
}

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class CityController : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	CityController( GameObject* pGameObject);
	void Uninit( void);
	void Init( float cityWidth, float cityHeight, int countDivide, int countCar, bool doConfirmGeometry);
	void Load( std::string filename);
	void ResetManagers( void);

private:
	void Update();

	HalfEdgeDataStructure::Model* m_Model;
	CityAttributeManager* m_CityAttributeManager;
	LandManager* m_LandManager;
	BuildingManager* m_BuildingManager;
	CarManager* m_CarManager;
	std::vector< BuildingSurfacePattern*> m_BuildingSurfacePatterns;

	void CreateFirstFace( float width, float height);
	void Divide( int count);
	void CreateAttribute( void);
	void CreateLand( LandParameter* parameter, int blockID);
	void CreateBuilding( GeometryParameter* parameter, int landID);
	void CreateCar( int spawnLandID, int targetLandID);

	float m_Width;
	float m_Height;

	Text m_ProcedualSaveData;
};


#endif

