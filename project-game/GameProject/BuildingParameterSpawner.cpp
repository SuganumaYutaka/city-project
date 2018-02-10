/*==============================================================================

    BuildingParameterSpawner.cpp - �����̎��������[�����̐����ɗp����p�����[�^�[�𐶐�
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/6
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "BuildingParameterSpawner.h"
#include "BuildingParameter.h"
#include "BuildingSurfacePattern.h"

/*------------------------------------------------------------------------------
	�W�I���g�������p�p�����[�^�[����
------------------------------------------------------------------------------*/
GeometryParameter* GeometryParameterSpawner::operator()( const std::vector< BuildingSurfacePattern*>& surfacePatterns)
{
	//�����_���̏�����
	m_Random.ResetSeed();
	
	//�p�����[�^�[����
	auto parameter = new GeometryParameter();
	
	//1�K�̍���
	m_Random.SetRangeFloat( 1.2f, 1.5f);
	parameter->m_GroundFloorHeight = m_Random.GetFloat();
	
	//�t���A�̍���
	m_Random.SetRangeFloat( 1.0f, 1.5f);
	parameter->m_FloorHeight = m_Random.GetFloat();

	//���̕�
	m_Random.SetRangeFloat( 0.8f, 1.5f);
	parameter->m_WindowWidth = m_Random.GetFloat();

	//���ւ̕�
	m_Random.SetRangeFloat( 2.0f, 2.5f);
	parameter->m_EntranceWidth = m_Random.GetFloat();

	//�\�ʃp�^�[��
	m_Random.SetRangeInt( 0, surfacePatterns.size() - 1);
	parameter->m_SurfacePatternID = m_Random.GetInt();

	//�����_���̃V�[�h�l
	parameter->m_RandomSeed = m_Random.GetSeed();

	return parameter;
}

/*------------------------------------------------------------------------------
	�`�󐶐��p�p�����[�^�[����
------------------------------------------------------------------------------*/
void ShapeParameterSpawner::operator()(const std::vector<Vector3>& vertices, GeometryParameter* geometryParameter)
{
	//�����_���̏�����
	m_Random.ResetSeed();
	
	//�y�n�̑傫��
	Vector3 vec01 = vertices[1] - vertices[0];
	Vector3 vec03 = vertices[3] - vertices[0];
	Vector3 landSize;
	landSize.x = vec01.Length();
	landSize.z = vec03.Length();

	//�`��̍���
	m_Random.SetRangeFloat( 20.0f, 25.0f);
	float heightMin = m_Random.GetFloat();
	m_Random.SetRangeFloat( 25.0f, 30.0f);
	float heightMax = heightMin + m_Random.GetFloat();

	//�`��̃T�C�Y�̔䗦
	float sizeRateMax = 0.9f;
	float sizeRateMin = 0.5f;
	
	//�`��̐�����
	m_Random.SetRangeInt( 1, 2);
	int shapeCount = m_Random.GetInt();

	//�`��̎��
	m_Random.SetRangeInt( 0, eShapeTypeCount - 1);
	E_SHAPE_TYPE type = (E_SHAPE_TYPE)m_Random.GetInt();

	//�`��̐���
	for (int i = 0; i < shapeCount; i++)
	{
		auto shapeParameter = new ShapeParameter();

		//�^�C�v�̐ݒ�
		shapeParameter->m_Type = type;

		//�����_���̏�����
		m_Random.ResetSeed();
		
		//��]�ʂ̐ݒ�
		shapeParameter->m_Rotation = 0.0f;

		//�傫���̐ݒ�
		m_Random.SetRangeFloat( sizeRateMin, sizeRateMax);
		shapeParameter->m_Size.x = landSize.x * m_Random.GetFloat();
		shapeParameter->m_Size.z = landSize.z * m_Random.GetFloat();

		//�����̐ݒ�
		m_Random.SetRangeFloat( heightMin, heightMax);
		shapeParameter->m_Size.y = m_Random.GetFloat();
	
		//�ʒu�̐ݒ�
		float range = ( landSize.x - shapeParameter->m_Size.x) * 0.5f;
		m_Random.SetRangeFloat( -range, range);
		shapeParameter->m_Position.x = m_Random.GetFloat();
		range = ( landSize.z - shapeParameter->m_Size.z) * 0.5f;
		m_Random.SetRangeFloat( -range, range);
		shapeParameter->m_Position.z = m_Random.GetFloat();
		//shapeParameter->m_Position.z = ( landSize.z - size.z) * 0.5f;

		//�W�I���g���ɒǉ�
		geometryParameter->AddShapeParameter( shapeParameter);
	}
}
