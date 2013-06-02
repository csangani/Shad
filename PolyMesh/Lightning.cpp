#include <PolyMesh\Lightning.h>

#define NUM_GENERATIONS (5)
#define INITIAL_OFFSET_AMOUNT (0.5)
#define MAX_ANGLE (5)  
#define LENGTH_SCALE (0.7)
#include <time.h>

Lightning::Lightning(OpenMesh::Vec3f startPoint, OpenMesh::Vec3f endPoint)
{
    srand((unsigned int)time(NULL));
    float offsetAmount = INITIAL_OFFSET_AMOUNT;
    Segment firstSegment;
    firstSegment.startPoint = startPoint;
    firstSegment.endPoint = endPoint;
    segments.push_back(firstSegment);
    
    for (int i = 0; i < NUM_GENERATIONS; i++) {
		std::vector<Segment> newSegments;
        
        int counter = 0;
		for (std::vector<Segment>::iterator it = segments.begin(); it != segments.end(); ++it) {
			OpenMesh::Vec3f midPoint = (it->startPoint + it->endPoint)/2.0;
			OpenMesh::Vec3f normal = OpenMesh::Vec3f(startPoint[1] - endPoint[1], endPoint[0] - startPoint[0], 0).normalize();
            
            //offset midpoint
            float randomOffset = -offsetAmount + (float)rand()/((float)RAND_MAX/(2 * offsetAmount));
            midPoint = midPoint + normal*randomOffset;
            
            Segment newSegment1;
            newSegment1.startPoint = it->startPoint;
            newSegment1.endPoint = midPoint;
            
            Segment newSegment2;
            newSegment2.startPoint = midPoint;
            newSegment2.endPoint = it->endPoint;
            
            if (it->dimFactor > 0) {
                newSegment1.dimFactor = it->dimFactor;
                newSegment2.dimFactor = it->dimFactor;
            }
            
            //branch off occasionally, more likely if towards the end of lightning
            float iterationRank = (float)counter/(float)segments.size();
            float doBranching = (float)rand()/(float)RAND_MAX;
            
            if (doBranching > 0.01/iterationRank) {
                
                //third segment to branch off
				OpenMesh::Vec3f direction = midPoint - it->startPoint;
                
                //rotate a little bit
                float angle = (float)rand()/((float)RAND_MAX/(MAX_ANGLE));        
                float angleRadians = (float)(angle * M_PI) / (float)180.0;
                float newX = (direction[0] - midPoint[0] * cos(angleRadians)) - ((midPoint[1] - direction[1]) * sin(angleRadians)) + midPoint[0];
                float newY = (direction[1] - midPoint[1] * cos(angleRadians)) - ((midPoint[0] - direction[0]) * sin(angleRadians)) + midPoint[1];
				OpenMesh::Vec3f splitEnd = OpenMesh::Vec3f(newX, newY, 0) * LENGTH_SCALE + midPoint;
                Segment newSegment3;
                newSegment3.startPoint = midPoint;
                newSegment3.endPoint = splitEnd;
                newSegment3.dimFactor = it->dimFactor + 1;
                newSegments.push_back(newSegment3);
            }
            
            newSegments.push_back(newSegment1);
            newSegments.push_back(newSegment2);
            
            counter++;
        }
        
        offsetAmount /= 2;
        segments = newSegments;
    }

	for (std::vector<Segment>::iterator it = segments.begin(); it != segments.end(); ++it) {
		OpenMesh::Vec3f startPoint = it->startPoint;
		OpenMesh::Vec3f endPoint = it->endPoint;
        float rayRankMultiplier = (float)1.0/(it->dimFactor+1);        
        float smallRayFactor = 0.005;
	 
		//middle quad as two triangles
		OpenMesh::Vec3f point1 = startPoint - it->normal() * smallRayFactor * rayRankMultiplier;
		OpenMesh::Vec3f point2 = startPoint + it->normal() * smallRayFactor * rayRankMultiplier;
		OpenMesh::Vec3f point3 = endPoint + it->normal() * smallRayFactor * rayRankMultiplier;
		OpenMesh::Vec3f point4 = endPoint - it->normal() * smallRayFactor * rayRankMultiplier;

		VertexHandle v1 = add_vertex(point1);
		VertexHandle v2 = add_vertex(point2);
		VertexHandle v3 = add_vertex(point3);
		VertexHandle v4 = add_vertex(point4);

		add_face(v1,v2,v3);
		add_face(v1,v3,v4);
	 }

}