void Level::Cassidy()
{
	Platform *platform;
	std::string cube = "assets\\obj\\cube.obj";

	setStartPosition(0.f, 0.f, -5.f);
	setFallLimit(-100.f);

	///////////////////
	// start area
	platform = new Platform(cube);
	platform->Scale(5,1,30);
	platform->Translate(0,-10,-10);
	platforms.push_back(platform);
	// start platform walls
	platform = new Platform(cube);
	platform->Scale(1,20,30);
	platform->Translate(-3,-0.5,-10);
	platforms.push_back(platform);
	platform = new Platform(cube);
	platform->Scale(1,20,30);
	platform->Translate(3,-0.5,-10);
	platforms.push_back(platform);
	// random things on walls
	for (int i = 0; i < 25; i++) {
		platform = new Platform(cube);
		platform->Scale(1.8,1,1);
		float rando = (float)rand()/(float)RAND_MAX - 0.5f;		// between -0.5 and 0.5
		float tx = (rando > 0.f) ? -1.8 : 1.8;					
		rando = (float)rand()/(2.f*(float)RAND_MAX) - 0.25f;
		tx += rando;
		rando = (float)rand()/(float)RAND_MAX - 0.5f;
		float ty = (int)(15.f*rando - 2.5f) % 15;				// between -10 and 5
		rando = (float)rand()/(float)RAND_MAX - 0.5f;
		float tz = (int)(28.f*rando - 10.f) % 28;				// between -24 and 4
		platform->Translate(tx, ty, tz);
		platforms.push_back(platform);
	}
	// pillars at wall ends
	platform = new Platform(cube);		// outer left
	platform->Scale(1,15,1);
	platform->Translate(-2, 0, -24.5);
	platforms.push_back(platform);
	platform = new Platform(cube);		// inner left
	platform->Scale(0.75,10,0.75);
	platform->Translate(-1.2, 0, -24.5);
	platforms.push_back(platform);
	platform = new Platform(cube);		// outer right
	platform->Scale(1,17,1);
	platform->Translate(2, 0, -24.5);
	platforms.push_back(platform);
	platform = new Platform(cube);		// inner right
	platform->Scale(0.75,12,0.75);
	platform->Translate(1.5, 0, -24.5);
	platforms.push_back(platform);
	// lightning at wall ends
	lightningBolts.push_back(new Lightning(OpenMesh::Vec3f(-3,-7,-25), OpenMesh::Vec3f(-2,-9,-40)));
	lightningBolts.push_back(new Lightning(OpenMesh::Vec3f(3,-7,-25), OpenMesh::Vec3f(2,-6,-40)));

	//////////////////////
	// main path start
	for (int i = 0; i < 4; i++) {
		platform = new Platform(cube);
		platform->setCollapsible();
		platform->Scale(5,1,4);
		platform->Translate(0,-10,-27-4*i);
		platforms.push_back(platform);
		collapsiblePlatforms.push_back(platform);
	}
	// left path
	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(2,1,2);
	platform->Translate(-2,-10,-45);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);
	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(2,1,2);
	platform->Translate(-4,-8,-52);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);
	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(2,1,2);
	platform->Translate(-8,-6,-59);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);
	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(2,1,2);
	platform->Translate(-14,-4,-66);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);
	// slide down to shrinking wall area
	platform = new Platform(cube);
	platform->Scale(20,1,7);
	platform->Rotate(35.f, 0.f, 0.f, 1.f);
	platform->Translate(-25, -20, -66);
	platforms.push_back(platform);
	// right path
	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(2,1,2);
	platform->Translate(2,-10,-45);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);
	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(2,1,2);
	platform->Translate(6,-7,-50);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);
	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(2,1,2);
	platform->Translate(12,-4,-55);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);
	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(2,1,2);
	platform->Translate(18,-2,-60);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);

	////////////////////
	// secret path start
	platform = new Platform(cube);
	platform->setMoving(300, 0.f, 0.1f, 0.05f);
	platform->Scale(3,1,3);
	platform->Translate(0,-28,-5);
	platforms.push_back(platform);
	movingPlatforms.push_back(platform);
	platform = new Platform(cube);
	platform->setMoving(300, -0.1f, 0.f, 0.f);
	platform->Scale(3,1,3);
	platform->Translate(0,-28,-10);
	platforms.push_back(platform);
	movingPlatforms.push_back(platform);
	platform = new Platform(cube);
	platform->setMoving(300, 0.f, 0.f, 0.1f);
	platform->Translate(-17.f, -28.f, -25.f);
	platforms.push_back(platform);
	movingPlatforms.push_back(platform);
	// elevator to runway
	platform = new Platform(cube);
	platform->setElevator(-1,-0.25,-0.25,-78,-20,-100);
	platform->Scale(5,1,5);
	platform->Translate(-17.f, -28.f, -27.5f);
	platforms.push_back(platform);
	elevatablePlatforms.push_back(platform);
	// the runway
	platform = new Platform(cube);
	platform->Scale(10, 5, 200);
	platform->Translate(-100, -50, -30);
	platforms.push_back(platform);
	////////////////////
	// paths converge
	//////////////////////
	// put stuff here

	/////////////
	// the end!
	platform = new Platform(cube);
	platform->Scale(20, 2, 20);
	platform->Translate(0, -12, -120);
	platforms.push_back(platform);
	// outer walls
	platform = new Platform(cube);
	platform->Scale(1,7,5);
	platform->Translate(-5, -8.5, -120);
	platforms.push_back(platform);
	platform = new Platform(cube);
	platform->Scale(1,7,5);
	platform->Translate(5, -8.5, -120);
	platforms.push_back(platform);
	// inner walls
	platform = new Platform(cube);
	platform->Scale(1,10,8);
	platform->Translate(-4, -6.5, -120);
	platforms.push_back(platform);
	platform = new Platform(cube);
	platform->Scale(1,10,8);
	platform->Translate(4, -6.5, -120);
	platforms.push_back(platform);
	// back wall
	platform = new Platform(cube);
	platform->Scale(8, 11, 1);
	platform->Translate(0, -6, -130);
	platforms.push_back(platform);
	// trees
	for (int i = 0; i < 10; i++) {
		platform = new Platform(cube);
		platform->setCollapsible();
		float rando = (float)rand()/(float)RAND_MAX - 0.5;
		rando *= 2;
		float tx = (rando > 0) ? 7 : -7;
		float txRand = (float)rand()/(float)RAND_MAX - 0.5;
		txRand = (int)(3.f*txRand) % 3;
		platform->Scale(0.8, 5+rando, 0.8);
		platform->Translate(tx+txRand, -10+rando/2, -120+txRand);
		platforms.push_back(platform);
		collapsiblePlatforms.push_back(platform);
	}

	// the target
	setTarget(0.f, -11.f, -120.f);
}