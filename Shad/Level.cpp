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
			platform->setMoving(true, 0, 0, 0.1);
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

		case 3:
			platform = new Platform(cube);
			platform->Scale(5,1,10);
			platform->Translate(0,-10,0);
			platforms.push_back(platform);
			//platform->setMoving(true, 0, 0, 1);
			//movingPlatforms.push_back(platform);

			//platforms.push_back((new Platform(cube))->Scale(1, 5, 1)->Translate(0,-13,-6));

			platforms.push_back((new Platform(cube))->Scale(0.25, 12, 10)->Translate(-3,-10,0));

			platform = new Platform(cube);
			platforms.push_back(platform->Scale(5, 1, 5)->Translate(-2,-10,-13));
			platform->setMoving(true, 0, 0, 0.9); 
			movingPlatforms.push_back(platform); 

			/*

			platforms.push_back((new Platform(cube))->Scale(1, 5, 1)->Translate(0,-15,-8));

			platforms.push_back((new Platform(cube))->Rotate(45, 0, 1, 1)->Scale(2, 2, 2)->Translate(0,-15,-11));

			platforms.push_back((new Platform(cube))->Rotate(35, 1, 0, 0)->Scale(10, 2, 2)->Translate(0,-15,-14));

			platforms.push_back((new Platform(cube))->Rotate(35, 1, 0, 0)->Scale(8, 2, 2)->Translate(0,-15,-16));

			platforms.push_back((new Platform(cube))->Rotate(35, 1, 0, 0)->Scale(6, 2, 2)->Translate(0,-15,-18));

			platforms.push_back((new Platform(cube))->Rotate(35, 1, 0, 0)->Scale(4, 2, 2)->Translate(0,-15,-20));

			platforms.push_back((new Platform(cube))->Scale(2, 2, 2)->Translate(0,-15,-22));

			*/

			lightningBolts.push_back(new Lightning(OpenMesh::Vec3f(-7,-10,-10), OpenMesh::Vec3f(-10,-5,-10)));

			target = OpenMesh::Vec3f(0, -14, -22);
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

void Level::drawCharacterShadow(float characterX, float characterY, float characterZ)
{
	Platform *foundPlatform = NULL;
	float maxY = -1000.f;
	for (unsigned int i = 0; i < platforms.size(); i++) {
		Platform *platform = platforms[i];
		btVector3 platformMin = BVECO(platform->platformMesh->min);
		btVector3 platformMax = BVECO(platform->platformMesh->max);
		btVector3 platformOrigin = platform->platformMesh->RigidBody->getCenterOfMassPosition();
		// skip if character is below platform
		if (characterY < platformOrigin.y())
			continue;
		// if character is within platform bounds, check platform Y-value to find highest platform (closest to character)
		if (abs(characterX - platformOrigin.x()) < platformMax.x()
			&& abs(characterZ - platformOrigin.z()) < platformMax.z())
		{
			if (platformOrigin.y() > maxY) {
				foundPlatform = platform;
				maxY = platformOrigin.y();
			}
		}
	}
	// draw shadow on platform
	if (foundPlatform != NULL) {
		float platformYScale = foundPlatform->platformMesh->max[1];
		float shadowOffset = 0.01f;
		float r = 0.2f/(characterY - maxY);
		int num_segments = 20;
		glColor4f(0.f, 0.f, 0.f, 0.3f);
		glBegin(GL_POLYGON); 
		for(int i = 0; i < num_segments; i++) 
		{ 
			float theta = 2.0f * float(M_PI) * float(i) / float(num_segments);//get the current angle 

			float x = r * cosf(theta);//calculate the x component 
			float z = r * sinf(theta);//calculate the z component 

			glVertex3d(x + characterX, maxY + platformYScale + shadowOffset, z + characterZ);//output vertex 
		} 
		glEnd(); 
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

void Level::move(int deltaPoint, bool onGround, float charX, float charY, float charZ) {
	if (onGround == true) {
		for(unsigned int i = 0; i < movingPlatforms.size(); i++) {
			movingPlatforms[i]->moveWChar(deltaPoint, charX, charY, charZ);
		}
	}
	else {
		for(unsigned int i = 0; i < movingPlatforms.size(); i++) {
			movingPlatforms[i]->move(deltaPoint);
		}
	}
}

OpenMesh::Vec3f Level::getTarget() {
	return target;
}

