/*==============================================================================
	
	TrafficJunction.h - ��ʃV�X�e���[�����_
														Author : Yutaka Suganuma
														Date   : 2017/12/25
==============================================================================*/
#ifndef _TRAFFIC_JUNCTION_H_
#define _TRAFFIC_JUNCTION_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class GameObject;
class JunctionAttribute;
class TrafficRoad;

/*------------------------------------------------------------------------------
	�񋓌^��`
------------------------------------------------------------------------------*/
enum TrafficJunctionBranch
{
	eJunctionBranchBack = 0,
	eJunctionBranchRight,
	eJunctionBranchLeft,
	eJunctionBranchFront
};

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class TrafficJunction : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	TrafficJunction( GameObject* pGameObject);
	void Uninit( void);

	JunctionAttribute* GetAttribute( void) { return m_Attribute;}
	void SetAttribute( JunctionAttribute* attribute);
	void UpdateAttribute( void) { m_IsUpdatedAttribute = true;}

	const std::vector< TrafficRoad*>& GetRoads( void){ return m_Roads;}
	
	bool CanTurnRight( TrafficRoad* road);
	bool CanTurnLeft( TrafficRoad* road);
	bool CanGoStraight( TrafficRoad* road);

private:
	void Update(void);

	JunctionAttribute* m_Attribute;
	bool m_IsUpdatedAttribute;

	void UpdateJunction( void);

	std::vector< TrafficRoad*> m_Roads;
	bool m_CanMoveVertical;
	bool m_CanMoveHorizontal;

};


#endif

