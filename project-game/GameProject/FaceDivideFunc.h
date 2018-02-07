/*==============================================================================

    FaceDivideFunc.h - 町の自動生成ー面を分割する処理
                                                       Author : Yutaka Suganuma
                                                       Date   : 2018/2/6
==============================================================================*/
#ifndef _FACE_DIVIDE_FUNC_H_
#define _FACE_DIVIDE_FUNC_H_

/*------------------------------------------------------------------------------
	インクルードファイル
------------------------------------------------------------------------------*/
#include "Manager.h"

/*------------------------------------------------------------------------------
	前方宣言
------------------------------------------------------------------------------*/
namespace HalfEdgeDataStructure
{
	class Vertex;
	class Edge;
	class Face;
	class HalfEdge;
	class Model;
}

/*------------------------------------------------------------------------------
	クラス定義
------------------------------------------------------------------------------*/
class FaceDivideFunc
{
public:
	bool operator() ( HalfEdgeDataStructure::Face* face);

private:
	
};

#endif

