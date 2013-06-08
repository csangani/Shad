#include <OpenMesh\Core\Geometry\VectorT.hh>

#include <PolyMesh\PolyMesh.h>

class Cloth : public PolyMesh {
public:
	Cloth(float mass, float drag, float damping, OpenMesh::Vec3f RowVec,OpenMesh::Vec3f ColVec,OpenMesh::Vec3f Origin,int rows, int columns, float stretch, float bend, float segmentlength, BVEC3F& wind);
	int Rows() { return rows; }
	int Cols() { return columns; }
	float Stretch() { return stretch; }
	float Bend() { return bend; }
	float SegLen() { return segmentLength; }
	btPoint2PointConstraint *Pin(int, int, btRigidBody *,btVector3 *);
	void Unpin(btPoint2PointConstraint *);
	void SimulationStep();

private:
	int rows;
	int columns;
	float damping;
	float stretch;
	float bend;
	float segmentLength;
	float drag;
	BVEC3F wind;
	OpenMesh::Vec3f RowVec;
	OpenMesh::Vec3f ColVec;
	OpenMesh::Vec3f Origin;
	std::vector<std::vector<VertexHandle>> Points;
	std::vector<std::vector<btRigidBody *>> RigidBody;
};