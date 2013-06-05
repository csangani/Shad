#include <Shad/PlatformEdge.h>
#include <Spline/Quaternion.h>

#define RADIUS (0.015)
#define NUM_SUBDIVISIONS (32)

#define RADIANS(angle) (angle*((float)M_PI)/180.0f)

PlatformEdge::PlatformEdge(OpenMesh::Vec3f startPoint, OpenMesh::Vec3f endPoint)
{
	startPoint_ = startPoint;
	endPoint_ = endPoint;
	quadric = gluNewQuadric();
}

void PlatformEdge::Scale(float scalex, float scaley, float scalez)
{
	startPoint_ = OpenMesh::Vec3f(startPoint_[0]*scalex, startPoint_[1]*scaley, startPoint_[2]*scalez);
	endPoint_ = OpenMesh::Vec3f(endPoint_[0]*scalex, endPoint_[1]*scaley, endPoint_[2]*scalez);
}

void PlatformEdge::Translate(float tx, float ty, float tz)
{
	startPoint_ = OpenMesh::Vec3f(startPoint_[0]+tx, startPoint_[1]+ty, startPoint_[2]+tz);
	endPoint_ = OpenMesh::Vec3f(endPoint_[0]+tx, endPoint_[1]+ty, endPoint_[2]+tz);
}

void PlatformEdge::Rotate(float angle, float x, float y, float z)
{
	startPoint_ = RotatePoint(startPoint_,angle,x,y,z);
	endPoint_ = RotatePoint(endPoint_,angle,x,y,z);
}

OpenMesh::Vec3f PlatformEdge::RotatePoint(OpenMesh::Vec3f point, float angle, float x, float y, float z)
{
	Quaternion q = Quaternion(angle,OpenMesh::Vec3f(x,y,z));
	return q.Rotate(point);
}

void PlatformEdge::Draw()
{
	//the same quadric can be re-used for drawing many cylinders
	
	gluQuadricNormals(quadric, GLU_SMOOTH);
	float x1 = startPoint_[0];
	float y1 = startPoint_[1];
	float z1 = startPoint_[2];
	float x2 = endPoint_[0];
	float y2 = endPoint_[1];
	float z2 = endPoint_[2];

	RenderCylinder(x1,y1,z1,x2,y2,z2,RADIUS,NUM_SUBDIVISIONS,quadric);
	
	//gluDeleteQuadric(quadric);
}

/* Code to draw cylinder found online.
   Source: http://lifeofaprogrammergeek.blogspot.com/2008/07/rendering-cylinder-between-two-points.html */


void PlatformEdge::RenderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions,GLUquadricObj *quadric)
{
	
	float vx = x2-x1;
	float vy = y2-y1;
	float vz = z2-z1;

	//handle the degenerate case of z1 == z2 with an approximation
	if(vz == 0)
		vz = .0001;

	float v = sqrt( vx*vx + vy*vy + vz*vz );
	float ax = 57.2957795f*acos( vz/v );
	if ( vz < 0.0 )
		ax = -ax;
	float rx = -vy*vz;
	float ry = vx*vz;
	glPushMatrix();
	glLoadIdentity();

	//hardcoding red color for edge
	glColor3f(1.0,0.0,0.0);

	//draw the cylinder body
	glTranslatef( x1,y1,z1 );
	glRotatef(ax, rx, ry, 0.0);
	gluQuadricOrientation(quadric,GLU_OUTSIDE);
	gluCylinder(quadric, radius, radius, v, subdivisions, 1);

	//draw the first cap
	gluQuadricOrientation(quadric,GLU_INSIDE);
	gluDisk( quadric, 0.0, radius, subdivisions, 1);
	glTranslatef( 0,0,v );

	//draw the second cap
	gluQuadricOrientation(quadric,GLU_OUTSIDE);
	gluDisk( quadric, 0.0, radius, subdivisions, 1);
	glPopMatrix();
}

