#ifndef __CS248_Assignments_V3__Lightning__
#define __CS248_Assignments_V3__Lightning__

#include <OpenMesh\Core\Geometry\VectorT.hh>
#include <PolyMesh\PolyMesh.h>

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

class Lightning : public PolyMesh {
    
public:
	Lightning(OpenMesh::Vec3f startPoint, OpenMesh::Vec3f endPoint);
	std::vector<Segment> segments;
};

#endif /* defined(__CS248_Assignments_V3__Lightning__) */
