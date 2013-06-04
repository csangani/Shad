#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <map>

#include <GL\glew.h>
#include <GL\glut.h>
#include <PolyMesh/bitmap_image.h>

#include <PolyMesh/PolyMesh.h>


using namespace std;

class Level {
	public:
	
	Level(int level);

	void generateBlocks(string shader, bitmap_image& space_image);
	void generateMesh(string shader, string object, float Diffuse[], float Ambient[], float Specular[], float Shininess[], bitmap_image &image, string texture); 

	private:
		void createPlatform(float scaleX, float scaleY, float scaleZ, float translateX, float translateY, float translateZ, float rotationAngle = 0, float rotateX = 0, float rotateY = 0, float rotateZ = 0);

		int _level;
		string LEVEL;
		string EXT;
		string filename;
		btTransform origin;
};

#endif