#include <Shad/init.h>
#include <Shad/Camera.h>
#include <Shad/TextureRender.h>
#include <Shad/Blur.h>
#include <Shad/Blender.h>
#include <Shad/MotionBlur.h>
#include <Shad/Level.h>

#include <PolyMesh/bitmap_image.h>
#include <PolyMesh/PolyMesh.h>
#include <PolyMesh/Cloth.h>
#include <PolyMesh/Character.h>
#include <PolyMesh/Lightning.h>

#include <ctime>

#include <GL/glew.h>
#include <GL/freeglut.h>

namespace Game
{
	enum GameState {
		MenuState,
		PlayState,
		PauseState
	};

	enum GameMenuState {
		StartGameState,
		QuitGameState
	};

	enum CharacterState {
		DefaultState,
		JumpingState,
		TeleportingState
	};

	uint64_t teleportStartTime = 0;
	uint64_t teleportDuration = 1000;

	GameState gameState = MenuState;
	GameMenuState gameMenuState = StartGameState;
	CharacterState characterState = DefaultState;

	BVEC3F Direction;

	bool moveForward = false;
	bool moveLeft = false;
	bool moveBackward = false;
	bool moveRight = false;
}

namespace Window
{
	std::string Title = "Shad";
	int Width = 600, Height = 480;

	Game::Camera* Camera;
	TextureRender *aaTexRenderTarget;
	TextureRender *glowMapRenderTarget;
	TextureRender *sceneRenderTarget;
	TextureRender *motionRenderTargets[NUM_BLUR_FRAMES];
	GLuint currBlurFrame = 0;
	
	Blur *blur;
	Blender *blender;
	MotionBlur *motionBlur;

	// Post-processing Shader IDs
	GLuint antialiasShaderID = 0;

	// Menu Textures
	GLuint menuStartTex;
	GLuint menuQuitTex;

	float MouseSensitivity;

	Lightning *lightning;

	void Reshape(int width, int height)
	{
		Width = width;
		Height = height;

		// resize render targets
		delete aaTexRenderTarget;
		delete glowMapRenderTarget;
		delete sceneRenderTarget;
		for (int i = 0; i < NUM_BLUR_FRAMES; i++)
			delete motionRenderTargets[i];
		aaTexRenderTarget = new TextureRender(2*Width, 2*Height, GL_RGBA);
		glowMapRenderTarget = new TextureRender(Width/2, Height/2, GL_RGBA);
		sceneRenderTarget = new TextureRender(Width, Height, GL_RGBA);
		for (int i = 0; i < NUM_BLUR_FRAMES; i++)
			motionRenderTargets[i] = new TextureRender(Width, Height, GL_RGBA);

		// clear frames from motion blur object
		motionBlur->clearFrames();

		glutPostRedisplay();
	}

	void _display(PolyMesh *mesh)
	{
		mesh->Draw();
	}

