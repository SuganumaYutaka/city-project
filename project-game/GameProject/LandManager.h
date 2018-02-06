/*==============================================================================

    LandManager.h - ���̎��������[�y�n���Ǘ�
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/5
==============================================================================*/
#ifndef _LAND_MANAGER_H_
#define _LAND_MANAGER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class Land;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class LandManager
{
private:
	std::vector<Land*> m_Lands;
	
public:
	~LandManager();

	Land* GetLand( int id);
	int GetLandID( Land* land);
	int GetLandCount( void){ return (int)m_Lands.size();}
	const std::vector<Land*>& GetLands( void){ return m_Lands;}
	void RegisterLand( Land* land){ m_Lands.push_back( land); }
	bool UnregisterLand( Land* land);
};


#endif

