///*==============================================================================
//
//    BuildingController.h - �����̎��������[�����R���g���[��
//                                                       Author : Yutaka Suganuma
//                                                       Date   : 2017/12/24
//==============================================================================*/
//#ifndef _BUILDING_CONTROLLER_H_
//#define _BUILDING_CONTROLLER_H_
//
///*------------------------------------------------------------------------------
//	�C���N���[�h�t�@�C��
//------------------------------------------------------------------------------*/
//#include "Manager.h"
//#include "Component.h"
//
///*------------------------------------------------------------------------------
//	�O���錾
//------------------------------------------------------------------------------*/
//class BuildingGeometry;
//class BuildingRule;
//class RoadAttribute;
//class BuildingManager;
//class CarManager;
//class WallRenderer;
//class Land;
//
///*------------------------------------------------------------------------------
//	�N���X��`
//------------------------------------------------------------------------------*/
//class BuildingController : public Component
//{
//public:
//	static Component* Create( GameObject* gameObject);
//
//	BuildingController( GameObject* pGameObject);
//	void Uninit( void);
//
//	bool Init( Land* land, BuildingRule* rule, std::list<RoadAttribute*> roads, BuildingManager* buildingManager, CarManager* carManager);
//
//	BuildingGeometry* GetGeometry( void){ return m_Geometry;}
//	
//	void ConfirmGeometry( void);
//	
//	void OnBuildingManagerDelete();
//
//private:
//	void Update(void);
//
//	BuildingManager* m_BuildingManager;
//	BuildingGeometry* m_Geometry;
//	
//};
//
//#endif
//
