#include <string>

#include "Level.h"

#include <GL\glew.h>
#include <GL\glut.h>

using namespace std;


Level::Level(int level) {
	_level = level;
	LEVEL = "level";
	EXT = ".ext";
	filename = LEVEL +  to_string(level) + EXT;
}

	
void Level::generateBlocks() {


}

