//---------------------------------------------------------------------------
#include "stdafx.h"
//---------------------------------------------------------------------------
#include "LinearAlgebra.h"
#include "LinearAlgebra.hpp"
//---------------------------------------------------------------------------



const Vector2ub NULL_VECTOR2UB = makeVector2ub(0, 0);
const Vector3ub NULL_VECTOR3UB = makeVector3ub(0, 0, 0);
const Vector4ub NULL_VECTOR4UB = makeVector4ub(0, 0, 0, 0);

const Vector2i NULL_VECTOR2I = makeVector2i(0, 0);
const Vector3i NULL_VECTOR3I = makeVector3i(0, 0, 0);
const Vector4i NULL_VECTOR4I = makeVector4i(0, 0, 0, 0);

const Vector2f NULL_VECTOR2F = makeVector2f(0.0, 0.0);
const Vector3f NULL_VECTOR3F = makeVector3f(0.0, 0.0, 0.0);
const Vector4f NULL_VECTOR4F = makeVector4f(0.0, 0.0, 0.0, 0.0);
const Vector6f NULL_VECTOR6F = makeVector6f(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
const Vector4f HOMOGENEOUS_NULL_VECTOR4F = makeVector4f(0.0, 0.0, 0.0, 1.0);

const Vector2d NULL_VECTOR2D = makeVector2d(0.0, 0.0);
const Vector3d NULL_VECTOR3D = makeVector3d(0.0, 0.0, 0.0);
const Vector4d NULL_VECTOR4D = makeVector4d(0.0, 0.0, 0.0, 0.0);

const Vector3f XAXIS_VECTOR3F = makeVector3f(1.0, 0.0, 0.0);
const Vector3f YAXIS_VECTOR3F = makeVector3f(0.0, 1.0, 0.0);
const Vector3f ZAXIS_VECTOR3F = makeVector3f(0.0, 0.0, 1.0);
const Vector4f XAXIS_VECTOR4F = makeVector4f(1.0, 0.0, 0.0, 1.0);
const Vector4f YAXIS_VECTOR4F = makeVector4f(0.0, 1.0, 0.0, 1.0);
const Vector4f ZAXIS_VECTOR4F = makeVector4f(0.0, 0.0, 1.0, 1.0);

const Matrix2f IDENTITY2F = makeMatrix2f(1.0, 0.0,
									              0.0, 1.0);
const Matrix3f IDENTITY3F = makeMatrix3f(1.0, 0.0, 0.0,
                                         0.0, 1.0, 0.0,
                                         0.0, 0.0, 1.0);
const Matrix4f IDENTITY4F = makeMatrix4f(1.0, 0.0, 0.0, 0.0,
                                         0.0, 1.0, 0.0, 0.0,
                                         0.0, 0.0, 1.0, 0.0,
                                         0.0, 0.0, 0.0, 1.0);
const Matrix6f IDENTITY6F = makeMatrix6f(1.0, 0.0, 0.0, 0.0, 0.0, 0.0,
                                         0.0, 1.0, 0.0, 0.0, 0.0, 0.0,
                                         0.0, 0.0, 1.0, 0.0, 0.0, 0.0,
                                         0.0, 0.0, 0.0, 1.0, 0.0, 0.0,
									              0.0, 0.0, 0.0, 0.0, 1.0, 0.0,
                                         0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
const Matrix3d IDENTITY3D = makeMatrix3d(1.0, 0.0, 0.0,
                                         0.0, 1.0, 0.0,
                                         0.0, 0.0, 1.0);


