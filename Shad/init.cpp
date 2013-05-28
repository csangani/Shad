#include <Shad/init.h>
#include <Shad/Camera.h>

#include <PolyMesh/bitmap_image.h>
#include <PolyMesh/PolyMesh.h>
#include <PolyMesh/Cloth.h>

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

		//gluLookAt(2.f, 2.f, 2.f,0,-1,0, 0, 1, 0);

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
				PolyMesh::Meshes[0]->RigidBody->applyImpulse(Game::Direction*10.f,btVector3(0,0,0));
				break;
			}
		case GLUT_KEY_DOWN:
			{
				PolyMesh::Meshes[0]->RigidBody->applyImpulse(-Game::Direction*10.f,btVector3(0,0,0));
				break;
			}
		case GLUT_KEY_LEFT:
			{
				PolyMesh::Meshes[0]->RigidBody->applyImpulse(Game::Direction.rotate(btVector3(0,1,0),RADIANS(90))*10.f,btVector3(0,0,0));
				break;
			}
		case GLUT_KEY_RIGHT:
			{
				PolyMesh::Meshes[0]->RigidBody->applyImpulse(-Game::Direction.rotate(btVector3(0,1,0),RADIANS(90))*10.f,btVector3(0,0,0));
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
		Physics::DynamicsWorld->stepSimulation(1.0f/FRAME_RATE, 20, 1.0f/FRAME_RATE);
		
		glutTimerFunc((int) FRAME_PERIOD, Timer, (int) FRAME_PERIOD);

		glutPostRedisplay();

		for (unsigned int i = 0; i < PolyMesh::Meshes.size(); i++)
			if (PolyMesh::Meshes[i]->Cloth)
				((Cloth *)PolyMesh::Meshes[i])->SimulationStep();
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
	glutFullScreen();

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
	PolyMesh *Mesh = (new PolyMesh())->LoadObj(OBJECT)->GenerateRigidBody()->SetMass(100.0f)->Translate(OpenMesh::Vec3f(0,-0.5f,0));
	Mesh->AttachShader(TOON_SHADER);
	Mesh->RigidBody->setRollingFriction(0.3f);
	Mesh->RigidBody->setActivationState(DISABLE_DEACTIVATION);
	Mesh->RigidBody->setAnisotropicFriction(Mesh->RigidBody->getCollisionShape()->getAnisotropicRollingFrictionDirection(),btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);
	
	Cloth *Cloak = new Cloth(0.001f, 0.0005f, OpenMesh::Vec3f(-1,0,0), OpenMesh::Vec3f(0,0,1), OpenMesh::Vec3f(0.5f,0,0.5f),10,10,1.2f,0.6f,0.1f);
	Cloak->AttachShader(TOON_SHADER);
	Cloak->EnableLighting();
	Cloak->Pin(9,0,Mesh, Mesh->vertex_handle(3));
	Cloak->Pin(0,0,Mesh, Mesh->vertex_handle(2));
	cloth_image = bitmap_image("assets\\bmp\\Cloth2.bmp");
	cloth_image.rgb_to_bgr();
	Cloak->ApplyTexture(cloth_image.data(), cloth_image.width(), cloth_image.height());

	PolyMesh *Plane = (new PolyMesh())->LoadObj("assets\\obj\\plane.obj")->GenerateRigidBody()->Scale(OpenMesh::Vec3f(1000,1000,1000))->Translate(OpenMesh::Vec3f(0,-1,0));
	Plane->AttachShader(TOON_SHADER);
	Plane->RigidBody->setRollingFriction(0.3f);
	Plane->RigidBody->setAnisotropicFriction(Plane->RigidBody->getCollisionShape()->getAnisotropicRollingFrictionDirection(),btCollisionObject::CF_ANISOTROPIC_ROLLING_FRICTION);

	space_image = bitmap_image("assets\\bmp\\checkerboard.bmp");
	space_image.rgb_to_bgr();
	Plane->ApplyTexture(space_image.data(), space_image.width(), space_image.height());
	
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