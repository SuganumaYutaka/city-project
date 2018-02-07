/*==============================================================================

    Building.h - 町の自動生成ー建物
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/5
==============================================================================*/
#ifndef _BUILDING_H_
#define _BUILDING_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "BuildingParameter.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class GameObject;
class Land;
class LandManager;
class BuildingGeometry;
class BuildingManager;
class GeometryParameter;

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class Building
{
public:
	Building( BuildingManager* manager, GameObject* parent);
	~Building();
	void Delete( void);
	void OnDeleteBlock( void);
	void LinkLand( LandManager* landManager, int landID);
	void ConfirmGeometry(void);
	void InitGeometry( GeometryParameter* parameter);
	
private:
	BuildingManager* m_Manager;

	GameObject* m_GameObject;
	BuildingGeometry* m_Geometry;

	LandManager* m_LandManager;
	int m_LandID;
};

#endif

