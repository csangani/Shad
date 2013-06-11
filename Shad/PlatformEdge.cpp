#include <Shad/PlatformEdge.h>
#include <Shad/Cylinder.h>

#include <LinearMath/btQuaternion.h>
#include <LinearMath/btVector3.h>

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

void PlatformEdge::SpecialScale(float scalex, float scaley, float scalez, OpenMesh::Vec3f parentCenterOfMass)
{
	Translate(-parentCenterOfMass[0],-parentCenterOfMass[1],-parentCenterOfMass[2]);
	Scale(scalex,scaley,scalez);
	Translate(parentCenterOfMass[0],parentCenterOfMass[1],parentCenterOfMass[2]);
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
	btVector3 vec(point[0],point[1],point[2]);
	btVector3 axis(x,y,z);
	axis.normalize();
	btVector3 rotatedVec = vec.rotate(axis,RADIANS(angle));

	return OpenMesh::Vec3f(rotatedVec.x(),rotatedVec.y(),rotatedVec.z());
}

#define RADIUS (0.025)
#define NUM_SUBDIVISIONS (32)

void PlatformEdge::Draw(float * color)
{
	//the same quadric can be re-used for drawing many cylinders
	gluQuadricNormals(quadric, GLU_SMOOTH);
	float x1 = startPoint_[0];
	float y1 = startPoint_[1];
	float z1 = startPoint_[2];
	float x2 = endPoint_[0];
	float y2 = endPoint_[1];
	float z2 = endPoint_[2];
	RenderCylinder(x1,y1,z1,x2,y2,z2,RADIUS,NUM_SUBDIVISIONS,color,quadric);
}

OpenMesh::Vec3f PlatformEdge::getStartPoint() {
	return startPoint_;
}
OpenMesh::Vec3f PlatformEdge::getEndPoint() {
	return endPoint_;
}