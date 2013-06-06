#ifndef CYLINDER_H
#define CYLINDER_H

#include <GL\glew.h>
#include <GL\glut.h>

void RenderCylinder(float x1, float y1, float z1, float x2,float y2, float z2, float radius,int subdivisions, float *color, GLUquadricObj *quadric);

#endif