#include <Shad/init.h>
#include <Shad/Camera.h>
#include <Shad/TextureRender.h>
#include <Shad/SkyBox.h>
#include <Shad/Blur.h>
#include <Shad/Blender.h>
#include <Shad/MotionBlur.h>
#include <Shad/Antialias.h>
#include <Shad/Level.h>
#ifdef USE_XBOX_CONTROLLER
#include <Shad/XboxController.h>
#endif

#include <PolyMesh/bitmap_image.h>
#include <PolyMesh/PolyMesh.h>
#include <PolyMesh/ParticleCloth.h>
#include <PolyMesh/Cloth.h>
#include <PolyMesh/Character.h>
#include <PolyMesh/Lightning.h>

#include <ctime>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include <FMODex/fmod.hpp>
#include <FMODex/fmod_errors.h>

namespace Game
{
	enum GameState {
		MenuState,
		PlayState,
		PauseState
	};

	enum GameMenuState {
		StartGameState,
		QuitGameState,
		InvertYesState,
		InvertNoState
	};

	enum CharacterState {
		DefaultState,
		JumpingState,
		TeleportingState,
		SecondJumpingState
	};

	uint64_t teleportStartTime = 0;
	uint64_t teleportDuration = 1000;

	GameState gameState = MenuState;
	GameMenuState gameMenuState = StartGameState;
	CharacterState characterState = DefaultState;

	bool invertCameraY = true;

	Level *currentLevel;

#ifdef USE_XBOX_CONTROLLER
	XboxController *controller;
#endif

	BVEC3F Direction;

	bool moveForward = false;
	bool moveLeft = false;
	bool moveBackward = false;
	bool moveRight = false;

	Character *Shad;

	int teleportLeft;


	void Teleport() {
		if (!((Character *)Shad)->RigidBody->canJump() && teleportLeft > 0) {
			characterState = TeleportingState;
			((Character *)Shad)->RigidBody->setVelocityForTimeInterval(Direction*50.0f, (float)teleportDuration/8000.f);
			((Character *)Shad)->RigidBody->setFallSpeed(0.0);
			teleportStartTime = PolyMesh::Time;
			teleportLeft--;
		}
	}

	int deltaPoint;
}

namespace Window
{
	std::string Title = "Shad";
	int Width = 600, Height = 480;

	SkyBox *skyBox;

	Game::Camera* Camera;
	TextureRender *glowMapRenderTarget;
	TextureRender *sceneRenderTargets[NUM_BLUR_FRAMES];
	GLuint currBlurFrame = 0;

	Blur *blur;
	Blender *blender;
	MotionBlur *motionBlur;
	Antialias *antialias;

	// Menu Textures
	GLuint menuStartTex;
	GLuint menuQuitTex;
	GLuint menuInvertYesTex;
	GLuint menuInvertNoTex;

	float MouseSensitivity;

	bool ContDpadDownPressed = false;
	bool ContDpadUpPressed = false;
	bool ContDpadLeftPressed = false;
	bool ContDpadRightPressed = false;
	bool ContBackPressed = false;
	bool ContAPressed = false;

	void Reshape(int width, int height)
	{
		Width = width;
		Height = height;

		// resize render targets
		delete glowMapRenderTarget;
		for (int i = 0; i < NUM_BLUR_FRAMES; i++)
			delete sceneRenderTargets[i];
		glowMapRenderTarget = new TextureRender(Width/2, Height/2, GL_RGBA);
		for (int i = 0; i < NUM_BLUR_FRAMES; i++)
			sceneRenderTargets[i] = new TextureRender(Width, Height, GL_RGBA);

		// clear frames from motion blur object
		motionBlur->clearFrames();

		glutPostRedisplay();
	}

	void _display(PolyMesh *mesh)
	{
		if (!mesh->particleCloth && !mesh->cloth && (!mesh->character || (mesh->character && Game::characterState != Game::TeleportingState)))
			mesh->Draw();
	}

	void setCamera()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(45,((float)Window::Width)/Window::Height,0.1f,100.f);

