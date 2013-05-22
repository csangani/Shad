#include <OpenMesh\Core\Geometry\VectorT.hh>

#include <PolyMesh\PolyMesh.h>

class Cloth : public PolyMesh {
public:

	static class PinnedPoint  {
	public:
		VertexHandle h;
		PolyMesh *m;
		PinnedPoint(VertexHandle _h, PolyMesh *_m) : h(_h), m(_m){}
	};

	Cloth(float mass, float damping, OpenMesh::Vec3f RowVec,OpenMesh::Vec3f ColVec,OpenMesh::Vec3f Origin,int rows, int columns, float stretch, float bend, float segmentlength);
	int Rows() { return rows; }
	int Cols() { return columns; }
	float Stretch() { return stretch; }
	float Bend() { return bend; }
	float SegLen() { return segmentLength; }
	int Pin(int, int, PolyMesh *,VertexHandle);
	void Unpin(int);
	void SimulationStep();

private:
	int rows;
	int columns;
	float stretch;
	float bend;
	float segmentLength;
	float drag;
	OpenMesh::Vec3f RowVec;
	OpenMesh::Vec3f ColVec;
	OpenMesh::Vec3f Origin;
	std::vector<std::vector<VertexHandle>> Points;
	std::vector<std::vector<btRigidBody *>> RigidBody;
	std::map<int, Cloth::PinnedPoint> PinList;
};