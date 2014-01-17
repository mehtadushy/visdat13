#ifndef MarchingCubes3DH
#define MarchingCubes3DH

#include "Persistent.h"
#include "LinearAlgebra.h"

/**
 *        v7             v6
 *        +----- e6 -----+
 *       /|             /|
 *     e10|          e11 |
 *     /  |	          /  |
 * v3 +----- e2 -----+   |
 *	   |   |        v2|   e5
 *	   |   e7         |   |
 *	   |   |          |   |
 *	   e3  |v4        e1  |
 *	   |   +----- e4 -|---+ v5 
 *	   |  /           |  /
 *	   | e8           | e9
 *	   |/             |/
 *    +----- e0 -----+ 
 *   v0               v1
 *
 *  order of voxel corners and edges
 **/

class MarchingCubes3D : public Persistent
{
	private:
		
		/// Compute the interpolated point on an edge
		static Vector3f getEdgePoint( const Vector3f* corners, const float32* values, unsigned int edgeIndex, const float& isoValue );
	
	public:
		MarchingCubes3D();
		~MarchingCubes3D();

		/// Triangulate a cube; return number of faces created
		static card32 triangulate( 
			const Vector3f* corners, 
			const float32* values, 
			const float& isoValue, 
			vector<Vector3f>& newVertices );
};

#endif