	void Display(void)
	{
		if (Game::gameState == Game::MenuState)
		{
			glUseProgram(0);
			
			if (Game::gameMenuState == Game::StartGameState)
				TextureRender::renderToScreen(menuStartTex, Window::Width, Window::Height, false, true);
			else if (Game::gameMenuState == Game::QuitGameState)
				TextureRender::renderToScreen(menuQuitTex, Window::Width, Window::Height, false, true);

			glutSwapBuffers();
		}
		else if (Game::gameState == Game::PlayState)
		{
			glUseProgram(0);
			glowMapRenderTarget->bind();

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				/* Set camera position and direction */
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();

				gluPerspective(45,((float)Window::Width)/Window::Height,0.1f,100.f);

				Character *character = (Character *)PolyMesh::Meshes[0];
				btTransform transform = character->RigidBody->getGhostObject()->getWorldTransform();
				Camera->UpdatePosition(OVEC3F(transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ()), OVECB(Game::Direction));

				gluLookAt(Camera->Position()[0],Camera->Position()[1]+1.0f,Camera->Position()[2],transform.getOrigin().getX(),transform.getOrigin().getY(),transform.getOrigin().getZ(), 0, 1, 0);

				// Draw objects
				std::for_each(PolyMesh::Meshes.begin(), PolyMesh::Meshes.end(), _display);
				//lightning->Draw();

				//glViewport(0, 0, aaTexRenderTarget->width(), aaTexRenderTarget->height());
				glViewport(0, 0, glowMapRenderTarget->width(), glowMapRenderTarget->height());

			glowMapRenderTarget->unbind();

			/* render entire scene to the current motion blur frame */
			motionRenderTargets[currBlurFrame]->bind();

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				/* Set camera position and direction */
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();

				gluPerspective(45,((float)Window::Width)/Window::Height,0.1f,100.f);

				character = (Character *)PolyMesh::Meshes[0];
				transform = character->RigidBody->getGhostObject()->getWorldTransform();
				Camera->UpdatePosition(OVEC3F(transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ()), OVECB(Game::Direction));

				gluLookAt(Camera->Position()[0],Camera->Position()[1]+1.0f,Camera->Position()[2],transform.getOrigin().getX(),transform.getOrigin().getY(),transform.getOrigin().getZ(), 0, 1, 0);

				// Draw objects
				std::for_each(PolyMesh::Meshes.begin(), PolyMesh::Meshes.end(), _display);
				//lightning->Draw();

				//glViewport(0, 0, aaTexRenderTarget->width(), aaTexRenderTarget->height());
				glViewport(0, 0, motionRenderTargets[currBlurFrame]->width(), motionRenderTargets[currBlurFrame]->height());
			
			motionRenderTargets[currBlurFrame]->unbind();

			/* apply glow effect */
			GLuint blurredTexID = blur->blurTexture(glowMapRenderTarget->textureID());
			GLuint blendedTexID = blender->blendTextures(motionRenderTargets[currBlurFrame]->textureID(), blurredTexID);

			/* apply motion blur */
			/*motionBlur->addFrame(motionRenderTargets[currBlurFrame]->textureID());
			std::cout << "added frame: " << motionRenderTargets[currBlurFrame]->textureID() << std::endl;
			motionBlur->printFrames();
			GLuint motionBlurredTexID = motionBlur->blurFrames();
			currBlurFrame++; currBlurFrame = currBlurFrame % NUM_BLUR_FRAMES;*/

			/* TODO: perform antialiasing */

			/* render final texture to the screen */
			TextureRender::renderToScreen(blendedTexID, Window::Width, Window::Height, false, false);
			
			glutSwapBuffers();
		}
		else if (Game::gameState == Game::PauseState)
		{
			glUseProgram(0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			/* TODO: add pause menu */
			glutSwapBuffers();
		}
	}

	void Keyboard(unsigned char key, int x, int y) {
		static int MatMode = 0;
		switch (key)
		{
		case 27: // esc
			if (Game::gameState == Game::MenuState)
				glutLeaveMainLoop();
			else if (Game::gameState == Game::PlayState)
				Game::gameState = Game::PauseState;
			else if (Game::gameState == Game::PauseState)
				Game::gameState = Game::MenuState;
			break;
		case 13: // enter
			if (Game::gameState == Game::PauseState)
				Game::gameState = Game::PlayState;
			else if (Game::gameState == Game::MenuState) {
				if (Game::gameMenuState == Game::StartGameState)
					Game::gameState = Game::PlayState;
				else if (Game::gameMenuState == Game::QuitGameState)
					glutLeaveMainLoop();
			}
			break;
		case 'p':
			aaTexRenderTarget->writeToFile("scene_texture.tga");
			break;
		case 'W':
		case 'w':
			Game::moveForward = true;
			break;
		case 'S':
		case 's':
			Game::moveBackward = true;
			break;
		case 'A':
		case 'a':
			Game::moveLeft = true;
			break;
		case 'D':
		case 'd':
			Game::moveRight = true;
			break;
		case '\t':
			if (!((Character *)PolyMesh::Meshes[0])->RigidBody->canJump()) {
				Game::characterState = Game::TeleportingState;
				((Character *)PolyMesh::Meshes[0])->RigidBody->setVelocityForTimeInterval(Game::Direction*50.0f, (float)Game::teleportDuration/8000.f);
				((Character *)PolyMesh::Meshes[0])->RigidBody->setFallSpeed(0.0);
				Game::teleportStartTime = PolyMesh::Time;
			}
			break;
		case JUMP:
			((Character *)PolyMesh::Meshes[0])->RigidBody->jump();
			break;
		default:
			break;
		}
	}

	void KeyboardUp(unsigned char key, int x, int y)
	{
		switch (key)
		{
		case 'W':
		case 'w':
			Game::moveForward = false;
			break;
		case 'S':
		case 's':
			Game::moveBackward = false;
			break;
		case 'A':
		case 'a':
			Game::moveLeft = false;
			break;
		case 'D':
		case 'd':
			Game::moveRight = false;
			break;
		}
	}

	void SpecialKeyboard(int key, int x, int y)
	{
		switch (key)
		{
		case GLUT_KEY_UP:
			{
				if (Game::gameState == Game::MenuState)
					Game::gameMenuState = (Game::gameMenuState == Game::StartGameState) ? Game::QuitGameState : Game::StartGameState;
				break;
			}
		case GLUT_KEY_DOWN:
			{
				if (Game::gameState == Game::MenuState)
					Game::gameMenuState = (Game::gameMenuState == Game::StartGameState) ? Game::QuitGameState : Game::StartGameState;
				break;
			}
		default:
			break;
		}
	}

	void SpecialKeyboardUp(int key, int x, int y)
	{

	}

	void Mouse(int button, int state, int x, int y) {
	}

	void PassiveMotion(int x, int y)  {
		if (x != Window::Width/2 && y != Window::Height/2 && Game::gameState == Game::PlayState) {
			int DeltaX, DeltaY;

			DeltaX = x - Window::Width/2;
			DeltaY = y - Window::Height/2;

			Game::Direction = Game::Direction.rotate(BVEC3F(0,1,0), -DeltaX*MouseSensitivity);
			PolyMesh::Meshes[0]->Rotate(DEGREES(-DeltaX*MouseSensitivity),0,1,0);
			glutWarpPointer(Window::Width/2, Window::Height/2);
		}
	}

	void Timer(int time)
	{
		if (Game::gameState == Game::MenuState)
		{
			/* menu animation? */
		}
		else if (Game::gameState == Game::PlayState)
		{
			PolyMesh::Time += (uint64_t) FRAME_PERIOD;

			/* update teleporting state if done */
			if (PolyMesh::Time - Game::teleportStartTime > Game::teleportDuration/4 - 100) {
				Game::characterState = Game::DefaultState;
				// need to reset fallspeed
				btScalar gravity = ((Character *)PolyMesh::Meshes[0])->RigidBody->getGravity();
				((Character *)PolyMesh::Meshes[0])->RigidBody->setFallSpeed(gravity);
			}

			/* Simulate Physics */
			Physics::DynamicsWorld->stepSimulation(1.0f/FRAME_RATE, 20, 1.0f/FRAME_RATE);

			for (unsigned int i = 0; i < PolyMesh::Meshes.size(); i++)
				if (PolyMesh::Meshes[i]->cloth)
					((Cloth *)PolyMesh::Meshes[i])->SimulationStep();

			BVEC3F WalkDirection(0,0,0);
			/* Character Controls */
			if (Game::moveForward)
				WalkDirection += Game::Direction*0.1f;
			if (Game::moveBackward)
				WalkDirection -= Game::Direction*0.1f;
			if (Game::moveLeft)
				WalkDirection += Game::Direction.rotate(BVEC3F(0,1,0),RADIANS(90))*0.1f;
			if (Game::moveRight)
				WalkDirection -= Game::Direction.rotate(BVEC3F(0,1,0),RADIANS(90))*0.1f;
			
			if (Game::characterState != Game::TeleportingState)
				((Character *)PolyMesh::Meshes[0])->RigidBody->setWalkDirection(WalkDirection);
			
			/* reset position on death */
			btTransform transform = ((Character *)PolyMesh::Meshes[0])->RigidBody->getGhostObject()->getWorldTransform();
			if (transform.getOrigin().getY() < -30.0) {
				btTransform id;
				id.setIdentity();
				((Character *)PolyMesh::Meshes[0])->RigidBody->getGhostObject()->setWorldTransform(id);
				Game::Direction = BVEC3F(0,0,-1);
			}

		}
		else if (Game::gameState == Game::PauseState)
		{
			/* animate pause menu? */
		}

		glutTimerFunc((int) FRAME_PERIOD, Timer, (int) FRAME_PERIOD);

		glutPostRedisplay();
	}
}

int main (int argc, char **argv)
{
	// Initialize GL
	glutInit(&argc, argv);

	// Initialize Display Mode
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);

