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
	platform->Translate(-5,-8,-52);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);
	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(2,1,2);
	platform->Translate(-10,-6,-59);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);
	platform = new Platform(cube);
	platform->setCollapsible();
	platform->Scale(2,1,2);
	platform->Translate(-16,-4,-66);
	platforms.push_back(platform);
	collapsiblePlatforms.push_back(platform);
	// drop down to shrinking wall area
	platform = new Platform(cube);
	platform->setShrinking(120, 0.97f);
	platform->Scale(10,1,10);
	platform->Translate(-25,-25,-69);
	platforms.push_back(platform);
	shrinkingPlatforms.push_back(platform);
	platform = new Platform(cube);
	platform->setShrinking(180, 0.97f);
	platform->Scale(10,1,10);
	platform->Translate(0,-25,-69);
	platforms.push_back(platform);
	shrinkingPlatforms.push_back(platform);
	platform = new Platform(cube);
	platform->setShrinking(210, 0.97f);
	platform->Scale(10,1,10);
	platform->Translate(0,-25,-95);
	platforms.push_back(platform);
	shrinkingPlatforms.push_back(platform);
	// eleveator to top
	platform = new Platform(cube);
	platform->setElevator(0,0,-1,0,0,35);
	platform->Scale(7,1,7);
	platform->Translate(0,-25,-110);
	platforms.push_back(platform);
	elevatablePlatforms.push_back(platform);
	platform = new Platform(cube);
	platform->setElevator(0,0.5f,0,0,15,0);
	platform->Scale(7,1,7);
	platform->Translate(0,-25,-155);
	platforms.push_back(platform);
	elevatablePlatforms.push_back(platform);

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
	// elevator to nowhere
	platform = new Platform(cube);
	platform->setElevator(1,0,0,100,0,0);
	platform->Scale(7,1,7);
	platform->Translate(25, 0, -60);
	platforms.push_back(platform);
	elevatablePlatforms.push_back(platform);

	//////////////////////
	//// secret path start
	//////////////////////
	//platform = new Platform(cube);
	//platform->setMoving(300, 0.f, 0.1f, 0.05f);
	//platform->Scale(3,1,3);
	//platform->Translate(0,-28,-5);
	//platforms.push_back(platform);
	//movingPlatforms.push_back(platform);
	//platform = new Platform(cube);
	//platform->setMoving(300, -0.1f, 0.f, 0.f);
	//platform->Scale(3,1,3);
	//platform->Translate(0,-28,-10);
	//platforms.push_back(platform);
	//movingPlatforms.push_back(platform);
	//platform = new Platform(cube);
	//platform->setMoving(300, 0.f, 0.f, 0.1f);
	//platform->Translate(-17.f, -28.f, -25.f);
	//platforms.push_back(platform);
	//movingPlatforms.push_back(platform);
	//// elevators to runway
	//platform = new Platform(cube);
	//platform->setElevator(0,0,1.5,0,0,105);
	//platform->Scale(5,1,5);
	//platform->Translate(-17.f, -28.f, -35.f);
	//platforms.push_back(platform);
	//elevatablePlatforms.push_back(platform);
	//platform = new Platform(cube);
	//platform->setElevator(-1,0,0,82.5,0,0);
	//platform->Scale(5,1,5);
	//platform->Translate(-17.f, -28.f, 80.f);
	//platforms.push_back(platform);
	//elevatablePlatforms.push_back(platform);
	//// the runway
	//platform = new Platform(cube);
	//platform->Scale(10, 5, 200);
	//platform->Translate(-100, -32, -30);
	//platforms.push_back(platform);
	//// speed down the runway
	//platform = new Platform(cube);
	//platform->setElevator(0,0,-2,0,0,190);
	//platform->Scale(5,1,5);
	//platform->Translate(-100.f, -29.f, 50.f);
	//platforms.push_back(platform);
	//elevatablePlatforms.push_back(platform);
	//platform = new Platform(cube);
	//platform->setElevator(1,0,0,80,0,0);
	//platform->Scale(5,1,5);
	//platform->Translate(-100.f, -26.f, -155.f);
	//platforms.push_back(platform);
	//elevatablePlatforms.push_back(platform);
	//// runway decorations
	//for (int i = -30; i > -130; i-=5)
	//{
	//	platform = new Platform(cube);
	//	platform->setCollapsible();
	//	float rando = (float)rand()/(float)RAND_MAX - 0.5f;
	//	platform->Scale(1+rando, -7.f*(i+30)/120 + 5.f, 1+rando);
	//	float tx = (rando > 0) ? -104 : -96;
	//	rando = (float)rand()/(float)RAND_MAX - 0.5f;
	//	platform->Translate(tx+rando, -32, float(i));
	//	platforms.push_back(platform);
	//	collapsiblePlatforms.push_back(platform);
	//}
	//////////////////////
	//// secret path end
	//////////////////////

	////////////////////
	// paths converge
	////////////////////
	// TODO: put stuff here

	/////////////
	// the end!
	platform = new Platform(cube);
	platform->Scale(20, 2, 20);
	platform->Translate(0, -12, -170);
	platforms.push_back(platform);
	// outer walls
	platform = new Platform(cube);
	platform->Scale(1,7,5);
	platform->Translate(-5, -8.5, -170);
	platforms.push_back(platform);
	platform = new Platform(cube);
	platform->Scale(1,7,5);
	platform->Translate(5, -8.5, -170);
	platforms.push_back(platform);
	// inner walls
	platform = new Platform(cube);
	platform->Scale(1,10,8);
	platform->Translate(-4, -6, -170);
	platforms.push_back(platform);
	platform = new Platform(cube);
	platform->Scale(1,10,8);
	platform->Translate(4, -6, -170);
	platforms.push_back(platform);
	// back wall
	platform = new Platform(cube);
	platform->Scale(8, 11, 1);
	platform->Translate(0, -5.5, -175);
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
		platform->Translate(tx+txRand, -10+rando/2, -170+txRand);
		platforms.push_back(platform);
		collapsiblePlatforms.push_back(platform);
	}

	// the target
	setTarget(0.f, -11.f, -170.f);
}