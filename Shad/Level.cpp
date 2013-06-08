#include <string>

#include <Shad/Level.h>

#include <GL\glew.h>
#include <GL\glut.h>

Level::Level(int level) {
	_level = level;
	platforms = std::vector<Platform *>();
	lightningBolts = std::vector<Lightning *>();

	platforms = std::vector<Platform *>();
	movingPlatforms = std::vector<Platform *>();
}
	
void Level::generateBlocks(std::string shader, bitmap_image& space_image) {
	std::string cube = "assets\\obj\\cube.obj";
	Platform *platform;
	switch(_level) {
		case 1:
			platform = new Platform(cube);
			platform->Scale(1,1,10);
			platform->Translate(0,-10,0);
			platforms.push_back(platform);
			platform->setMoving(true, 0, 0, 1);
			movingPlatforms.push_back(platform);

			platforms.push_back((new Platform(cube))->Scale(1, 5, 1)->Translate(0,-13,-6));

			platforms.push_back((new Platform(cube))->Scale(1, 1, 5)->Translate(2,-10,0));

			platforms.push_back((new Platform(cube))->Scale(1, 1, 5)->Translate(-2,-10,0));

			platforms.push_back((new Platform(cube))->Scale(1, 5, 1)->Translate(0,-15,-8));

			platforms.push_back((new Platform(cube))->Rotate(45, 0, 1, 1)->Scale(2, 2, 2)->Translate(0,-15,-11));

			platforms.push_back((new Platform(cube))->Rotate(35, 1, 0, 0)->Scale(10, 2, 2)->Translate(0,-15,-14));

			platforms.push_back((new Platform(cube))->Rotate(35, 1, 0, 0)->Scale(8, 2, 2)->Translate(0,-15,-16));

			platforms.push_back((new Platform(cube))->Rotate(35, 1, 0, 0)->Scale(6, 2, 2)->Translate(0,-15,-18));

			platforms.push_back((new Platform(cube))->Rotate(35, 1, 0, 0)->Scale(4, 2, 2)->Translate(0,-15,-20));

			platforms.push_back((new Platform(cube))->Scale(2, 2, 2)->Translate(0,-15,-22));

			lightningBolts.push_back(new Lightning(OpenMesh::Vec3f(-1,-10,0), OpenMesh::Vec3f(1,-5,0)));

			target = OpenMesh::Vec3f(0, -14, -22);
			break;
		case 2:
			platform = new Platform(cube);
			platform->Scale(1, 1, 1);
			platform->Translate(0, -5, 0);
			platforms.push_back(platform);
			platform->setMoving(true, 0, 0, 1);
			movingPlatforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(1, 5, 1);
			platform->Translate(0, -5, -3);
			platforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(5, 1, 5);
			platform->Translate(0, -5, -8);
			platforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(5, 10, 1);
			platform->Translate(0, -11, -14);
			platforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(1, 10, 5);
			platform->Translate(3, -11, -16);
			platforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(1, 4, 5);
			platform->Translate(-2, -8, -16);
			platforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(10, 1, 5);
			platform->Translate(-6, -12, -16);
			platforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(5, 10, 1);
			platform->Translate(0, -11, -18);
			platforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(1, 5, 1);
			platform->Translate(-13, -10, -14);
			platforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(1, 5, 1);
			platform->Translate(-13, -10, -19);
			platforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(5, 1, 6);
			platform->Translate(-12, -8, -16);
			platforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(5, 1, 6);
			platform->Translate(-14, -10, -16);
			platforms.push_back(platform);

			platform = new Platform(cube);
			platform->Scale(2, 2, 2);
			platform->Translate(-20, -10, -16);
			platforms.push_back(platform);

			break;
	};
}

void Level::changeUp() {
	_level++;
}

void Level::drawPlatformEdges()
{
	for (unsigned int i = 0; i < platforms.size(); i++) {
		Platform *platform = platforms[i];
		for (unsigned int j = 0; j < platform->edges.size(); j++) {
			PlatformEdge *edge = platform->edges[j];
			edge->Draw();
		}
	}
}

void Level::drawLightningBolts()
{
	for (unsigned int i = 0; i < lightningBolts.size(); i++) {
		Lightning *lightning = lightningBolts[i];
		lightning->Draw();
	}
}

void Level::applyLightningAnimationStep()
{
	for (unsigned int i = 0; i < lightningBolts.size(); i++) {
		Lightning *lightning = lightningBolts[i];
		lightning->Dim();

		if (lightning->isOff()) {
			lightning->Regenerate();
		}
	}
}

void Level::destroyPlatforms() {
	for(unsigned int i = 0; i < platforms.size(); i++) {
		Platform * p =  platforms[i];
		delete p;
	}
	platforms.clear();
	movingPlatforms.clear();
}

void Level::setTarget(float x, float y, float z) {
	target = OpenMesh::Vec3f(x, y, z);
}

void Level::move(int deltaPoint, bool onGround) {
	if (onGround == true) {
		//movingPlatforms[i]->moveWChar(deltaPoint);
	}
	else {
		for(uint i = 0; i < movingPlatforms.size(); i++) {
			movingPlatforms[i]->move(deltaPoint);
		}
	}
}





OpenMesh::Vec3f Level::getTarget() {
	return target;
}

