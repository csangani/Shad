#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <map>

#include <Shad/Platform.h>
#include <PolyMesh/Lightning.h>

#include <PolyMesh/Cloth.h>
#include <PolyMesh/ParticleCloth.h>

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

	void move(uint64_t deltaPoint, bool onGround, float charX, float charY, float charZ, Character * Shad, ParticleCloth * cape);
	bool elevate(bool onGround, float charX, float charY, float charZ, Character * Shad, ParticleCloth * cape);
	void shrink(uint64_t deltaPoint, float charX, float charY, float charZ, Character * Shad);

	void reset();

	void collapse(bool onGround, float charX, float charY, float charZ);

	OpenMesh::Vec3f getTarget();

	void deform(bool onGround, float charX, float charY, float charZ);

	void setStartPosition(float startX, float startY, float startZ);

	btTransform getStartPosition();

	void setFallLimit(float threshold);

	float getFallLimit();

private:
	int _level;

	btTransform origin;
	OpenMesh::Vec3f start;
	OpenMesh::Vec3f target;
	float fallLimit;
	std::vector<Platform *> platforms;
	std::vector<Lightning *> lightningBolts;
	std::vector<Platform *> movingPlatforms;
	std::vector<Platform *> shrinkingPlatforms;
	std::vector<Platform *> collapsiblePlatforms;
	std::vector<Platform *> elevatablePlatforms;
	PolyMesh *pinTarget;
	Cloth *Cloak;
	bitmap_image cloth_image;

	void Gavin();
	void Chirag();
	void Johan();
	void Cassidy();
	void Amit();
};

#endif