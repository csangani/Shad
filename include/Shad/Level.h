#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <map>

#include <Shad/Platform.h>

#include <GL\glew.h>
#include <GL\glut.h>
#include <PolyMesh/bitmap_image.h>

#include <PolyMesh/PolyMesh.h>

#include <Shad/Platform.h>

using namespace std;

class Level {
	public:
	
	Level(int level);

	void generateBlocks(string shader, bitmap_image& space_image);
	void generateMesh(string shader, string object, float Diffuse[], float Ambient[], float Specular[], float Shininess[], bitmap_image &image, string texture); 

	void setTarget(float x, float y , float z);
	void destroyPlatforms();
	
	void changeUp();
	
	void drawPlatformEdges();

	OpenMesh::Vec3f getTarget();
	private:
		void createPlatform(float scaleX, float scaleY, float scaleZ, float translateX, float translateY, float translateZ, float rotationAngle = 0, float rotateX = 0, float rotateY = 0, float rotateZ = 0);

		int _level;
		string LEVEL;
		string EXT;
		string filename;
		btTransform origin;
		OpenMesh::Vec3f target;
		vector<Platform *> platforms;
};

#endif