#include <string>

#include <Shad/Level.h>

#include <PolyMesh/Cloth.h>

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
	deformablePlatforms = std::vector<Platform *>();
	btTransform id;
	id.setIdentity();
	id.setOrigin(BVEC3F(0, -1, 0));
	origin = id;
	fallLimit = -40.0;
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
	float *clear = new float[4];
	clear[0] = .5;
	clear[1] = .5;
	clear[2] = .5;
	clear[3] = 1.0;
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
	case 1:

		Gavin();
	
		break;

	case 2:
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

		platforms.push_back((new Platform("assets\\obj\\cube.obj"))->Scale(0.1f,1.4f,0.1f)->Translate(-0.7f,-11.7f, -11));
		platforms.push_back((new Platform("assets\\obj\\cube.obj"))->Scale(0.1f,1.4f,0.1f)->Translate(0.7f,-11.7f, -11));
		platforms.push_back((new Platform("assets\\obj\\cube.obj"))->Scale(1.5f,0.1f,0.1f)->Translate(0,-10.95, -11));
		pinTarget = (*platforms.rbegin())->platformMesh;

		Cloak = new Cloth(0.001f, 0.0005f, 0.0005f, OVEC3F(0,-1,0), OVEC3F(1,0,0), OVEC3F(-0.6f, -11.05f, -11),12,12,1.2f,0.1f,0.1f, BVEC3F(0,0,0.0006f));
		Cloak->EnableLighting();
		Cloak->Pin(0,0,pinTarget->RigidBody, new BVEC3F(-0.5f,-0.1f,0));
		Cloak->Pin(0,11,pinTarget->RigidBody, new BVEC3F(0.5f,-0.1f,0));
		Cloak->MaterialAmbient = clear;
		Cloak->MaterialDiffuse = clear;
		Cloak->MaterialSpecular = clear;
		Cloak->MaterialShininess = clear;
		cloth_image = bitmap_image("assets\\bmp\\flag_texture.bmp");
		cloth_image.rgb_to_bgr();
		Cloak->ApplyTexture(cloth_image.data(), cloth_image.width(), cloth_image.height());

		target = OpenMesh::Vec3f(0, -12, -11);
		break;
	case 3:

		platform = new Platform(cube);
		platform->Scale(1,1,10);
		platform->Translate(0,-10,0);
		platforms.push_back(platform);
		platform->setMoving(10, 0, 0, 0.01);
		movingPlatforms.push_back(platform);

		platforms.push_back((new Platform(cube))->Scale(1, 5, 1)->Translate(0,-13,-6));


		platform = new Platform(cube);
		platform->Scale(1, 1, 5);
		platform->Translate(2,-10,0);
		platforms.push_back(platform);
		platform->setShrinking(true, 1.0, 1.0, 0.985);
		shrinkingPlatforms.push_back(platform);

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
		
		platforms.push_back((new Platform("assets\\obj\\cube.obj"))->Scale(0.1f,1.4f,0.1f)->Translate(-0.7f,-13.2f, -22));
		platforms.push_back((new Platform("assets\\obj\\cube.obj"))->Scale(0.1f,1.4f,0.1f)->Translate(0.7f,-13.2f, -22));
		platforms.push_back((new Platform("assets\\obj\\cube.obj"))->Scale(1.5f,0.1f,0.1f)->Translate(0,-12.45, -22));
		pinTarget = (*platforms.rbegin())->platformMesh;

		Cloak = new Cloth(0.001f, 0.0005f, 0.0005f, OVEC3F(0,-1,0), OVEC3F(1,0,0), OVEC3F(-0.6f, -12.5f, -22),12,12,1.2f,0.1f,0.1f, BVEC3F(0,0,0.0002f));
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

		target = OpenMesh::Vec3f(0, -14, -22);
		break;
	case 4:
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
}

