#include <Shad/init.h>
#include <Shad/Camera.h>

#include <PolyMesh/bitmap_image.h>
#include <PolyMesh/PolyMesh.h>

#include <ctime>

#include <GL/glew.h>
#include <GL/freeglut.h>

namespace Game
{
	btVector3 Direction;
}

namespace Window
{
	std::string Title = "Shad";
	int Width = 600, Height = 480;

	Game::Camera* Camera;

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
		/* Draw objects */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		std::for_each(PolyMesh::Meshes.begin(), PolyMesh::Meshes.end(), _display);

		/* Set camera position and direction */
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(45,((float)Window::Width)/Window::Height,0.1f,100.f);

		btTransform transform = PolyMesh::Meshes[0]->RigidBody->getCenterOfMassTransform();
		Camera->UpdatePosition(OpenMesh::Vec3f(transform.getOrigin().getX(), transform.getOrigin().getY(), transform.getOrigin().getZ()));

		gluLookAt(Camera->Position()[0],Camera->Position()[1]+1.0f,Camera->Position()[2],transform.getOrigin().getX(),transform.getOrigin().getY(),transform.getOrigin().getZ(), 0, 1, 0);

		glViewport(0,0,Window::Width,Window::Height);

		glutSwapBuffers();
	}

	void Keyboard(unsigned char key, int x, int y) {
		static int MatMode = 0;
		switch (key)
		{
		case 27: 
			glutLeaveMainLoop();
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
		case SUBDIVIDE:
			PolyMesh::Meshes[0]->RigidBody->applyImpulse(btVector3(0,5,0),btVector3(0,0,0));
			break;
		case SHADEMODE:
			PolyMesh::Meshes[0]->ShadeMode = PolyMesh::Meshes[0]->ShadeMode == GL_SMOOTH ? GL_FLAT : GL_SMOOTH;
			break;
		case DRAWMODE:
			PolyMesh::Meshes[0]->DrawMode = PolyMesh::Meshes[0]->DrawMode == GL_LINE_LOOP ? GL_TRIANGLES : GL_LINE_LOOP;
			break;
		case RESET:
			{
				PolyMesh::Meshes[0]->Delete();
				// Load Mesh
				PolyMesh *Mesh = new PolyMesh(OBJECT, GL_TRIANGLES, GL_SMOOTH);
				//Mesh->AttachShader(PHONG_SHADER);

				// Set Mesh and Plane Material Parameters
				Mesh->MaterialSpecular = Specular;

				Mesh->MaterialDiffuse = Diffuse;

				Mesh->MaterialAmbient = Ambient;

				Mesh->MaterialShininess = Shininess;

				// Apply Texture to Mesh
				Mesh->ApplyTexture(image.data(), image.width(), image.height());

				Mesh->EnableLighting();
				PolyMesh::Meshes[0] = Mesh;
				PolyMesh::Meshes.pop_back();
				break;
			}
		default:
			break;
		}
	}

	void SpecialKeyboard(int key, int x, int y)
	{
		switch (key)
		{
		case GLUT_KEY_UP:
			{
				PolyMesh::Meshes[0]->RigidBody->applyImpulse(Game::Direction + btVector3(0,0.05f,0),btVector3(0,0,0));
				break;
			}
		case GLUT_KEY_DOWN:
			{
				PolyMesh::Meshes[0]->RigidBody->applyImpulse(-Game::Direction + btVector3(0,0.05f,0),btVector3(0,0,0));
				break;
			}
		case GLUT_KEY_LEFT:
			{
				Game::Direction = Game::Direction.rotate(btVector3(0,1,0),RADIANS(90));
				PolyMesh::Meshes[0]->Translate(OpenMesh::Vec3f(0,0.05f,0))->Rotate(90, 0,1,0);
				break;
			}
		case GLUT_KEY_RIGHT:
			{
				Game::Direction = Game::Direction.rotate(btVector3(0,1,0),RADIANS(-90));
				PolyMesh::Meshes[0]->Translate(OpenMesh::Vec3f(0,0.05f,0))->Rotate(-90, 0,1,0);
				break;
			}
		default:
			break;
		}
	}

	void Timer(int time)
	{
		PolyMesh::Time += (uint64_t) FRAME_PERIOD;

		/* Simulate Physics */
		Physics::DynamicsWorld->stepSimulation(1.0f/FRAME_RATE, 10);
		
		glutTimerFunc((int) FRAME_PERIOD, Timer, (int) FRAME_PERIOD);

		glutPostRedisplay();
	};
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
	//glutFullScreen();

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

	// Register Reshape Handler
	glutReshapeFunc(Window::Reshape);

	// Register Display Handler
	glutDisplayFunc(Window::Display);

	// Register Keyboard Handler
	glutKeyboardFunc(Window::Keyboard);

	// Register Special Keyboard Handler
	glutSpecialFunc(Window::SpecialKeyboard);

	// Register Timer Handler
	glutTimerFunc((int) FRAME_PERIOD, Window::Timer, (int) FRAME_PERIOD);

	// Enable Normalization
	glEnable(GL_NORMALIZE);

	// Enable Depth Test
	glEnable(GL_DEPTH_TEST);

	// Create Camera
	Window::Camera = new Game::Camera();

	// Initialize Physics
	Physics::InitializePhysics();

	// Load Mesh
	PolyMesh *Mesh = new PolyMesh(OBJECT, GL_TRIANGLES, GL_SMOOTH);
	//Mesh->AttachShader(PHONG_SHADER);

	// Set Mesh Size and Location
	Mesh->SetMass(1.0f);
	Mesh->Translate(OpenMesh::Vec3f(0,1,0));
	PolyMesh *Plane = new PolyMesh("assets\\obj\\plane.obj", GL_TRIANGLES, GL_SMOOTH);
	Plane->Scale(OpenMesh::Vec3f(1000,1000,1000))->Translate(OpenMesh::Vec3f(0,-1,0));
	space_image = bitmap_image("assets\\bmp\\checkerboard.bmp");
	space_image.rgb_to_bgr();
	Plane->ApplyTexture(space_image.data(), space_image.width(), space_image.height());
	// Load and set up environment sphere

	// Apply Animation
	//AnimationRoutine *routine = new AnimationRoutine("D:\\SkyDrive\\Documents\\Workspace\\obj\\routine.ani");
	//Mesh->Animate(routine, 0, true);


	// Set Mesh and Plane Material Parameters
	Mesh->MaterialSpecular = Specular;

	Mesh->MaterialDiffuse = Diffuse;

	Mesh->MaterialAmbient = Ambient;

	Mesh->MaterialShininess = Shininess;

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

	// Set ship speed and direction
	Game::Direction = btVector3(0,0,-1);

	// Run Loop
	glutMainLoop();

	return 0;
}