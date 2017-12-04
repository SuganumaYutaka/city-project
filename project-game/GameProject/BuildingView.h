/*==============================================================================

    BuildingView.h - ���̎��������[�����r���[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/3
==============================================================================*/
#ifndef _BUILDING_VIEW_H_
#define _BUILDING_VIEW_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class BlockAttribute;
class MeshPolygonRenderer;
class Building;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class BuildingView : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	BuildingView( GameObject* pGameObject);
	void Uninit( void);

	void SetBuilding( Building* building, BlockAttribute* attribute);

private:
	void Update(void);
	Building* m_Building;
	BlockAttribute* m_Attribute;
	MeshPolygonRenderer* m_Renderer;
};

#endif

