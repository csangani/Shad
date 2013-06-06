#include <Shad/Cylinder.h>
#include <math.h>

/* Code to draw cylinder found online.
   Source: http://lifeofaprogrammergeek.blogspot.com/2008/07/rendering-cylinder-between-two-points.html */

void RenderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions, float *color, GLUquadricObj *quadric)
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

	if (color != NULL) {
		glColor4fv(color);
	} else {
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	}

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