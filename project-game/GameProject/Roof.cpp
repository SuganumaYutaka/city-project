/*==============================================================================

    Roof.cpp - �����̎��������[����
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/13
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Roof.h"
#include "GameObject.h"
#include "ComponentInclude.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
Roof::Roof(GameObject* buildingObject)
{
	m_RoofObject = new GameObject( buildingObject);
	m_Renderer = NULL;
}

/*------------------------------------------------------------------------------
	������
------------------------------------------------------------------------------*/
void Roof::Init(const Vector3& position, float rotation, const Vector3& size)
{
	if (m_Renderer)
	{
		m_Renderer->ReleaseReserve();
		m_Renderer = NULL;
	}

	auto roofPosition = position;
	roofPosition.y += size.y;

	m_Renderer = m_RoofObject->AddComponent<Polygon3DRenderer>();
	m_RoofObject->m_pTransform->SetLocalPosition( roofPosition);
	m_RoofObject->m_pTransform->SetLocalRotationEuler( 0.0f, rotation, 0.0f);
	m_RoofObject->m_pTransform->SetLocalScale( size);
}
