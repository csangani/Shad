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
		void SpecialScale(float scalex, float scaley, float scalez, OpenMesh::Vec3f parentCenterOfMass);

		void Translate(float tx, float ty, float tz);
		void Rotate(float angle, float x, float y, float z);
		void Draw(float * color);
		OpenMesh::Vec3f getStartPoint();
		OpenMesh::Vec3f getEndPoint();

		
	private:
		OpenMesh::Vec3f startPoint_, endPoint_;
		OpenMesh::Vec3f RotatePoint(OpenMesh::Vec3f point, float angle, float x, float y, float z);
		GLUquadricObj *quadric;
		
};

#endif