		btTransform transform = Game::Shad->RigidBody->getGhostObject()->getWorldTransform();
		Camera->UpdatePosition(OVEC3F(transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ()), OVECB(Game::Direction));

		gluLookAt(Camera->Position()[0],Camera->Position()[1]+Camera->VerticalAxis[1],Camera->Position()[2],transform.getOrigin().getX(),transform.getOrigin().getY(),transform.getOrigin().getZ(), 0, 1, 0);
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
			else if (Game::gameMenuState == Game::InvertYesState)
				TextureRender::renderToScreen(menuInvertYesTex, Window::Width, Window::Height, false, true);
			else if (Game::gameMenuState == Game::InvertNoState)
				TextureRender::renderToScreen(menuInvertNoTex, Window::Width, Window::Height, false, true);

			glutSwapBuffers();
		}
		else if (Game::gameState == Game::PlayState)
		{
			glUseProgram(0);
			glowMapRenderTarget->bind();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			/* Set camera position and direction */
			setCamera();

			// Render depth of occlusive objects
			glColorMask(false,false,false,false);
			std::for_each(PolyMesh::Meshes.begin(), PolyMesh::Meshes.end(), _display);

			// Draw glowing objects
			glColorMask(true,true,true,true);
			Game::currentLevel->drawPlatformEdges();
			Game::currentLevel->drawLightningBolts();

			for (std::list<PolyMesh *>::iterator i = PolyMesh::Meshes.begin(); i != PolyMesh::Meshes.end(); i++) {
				if ((*i)->particleCloth || (*i)->cloth) {
					(*i)->Draw();
				}
			}

			if (Game::characterState == Game::TeleportingState)
				Game::Shad->Draw();

			glViewport(0, 0, glowMapRenderTarget->width(), glowMapRenderTarget->height());

			glowMapRenderTarget->unbind();

			/* render entire scene to the current frame */
			sceneRenderTargets[currBlurFrame]->bind();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			/* Set camera position and direction */
			setCamera();

			// Draw objects
			std::for_each(PolyMesh::Meshes.begin(), PolyMesh::Meshes.end(), _display);
			for (std::list<PolyMesh *>::iterator i = PolyMesh::Meshes.begin(); i != PolyMesh::Meshes.end(); i++) {
				if ((*i)->particleCloth || (*i)->cloth) {
					(*i)->Draw();
				}
			}

			if (Game::characterState == Game::TeleportingState)
				Game::Shad->Draw();
			Game::currentLevel->drawPlatformEdges();
			Game::currentLevel->drawLightningBolts();
			btVector3 characterPos = Game::Shad->GetPosition();
			float r = Game::currentLevel->drawCharacterShadow(characterPos.x(), characterPos.y(), characterPos.z());
			float diff = 0.202093-r;
			float EPSILON = 0.0001;
			if ((diff < EPSILON)) {
				Game::teleportLeft = 4;
			}
			glViewport(0, 0, sceneRenderTargets[currBlurFrame]->width(), sceneRenderTargets[currBlurFrame]->height());

			sceneRenderTargets[currBlurFrame]->unbind();

			/* apply motion blur (not really, this code is just for show :P) */
			motionBlur->addFrame(sceneRenderTargets[currBlurFrame]->textureID());
			GLuint motionBlurredTexID = motionBlur->blurFrames();

			/* apply glow effect to current frame */
			// blur the glow map
			GLuint blurredTexID = blur->blurTexture(glowMapRenderTarget->textureID());
			// blend glow map with current frame (or previous when teleporting...for "motion blur")
			GLuint frameNum = (Game::characterState == Game::TeleportingState) ? (currBlurFrame+1) % NUM_BLUR_FRAMES : currBlurFrame;
			GLuint blendedTexID = blender->blendTextures(sceneRenderTargets[frameNum]->textureID(), blurredTexID);

			/* TODO: perform antialiasing */
			GLuint aaTexID = antialias->antialiasTexture(blendedTexID, blender->width(), blender->height());

			/* render final texture to the screen */

			TextureRender::renderToScreen(blendedTexID, Window::Width, Window::Height, false, false);

			currBlurFrame++; currBlurFrame = currBlurFrame % NUM_BLUR_FRAMES;

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
			Game::Teleport();
			break;
		case JUMP:
			((Character *)Game::Shad)->RigidBody->jump();
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
				{
					if (Game::gameMenuState == Game::QuitGameState)
						Game::gameMenuState = Game::StartGameState;
					else if (Game::gameMenuState == Game::InvertNoState || Game::gameMenuState == Game::InvertYesState)
						Game::gameMenuState = Game::QuitGameState;
				}
				break;
			}
		case GLUT_KEY_DOWN:
			{
				if (Game::gameState == Game::MenuState)
				{
					if (Game::gameMenuState == Game::StartGameState)
						Game::gameMenuState = Game::QuitGameState;
					else if (Game::gameMenuState == Game::QuitGameState)
						Game::gameMenuState = (Game::invertCameraY) ? Game::InvertYesState : Game::InvertNoState;
				}
				break;
			}
		case GLUT_KEY_LEFT:
			{
				if (Game::gameState == Game::MenuState)
				{
					if (Game::gameMenuState == Game::InvertNoState) {
						Game::gameMenuState = Game::InvertYesState;
						Game::invertCameraY = true;
					}
				}
				break;
			}
		case GLUT_KEY_RIGHT:
			{
				if (Game::gameState == Game::MenuState)
				{
					if (Game::gameMenuState == Game::InvertYesState) {
						Game::gameMenuState = Game::InvertNoState;
						Game::invertCameraY = false;
					}
				}
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
			Game::Shad->Rotate(DEGREES(-DeltaX*MouseSensitivity),0,1,0);

			float rotationAngle = (Game::invertCameraY) ? -DeltaY*MouseSensitivity : DeltaY*MouseSensitivity;
			OVEC3F NewCamera = OVECB(BVECO(Camera->VerticalAxis).rotate(BVEC3F(1,0,0), rotationAngle));
			Camera->VerticalAxis = NewCamera[2] * Camera->VerticalAxis[2] >= 0 ? NewCamera : Camera->VerticalAxis;

			glutWarpPointer(Window::Width/2, Window::Height/2);
		}
	}

	void Timer(int time)
	{
		if (Game::gameState == Game::MenuState)
		{
#ifdef USE_XBOX_CONTROLLER
			/* Poll Xbox controller */
			if (Game::controller->isConnected()) {
				/* A button controls */
				if (!(Game::controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A) && Window::ContAPressed) {
					if (Game::gameMenuState == Game::StartGameState)
						Game::gameState = Game::PlayState;
					else if (Game::gameMenuState == Game::QuitGameState)
						glutLeaveMainLoop();
					Window::ContAPressed = false;
				}

				/* Back button controls */
				if (!(Game::controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK) && Window::ContBackPressed)
					glutLeaveMainLoop();

				/* D-pad controls */
				if (!(Game::controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) && Window::ContDpadUpPressed) {
					if (Game::gameMenuState == Game::QuitGameState)
						Game::gameMenuState = Game::StartGameState;
					else if (Game::gameMenuState == Game::InvertNoState || Game::gameMenuState == Game::InvertYesState)
						Game::gameMenuState = Game::QuitGameState;
					Window::ContDpadUpPressed = false;
				}
				if (!(Game::controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) && Window::ContDpadDownPressed) {
					if (Game::gameMenuState == Game::StartGameState)
						Game::gameMenuState = Game::QuitGameState;
					else if (Game::gameMenuState == Game::QuitGameState)
						Game::gameMenuState = (Game::invertCameraY) ? Game::InvertYesState : Game::InvertNoState;
					Window::ContDpadDownPressed = false;
				}
				if (!(Game::controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) && Window::ContDpadLeftPressed) {
					if (Game::gameMenuState == Game::InvertNoState) {
						Game::gameMenuState = Game::InvertYesState;
						Game::invertCameraY = true;
					}
					Window::ContDpadLeftPressed = false;
				}
				if (!(Game::controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) && Window::ContDpadRightPressed) {
					if (Game::gameMenuState == Game::InvertYesState) {
						Game::gameMenuState = Game::InvertNoState;
						Game::invertCameraY = false;
					}
					Window::ContDpadRightPressed = false;
				}

				/* Control Dpad, A, and Back button poll events to only occur on button release */
				if (Game::controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
					Window::ContDpadUpPressed = true;
				if (Game::controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
					Window::ContDpadDownPressed = true;
				if (Game::controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)
					Window::ContDpadLeftPressed = true;
				if (Game::controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)
					Window::ContDpadRightPressed = true;
				if (Game::controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
					Window::ContBackPressed = true;
				if (Game::controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
					Window::ContAPressed = true;
			}
#endif
		}
		else if (Game::gameState == Game::PlayState)
		{
			PolyMesh::Time += (uint64_t) FRAME_PERIOD;

			/* update teleporting state if done */
			if (PolyMesh::Time - Game::teleportStartTime > Game::teleportDuration/4 - 100) {
				Game::characterState = Game::DefaultState;
				// need to reset fallspeed
				btScalar gravity = ((Character *)Game::Shad)->RigidBody->getGravity();
				((Character *)Game::Shad)->RigidBody->setFallSpeed(gravity);
			}

			/* Simulate Physics */
			Physics::DynamicsWorld->stepSimulation(1.0f/FRAME_RATE, 20, 1.0f/FRAME_RATE);
			Game::Shad->SyncDummy();

			for (std::list<PolyMesh *>::iterator i = PolyMesh::Meshes.begin(); i != PolyMesh::Meshes.end(); i++)
				if ((*i)->cloth)
					((Cloth *)(*i))->SimulationStep();

			for (std::list<PolyMesh *>::iterator i = PolyMesh::Meshes.begin(); i != PolyMesh::Meshes.end(); i++)
				if ((*i)->particleCloth)
					((ParticleCloth *)(*i))->SimulationStep();

			/* Animate lightning */
			Game::currentLevel->applyLightningAnimationStep();

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


#ifdef USE_XBOX_CONTROLLER
			/* Poll Xbox controller */
			if (Game::controller->isConnected()) {
				/* A button controls */
				if (Game::controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A) {
					((Character *)Game::Shad)->RigidBody->jump();
				}

				/* back button controls */
				if (!(Game::controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK) && Window::ContBackPressed) {
					Game::gameState = Game::PauseState;
					Window::ContBackPressed = false;
				}

				/* right trigger controls */
				if (Game::controller->GetState().Gamepad.bRightTrigger) {
					Game::Teleport();
				}

				/* left stick controls */
				if (Game::controller->LeftStickMoved()) {
					float directionAngle = -Game::controller->GetDirectionAngle();
					WalkDirection += Game::Direction.rotate(BVEC3F(0,1,0),RADIANS(directionAngle))*0.1f;
				}

				/* right stick controls */
				if (Game::controller->RightStickMoved()) {
					if (Game::gameState == Game::PlayState) {
						float cameraSensitivityX = 0.1;
						float cameraSensitivityY = 0.03;
						int xCameraCoef = Game::controller->GetXCameraCoefficient();
						int yCameraCoef = Game::controller->GetYCameraCoefficient();
						Game::Direction = Game::Direction.rotate(BVEC3F(0,1,0), -xCameraCoef*cameraSensitivityX);
						Game::Shad->Rotate(DEGREES(-xCameraCoef*cameraSensitivityX),0,1,0);
						//Rotate in Y direction as well?
						float rotationAngle = (Game::invertCameraY) ? yCameraCoef*cameraSensitivityY : -yCameraCoef*cameraSensitivityY;
						OVEC3F NewCamera = OVECB(BVECO(Camera->VerticalAxis).rotate(BVEC3F(1,0,0), rotationAngle));
						Camera->VerticalAxis = NewCamera[2] * Camera->VerticalAxis[2] >= 0 ? NewCamera : Camera->VerticalAxis;
					}
				}

				/* Control Back button poll events to only occur on button release */
				if (Game::controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
					Window::ContBackPressed = true;

			}
#endif

			if (Game::characterState != Game::TeleportingState)
				((Character *)Game::Shad)->RigidBody->setWalkDirection(WalkDirection);
			
			/* check lightning collision */
			
			btVector3 characterPos = Game::Shad->GetPosition();
			if (Game::currentLevel->lightningCollisionWithPoint(OpenMesh::Vec3f(characterPos.x(),characterPos.y(),characterPos.z()))) {
				//WHAT DO WE WANT TO DO WHEN CHARACTER COLLIDES WITH LIGHTNING?
			}

			/* reset position on death */
			btTransform transform = ((Character *)Game::Shad)->RigidBody->getGhostObject()->getWorldTransform();
			if (transform.getOrigin().getY() < -30.0) {
				btTransform id;
				id.setIdentity();
				((Character *)Game::Shad)->RigidBody->getGhostObject()->setWorldTransform(id);
				Game::Direction = BVEC3F(0,0,-1);
				Game::currentLevel->reset();
			}

			/*Code to finish level*/
			OpenMesh::Vec3f goal = Game::currentLevel->getTarget();
			float xD = transform.getOrigin().getX() - goal[0];
			float yD = transform.getOrigin().getY() - goal[1];
			float zD = transform.getOrigin().getZ() - goal[2];
			float distance = xD*xD + yD*yD + zD*zD;
			distance = sqrt(distance);
			if (distance < 1.0) {
				Game::currentLevel->destroyPlatforms();
				Game::currentLevel->changeUp();
				Game::currentLevel->generateBlocks(TOON_SHADER, space_image);
				Game::Direction = BVEC3F(0,0,-1);

				//GET RID OF THIS?
				Game::currentLevel->drawPlatformEdges();

				btTransform id;
				id.setIdentity();
				((Character *)Game::Shad)->RigidBody->getGhostObject()->setWorldTransform(id);

			}

			/*Code to move platforms*/
			uint64_t time= PolyMesh::Time;
			time /= 1000;
			time %= 10;
			bool onGround = Game::Shad->RigidBody->onGround();
			((Character *)Game::Shad)->RigidBody->getGhostObject()->getWorldTransform();
			float charX = transform.getOrigin().getX();
			float charY = transform.getOrigin().getY();
			float charZ = transform.getOrigin().getZ();

			Game::currentLevel->move(time, onGround, charX, charY, charZ, Game::Shad);
			Game::currentLevel->collapse(onGround, charX, charY, charZ);
			Game::currentLevel->deform(onGround, charX, charY, charZ);

			Game::currentLevel->shrink(time, charX, charY, charZ, Game::Shad);
			
			//Game::deltaPoint++;
		}
		else if (Game::gameState == Game::PauseState)
		{
#ifdef USE_XBOX_CONTROLLER
			/* Poll Xbox controller */
			if (Game::controller->isConnected()) {
				/* A button controls */
				if (!(Game::controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A) && Window::ContAPressed) {
					Game::gameState = Game::PlayState;
					Window::ContAPressed = false;
				}

				/* back button controls */
				if (!(Game::controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK) && Window::ContBackPressed) {
					Game::gameState = Game::MenuState;
					Window::ContBackPressed = false;
				}

				/* Control A and Back button poll events to only occur on button release */
				if (Game::controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_BACK)
					Window::ContBackPressed = true;
				if (Game::controller->GetState().Gamepad.wButtons & XINPUT_GAMEPAD_A)
					Window::ContAPressed = true;
			}
#endif
		}

		glutTimerFunc((int) FRAME_PERIOD, Timer, (int) FRAME_PERIOD);

		glutPostRedisplay();
	}
}

void ERRCHECK(FMOD_RESULT result)
{
	if (result != FMOD_OK)
	{
		printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
		exit(-1);
	}
}

namespace Sound {
	FMOD::System *system;

	FMOD::Channel *MainMusicChannel = 0;
	FMOD::Sound *MainMenuMusic;

	void InitSound() {
		FMOD_RESULT      result;
		unsigned int     version;

		result = FMOD::System_Create(&system);
		ERRCHECK(result);

		result = system->getVersion(&version);
		ERRCHECK(result);

		if (version < FMOD_VERSION)
		{
			printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
			exit(-1);
		}

		result = system->init(32, FMOD_INIT_NORMAL, 0);
		ERRCHECK(result);

		ERRCHECK(Sound::system->createSound("assets/mp3/MainMenu.mp3", FMOD_HARDWARE, 0, &MainMenuMusic));
		ERRCHECK(MainMenuMusic->setMode(FMOD_LOOP_NORMAL));
		ERRCHECK(system->playSound(FMOD_CHANNEL_FREE, MainMenuMusic, false, &MainMusicChannel));


		ERRCHECK(system->update());
	}
}

int main (int argc, char **argv)
{
	// First call to seed to randomizer, yes man
	srand((unsigned int)time(NULL));

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

#ifdef USE_XBOX_CONTROLLER
	// Setup Xbox controller
	Game::controller = new XboxController(1);
#endif

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
		std::cerr << "Your current GL version is: " << version << std::endl;
		const char *glslVersion = (const char *)glGetString(GL_SHADING_LANGUAGE_VERSION);
		std::cout << "Your current GLSL version is: " << glslVersion << std::endl;
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

	// Enable alpha blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Create Camera
	Window::Camera = new Game::Camera();

	// Create Skybox
	/*Window::skyBox = new SkyBox(0.f, 0.f, 0.f, 100.f);
	Window::skyBox->addTexture(SKYBOX_FRONT_TEXTURE, SkyBoxFront);
	Window::skyBox->addTexture(SKYBOX_BACK_TEXTURE, SkyBoxBack);
	Window::skyBox->addTexture(SKYBOX_LEFT_TEXTURE, SkyBoxLeft);
	Window::skyBox->addTexture(SKYBOX_RIGHT_TEXTURE, SkyBoxRight);
	Window::skyBox->addTexture(SKYBOX_TOP_TEXTURE, SkyBoxTop);
	Window::skyBox->addTexture(SKYBOX_BOTTOM_TEXTURE, SkyBoxBottom);*/

	// Create render-to-texture targets
	Window::glowMapRenderTarget = new TextureRender(Window::Width/2, Window::Height/2, GL_RGBA);
	for (int i = 0; i < NUM_BLUR_FRAMES; i++)
		Window::sceneRenderTargets[i] = new TextureRender(2*Window::Width, 2*Window::Height, GL_RGBA);

	// Create post-processing objects
	Window::blur = new Blur(Window::glowMapRenderTarget->width(), Window::glowMapRenderTarget->height());
	Window::blender = new Blender(SCREEN, 2*Window::Width, 2*Window::Height);
	Window::motionBlur = new MotionBlur(2*Window::Width, 2*Window::Height, NUM_BLUR_FRAMES);
	Window::antialias = new Antialias(Window::Width, Window::Height);

	// Initialize Physics
	Physics::InitializePhysics();

	/* TODO: (potentially) abstract out into a Menu class */
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

	menu_invert_yes_image = bitmap_image(MENU_INVERT_YES_TEXTURE);
	menu_invert_yes_image.rgb_to_bgr();
	glGenTextures(1, &Window::menuInvertYesTex);
	glBindTexture(GL_TEXTURE_2D, Window::menuInvertYesTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, menu_invert_yes_image.width(), menu_invert_yes_image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, menu_invert_yes_image.data());
	glBindTexture(GL_TEXTURE_2D, 0);

	menu_invert_no_image = bitmap_image(MENU_INVERT_NO_TEXTURE);
	menu_invert_no_image.rgb_to_bgr();
	glGenTextures(1, &Window::menuInvertNoTex);
	glBindTexture(GL_TEXTURE_2D, Window::menuInvertNoTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, menu_invert_no_image.width(), menu_invert_no_image.height(), 0, GL_RGB, GL_UNSIGNED_BYTE, menu_invert_no_image.data());
	glBindTexture(GL_TEXTURE_2D, 0);

	// Load Mesh
	Game::Shad = new Character();
	Game::Shad->LoadObj(OBJECT);
	Game::Shad->GenerateCharacter();
	Game::Shad->AttachShader(TOON_SHADER);
	Game::Shad->RigidBody->setJumpSpeed(20.0f);
	Game::Shad->RigidBody->setGravity(100.0f);

	ParticleCloth *cape = new ParticleCloth(25,10,0.025, BVEC3F(-0.11f, 0.15f, 0.15f), BVEC3F(0.29f, 0.15f, 0.15f), BVEC3F(0,1,0), 0.1f, 1, Game::Shad);
	cape_image = bitmap_image("assets\\bmp\\cape_texture.bmp");
	cape_image.rgb_to_bgr();
	cape->EnableLighting()->ApplyTexture(cape_image.data(),cape_image.width(), cape_image.height());

	Game::currentLevel = new Level(1);
	Game::currentLevel->generateBlocks(TOON_SHADER, space_image);

	(new Platform("assets\\obj\\cube.obj"))->Scale(0.1f,1.4f,0.1f)->Translate(-0.7f,-13.2f, -22);
	(new Platform("assets\\obj\\cube.obj"))->Scale(0.1f,1.4f,0.1f)->Translate(0.7f,-13.2f, -22);
	PolyMesh * pinTarget = (new Platform("assets\\obj\\cube.obj"))->Scale(1.5f,0.1f,0.1f)->Translate(0,-12.45, -22)->platformMesh;

	Cloth *Cloak = new Cloth(0.001f, 0.0005f, 0.0005f, OVEC3F(0,-1,0), OVEC3F(1,0,0), OVEC3F(-0.6f, -12.55f, -22),12,12,1.2f,0.1f,0.1f, BVEC3F(0,0,0.0003f));
	Cloak->AttachShader(TOON_SHADER);
	Cloak->EnableLighting();
	Cloak->Pin(0,0,pinTarget->RigidBody, new BVEC3F(-0.5f,-0.1f,0));
	Cloak->Pin(0,11,pinTarget->RigidBody, new BVEC3F(0.5f,-0.1f,0));
	cloth_image = bitmap_image("assets\\bmp\\flag_texture.bmp");
	//cloth_image.rgb_to_bgr();
	Cloak->ApplyTexture(cloth_image.data(), cloth_image.width(), cloth_image.height());

	// Set Mesh and Plane Material Parameters
	Game::Shad->MaterialSpecular = Specular;
	Cloak->MaterialSpecular = Specular;
	cape->MaterialSpecular = CapeSpecular;

	Game::Shad->MaterialDiffuse = Diffuse;
	Cloak->MaterialDiffuse = Diffuse;
	cape->MaterialDiffuse = CapeDiffuse;

	Game::Shad->MaterialAmbient = Ambient;
	Cloak->MaterialAmbient = Ambient;
	cape->MaterialAmbient = CapeAmbient;

	Game::Shad->MaterialShininess = Shininess;
	Cloak->MaterialShininess = Shininess;
	cape->MaterialShininess = CapeShininess;

	// Apply Texture to Mesh
	image = bitmap_image(TEXTURE);
	image.rgb_to_bgr();
	Game::Shad->ApplyTexture(image.data(), image.width(), image.height());

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

	Game::Shad->EnableLighting();

	Game::Direction = BVEC3F(0,0,-1);
	Game::deltaPoint = 0;

	// Init sound
	Sound::InitSound();

	// Run Loop
	glutMainLoop();

	return 0;
}