	// Enable multisampling
	glEnable(GLUT_MULTISAMPLE);

	// Initialize Window Size Parameters
	glutInitWindowSize(Window::Width, Window::Height);

	// Create Window
	glutCreateWindow(Window::Title.c_str());

	// Go fullscreen
	glutFullScreen();

	Window::Width = glutGet(GLUT_WINDOW_WIDTH);
	Window::Height = glutGet(GLUT_WINDOW_HEIGHT);

	// Initialize GLEW (for shaders)
	GLint error = glewInit();
	if (GLEW_OK != error)
	{
		std::cerr << glewGetErrorString(error) << std::endl;
		exit(-1);
	}
	if (!GLEW_VERSION_3_0)
	{
		std::cerr << "This program requires OpenGL 3.0 or higher." << std::endl;
		const char *version = (const char *)glGetString(GL_VERSION);
		std::cerr << "Your current version is: " << version << std::endl;
		exit(-1);
	}

	// Pre-load and Compile Shaders
	Shader *defaultShader = new Shader(DEFAULT_SHADER);
	if (!defaultShader->loaded()) {
		std::cerr << "Failed to load shader: " << defaultShader->path() << std::endl;
		std::cerr << defaultShader->errors() << std::endl;
	}
	Shader *phongShader = new Shader(PHONG_SHADER);
	if (!phongShader->loaded()) {
		std::cerr << "Failed to load shader: " << phongShader->path() << std::endl;
		std::cerr << phongShader->errors() << std::endl;
	}
	Shader *toonShader = new Shader(TOON_SHADER);
	if (!toonShader->loaded()) {
		std::cerr << "Failed to load shader: " << toonShader->path() << std::endl;
		std::cerr << toonShader->errors() << std::endl;
	}
	Shader *normalShader = new Shader(NORMAL_SHADER);
	if (!normalShader->loaded()) {
		std::cerr << "Failed to load shader: " << normalShader->path() << std::endl;
		std::cerr << normalShader->errors() << std::endl;
	}
	Shader *antialiasShader = new Shader(ANTIALIAS_SHADER);
	if (!antialiasShader->loaded()) {
		std::cerr << "Failed to load shader: " << antialiasShader->path() << std::endl;
		std::cerr << antialiasShader->errors() << std::endl;
	}
	// store AA shader ID for use in Display callback
	Window::antialiasShaderID = antialiasShader->programID();

