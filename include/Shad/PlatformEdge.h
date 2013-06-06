#ifndef PLATFORMEDGE_H
#define PLATFORMEDGE_H

#include <GL\glew.h>
#include <GL\glut.h>

#include <PolyMesh/PolyMesh.h>

class PlatformEdge {
	public:
		PlatformEdge(OpenMesh::Vec3f startPoint, OpenMesh::Vec3f endPoint);
		void Draw();
		void Scale(float scalex, float scaley, float scalez);
		void Translate(float tx, float ty, float tz);
		void Rotate(float angle, float x, float y, float z);

	private:
		OpenMesh::Vec3f startPoint_, endPoint_;
		OpenMesh::Vec3f RotatePoint(OpenMesh::Vec3f point, float angle, float x, float y, float z);
		void RenderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions);
		GLUquadricObj *quadric;
};

#endif