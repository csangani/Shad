#include <Shad/Vertex.h>
#include <Shad/Cylinder.h>

#include <LinearMath/btQuaternion.h>
#include <LinearMath/btVector3.h>


Vertex::Vertex(OpenMesh::Vec3f _coordinate, OpenMesh::VertexHandle _vh)
{
	coordinate = _coordinate;
	vh = _vh;
}

OpenMesh::VertexHandle Vertex::getVH() {
	return vh;
}

OpenMesh::Vec3f Vertex::getCoordinates() {
	return coordinate;
}

void Vertex::Scale(float scalex, float scaley, float scalez) {
}
		
void Vertex::Translate(float tx, float ty, float tz) {
	coordinate[0] = coordinate[0] + tx;
	coordinate[1] = coordinate[1] + ty;
	coordinate[2] = coordinate[2] + tz;
}

void Vertex::Rotate(float angle, float x, float y, float z) {
}