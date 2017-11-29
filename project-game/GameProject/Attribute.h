/*==============================================================================

    Attribute.h - �n�[�t�G�b�W�\���[�����̒��ۃC���^�[�t�F�C�X
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/28
==============================================================================*/
#ifndef _ATTRIBUTE_H_
#define _ATTRIBUTE_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	//���ۃC���^�[�t�F�C�X
	class VertexAttribute
	{
	public:
		virtual ~VertexAttribute() {}
	};

	class EdgeAttribute
	{
	public:
		virtual ~EdgeAttribute() {}
	};

	class FaceAttribute
	{
	public:
		virtual ~FaceAttribute() {}
	};

	class AttributeFactory
	{
	public:
		virtual VertexAttribute* CreateVertexAttribute( void) = 0;
		virtual EdgeAttribute* CreateEdgeAttribute( void) = 0;
		virtual FaceAttribute* CreateFaceAttribute( void) = 0;
	};
}

#endif

