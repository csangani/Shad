#include <PolyMesh\Lightning.h>
#include <Shad/Cylinder.h>

#include <time.h>


#define NUM_GENERATIONS (5)
#define INITIAL_OFFSET_AMOUNT (1.5)
#define MAX_ANGLE (2.5f)  
#define LENGTH_SCALE (1)

Lightning::Lightning(OpenMesh::Vec3f startPoint, OpenMesh::Vec3f endPoint)
{
	color_ = new float[4];
	ResetColor();

    Segment firstSegment;
	originalStartPoint = startPoint;
	originalEndPoint = endPoint;
    firstSegment.startPoint = startPoint;
    firstSegment.endPoint = endPoint;
    segments.push_back(firstSegment);
    quadric = gluNewQuadric();

	GenerateGeometry();
}

bool Lightning::isOff()
{
	return color_[3] <= 0.0f;
}

#define OPACITY_STEP_SIZE (0.025f)
#define	COLOR_STEP_DIVIDER (1.1f)

void Lightning::Dim()
{
	float Random = sinf((float)rand());
	//color_[0] /= COLOR_STEP_DIVIDER;
	//color_[1] /= COLOR_STEP_DIVIDER;
	//color_[2] /= COLOR_STEP_DIVIDER;
	color_[0] -= OPACITY_STEP_SIZE* (1 + 0.9f * Random);
	color_[1] -= OPACITY_STEP_SIZE* (1 + 0.9f * Random);
	color_[2] -= OPACITY_STEP_SIZE* (1 + 0.9f * Random);
	color_[3] -= OPACITY_STEP_SIZE * (1 + 0.9f * Random);
}

void Lightning::Brighten()
{
	//color_[0] *= COLOR_STEP_DIVIDER;
	//color_[1] *= COLOR_STEP_DIVIDER;
	//color_[2] *= COLOR_STEP_DIVIDER;
	color_[3] += OPACITY_STEP_SIZE;
}

void Lightning::Regenerate()
{
	segments.clear();
	Segment firstSegment;
	firstSegment.startPoint = originalStartPoint;
    firstSegment.endPoint = originalEndPoint;
    segments.push_back(firstSegment);

	ResetColor();
	GenerateGeometry();
}

void Lightning::ResetColor()
{
	color_[0] = 1.0;//102.0/255;
	color_[1] = 1.0;//163.0/255;
	color_[2] = 1.0;//210.0/255;
	color_[3] = 1.0;
}

void Lightning::Jitter()
{
	std::vector<Segment> newSegments;
	float jitterX, jitterY, jitterZ = 0.0f;

	for (unsigned int i = 0; i < segments.size(); i++) {
		Segment segment = segments[i];
		Segment newSegment;

		newSegment.startPoint = OpenMesh::Vec3f(segment.startPoint[0] + jitterX, segment.startPoint[1] + jitterY, segment.startPoint[2] + jitterZ);

		jitterX = ((float)rand()/(float)RAND_MAX)/10.0f;
		jitterY = ((float)rand()/(float)RAND_MAX)/10.0f;
		jitterZ = ((float)rand()/(float)RAND_MAX)/10.0f;
		
		newSegment.endPoint = OpenMesh::Vec3f(segment.endPoint[0] + jitterX, segment.endPoint[1] + jitterY, segment.endPoint[2] + jitterZ);
		newSegments.push_back(newSegment);
	}

	segments = newSegments;
}

void Lightning::GenerateGeometry()
{
	Segment firstSegment = segments[0];
	OpenMesh::Vec3f startPoint = firstSegment.startPoint;
	OpenMesh::Vec3f endPoint = firstSegment.endPoint;

	float offsetAmount = INITIAL_OFFSET_AMOUNT;
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
                float angle = -MAX_ANGLE + (float)rand()/((float)RAND_MAX/(2 * MAX_ANGLE));        
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
}

#define COLLISION_THRESHOLD (0.7)
#define MAX(a,b) ((a >= b) ? a : b)
#define MIN(a,b) ((a <= b) ? a : b)

bool Lightning::CollidesWithPoint(OpenMesh::Vec3f point)
{
	OpenMesh::Vec3f lightningLineDirection = (originalEndPoint - originalStartPoint).normalize();
	OpenMesh::Vec3f convertedPoint = point - originalStartPoint;
	OpenMesh::Vec3f projectedPoint = (convertedPoint[0] * lightningLineDirection[0] + convertedPoint[1] * lightningLineDirection[1] + convertedPoint[2] * lightningLineDirection[2])*lightningLineDirection;
	OpenMesh::Vec3f finalProjectedPoint = projectedPoint + originalStartPoint;

	// Check if projected point belongs to the line segment
	if (finalProjectedPoint[0] >= MIN(originalStartPoint[0],originalEndPoint[0]) && finalProjectedPoint[0] <= MAX(originalStartPoint[0],originalEndPoint[0])  
		&& finalProjectedPoint[1] >= MIN(originalStartPoint[1],originalEndPoint[1]) && finalProjectedPoint[1] <= MAX(originalStartPoint[1],originalEndPoint[1])
		&& finalProjectedPoint[2] >= MIN(originalStartPoint[2],originalEndPoint[2]) && finalProjectedPoint[2] <= MAX(originalStartPoint[2],originalEndPoint[2])) {
		return (point - finalProjectedPoint).length() < COLLISION_THRESHOLD;
	} else {
		return false;
	}
}

#define RADIUS (0.05f)
#define NUM_SUBDIVISIONS (32)
#define BRANCH_DECREASE_COEF (3)

void Lightning::Draw()
{
	for (unsigned int i = 0; i < segments.size(); i++) {
		Segment segment = segments[i];
		OpenMesh::Vec3f startPoint = segment.startPoint;
		OpenMesh::Vec3f endPoint = segment.endPoint;

		gluQuadricNormals(quadric, GLU_SMOOTH);
		float x1 = startPoint[0];
		float y1 = startPoint[1];
		float z1 = startPoint[2];
		float x2 = endPoint[0];
		float y2 = endPoint[1];
		float z2 = endPoint[2];
	
		RenderCylinder(x1,y1,z1,x2,y2,z2,RADIUS/(1+BRANCH_DECREASE_COEF*segment.dimFactor),NUM_SUBDIVISIONS,color_,quadric);
	}
}