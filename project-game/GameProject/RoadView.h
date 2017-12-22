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
class MeshPolygonRenderer;

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
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
	MeshPolygonRenderer* m_GrayRenderer;
	MeshPolygonRenderer* m_WhiteLineRenderer;

	bool m_IsUpdatedAttribute;

	void UpdateRenderer( void);
};

#endif

