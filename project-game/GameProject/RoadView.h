/*==============================================================================

    RoadView.h - ���̎��������[���H�r���[
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/12/1
==============================================================================*/
#ifndef _ROAD_VIEW_H_
#define _ROAD_VIEW_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class RoadAttribute;
class Polygon3DRenderer;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
//�_�i�����_�j
class RoadView : public Component
{
public:
	static Component* Create( GameObject* gameObject);

	RoadView( GameObject* pGameObject);
	void Uninit( void);

	void SetAttribute( RoadAttribute* attribute);

	void UpdateAttribute( void) { m_IsUpdatedAttribute = true;}

private:
	void Update(void);

	RoadAttribute* m_Attribute;
	Polygon3DRenderer* m_Renderer;

	bool m_IsUpdatedAttribute;
};

#endif

