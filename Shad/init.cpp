#include <Shad/init.h>
#include <Shad/Camera.h>
#include <Shad/TextureRender.h>

#include <PolyMesh/bitmap_image.h>
#include <PolyMesh/PolyMesh.h>
#include <PolyMesh/Cloth.h>

#include "Level.h"

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

	GameState gameState = MenuState;

	btVector3 Direction;

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
	TextureRender *texRenderTarget;

	void Reshape(int width, int height)
	{
		Width = width;
		Height = height;

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
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			/* TODO: add initial game menu */
			glutSwapBuffers();
		}
		else if (Game::gameState == Game::PlayState)
		{
			/* Render the scene to a texture */
			texRenderTarget->bind();

				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				/* Set camera position and direction */
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();

				gluPerspective(45,((float)Window::Width)/Window::Height,0.1f,100.f);

				btTransform transform = PolyMesh::Meshes[0]->RigidBody->getCenterOfMassTransform();
				Camera->UpdatePosition(OpenMesh::Vec3f(transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ()));

				gluLookAt(Camera->Position()[0],Camera->Position()[1]+1.0f,Camera->Position()[2],transform.getOrigin().getX(),transform.getOrigin().getY(),transform.getOrigin().getZ(), 0, 1, 0);

				/* Draw objects (AFTER setting camera) */
				std::for_each(PolyMesh::Meshes.begin(), PolyMesh::Meshes.end(), _display);

				glViewport(0,0,Window::Width,Window::Height);

			texRenderTarget->unbind();

			/* Render texture to full-screen quad */
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				/* set orthographic projectionation */
				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				gluOrtho2D(-1,1,-1,1);

				/* draw textured quad */
				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, texRenderTarget->textureID());

				glBegin(GL_QUADS);
					glNormal3f(0, 0, 1);
					glTexCoord2f(0.f, 0.f);
					glVertex3f(-1.f, -1.f, 0.f);
					glTexCoord2f(1.f, 0.f);
					glVertex3f(1.f, -1.f, 0.f);
					glTexCoord2f(1.f, 1.f);
					glVertex3f(1.f, 1.f, 0.f);
					glTexCoord2f(0.f, 1.f);
					glVertex3f(-1.f, 1.f, 0.f);
				glEnd();

				glBindTexture(GL_TEXTURE_2D, 0);
				glDisable(GL_TEXTURE_2D);

				glViewport(0, 0, Window::Width, Window::Height);

			glutSwapBuffers();
		}
		else if (Game::gameState == Game::PauseState)
		{
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
			glutLeaveMainLoop();
			break;
		case 13: // enter
			if (Game::gameState == Game::PlayState)
				Game::gameState = Game::PauseState;
			else
				Game::gameState = Game::PlayState;
			break;
		case 'w':
			texRenderTarget->writeToFile("scene_texture.tga");
			break;
		case AMBIENT:
			MatMode = AMBIENT;
			break;
		case SPECULAR:
			MatMode = SPECULAR;
			break;
		case DIFFUSE:
			MatMode = DIFFUSE;
			break;
		case EMISSION:
			MatMode = EMISSION;
			break;
		case SHININESS:
			MatMode = SHININESS;
			break;
		case UP:
			switch (MatMode)
			{
			case AMBIENT:
				Boost(PolyMesh::Meshes[0]->MaterialAmbient, 4, 0.1f);
				break;
			case SPECULAR:
				Boost(PolyMesh::Meshes[0]->MaterialSpecular, 4, 0.1f);
				break;
			case DIFFUSE:
				Boost(PolyMesh::Meshes[0]->MaterialDiffuse, 4, 0.1f);
				break;
			case EMISSION:
				Boost(PolyMesh::Meshes[0]->MaterialEmission, 4, 0.1f);
				break;
			case SHININESS:
				Boost(PolyMesh::Meshes[0]->MaterialShininess, 1, 1.f);
				break;
			default:
				break;
			}
			break;
		case DOWN:
			switch (MatMode)
			{
			case AMBIENT:
				Boost(PolyMesh::Meshes[0]->MaterialAmbient, 4, -0.1f);
				break;
			case SPECULAR:
				Boost(PolyMesh::Meshes[0]->MaterialSpecular, 4, -0.1f);
				break;
			case DIFFUSE:
				Boost(PolyMesh::Meshes[0]->MaterialDiffuse, 4, -0.1f);
				break;
			case EMISSION:
				Boost(PolyMesh::Meshes[0]->MaterialEmission, 4, -0.1f);
				break;
			case SHININESS:
				Boost(PolyMesh::Meshes[0]->MaterialShininess, 1, -1.f);
				break;
			default:
				break;
			}
			break;
		case SUBDIVIDEP:
			PolyMesh::Meshes[0]->LoopSubdivideP(NUM_THREADS);
			glutPostRedisplay();
			break;
		case JUMP:
			PolyMesh::Meshes[0]->RigidBody->applyImpulse(btVector3(0,500,0),btVector3(0,0,0));
			break;
		case SHADEMODE:
			PolyMesh::Meshes[0]->ShadeMode = PolyMesh::Meshes[0]->ShadeMode == GL_SMOOTH ? GL_FLAT : GL_SMOOTH;
			break;
		case DRAWMODE:
			PolyMesh::Meshes[0]->DrawMode = PolyMesh::Meshes[0]->DrawMode == GL_LINE_LOOP ? GL_TRIANGLES : GL_LINE_LOOP;
			break;
		default:
			break;
		}
	}

	void KeyboardUp(unsigned char key, int x, int y)
	{
		// handle other keyboard controls (teleport n such)
	}

	void SpecialKeyboard(int key, int x, int y)
	{
		switch (key)
		{
		case GLUT_KEY_UP:
			{
				//PolyMesh::Meshes[0]->RigidBody->applyImpulse(Game::Direction*10.f,btVector3(0,0,0));
				Game::moveForward = true;
				break;
			}
		case GLUT_KEY_DOWN:
			{
				//PolyMesh::Meshes[0]->RigidBody->applyImpulse(-Game::Direction*10.f,btVector3(0,0,0));
				Game::moveBackward = true;
				break;
			}
		case GLUT_KEY_LEFT:
			{
				//PolyMesh::Meshes[0]->RigidBody->applyImpulse(Game::Direction.rotate(btVector3(0,1,0),RADIANS(90))*10.f,btVector3(0,0,0));
				Game::moveLeft = true;
				break;
			}
		case GLUT_KEY_RIGHT:
			{
				//PolyMesh::Meshes[0]->RigidBody->applyImpulse(-Game::Direction.rotate(btVector3(0,1,0),RADIANS(90))*10.f,btVector3(0,0,0));
				Game::moveRight = true;
				break;
			}
		default:
			break;
		}
	}

	void SpecialKeyboardUp(int key, int x, int y)
	{
		switch (key)
		{
		case GLUT_KEY_UP:
			{
				//PolyMesh::Meshes[0]->RigidBody->applyImpulse(Game::Direction*10.f,btVector3(0,0,0));
				Game::moveForward = false;
				break;
			}
		case GLUT_KEY_DOWN:
			{
				//PolyMesh::Meshes[0]->RigidBody->applyImpulse(-Game::Direction*10.f,btVector3(0,0,0));
				Game::moveBackward = false;
				break;
			}
		case GLUT_KEY_LEFT:
			{
				//PolyMesh::Meshes[0]->RigidBody->applyImpulse(Game::Direction.rotate(btVector3(0,1,0),RADIANS(90))*10.f,btVector3(0,0,0));
				Game::moveLeft = false;
				break;
			}
		case GLUT_KEY_RIGHT:
			{
				//PolyMesh::Meshes[0]->RigidBody->applyImpulse(-Game::Direction.rotate(btVector3(0,1,0),RADIANS(90))*10.f,btVector3(0,0,0));
				Game::moveRight = false;
				break;
			}
		default:
			break;
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

			/* Simulate Physics */
			Physics::DynamicsWorld->stepSimulation(1.0f/FRAME_RATE, 20, 1.0f/FRAME_RATE);

			for (unsigned int i = 0; i < PolyMesh::Meshes.size(); i++)
				if (PolyMesh::Meshes[i]->Cloth)
					((Cloth *)PolyMesh::Meshes[i])->SimulationStep();

			/* keep character from falling */
			PolyMesh::Meshes[0]->RigidBody->setAngularFactor(0.f);
			PolyMesh::Meshes[0]->RigidBody->setAngularVelocity(btVector3(0.f, 0.f, 0.f));

			/* Character Controls */
			if (Game::moveForward)
					PolyMesh::Meshes[0]->RigidBody->applyImpulse(Game::Direction*10.f,btVector3(0,0,0));
			if (Game::moveBackward)
					PolyMesh::Meshes[0]->RigidBody->applyImpulse(-Game::Direction*10.f,btVector3(0,0,0));
			if (Game::moveLeft)
					PolyMesh::Meshes[0]->RigidBody->applyImpulse(Game::Direction.rotate(btVector3(0,1,0),RADIANS(90))*10.f,btVector3(0,0,0));
			if (Game::moveRight)
					PolyMesh::Meshes[0]->RigidBody->applyImpulse(-Game::Direction.rotate(btVector3(0,1,0),RADIANS(90))*10.f,btVector3(0,0,0));
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
	if (!GLEW_VERSION_2_0)
	{
		std::cerr << "This program requires OpenGL 2.0 or higher." << std::endl;
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

	// Register Reshape Handler
	glutReshapeFunc(Window::Reshape);

	// Register Display Handler
	glutDisplayFunc(Window::Display);

	// Register Keyboard Handler
	glutKeyboardFunc(Window::Keyboard);
	glutKeyboardUpFunc(Window::KeyboardUp);

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

	// Crete render-to-texture target
	Window::texRenderTarget = new TextureRender(Window::Width, Window::Height, GL_RGB);

	// Initialize Physics
	//Physics::InitializePhysics();

	// Load Mesh
	PolyMesh *Mesh = (new PolyMesh())->LoadObj(OBJECT)->GenerateRigidBody()->SetMass(100.0f)->Translate(OpenMesh::Vec3f(0,-0.5f,0));
	Mesh->AttachShader(TOON_SHADER);
	Mesh->RigidBody->setRollingFriction(0.3f);
	Mesh->RigidBody->setActivationState(DISABLE_DEACTIVATION);
	Mesh->RigidBody->setAnisotropicFriction(Mesh->RigidBody->getCollisionShape()->getAnisotropicRollingFrictionDirection(),btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);
	
	Cloth *Cloak = new Cloth(0.001f, 0.0005f, OpenMesh::Vec3f(-1,0,0), OpenMesh::Vec3f(0,0,1), OpenMesh::Vec3f(0.5f,0,0.5f),10,10,1.2f,0.6f,0.1f);
	Cloak->AttachShader(TOON_SHADER);
	Cloak->EnableLighting();
	//Cloak->Pin(9,0,Mesh, Mesh->vertex_handle(3));
	//Cloak->Pin(0,0,Mesh, Mesh->vertex_handle(2));
	cloth_image = bitmap_image("assets\\bmp\\Cloth2.bmp");
	cloth_image.rgb_to_bgr();
	Cloak->ApplyTexture(cloth_image.data(), cloth_image.width(), cloth_image.height());
	
	
	
	Level * one = new Level(1);
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

	Game::Direction = btVector3(0,0,-1);

	// Run Loop
	glutMainLoop();

	return 0;
}