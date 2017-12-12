/*==============================================================================

    BuildingGeometry.cpp - �����̎��������[�����W�I���g��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/7
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "BuildingGeometry.h"
#include "GameObject.h"
#include "ComponentInclude.h"
#include "Land.h"

#include "BuildingRule.h"

/*------------------------------------------------------------------------------
	�R���|�[�l���g����
------------------------------------------------------------------------------*/
Component* BuildingGeometry::Create(GameObject* gameObject)
{
	return gameObject->AddComponent<BuildingGeometry>();
}

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
BuildingGeometry::BuildingGeometry( GameObject* pGameObject)
{
	m_pGameObject = pGameObject;
	m_pTransform = m_pGameObject->GetComponent<Transform>();
}

/*------------------------------------------------------------------------------
	�I������
------------------------------------------------------------------------------*/
void BuildingGeometry::Uninit( void)
{
	
}

/*------------------------------------------------------------------------------
	����������
------------------------------------------------------------------------------*/
bool BuildingGeometry::Init( const std::vector<Vector3>& vertices, BuildingRule* rule)
{
	//�y�n�̐���
	m_Land = new Land( m_pGameObject);
	
	//Position(World���W)��y�n�̒��S��
	Vector3 vec02 = vertices[2] - vertices[0];
	auto center = vertices[0] + vec02 * 0.5f;
	m_pTransform->SetWorldPosition( center);

	//�y�n�����H�ƕ��s�ƂȂ�悤�ɉ�]
	Vector3 vec21 = vertices[1] - vertices[2];
	m_pTransform->SetWorldRotationLookDirection( vec21);

	//���Έʒu���Z�o���ēy�n��ݒ�
	std::vector<Vector3> landVertices = vertices;
	for (auto& vertex : landVertices)
	{
		vertex -= center;
	}
	m_Land->Init( landVertices);

	//�`��̐���
	rule->ProceduralShape( this);

	return true;
}

/*------------------------------------------------------------------------------
	�X�V����
------------------------------------------------------------------------------*/
void BuildingGeometry::Update( void)
{
	
}

