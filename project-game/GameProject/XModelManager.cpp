/*==============================================================================

    XModelManager.cpp - X���f���Ǘ�
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/7/5
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "XModelManager.h"
#include "XModelData.h"

/*------------------------------------------------------------------------------
	�R���X�g���N�^
------------------------------------------------------------------------------*/
XModelManager::XModelManager()
{
	
}

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
XModelManager::~XModelManager()
{
	for (auto itr = m_Map.begin(); itr != m_Map.end(); ++itr)
	{
		auto pXModelData = itr->second;
		delete pXModelData;
	}
	m_Map.clear();
}

/*------------------------------------------------------------------------------
	�e�N�X�`�����[�h
	����
	const char* pFileName
	�߂�l
	XModelData *
------------------------------------------------------------------------------*/
XModelData *XModelManager::Load( std::string FileName)
{
	//���łɓǂݍ��܂�Ă��邩
	auto iteTexture = m_Map.find( FileName);
	if (iteTexture != m_Map.end())
	{
		return m_Map[FileName];
	}

	//���[�h
	XModelData *pXModelData = new XModelData( FileName);
	m_Map[FileName] = pXModelData;
	return pXModelData;
}

