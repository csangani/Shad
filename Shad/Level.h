#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <map>

#include <GL\glew.h>
#include <GL\glut.h>
#include <PolyMesh/bitmap_image.h>



using namespace std;

class Level {
	public:
	
	Level(int level);

	void generateBlocks(string shader, bitmap_image& space_image);
	void generateMesh(string shader, string object, float Diffuse[], float Ambient[], float Specular[], float Shininess[], bitmap_image &image, string texture); 

	private:
		int _level;
		string LEVEL;
		string EXT;
		string filename;
};

#endif