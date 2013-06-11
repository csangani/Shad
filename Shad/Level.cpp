#include <string>

#include <Shad/Level.h>

#include <PolyMesh/Cloth.h>
#include <PolyMesh/ParticleCloth.h>

#include <GL\glew.h>
#include <GL\glut.h>

Level::Level(int level) {
	_level = level;
	platforms = std::vector<Platform *>();
	lightningBolts = std::vector<Lightning *>();

	platforms = std::vector<Platform *>();
	movingPlatforms = std::vector<Platform *>();
	shrinkingPlatforms = std::vector<Platform *>(); 
	collapsiblePlatforms = std::vector<Platform *>();
	elevatablePlatforms = std::vector<Platform *>();
	btTransform id;
	id.setIdentity();
	id.setOrigin(BVEC3F(0, -1, 0));
	origin = id;
	fallLimit = -40.0;
	target = OpenMesh::Vec3f(10,10,10);
}

void Level::setStartPosition(float startX, float startY, float startZ) {
	btTransform id;
	id.setIdentity();
	id.setOrigin(BVEC3F(startX, startY, startZ));
	origin = id;
	start = OpenMesh::Vec3f(startX, startY, startZ);
}

btTransform Level::getStartPosition() {
	return origin;
}

void Level::setFallLimit(float threshold) {
	fallLimit = threshold;
}

float Level::getFallLimit() {
	return fallLimit;
}