	// Register Reshape Handler
	glutReshapeFunc(Window::Reshape);

	// Register Display Handler
	glutDisplayFunc(Window::Display);

	// Register Keyboard Handler
	glutKeyboardFunc(Window::Keyboard);
	glutKeyboardUpFunc(Window::KeyboardUp);

	// Register Mouse Move Handler
	glutMouseFunc(Window::Mouse);
	glutPassiveMotionFunc(Window::PassiveMotion);
	Window::MouseSensitivity = 0.002f;
	glutWarpPointer(Window::Width/2, Window::Height/2);

	// Hide mouse
	glutSetCursor(GLUT_CURSOR_NONE);

	// Register Special Keyboard Handler
	glutSpecialFunc(Window::SpecialKeyboard);
	glutSpecialUpFunc(Window::SpecialKeyboardUp);

	// Register Timer Handler
	glutTimerFunc((int) FRAME_PERIOD, Window::Timer, (int) FRAME_PERIOD);

	// Enable Normalization
	glEnable(GL_NORMALIZE);

	// Enable Depth Test
	glEnable(GL_DEPTH_TEST);

	// Create Camera
	Window::Camera = new Game::Camera();

	// Create render-to-texture targets
	Window::aaTexRenderTarget = new TextureRender(2*Window::Width, 2*Window::Height, GL_RGB);
	Window::glowMapRenderTarget = new TextureRender(Window::Width/2, Window::Height/2, GL_RGB);
	Window::sceneRenderTarget = new TextureRender(Window::Width, Window::Height, GL_RGB);
	for (int i = 0; i < NUM_BLUR_FRAMES; i++)
		Window::motionRenderTargets[i] = new TextureRender(Window::Width, Window::Height, GL_RGB);

