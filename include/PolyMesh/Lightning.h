#ifndef __CS248_Assignments_V3__Lightning__
#define __CS248_Assignments_V3__Lightning__

#include <OpenMesh\Core\Geometry\VectorT.hh>
#include <vector>

#include <GL\glew.h>
#include <GL\glut.h>

struct Segment {
	OpenMesh::Vec3f startPoint, endPoint;
    int dimFactor;
    
    Segment() {
        dimFactor = 0;
    }
    
	OpenMesh::Vec3f normal() {
		return OpenMesh::Vec3f(startPoint[1] - endPoint[1], endPoint[0] - startPoint[0], 0).normalize();
    };
};

class Lightning {    
	public:
		Lightning(OpenMesh::Vec3f startPoint, OpenMesh::Vec3f endPoint);
		void Regenerate();

		// Rendering
		void Draw();

		// Animation
		void Dim();
		void Brighten();
		bool isOff();
		void Jitter();

		// Collision
		bool CollidesWithPoint(OpenMesh::Vec3f point);

		std::vector<Segment> segments;
	
	private:
		float *color_;
		GLUquadricObj *quadric;
		void GenerateGeometry();
		void ResetColor();
		OpenMesh::Vec3f originalStartPoint, originalEndPoint;
};

#endif /* defined(__CS248_Assignments_V3__Lightning__) */
