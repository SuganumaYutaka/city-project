/*==============================================================================

    CityAttribute.h - ���̎��������[����
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/29
==============================================================================*/
#ifndef _CITY_ATTRIBUTE_H_
#define _CITY_ATTRIBUTE_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Attribute.h"

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	//�_�i�����_�j
	class JunctionAttribute : public VertexAttribute
	{
	public:
		
	};

	//�Ӂi���H�j
	class RoadAttribute : public EdgeAttribute
	{
	private:
		float Width;
	public:
		RoadAttribute() : Width( 0.0f) {}
		float GetWidth( void) const{ return Width;}
		void SetWidth( float width){ Width = width;}
	};

	//�ʁi���j
	class LandAttribute : public FaceAttribute
	{
	public:
		
	};

	//�t�@�N�g���[
	class CityAttributeFactory : public AttributeFactory
	{
	public:
		virtual VertexAttribute* CreateVertexAttribute( void) { return new JunctionAttribute(); }
		virtual EdgeAttribute* CreateEdgeAttribute( void) { return new RoadAttribute(); }
		virtual FaceAttribute* CreateFaceAttribute( void){ return new LandAttribute(); }
	};
}

#endif

