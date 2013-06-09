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
	float drawCharacterShadow(float characterX, float characterY, float characterZ);
	void applyLightningAnimationStep();
	bool lightningCollisionWithPoint(OpenMesh::Vec3f point);
	
	void move(uint64_t deltaPoint, bool onGround, float charX, float charY, float charZ, Character * Shad);

	void shrink(uint64_t deltaPoint, float charX, float charY, float charZ, Character * Shad);

	void reset();

	void collapse(bool onGround, float charX, float charY, float charZ);
	
	OpenMesh::Vec3f getTarget();

	void deform(bool onGround, float charX, float charY, float charZ);

	private:
		int _level;
		btTransform origin;
		OpenMesh::Vec3f target;
		std::vector<Platform *> platforms;
		std::vector<Lightning *> lightningBolts;
		std::vector<Platform *> movingPlatforms;
		std::vector<Platform *> shrinkingPlatforms;
		std::vector<Platform *> collapsiblePlatforms;
		std::vector<Platform *> deformablePlatforms;

		PolyMesh * placeholder;
};

#endif