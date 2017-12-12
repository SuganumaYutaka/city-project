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

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
BuildingRuleFactory::BuildingRuleFactory()
{
	std::random_device ran;
	m_rand.seed( ran());

	//�����̕\�ʃp�^�[����ǂݍ���
	m_SurfacePatterns.push_back( new BuildingSurfacePattern( "data/SCRIPT/BuildingSurfacePattern/test.txt"));

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

	//Simple
	rule = BuildingRuleSimple::Create(m_SurfacePatterns[0], m_rand);
	if (!rule)
	{
		DebugLog::Add( "BuildingRuleFactory:(simple)���[���̐����Ɏ��s���܂���\n");
		return NULL;
	}

	return rule;
}