void Level::generateBlocks(std::string shader, bitmap_image& space_image) {
	std::string cube = "assets\\obj\\cube.obj";
	Platform *platform;

	/*
	* While there are functions that return booleans to check the kind of platform,
	*	I (Gavin) don't use them, and instead plug them into the corresponding vector
	*  Guide:
	*	All platforms go into platforms
	*	moving one go into movingPlatforms
	*	shrinking ones go into shrinkingPlatforms
	*	collapsible ones go into collapsiblePlatforms
	*  a collapsible platform always need to be set to collapsible after construction
	*	deformable ones go into deformablePlatforms
	*/
	switch(_level) {
	//Gavin level
	case 4:

		Gavin();
		break;
	
		//Gavin level
	case 1:
	case 6:

		Johan();
		break;


	case 3:
		Cassidy();
		break;


	case 2:
		Chirag();
		//Johan();

		break;
	case 5: 
		Amit(); 
		break; 

	case 10:
		platform = new Platform(cube);
		platform->Scale(3,1,3);
		platform->Translate(0,-10,0);
		platforms.push_back(platform);

		platform = new Platform(cube);
		platform->Scale(10,10,1);
		platform->Translate(0,-10,-3);
		platforms.push_back(platform);

		platform = new Platform(cube);
		platform->Scale(8,1,8);
		platform->Translate(0,-13,-10);
		platforms.push_back(platform);

		setTarget(0, -12, -11);
		break;



	case 8:

		platform = new Platform(cube);
		platform->Scale(1,1,10);
		platform->Translate(0,-10,0);
		platforms.push_back(platform);
		platform->setMoving(100, 0, 0.1, 0.0);
		movingPlatforms.push_back(platform);

		platforms.push_back((new Platform(cube))->Scale(1, 5, 1)->Translate(0,-13,-6));


		platform = new Platform(cube);
		platform->setElevator(0.0f, 0.1f, 0.0f, 0.0f, 20.0f, 0.0f);
		platform->Scale(1, 1, 5);
		platform->Translate(2,-10,-10);
		platforms.push_back(platform);
		elevatablePlatforms.push_back(platform);


		platform = new Platform(cube);
		//platform->subdivide();
		platform->Scale(1,1,8);
		platform->Translate(-3,-10,0);
		//deformablePlatforms.push_back(platform);


		platforms.push_back(platform);

		platforms.push_back((new Platform(cube))->Scale(1, 5, 1)->Translate(0,-15,-8));

		platforms.push_back((new Platform(cube))->Rotate(45, 0, 1, 1)->Scale(2, 2, 2)->Translate(0,-15,-11));

		platforms.push_back((new Platform(cube))->Rotate(35, 1, 0, 0)->Scale(10, 2, 2)->Translate(0,-15,-14));

		platforms.push_back((new Platform(cube))->Rotate(35, 1, 0, 0)->Scale(8, 2, 2)->Translate(0,-15,-16));

		platform = new Platform(cube);
		platform->setCollapsible();
		platform->Rotate(35, 1, 0, 0);
		platform->Scale(6, 2, 2);
		platform->Translate(0,-15,-18);
		platforms.push_back(platform);
		collapsiblePlatforms.push_back(platform);

		platforms.push_back((new Platform(cube))->Rotate(35, 1, 0, 0)->Scale(4, 2, 2)->Translate(0,-15,-20));

		platforms.push_back((new Platform(cube))->Scale(2, 2, 2)->Translate(0,-15,-22));

		lightningBolts.push_back(new Lightning(OpenMesh::Vec3f(-1,-10,0), OpenMesh::Vec3f(1,-5,-4)));

		setTarget(0, -14, -22);
		break;

	case 11:
		platform = new Platform(cube);
		platform->Scale(3, 1, 3);
		platform->Translate(0, -5, 0);
		platforms.push_back(platform);
		//platform->setMoving(true, 0, 0, 1);
		//movingPlatforms.push_back(platform);

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

	platforms.push_back((new Platform("assets\\obj\\cube.obj"))->Scale(0.1f,1.4f,0.1f)->Translate(target[0] - 0.7f, target[1] + 0.8f, target[2]));
	platforms.push_back((new Platform("assets\\obj\\cube.obj"))->Scale(0.1f,1.4f,0.1f)->Translate(target[0] + 0.7f, target[1] + 0.8f, target[2]));
	platforms.push_back((new Platform("assets\\obj\\cube.obj"))->Scale(1.5f,0.1f,0.1f)->Translate(target[0], target[1] + 1.55f, target[2]));
	pinTarget = (*platforms.rbegin())->platformMesh;

	float *clear = new float[4];
	clear[0] = .5;
	clear[1] = .5;
	clear[2] = .5;
	clear[3] = 1.0;

	Cloak = new Cloth(0.001f, 0.0005f, 0.0005f, OVEC3F(0,-1,0), OVEC3F(1,0,0), target + OVEC3F(-0.6f, 1.5f, 0),12,12,1.2f,0.1f,0.1f, BVEC3F(0,0,0.0006f));
	Cloak->EnableLighting();
	Cloak->MaterialAmbient = clear;
	Cloak->MaterialDiffuse = clear;
	Cloak->MaterialSpecular = clear;
	Cloak->MaterialShininess = clear;
	Cloak->Pin(0,0,pinTarget->RigidBody, new BVEC3F(-0.5f,-0.1f,0));
	Cloak->Pin(0,11,pinTarget->RigidBody, new BVEC3F(0.5f,-0.1f,0));
	cloth_image = bitmap_image("assets\\bmp\\flag_texture.bmp");
	cloth_image.rgb_to_bgr();
	Cloak->ApplyTexture(cloth_image.data(), cloth_image.width(), cloth_image.height());
}

void Level::reset() {
	for (unsigned int i = 0; i < collapsiblePlatforms.size(); i++)
	{
		collapsiblePlatforms[i]->reset();
	}
	for (unsigned int i = 0; i < elevatablePlatforms.size(); i++)
	{
		elevatablePlatforms[i]->reset();
	}
}

void Level::changeUp() {
	_level++;
}

void Level::drawPlatformEdges()
{
	for (unsigned int i = 0; i < platforms.size(); i++) {
		Platform *platform = platforms[i];
		float *color = platform->getColor();
		for (unsigned int j = 0; j < platform->edges.size(); j++) {
			PlatformEdge *edge = platform->edges[j];
			edge->Draw(color);
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

bool Level::lightningCollisionWithPoint(OpenMesh::Vec3f point)
{
	for (unsigned int i = 0; i < lightningBolts.size(); i++) {
		Lightning *lightning = lightningBolts[i];
		if (lightning->CollidesWithPoint(point)) {
			return true;
		}
	}
	return false;
}

float Level::drawCharacterShadow(float characterX, float characterY, float characterZ)
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
	float r = 0;
	// draw shadow on platform
	if (foundPlatform != NULL) {
		float platformYScale = foundPlatform->platformMesh->max[1];
		float shadowOffset = 0.01f;
		r = 0.2f/sqrt(characterY - maxY - platformYScale);
		int num_segments = 20;
		glColor4f(1.f, 1.f, 1.f, 1.0f);
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
	return r;
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
	delete Cloak;
	for(unsigned int i = 0; i < platforms.size(); i++) {
		Platform * p =  platforms[i];
		delete p;
	}
	platforms.clear();
	movingPlatforms.clear();
	collapsiblePlatforms.clear();
	shrinkingPlatforms.clear();
	elevatablePlatforms.clear();
	//delete placeholder;
}

void Level::setTarget(float x, float y, float z) {
	target = OpenMesh::Vec3f(x, y, z);
}

void Level::deform(bool onGround, float charX, float charY, float charZ) {
	/*	if (onGround) {
	for (unsigned int i = 0; i < deformablePlatforms.size(); i++)
	{
	deformablePlatforms[i]->deform(onGround, charX, charY, charZ);
	}
	}*/
}

void Level::collapse(bool onGround, float charX, float charY, float charZ) {
	if (onGround) {
		for (unsigned int i = 0; i < collapsiblePlatforms.size(); i++)
		{
			collapsiblePlatforms[i]->collapse(onGround, charX, charY, charZ);
		}
	}
}

void Level::move(uint64_t deltaPoint, bool onGround, float charX, float charY, float charZ, Character * Shad, ParticleCloth * cape) {
	if (onGround == true) {
		bool platformFound = false;
		for(unsigned int i = 0; i < movingPlatforms.size(); i++) {
			if (!platformFound) {
				platformFound = movingPlatforms[i]->moveWChar(deltaPoint, charX, charY, charZ);
				int counter = movingPlatforms[i]->getCounter();

				if (platformFound) {
					OpenMesh::Vec3f delta = movingPlatforms[i]->getDirection();
					int beat = movingPlatforms[i]->getBeat();
					deltaPoint %= beat;
					counter--;
					if (counter < beat/2) {
					}
					else {
						delta[0] = -delta[0];
						delta[1] = -delta[1];
						delta[2] = -delta[2];
					}
					btTransform id;
					id.setIdentity();
					if (delta[1] > 0 ){
						id.setOrigin(BVEC3F(delta[0] + charX, delta[1] + charY, delta[2] + charZ));
					}
					else {
						id.setOrigin(BVEC3F(delta[0] + charX, delta[1] + charY, delta[2] + charZ));
					}
					id.setRotation(((Character *)Shad)->RigidBody->getGhostObject()->getWorldTransform().getRotation());

					((Character *)Shad)->RigidBody->getGhostObject()->setWorldTransform(id);
					((Character *)Shad)->SyncDummy();
				}
			}
			else {
				movingPlatforms[i]->move(deltaPoint);
			}
		}
	}
	else {
		for(unsigned int i = 0; i < movingPlatforms.size(); i++) {
			movingPlatforms[i]->move(deltaPoint);
		}
	}
}

bool Level::elevate(bool onGround, float charX, float charY, float charZ, Character * Shad, ParticleCloth * cape) {
	if (onGround == true) {
		bool platformFound = false;
		for(unsigned int i = 0; i < elevatablePlatforms.size(); i++) {
			if (!platformFound) {

				platformFound = elevatablePlatforms[i]->carry(onGround, charX, charY, charZ);
				int counter = elevatablePlatforms[i]->getCounter();

				if (platformFound && elevatablePlatforms[i]->stopped() == false) {

					OpenMesh::Vec3f delta = elevatablePlatforms[i]->getDirection();

					btTransform id;
					id.setIdentity();
					if (delta[1] > 0 ){
						id.setOrigin(BVEC3F(delta[0] + charX, delta[1] + charY, delta[2] + charZ));
					}
					else {
						id.setOrigin(BVEC3F(delta[0] + charX, delta[1] + charY, delta[2] + charZ));
					}

					id.setRotation(((Character *)Shad)->RigidBody->getGhostObject()->getWorldTransform().getRotation());

					((Character *)Shad)->RigidBody->getGhostObject()->setWorldTransform(id);
					((Character *)Shad)->SyncDummy();


					btTransform armMovement;
					armMovement.setIdentity();
					armMovement.setOrigin(BVEC3F(delta[0], delta[1], delta[2]));
					if (delta[1] > 0) { 
						//	((Character *)Shad)->Arms->SetOrigin(OpenMesh::Vec3f(delta[0] + charX, delta[1] + charY + 1, delta[2] + charZ));
					}
					else {
						//	((Character *)Shad)->Arms->SetOrigin(OpenMesh::Vec3f(delta[0] + charX, delta[1] + charY, delta[2] + charZ));
					}
					return true;

					//cape->SetOrigin(OpenMesh::Vec3f());
				}
			}
		}
	}
	return false;
}

void Level::shrink(uint64_t deltaPoint, float charX, float charY, float charZ, Character * Shad) {
	for(unsigned int i = 0; i < shrinkingPlatforms.size(); i++) {
		shrinkingPlatforms[i]->shrink(deltaPoint);
	}
}

OpenMesh::Vec3f Level::getTarget() {
	return target;
}

void Level::Johan()
{
	Platform *platform;
	std::string cube = "assets\\obj\\cube.obj";
	setFallLimit(-100.0);
	setStartPosition(0,5,10);

	//Intro
	platform = new Platform(cube);
	platform->Scale(3,1,1);
	platform->Translate(0,0,10);
	platforms.push_back(platform);

	platform = new Platform(cube);
	platform->setElevator(0.0f, -0.2f, 0.1f, 0.0f, -50.0f, -14.0f);
	platform->Scale(1, 1, 1);
	platform->Translate(0,0,11);
	platforms.push_back(platform);
	elevatablePlatforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(1,1,1);
	platform->Translate(-1.5,0,9);
	platforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(1,1,1);
	platform->Translate(1.5,0,9);
	platforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(1,1,1);
	platform->Translate(0,0,9);
	platform->setCollapsible();
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(3,1,1);
	platform->Translate(0,0,8);
	platforms.push_back(platform);

	//1st
	platform = new Platform(cube);
	platform->Scale(3,1,3);
	platform->Translate(0,-10,0);
	platforms.push_back(platform);

	//2nd
	platform = new Platform(cube);
	platform->Scale(3,1,3);
	platform->Translate(0,-15,-20);
	platforms.push_back(platform);

	//Lightning between 1st and 2nd
	lightningBolts.push_back(new Lightning(OpenMesh::Vec3f(-1,-20,-5), OpenMesh::Vec3f(1,-5,-5)));
	lightningBolts.push_back(new Lightning(OpenMesh::Vec3f(-1,-20,-15), OpenMesh::Vec3f(1,-5,-15)));

	//3rd
	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(3, 1, 3);
	platform->Translate(15, -17, -25);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);

	//4th
	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(3, 1, 3);
	platform->Translate(30, -20, -30);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);

	//5th
	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(3, 1, 3);
	platform->Translate(45, -23, -25);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);

	//6th
	platform = new Platform(cube);
	platform->Scale(3,1,3);
	platform->Translate(55,-25,-25);
	platforms.push_back(platform);

	//Stairs
	for (int i = 0; i < 10; i++) {
		platform = new Platform(cube);
		platform->Scale(3,2,1);
		platform->Translate(55,-25+i,-28-i);
		platforms.push_back(platform);
	}

	//Elevator
	platform = new Platform(cube);
	platform->setElevator(0.0f, 0.1f, 0.0f, 0.0f, 15.0f, 0.0f);
	platform->Scale(3, 1, 3);
	platform->Translate(55,-15,-39);
	platforms.push_back(platform);
	elevatablePlatforms.push_back(platform);

	//Wall
	platform = new Platform(cube);
	platform->Scale(1,10,7);
	platform->Translate(50,0,-39);
	platforms.push_back(platform);

	//7th
	platform = new Platform(cube);
	platform->Scale(5,1,5);
	platform->Translate(40,0,-39);
	platform->setShrinking(100,0.98);
	platforms.push_back(platform);
	shrinkingPlatforms.push_back(platform);

	//8th
	platform = new Platform(cube);
	platform->Scale(3,1,3);
	platform->Translate(20,0,-39);
	platforms.push_back(platform);

	//Fake elevator
	platform = new Platform(cube);
	platform->setElevator(0.0f, -0.2f, 0.1f, 0.0f, -50.0f, -14.0f);
	platform->Scale(1, 1, 1);
	platform->Translate(20,0,-37);
	platforms.push_back(platform);
	elevatablePlatforms.push_back(platform);

	//Target platform
	platform = new Platform(cube);
	platform->Scale(3,1,5);
	platform->Translate(20,-50,-59);
	platforms.push_back(platform);

	target = OpenMesh::Vec3f(20, -50+0.5, -60);
}

void Level::Gavin() {

	std::string cube = "assets\\obj\\cube.obj";
	Platform *platform;
	float *clear = new float[4];
	clear[0] = .5;
	clear[1] = .5;
	clear[2] = .5;
	clear[3] = 1.0;

	setFallLimit(-50);
	platform = new Platform(cube);
	platform->Scale(5,1,5);
	platform->Translate(0,-10,0);
	platforms.push_back(platform);

	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(6, 2, 2);
	platform->Translate(0, -12, -4);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);

	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(6, 2, 2);
	platform->Translate(0, -14, -6);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);

	

	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(6, 2, 2);
	platform->Translate(0, -16, -8);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);

	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(6, 2, 2);
	platform->Translate(0, -16, -10);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);

	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(6, 2, 2);
	platform->Translate(0, -16, -12);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(2, 15, 5);
	platform->Translate(2, -16, -16);
	platforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(2, 15, 5);
	platform->Translate(-2, -16, -16);
	platforms.push_back(platform);
	lightningBolts.push_back(new Lightning(OpenMesh::Vec3f(2,-8,-17),OpenMesh::Vec3f(2,-2,-17)));

	platform = new Platform(cube);
	platform->Scale(6, 2, 0.5);
	platform->Translate(0, -10, -16);
	platforms.push_back(platform);
	lightningBolts.push_back(new Lightning(OpenMesh::Vec3f(-2,-8,-17),OpenMesh::Vec3f(-2,-2,-17)));
	lightningBolts.push_back(new Lightning(OpenMesh::Vec3f(0,-8,-17),OpenMesh::Vec3f(0,-2,-17)));

	platform = new Platform(cube);
	platform->Scale(6, 2, 0.5);
	platform->Translate(0, -16, -16);
	platforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(6, 2, 5);
	platform->Translate(0, -16, -22);
	platforms.push_back(platform);



	for (signed int  i = 35; i < 40; i ++) {

		platform = new Platform(cube);
		platform->setCollapsible();
		platform->Scale(1, 1, 1);
		platform->Translate(0, -16, -i);
		platforms.push_back(platform);
		collapsiblePlatforms.push_back(platform);

		platform = new Platform(cube);
		platform->setCollapsible();
		platform->Scale(1, 1, 1);
		platform->Translate(1, -16, -i);
		platforms.push_back(platform);
		collapsiblePlatforms.push_back(platform);

		platform = new Platform(cube);
		platform->setCollapsible();
		platform->Scale(1, 1, 1);
		platform->Translate(-1, -16, -i);
		platforms.push_back(platform);
		collapsiblePlatforms.push_back(platform);

		platform = new Platform(cube);
		platform->setCollapsible();
		platform->Scale(1, 1,1);
		platform->Translate(2, -16, -i);
		platforms.push_back(platform);
		collapsiblePlatforms.push_back(platform);

		platform = new Platform(cube);
		platform->setCollapsible();
		platform->Scale(1, 1, 1);
		platform->Translate(-2, -16, -i);
		platforms.push_back(platform);
		collapsiblePlatforms.push_back(platform);

	}

	lightningBolts.push_back(new Lightning(OpenMesh::Vec3f(0,-16,-38),OpenMesh::Vec3f(0,-12,-38)));

	platform = new Platform(cube);
	platform->Scale(6, 2, 5);
	platform->Translate(0, -16, -55);
	platforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(2, 10, 10);
	platform->Translate(-2, -16, -55);
	platforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(2, 10, 10);
	platform->Translate(2, -16, -57);
	platforms.push_back(platform);

	platform = new Platform(cube);
	platform->setShrinking(100, .95);
	platform->Scale(5, 5, 2);
	platform->Translate(0, -12, -40);
	platforms.push_back(platform);
	shrinkingPlatforms.push_back(platform);

	platform = new Platform(cube);
	platform->setShrinking(50, .95);
	platform->Scale(7, 5, 2);
	platform->Translate(0, -12, -42);
	platforms.push_back(platform);
	shrinkingPlatforms.push_back(platform);

	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(2, 2, 2);
	platform->Translate(0, -16, -70);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);

	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(2, 2, 2);
	platform->Translate(10, -16, -70);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);

	platform = new Platform(cube);
	platform->setMoving(100, 0.1f, 0.0f, 0.0f);
	platform->Scale(2, 2, 2);
	platform->Translate(10, -16, -75);
	platforms.push_back(platform);
	movingPlatforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(2, 2, 2);
	platform->Translate(19, -16, -75);
	platforms.push_back(platform);

	target = OpenMesh::Vec3f(19, -15, -75);
}


