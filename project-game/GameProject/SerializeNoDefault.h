/*==============================================================================
	
	SerializeNoDefault.h - �f�t�H���g�R���X�g���N�^�̂Ȃ��N���X���V���A���C�Y����⏕
														Author : Yutaka Suganuma
														Date   : 2017/11/9
==============================================================================*/
#ifndef _SERIALIZE_NO_DEFAULT_H_
#define _SERIALIZE_NO_DEFAULT_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	�f�t�H���g�R���X�g���N�^�̂Ȃ��N���X���V���A���C�Y����⏕
------------------------------------------------------------------------------*/
//namespace boost 
//{
//	namespace serialize 
//	{
//		//Transform
//		template <class Archive>
//		inline void save_construct_data (Archive& ar, const Transform* pTransform, const unsigned long int version)
//		{
//			ar << boost::serialization::make_nvp("GameObject",pTransform->m_pGameObject);
//			ar & boost::serialization::make_nvp("LocalMatrix", m_LocalMatrix);
//			ar & boost::serialization::make_nvp("Position", m_Position);
//			ar & boost::serialization::make_nvp("Scale", m_Scale);
//			ar & boost::serialization::make_nvp("Rotation", m_Rotation);
//		}
//
//		template <class Archive>
//		inline void load_construct_data ( Archive& ar, Transform* pTransform, const unsigned long int version)
//		{
//			GameObject* pGameObject;
//			ar >> boost::serialization::make_nvp("GameObject",pGameObject);
//			::new(pTransform)Transform( pGameObject);
//			ar & boost::serialization::make_nvp("LocalMatrix", m_LocalMatrix);
//			ar & boost::serialization::make_nvp("Position", m_Position);
//			ar & boost::serialization::make_nvp("Scale", m_Scale);
//			ar & boost::serialization::make_nvp("Rotation", m_Rotation);
//		}
//	}
//}


#endif