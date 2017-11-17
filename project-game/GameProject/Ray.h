/*==============================================================================

    Ray.h - ����
                                                       Author : Yutaka Suganuma
                                                       Date   : 2017/11/17
==============================================================================*/
#ifndef _RAY_H_
#define _RAY_H_

/*------------------------------------------------------------------------------
	�C���N���[�h�t�@�C��
------------------------------------------------------------------------------*/
#include <string>
#include "Vector3.h"

/*------------------------------------------------------------------------------
	�N���X��`
------------------------------------------------------------------------------*/
class Ray
{
public:
	Vector3 Position;
	Vector3 Direction;

	Ray() {}
	Ray( Vector3 position, Vector3 direction) : Position(position) { Direction = direction.Normalize();}

	Vector3 GetPoint(float distance){return Position + Direction * distance;}

	std::string ConvertToString();
	int ConvertFromString(std::string& str, int current);

private:

};


#endif