void Level::Amit() {

	std::string cube = "assets\\obj\\cube.obj";
	Platform *platform;
	float *clear = new float[4];
	clear[0] = .5;
	clear[1] = .5;
	clear[2] = .5;
	clear[3] = 1.0;

	setFallLimit(-300);

	// start
	platform = new Platform(cube);
	platform->Scale(5,1,5);
	platform->Translate(0,-10,0);
	platforms.push_back(platform);

	// 1
	platform = new Platform(cube);
	platform->setMoving(200, 0, 0.2, 0);
	platform->Scale(4, 1, 4);
	platform->Translate(0, -20, -10);
	platforms.push_back(platform);
	movingPlatforms.push_back(platform); 


	// 2
	platform = new Platform(cube);
	platform->setMoving(100, 0.2, 0, 0);
	platform->Scale(8, 1, 8);
	platform->Translate(-10, -30, -30);
	platforms.push_back(platform);
	movingPlatforms.push_back(platform);

	lightningBolts.push_back(new Lightning(OpenMesh::Vec3f(-5,-35,-20), OpenMesh::Vec3f(-5,-5,-20)));
	//lightningBolts.push_back(new Lightning(OpenMesh::Vec3f(0,-25,-22), OpenMesh::Vec3f(0,-5,-22)));

	// 3
	platform = new Platform(cube);
	platform->Scale(4, 1, 4);
	platform->Translate(-25, -30, -30);
	platforms.push_back(platform);


	//Transit 1
	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(6, 2, 6);
	platform->Translate(-35, -29, -40);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);

	//Transit 2
	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(6, 2, 6);
	platform->Translate(-45, -28, -30);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);

	//Transit 3
	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(6, 2, 6);
	platform->Translate(-55, -27, -20);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);

	// 4
	platform = new Platform(cube);
	platform->Scale(4, 1, 4);
	platform->Translate(-35, -26, -10);
	platforms.push_back(platform);

	// 5
	platform = new Platform(cube);
	platform->Scale(4, 1, 4);
	platform->Translate(-35, -26, -10);
	platforms.push_back(platform);

	// Downward slopes

	for (int i = 0; i < 10; i++) {
		platform = new Platform(cube);
		platform->Scale(5,1,3);
		platform->Translate(-55-(2*i),-45-i,-28);
		platforms.push_back(platform);

		// Shrinks
		platform = new Platform(cube);
		platform->Scale(4, 1, 4);
		platform->Translate(-85, -50, -28);
		platforms.push_back(platform);
		platform->setShrinking(100, 0.98); 
		shrinkingPlatforms.push_back(platform); 


		// Wall
		platform = new Platform(cube);
		platform->Scale(2, 20, 20);
		platform->Translate(-90, -40, -28);
		platforms.push_back(platform);


		// Fall
		platform = new Platform(cube);
		platform->setCollapsible();
		platform->Scale(4, 1, 4);
		platform->Translate(-95, -50, -28);
		platforms.push_back(platform);
		collapsiblePlatforms.push_back(platform);

		// Done
		platform = new Platform(cube);
		platform->Scale(4, 1, 4);
		platform->Translate(-105, -200, -28);
		platforms.push_back(platform);


	}
	target = OpenMesh::Vec3f(-102, -195, -27);
}

