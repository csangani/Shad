#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <map>

#include <Shad/Platform.h>
#include <PolyMesh/Lightning.h>

#include <GL\glew.h>
#include <GL\glut.h>

#include <PolyMesh/bitmap_image.h>
#include <PolyMesh/PolyMesh.h>

class Level {
	public:
	
	Level(int level);

	void generateBlocks(std::string shader, bitmap_image& space_image);
	
	void setTarget(float x, float y , float z);
	void destroyPlatforms();
	void changeUp();
	void drawPlatformEdges();
	void drawLightningBolts();
	void drawCharacterShadow(float characterX, float characterY, float characterZ);
	void applyLightningAnimationStep();

	void move(int deltaPoint, bool onGround, float charX, float charY, float charZ, Character * Shad);

	void reset();

	void collapse(bool onGround, float charX, float charY, float charZ);
	
	OpenMesh::Vec3f getTarget();

	private:
		int _level;
		btTransform origin;
		OpenMesh::Vec3f target;
		std::vector<Platform *> platforms;
		std::vector<Lightning *> lightningBolts;
		std::vector<Platform *> movingPlatforms;
		std::vector<Platform *> collapsiblePlatforms;
};

#endif