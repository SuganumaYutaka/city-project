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
#define REGISTER(className) CreateMethods["className"] = className::Create

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
ComponentFactory::ComponentFactory()
{
	// 各コンポーネントのCreate関数を登録
	CreateMethods.clear();
	//CreateMethods["Transform"] = Transform::Create;
	
	REGISTER(Transform);
	REGISTER(Camera);

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