void Level::Chirag() {
	std::string cube = "assets\\obj\\cube.obj";
	Platform *platform;

	setStartPosition(0,-18.0f,0);
	setFallLimit(-50);

	// start
	platform = new Platform(cube);
	platform->Scale(5,1,5);
	platform->Translate(0,-20.0f,0);
	platforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(5,1,5);
	platform->Translate(0,-0.0f,0);
	platforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(5,1,5);
	platform->Translate(0,-20.0f,-6.0f);
	platforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(5,1,5);
	platform->Translate(0,-0.0f,-6.0f);
	platforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(5,1,5);
	platform->Translate(0,-20.0f,-12.0f);
	platforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(5,1,5);
	platform->Translate(0,-0.0f,-12.0f);
	platforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(5,1,5);
	platform->Translate(6.0f,-20.0f,-12.0f);
	platforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(5,1,5);
	platform->Translate(6.0f,-0.0f,-12.0f);
	platforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(5, 1, 5);
	platform->Translate(12.0f, -20.0f, -12.0f);
	platform->setMoving(200, 0, 0.2f, 0);
	platforms.push_back(platform);
	movingPlatforms.push_back(platform);

	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(1,1,5);
	platform->Translate(9, 0.0f,-12);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);

	// collapses
	platform = new Platform(cube);
	platform->Scale(10, 1, 1);
	platform->Translate(-9, 0, 0);
	platforms.push_back(platform);
	platform->setCollapsible(); 
	collapsiblePlatforms.push_back(platform); 

	platform = new Platform(cube);
	platform->Scale(10, 1, 1);
	platform->Translate(-9, 0, 3);
	platforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(10, 1, 1);
	platform->Translate(-9, 0, -3);
	platforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(3, 1, 3);
	platform->Translate(-18, 0, 0);
	platforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(2, 5, 1);
	platform->Translate(-21.0f, 0.0f, -3.0f);
	platform->setMoving(20, 0, 0, 0.75f);
	platforms.push_back(platform);
	movingPlatforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(3, 1, 3);
	platform->Translate(-24, 0, 0);
	platforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(3, 1, 3);
	platform->Translate(-24.0f, -2.0f, -4.0f);
	platform->setMoving(20, 0, 1.0, 0);
	platforms.push_back(platform);
	movingPlatforms.push_back(platform);
	
	platform = new Platform(cube);
	platform->Scale(3, 1, 3);
	platform->Translate(-24, 0, -8);
	platforms.push_back(platform);
	
	platform = new Platform(cube);
	platform->Scale(1, 1, 1);
	platform->Translate(-24, 0, -10);
	platform->setCollapsible();
	collapsiblePlatforms.push_back(platform);
	platforms.push_back(platform);
	platform = new Platform(cube);
	platform->Scale(1, 1, 1);
	platform->Translate(-24, 0, -11);
	platform->setCollapsible();
	collapsiblePlatforms.push_back(platform);
	platforms.push_back(platform);
	platform = new Platform(cube);
	platform->Scale(1, 1, 1);
	platform->Translate(-24, 0, -12);
	platform->setCollapsible();
	collapsiblePlatforms.push_back(platform);
	platforms.push_back(platform);
	platform = new Platform(cube);
	platform->Scale(1, 1, 1);
	platform->Translate(-24, 0, -13);
	platform->setCollapsible();
	collapsiblePlatforms.push_back(platform);
	platforms.push_back(platform);
	platform = new Platform(cube);
	platform->Scale(1, 1, 1);
	platform->Translate(-24, 0, -14);
	platform->setCollapsible();
	collapsiblePlatforms.push_back(platform);
	platforms.push_back(platform);

	platform = new Platform(cube);
	platform->Scale(3, 1, 5);
	platform->Translate(-24, 0, -17);
	platforms.push_back(platform);

	lightningBolts.push_back(new Lightning(OVEC3F(-9.0f, 0, -3), OVEC3F(-9.0f, 5, -3.1f)));

	lightningBolts.push_back(new Lightning(OVEC3F(-4.5f, 0, 3), OVEC3F(-4.5f, 5, 3)));

	lightningBolts.push_back(new Lightning(OVEC3F(-13.5f, 0, -3), OVEC3F(-13.5f, 5, -3.1f)));

	setTarget(-24.0f, 0.5f, -18.0f);
}


#include <Shad/CassidyLevel.h>