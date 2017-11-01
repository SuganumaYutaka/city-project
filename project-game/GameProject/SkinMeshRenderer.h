/*==============================================================================

    SkinMeshRenderer.h - スキンメッシュの描画
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/9/21
==============================================================================*/
#ifndef _SKIN_MESH_RENDERER_H_
#define _SKIN_MESH_RENDERER_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"
#include "Renderer.h"
#include "SkinMeshDataFormat.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
class Material;
class SkinMeshModel;

/*------------------------------------------------------------------------------
	マクロ定義
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class SkinMeshRenderer : public Renderer
{
public:
	SkinMeshRenderer( GameObject *pGameObject);
	void Uninit( void);

	void Update( void);
	void Draw( Camera* pCamera);

	SkinMeshModel* GetSkinMeshModel(){ return m_pSkinMeshModel;}
	std::vector<Animation> GetAnimation() { return m_vecAnimation;}
	Animation GetAnimation( int index) { return m_vecAnimation[ index];}
	void LoadMeshData(FILE *pFile, SkinMeshModel* pModel);

	void SetScale( float rate);

private:
	std::vector<Material> m_vecMaterial;
	std::vector<Point> m_vecPoint;
	std::vector<D3DXVECTOR3> m_vecNormal;
	std::vector<D3DXVECTOR2> m_vecTexcoord;
	std::vector<unsigned short> m_vecPositionIndex;
	std::vector<unsigned short> m_vecNormalIndex;
	std::vector<unsigned short> m_vecTexcoordIndex;
	int m_NumPolygon;
	std::vector<Animation> m_vecAnimation;
	SkinMeshModel* m_pSkinMeshModel;
	void CreateVtxBuffer();
	void SetVtxBuffer();
};

#endif