void Level::reset() {
	for (unsigned int i = 0; i < collapsiblePlatforms.size(); i++)
	{
		collapsiblePlatforms[i]->reset();
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
		r = 0.2f/(characterY - maxY);
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
	deformablePlatforms.clear();
	//delete placeholder;
}

void Level::setTarget(float x, float y, float z) {
	target = OpenMesh::Vec3f(x, y, z);
}

void Level::deform(bool onGround, float charX, float charY, float charZ) {
	if (onGround) {
		for (unsigned int i = 0; i < deformablePlatforms.size(); i++)
		{
			deformablePlatforms[i]->deform(onGround, charX, charY, charZ);
		}
	}
}

void Level::collapse(bool onGround, float charX, float charY, float charZ) {
	if (onGround) {
		for (unsigned int i = 0; i < collapsiblePlatforms.size(); i++)
		{
			collapsiblePlatforms[i]->collapse(onGround, charX, charY, charZ);
		}
	}
}

void Level::move(uint64_t deltaPoint, bool onGround, float charX, float charY, float charZ, Character * Shad) {
	if (onGround == true) {
		bool platformFound = false;
		for(unsigned int i = 0; i < movingPlatforms.size(); i++) {
			if (!platformFound) {
				platformFound = movingPlatforms[i]->moveWChar(deltaPoint, charX, charY, charZ);
				if (platformFound) {
					OpenMesh::Vec3f delta = movingPlatforms[i]->getDirection();
					int beat = movingPlatforms[i]->getBeat();
					if (deltaPoint < beat) {
					}
					else {
						delta = -delta;
					}
					btTransform id;
					id.setIdentity();
					id.setOrigin(BVEC3F(delta[0] + charX, delta[1] + charY, delta[2] + charZ));
					id.setRotation(((Character *)Shad)->RigidBody->getGhostObject()->getWorldTransform().getRotation());
					((Character *)Shad)->RigidBody->getGhostObject()->setWorldTransform(id);

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

void Level::shrink(uint64_t deltaPoint, float charX, float charY, float charZ, Character * Shad) {
	for(unsigned int i = 0; i < shrinkingPlatforms.size(); i++) {
		shrinkingPlatforms[i]->shrink(deltaPoint);
	}
}

OpenMesh::Vec3f Level::getTarget() {
	return target;
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
		
		lightningBolts.push_back(new Lightning(OpenMesh::Vec3f(0,-14,-6),OpenMesh::Vec3f(0,-10,-6)));

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

		platform = new Platform(cube);
		platform->Scale(6, 2, 0.5);
		platform->Translate(0, -10, -16);
		platforms.push_back(platform);

		platform = new Platform(cube);
		platform->Scale(6, 2, 0.5);
		platform->Translate(0, -16, -16);
		platforms.push_back(platform);

		platform = new Platform(cube);
		platform->Scale(6, 2, 5);
		platform->Translate(0, -16, -22);
		platforms.push_back(platform);


		for (signed int  i = 26; i < 50; i ++) {
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

		platform = new Platform(cube);
		platform->Scale(6, 2, 5);
		platform->Translate(0, -16, -50);
		platforms.push_back(platform);

		platform = new Platform(cube);
		platform->Scale(6, 2, 5);
		platform->Translate(4, -14, -50);
		platform->setMoving(4, 0, 1, 0);
		platforms.push_back(platform);
		movingPlatforms.push_back(platform);


		/*
		platforms.push_back((new Platform("assets\\obj\\cube.obj"))->Scale(0.1f,1.4f,0.1f)->Translate(-0.7f,-11.7f, -25));
		platforms.push_back((new Platform("assets\\obj\\cube.obj"))->Scale(0.1f,1.4f,0.1f)->Translate(0.7f,-11.7f, -25));
		platforms.push_back((new Platform("assets\\obj\\cube.obj"))->Scale(1.5f,0.1f,0.1f)->Translate(0,-10.95, -25));
		pinTarget = (*platforms.rbegin())->platformMesh;

		Cloak = new Cloth(0.001f, 0.0005f, 0.0005f, OVEC3F(0,-1,0), OVEC3F(1,0,0), OVEC3F(-0.6f, -11.05f, -11),12,12,1.2f,0.1f,0.1f, BVEC3F(0,0,0.0006f));
		Cloak->EnableLighting();
		Cloak->Pin(0,0,pinTarget->RigidBody, new BVEC3F(-0.5f,-0.1f,0));
		Cloak->Pin(0,11,pinTarget->RigidBody, new BVEC3F(0.5f,-0.1f,0));
		Cloak->MaterialAmbient = clear;
		Cloak->MaterialDiffuse = clear;
		Cloak->MaterialSpecular = clear;
		Cloak->MaterialShininess = clear;
		cloth_image = bitmap_image("assets\\bmp\\flag_texture.bmp");
		cloth_image.rgb_to_bgr();
		Cloak->ApplyTexture(cloth_image.data(), cloth_image.width(), cloth_image.height());
		*/




		target = OpenMesh::Vec3f(0, 12, -25);
}