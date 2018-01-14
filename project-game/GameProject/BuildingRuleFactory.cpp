/*==============================================================================

    BuildingRuleFactory.cpp - �����̎��������[�����𐶐����郋�[���𐶐�����
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/8
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "BuildingRuleFactory.h"
#include "BuildingRule.h"
#include "Land.h"
#include "BuildingSurfacePattern.h"
#include "DebugLog.h"

#include "BuildingRuleSimple.h"
#include "BuildingRuleCylinder.h"
#include "BuildingRuleMultiShape.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
BuildingRuleFactory::BuildingRuleFactory()
{
	m_Random = new Random();

	//�����̕\�ʃp�^�[����ǂݍ���
	m_SurfacePatterns.push_back( new BuildingSurfacePattern( "data/SCRIPT/BuildingSurfacePattern/test03.txt"));
	m_SurfacePatterns.push_back( new BuildingSurfacePattern( "data/SCRIPT/BuildingSurfacePattern/test02.txt"));
	

	m_Random->SetRangeInt( 0, (int)m_SurfacePatterns.size() - 1);
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
BuildingRuleFactory::~BuildingRuleFactory()
{
	for (auto pattern : m_SurfacePatterns)
	{
		delete pattern;
	}
	m_SurfacePatterns.clear();
	m_SurfacePatterns.shrink_to_fit();
}

/*------------------------------------------------------------------------------
	�����������[���𐶐�
------------------------------------------------------------------------------*/
BuildingRule* BuildingRuleFactory::CreateBuildingRule(const std::vector<Vector3>& vertices, BlockAttribute* attribute)
{
	//TODO:�y�n�̑傫������̑����ɉ����ă��[���𐶐�

	BuildingRule* rule;

	//�T�[�t�F�X�p�^�[���̌���
	int pattern = m_Random->GetInt();

	//���[���̐���
	//rule = BuildingRuleSimple::Create(m_SurfacePatterns[ pattern]);
	//rule = BuildingRuleCylinder::Create(m_SurfacePatterns[ pattern]);
	rule = BuildingRuleMultiShape::Create(m_SurfacePatterns[ pattern]);
	
	if (!rule)
	{
		DebugLog::Add( "BuildingRuleFactory:���[���̐����Ɏ��s���܂���\n");
		return NULL;
	}

	return rule;
}
