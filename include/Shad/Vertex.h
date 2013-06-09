#ifndef VERTEX_H
#define VERTEX_H

#include <GL\glew.h>
#include <GL\glut.h>

#include <PolyMesh/PolyMesh.h>

class Vertex {
	public:
		Vertex(OpenMesh::Vec3f _coordinate, OpenMesh::VertexHandle _vh);
		OpenMesh::Vec3f getCoordinates();
		OpenMesh::VertexHandle getVH();
		void Scale(float scalex, float scaley, float scalez);
		void Translate(float tx, float ty, float tz);
		void Rotate(float angle, float x, float y, float z);
		
	private:
		OpenMesh::Vec3f coordinate;
		OpenMesh::VertexHandle vh;
		
};

#endif