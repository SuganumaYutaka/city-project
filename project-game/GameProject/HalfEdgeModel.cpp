/*==============================================================================

    HalfEdgeModel.cpp - �n�[�t�G�b�W�\���[���f��
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/25
==============================================================================*/

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include "HalfEdgeModel.h"
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
#include "HalfEdge.h"
#include "Rule.h"

using namespace HalfEdgeDataStructure;

/*------------------------------------------------------------------------------
	�f�X�g���N�^
------------------------------------------------------------------------------*/
Model::~Model()
{
	for (auto face : m_Faces)
	{
		delete face;
	}
	m_Faces.clear();

	for (auto edge : m_Edges)
	{
		delete edge;
	}
	m_Edges.clear();
	
	for (auto vertex : m_Vertices)
	{
		delete vertex;
	}
	m_Vertices.clear();

	if (m_Rule)
	{
		delete m_Rule;
	}

	if (m_AttributeFactory)
	{
		delete m_AttributeFactory;
	}
}

/*------------------------------------------------------------------------------
	�_�𐶐�
------------------------------------------------------------------------------*/
Vertex* Model::CreateVertex(const Vector3& position)
{
	return new Vertex( this, position, m_AttributeFactory->CreateVertexAttribute());
}

/*------------------------------------------------------------------------------
	�ӂ𐶐�
------------------------------------------------------------------------------*/
Edge* Model::CreateEdge( Vertex* start, Vertex* end)
{
	return new Edge( this, start, end, m_AttributeFactory->CreateEdgeAttribute());
}

/*------------------------------------------------------------------------------
	�ʂ𐶐�
------------------------------------------------------------------------------*/
Face* Model::CreateFace( HalfEdge* he)
{
	return new Face( this, he, m_AttributeFactory->CreateFaceAttribute());
}

/*------------------------------------------------------------------------------
	�͂��߂Ă̓_�𐶐�
------------------------------------------------------------------------------*/
bool Model::CreateFirstVertex(const Vector3& position)
{
	if (m_Vertices.size() > 0)
	{
		return false;
	}

	//�_�𐶐�
	CreateVertex( position);
	
	return true;
}

/*------------------------------------------------------------------------------
	�͂��߂Ă̕ӂ𐶐�
------------------------------------------------------------------------------*/
bool Model::CreateFirstEdge(const Vector3& startPosition, const Vector3& endPosition)
{
	if (m_Vertices.size() > 0)
	{
		return false;
	}

	//�_�𐶐�
	auto start = CreateVertex( startPosition);
	auto end = CreateVertex( endPosition);

	//�ӂ𐶐�
	auto edge = CreateEdge( start, end);
	
	//�_�ɕӂ�ݒ�
	start->RegisterEdge( edge);
	end->RegisterEdge( edge);

	return true;
}

/*------------------------------------------------------------------------------
	�͂��߂Ă̖ʂ𐶐�
------------------------------------------------------------------------------*/
bool Model::CreateFirstFace(
	const Vector3& topLeftPosition, const Vector3& topRightPosition,
	const Vector3& bottomLeftPosition, const Vector3& bottomRightPosition)
{
	if (m_Vertices.size() > 0)
	{
		return false;
	}

	//�_�𐶐�
	auto topLeft = CreateVertex( topLeftPosition);
	auto topRight = CreateVertex( topRightPosition);
	auto bottomLeft = CreateVertex( bottomLeftPosition);
	auto bottomRight = CreateVertex( bottomRightPosition);
	

	//�ӂ𐶐�
	auto top = CreateEdge( topLeft, topRight);
	auto bottom = CreateEdge( bottomRight, bottomLeft);
	auto left = CreateEdge( bottomLeft, topLeft);
	auto right = CreateEdge( topRight, bottomRight);

	//�_�ɕӂ�ݒ�
	topLeft->RegisterEdge( top);
	topLeft->RegisterEdge( left);
	topRight->RegisterEdge( top);
	topRight->RegisterEdge( right);
	bottomLeft->RegisterEdge( bottom);
	bottomLeft->RegisterEdge( left);
	bottomRight->RegisterEdge( bottom);
	bottomRight->RegisterEdge( right);

	//�n�[�t�G�b�W�𐶐�
	auto heTop = new HalfEdge( this, topRight);
	auto heRight = new HalfEdge( this, bottomRight);
	auto heBottom = new HalfEdge( this, bottomLeft);
	auto heLeft = new HalfEdge( this, topLeft);

	//�ӂɃn�[�t�G�b�W��ݒ�
	top->SetRight( heTop);
	right->SetRight( heRight);
	bottom->SetRight( heBottom);
	left->SetRight( heLeft);

	//�n�[�t�G�b�W�ɕӂ�next��ݒ�
	heTop->SetEdge( top);
	heRight->SetEdge( right);
	heBottom->SetEdge( bottom);
	heLeft->SetEdge( left);
	heTop->SetNext( heRight);
	heRight->SetNext( heBottom);
	heBottom->SetNext( heLeft);
	heLeft->SetNext( heTop);

	//�ʂ𐶐�
	auto face = CreateFace( heTop);

	//�n�[�t�G�b�W�ɖʂ�ݒ�
	heTop->SetFace( face);
	heRight->SetFace( face);
	heBottom->SetFace( face);
	heLeft->SetFace( face);
	
	return true;
}

/*------------------------------------------------------------------------------
	���ׂĂ̖ʂ𕪊�
------------------------------------------------------------------------------*/
bool Model::DivideAllFaces(void)
{
	if (m_Faces.size() == 0)
	{
		return false;
	}

	auto faces = m_Faces;

	for (auto face : faces)
	{
		//�����`�E�����`�Ɖ��肵�ĕ������[����ݒ�

		if (!m_Rule->DivideFace( face))
		{
			continue;
		}
	}

	return true;
}
