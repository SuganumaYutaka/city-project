/*==============================================================================

    LandManager.h - 町の自動生成ー土地情報管理
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/5
==============================================================================*/
#ifndef _LAND_MANAGER_H_
#define _LAND_MANAGER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class Land;

/*------------------------------------------------------------------------------
	クラス定義
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

