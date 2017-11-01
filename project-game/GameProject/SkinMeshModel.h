/*==============================================================================

    SkinMeshModel.h - �X�L�����b�V�����f��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/10/09
==============================================================================*/
#ifndef _SKIN_MESH_MODEL_H_
#define _SKIN_MESH_MODEL_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Component.h"
#include "SkinMeshDataFormat.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class Material;

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class SkinMeshModel : public Component
{
public:
	SkinMeshModel( GameObject *pGameObject);
	void Uninit( void);

	void LoadModel( std::string FileName, float scaleRate = 1.0f);
	void Update( void);
	
	int GetAnimationState() const { return m_AnimationState;}
	int GetFrame() const {return m_CntFrame;}

	void SetScale( float rate);

private:
	std::string m_FileName;
	std::vector<AnimationClip> vecAnimationClip;
	bool m_bLoad;
	
	int m_CntFrame;			//���݂̃t���[��
	int m_AnimationState;	//���݂̃A�j���[�V����
};

#endif

