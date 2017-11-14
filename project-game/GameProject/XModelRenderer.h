/*==============================================================================

    XModelRenderer.h - モデル（Xファイル）の描画
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/5/7
==============================================================================*/
#ifndef _X_MODEL_RENDERER_H_
#define _X_MODEL_RENDERER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Renderer.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class XModelData;
class Material;

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	クラス定義
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
	XModelData* m_pData;							//Xモデルデータ
	std::vector<Material> m_vecMaterial;			//マテリアル

};

#endif

