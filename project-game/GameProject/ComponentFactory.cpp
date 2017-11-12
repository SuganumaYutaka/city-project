/*==============================================================================

    ComponentFactory.cpp - �R���|�[�l���g�����t�@�N�g���[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/11
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "ComponentFactory.h"
#include "ComponentInclude.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/
#define REGISTER(className) CreateMethods["className"] = className::Create

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
ComponentFactory::ComponentFactory()
{
	// �e�R���|�[�l���g��Create�֐���o�^
	CreateMethods.clear();
	//CreateMethods["Transform"] = Transform::Create;
	
	REGISTER(Transform);
	REGISTER(Camera);

}

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* ComponentFactory::Create(std::string componentName, GameObject* gameObject)
{
	if (CreateMethods.find(componentName) == CreateMethods.end())
	{
		std::string message = "ComponentFactory�� " + componentName + " ��o�^���Ă�������";
		MessageBox( NULL, message.c_str(), "�G���[", MB_OK);
		return NULL;
	}

	return CreateMethods[ componentName]( gameObject);
}

