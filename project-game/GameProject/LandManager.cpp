/*==============================================================================

    LandManager.cpp - 町の自動生成ー土地情報管理
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/5
==============================================================================*/

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "LandManager.h"
#include "Land.h"
#include "GameObject.h"

/*------------------------------------------------------------------------------
	コンストラクタ
------------------------------------------------------------------------------*/
LandManager::LandManager(GameObject* parent)
{
	m_GameObject = new GameObject( parent);
}

/*------------------------------------------------------------------------------
	デストラクタ
------------------------------------------------------------------------------*/
LandManager::~LandManager()
{
	for (auto land : m_Lands)
	{
		if(land)
		{
			delete land;
		}
	}
	m_Lands.clear();
	m_Lands.shrink_to_fit();
}

/*------------------------------------------------------------------------------
	GameObjectの消去
------------------------------------------------------------------------------*/
void LandManager::DeleteGameObject(void)
{
	if (m_GameObject)
	{
		m_GameObject->ReleaseReserve();
		m_GameObject = NULL;
	}
}

/*------------------------------------------------------------------------------
	土地を生成
------------------------------------------------------------------------------*/
Land* LandManager::CreateLand(const std::vector<Vector3>& vertices)
{
	auto land = new Land( this, m_GameObject);
	land->Init( vertices);

	return land;
}

/*------------------------------------------------------------------------------
	土地を取得
------------------------------------------------------------------------------*/
Land* LandManager::GetLand(int id)
{
	if (m_Lands.size() <= id)
	{
		return NULL;
	}

	return m_Lands[ id];
}

/*------------------------------------------------------------------------------
	土地のIDを取得
------------------------------------------------------------------------------*/
int LandManager::GetLandID( Land* land)
{
	int size = m_Lands.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Lands[i] == land)
		{
			return i;
		}
	}

	//発見できない
	return -1;
}

/*------------------------------------------------------------------------------
	登録を解除
------------------------------------------------------------------------------*/
bool LandManager::UnregisterLand( Land* land)
{
	int size = m_Lands.size();
	for (int i = 0; i < size; i++)
	{
		if (m_Lands[i] == land)
		{
			m_Lands[i] = NULL;
			return true;
		}
	}

	return false;
}

/*------------------------------------------------------------------------------
	土地の数を取得
------------------------------------------------------------------------------*/
int LandManager::GetLandCount(void)
{
	int size = m_Lands.size();
	int count = 0;
	for (int i = 0; i < size; i++)
	{
		if (m_Lands[i])
		{
			count++;
		}
	}

	return count;
}
