#include <string>

#include <Shad/Level.h>

#include <GL\glew.h>
#include <GL\glut.h>

Level::Level(int level) {
	_level = level;
	platforms = std::vector<Platform *>();
	lightningBolts = std::vector<Lightning *>();
}
	
void Level::generateBlocks(std::string shader, bitmap_image& space_image) {
	std::string cube = "assets\\obj\\cube.obj";
	Platform *platform;
	float *brightColor, *dimColor;
	switch(_level) {
		case 1:
			platforms.push_back((new Platform(cube))->Scale(1, 1, 10)->Translate(0,-10,0));

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

			brightColor = new float[4];
			brightColor[0] = 1.0;
			brightColor[1] = 1.0;
			brightColor[2] = 1.0;
			brightColor[3] = 1.0;

			dimColor = new float[4];
			dimColor[0] = 0.2;
			dimColor[1] = 0.2;
			dimColor[2] = 0.2;
			dimColor[3] = 1.0;

			lightningBolts.push_back(new Lightning(OpenMesh::Vec3f(-1,-10,0), OpenMesh::Vec3f(1,-5,0), brightColor));
			lightningBolts.push_back(new Lightning(OpenMesh::Vec3f(1,-10,0), OpenMesh::Vec3f(3,-5,0), dimColor));

			target = OpenMesh::Vec3f(0, -14, -22);
			break;
		case 2:
			platform = new Platform(cube);
			platform->Scale(1, 1, 1);
			platform->Translate(0, -5, 0);
			platforms.push_back(platform);

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

void Level::destroyPlatforms() {
	for(unsigned int i = 0; i < platforms.size(); i++) {
		Platform * p =  platforms[i];
		delete p;
	}
	platforms.clear();
}

void Level::setTarget(float x, float y, float z) {
	target = OpenMesh::Vec3f(x, y, z);
}

OpenMesh::Vec3f Level::getTarget() {
	return target;
}