	// Create post-processing objects
	Window::blur = new Blur(Window::glowMapRenderTarget->width(), Window::glowMapRenderTarget->height());
	Window::blender = new Blender(ADDITIVE, Window::Width, Window::Height);
	Window::motionBlur = new MotionBlur(Window::Width, Window::Height, NUM_BLUR_FRAMES);
	
	// Initialize Physics
	Physics::InitializePhysics();

	// Load Menu Images
	menu_start_image = bitmap_image(MENU_START_TEXTURE);
	menu_start_image.rgb_to_bgr();
	glGenTextures(1, &Window::menuStartTex);
	glBindTexture(GL_TEXTURE_2D, Window::menuStartTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, menu_start_image.width(), menu_start_image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, menu_start_image.data());
	glBindTexture(GL_TEXTURE_2D, 0);

	menu_quit_image = bitmap_image(MENU_QUIT_TEXTURE);
	menu_quit_image.rgb_to_bgr();
	glGenTextures(1, &Window::menuQuitTex);
	glBindTexture(GL_TEXTURE_2D, Window::menuQuitTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, menu_quit_image.width(), menu_quit_image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, menu_quit_image.data());
	glBindTexture(GL_TEXTURE_2D, 0);

	// Load Mesh
	Character *Mesh = new Character();
	Mesh->LoadObj(OBJECT);
	Mesh->GenerateCharacter();
	//Mesh->Translate(OVEC3F(0,-0.5f,0));
	Mesh->AttachShader(TOON_SHADER);
	Mesh->RigidBody->setJumpSpeed(20.0f);
	Mesh->RigidBody->setGravity(100.0f);

	Cloth *Cloak = new Cloth(0.001f, 0.0005f, 0.0005f, OVEC3F(-1,0,0), OVEC3F(0,0,1), OVEC3F(0.5f,0,0.5f),10,10,1.2f,0.6f,0.1f);
	Cloak->AttachShader(TOON_SHADER);
	Cloak->EnableLighting();
	//Cloak->Pin(9,0,Mesh->RigidBody->getGhostObject(), new BVEC3F(-0.5f,0.2f,-0.3f));
	//Cloak->Pin(0,0,Mesh->RigidBody->, new BVEC3F(0.5f,0.2f,-0.3f));
	cloth_image = bitmap_image("assets\\bmp\\Cloth2.bmp");
	cloth_image.rgb_to_bgr();
	Cloak->ApplyTexture(cloth_image.data(), cloth_image.width(), cloth_image.height());

	Window::lightning = new Lightning(OVEC3F(-1.f, -1.f, -1.f), OVEC3F(1.f, 1.f, 1.f));

	Level *one = new Level(1);
	one->generateBlocks(TOON_SHADER, space_image);

	// Set Mesh and Plane Material Parameters
	Mesh->MaterialSpecular = Specular;
	Cloak->MaterialSpecular = Specular;

	Mesh->MaterialDiffuse = Diffuse;
	Cloak->MaterialDiffuse = Diffuse;

	Mesh->MaterialAmbient = Ambient;
	Cloak->MaterialAmbient = Ambient;

	Mesh->MaterialShininess = Shininess;
	Cloak->MaterialShininess = Shininess;

	// Apply Texture to Mesh
	image = bitmap_image(TEXTURE);
	image.rgb_to_bgr();
	Mesh->ApplyTexture(image.data(), image.width(), image.height());

	// Setup Lights
	GLfloat LightPosition[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

	GLfloat LightAmbient[] = { 0.6f ,0.6f ,0.6f , 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);

	GLfloat LightDiffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);

	GLfloat LightSpecular[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);

	glEnable(GL_LIGHT0);

	Mesh->EnableLighting();

	Game::Direction = BVEC3F(0,0,-1);

	// Run Loop
	glutMainLoop();

	return 0;
}