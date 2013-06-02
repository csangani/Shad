#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <map>

#include <GL\glew.h>
#include <GL\glut.h>

using namespace std;

class Level {
	public:
	
	Level(int level);

	void generateBlocks(string shader);

	private:
		int _level;
		string LEVEL;
		string EXT;
		string filename;
};

#endif