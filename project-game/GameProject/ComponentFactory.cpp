/*==============================================================================

    ComponentFactory.cpp - コンポーネント生成ファクトリー
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/11
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "ComponentFactory.h"
#include "ComponentInclude.h"

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/
#define REGISTER(className) CreateMethods[#className] = className::Create

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
ComponentFactory::ComponentFactory()
{
	// 各コンポーネントのCreate関数を登録
	CreateMethods.clear();
	//CreateMethods["Transform"] = Transform::Create;
	
	REGISTER(Transform);
	REGISTER(BillboardRenderer);
	REGISTER(MeshBoxRenderer);
	REGISTER(MeshDomeRenderer);
	REGISTER(MeshFieldRenderer);
	REGISTER(MeshPlaneRenderer);
	REGISTER(ParticleRenderer);
	REGISTER(Polygon3DRenderer);
	REGISTER(SkinMeshRenderer);
	REGISTER(SpriteRenderer);
	REGISTER(XModelRenderer);
	REGISTER(AudioSource);
	REGISTER(BoxCollider);
	REGISTER(Camera);
	REGISTER(CameraController);
	REGISTER(Light);
	REGISTER(MeshField);
	REGISTER(MeshFieldCollider);
	REGISTER(PartsAnimator);
	REGISTER(SkinMeshModel);
	REGISTER(Slider);
	REGISTER(Sun);
	REGISTER(EditSystem);
	REGISTER(CityController);
	REGISTER(JunctionView);
	REGISTER(RoadView);
	REGISTER(BlockView);
}

/*------------------------------------------------------------------------------
	コンポーネント生成
------------------------------------------------------------------------------*/
Component* ComponentFactory::Create(std::string componentName, GameObject* gameObject)
{
	if (CreateMethods.find(componentName) == CreateMethods.end())
	{
		std::string message = "ComponentFactoryに " + componentName + " を登録してください";
		MessageBox( NULL, message.c_str(), "エラー", MB_OK);
		return NULL;
	}

	return CreateMethods[ componentName]( gameObject);
}

