#ifndef __CS248_Assignments_V3__Lightning__
#define __CS248_Assignments_V3__Lightning__

#include <OpenMesh\Core\Geometry\VectorT.hh>
#include <PolyMesh\PolyMesh.h>

using namespace std;
using namespace OpenMesh;

struct Segment {
    Vec3f startPoint, endPoint;
    int dimFactor;
    
    Segment() {
        dimFactor = 0;
    }
    
    Vec3f normal() {
        return Vec3f(startPoint[1] - endPoint[1], endPoint[0] - startPoint[0], 0).normalize();
    };
};

class Lightning : public PolyMesh {
    
public:
    Lightning(Vec3f startPoint, Vec3f endPoint);
    vector<Segment> segments;
};

#endif /* defined(__CS248_Assignments_V3__Lightning__) */
