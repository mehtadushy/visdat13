#ifndef VisTypesH
#define VisTypesH

#include "LinearAlgebra.h"

struct Point3D
{
	Vector3f position;
	Vector4f color;
	float32 size;
	bool canBeModified;
	Vector3f normal;

	Point3D()
	{
		color= makeVector4f(1.0f,1.0f,0.0f,1.0f);
		size = 3.0f;
		canBeModified = true;
	}
	
	Point3D(Vector3f pos)
	{
		position = pos;
		color= makeVector4f(1.0f,1.0f,0.0f,1.0f);
		size = 3.0f;
		canBeModified = true;
	}
	Point3D(float x, float y, float z)
	{
		position = makeVector3f(x,y,z);
		color= makeVector4f(1.0f,1.0f,0.0f,1.0f);
		size = 3.0f;
		canBeModified = true;
	}
};

struct Point2D
{
	Vector2f position;
	Vector4f color;
	float32 size;
	bool canBeModified;
	
	Point2D()
	{
		color= makeVector4f(1.0f,1.0f,0.0f,1.0f);
		size = 3.0f;
		canBeModified = true;
	}
	
	Point2D(Vector2f pos)
	{
		position = pos;
		color= makeVector4f(1.0f,1.0f,0.0f,1.0f);
		size = 3.0f;
		canBeModified = true;
	}
	Point2D(float x, float y)
	{
		position = makeVector2f(x,y);
		color= makeVector4f(1.0f,1.0f,0.0f,1.0f);
		size = 3.0f;
		canBeModified = true;
	}
};

struct Line
{
	int vertices[2];
	Vector4f color;
	float32 thickness;
	
	Line()
	{
		color = makeVector4f(0.0f,1.0f,1.0f,1.0f);
		thickness  = 1.0f;
	}
};

struct Triangle
{
	int vertices[3];
	Vector4f color;
	Vector3f normal;
	
	Triangle()
	{
		color = makeVector4f(0.27f,0.27f,0.27f,1.0f);
		normal = makeVector3f(0.0,0.0,0.0);
	}	
};

#endif
