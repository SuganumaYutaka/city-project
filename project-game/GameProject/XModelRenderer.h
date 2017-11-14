/*==============================================================================

    XModelRenderer.h - ���f���iX�t�@�C���j�̕`��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/7
==============================================================================*/
#ifndef _X_MODEL_RENDERER_H_
#define _X_MODEL_RENDERER_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Renderer.h"

/*------------------------------------------------------------------------------
	�O���錾
------------------------------------------------------------------------------*/
class XModelData;
class Material;

/*------------------------------------------------------------------------------
	�}�N����`
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class XModelRenderer : public Renderer
{
public:
	static Component* Create( GameObject* gameObject);

	XModelRenderer( GameObject *pGameObject);
	void Uninit( void);

	void LoadXModel( std::string FileName);
	void Update( void);
	void Draw( Camera* pCamera);
	std::vector<Material>& GetAllMaterial();

	virtual void Save( Text& text);
	virtual void Load( Text& text);

private:
	XModelData* m_pData;							//X���f���f�[�^
	std::vector<Material> m_vecMaterial;			//�}�e���A��

};

#endif

