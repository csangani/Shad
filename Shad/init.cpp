#include <Shad/init.h>
#include <Shad/Camera.h>

#include <PolyMesh/bitmap_image.h>
#include <PolyMesh/PolyMesh.h>

#include <ctime>

#include <GL/glew.h>
#include <GL/freeglut.h>

namespace Game
{
	float Speed;
	OpenMesh::Vec3f Direction;
	OpenMesh::Vec3f Up;
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

		Camera->UpdatePosition(OpenMesh::Vec3f(PolyMesh::Meshes[0]->cen_x, PolyMesh::Meshes[0]->cen_y, PolyMesh::Meshes[0]->cen_z));

		gluLookAt(Camera->Position()[0],Camera->Position()[1],Camera->Position()[2],PolyMesh::Meshes[0]->cen_x,PolyMesh::Meshes[0]->cen_y,PolyMesh::Meshes[0]->cen_z, 0, 1, 0);

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
			for (int i = 3; i <= 26; i++)
			{
				PolyMesh::Meshes[0]->LoopSubdivideP(i/3)->LoopSubdivideP(i/3)->LoopSubdivideP(i/3)->LoopSubdivideP(i/3);
				clock_t start, end;
				start = clock();
				PolyMesh::Meshes[0]->LoopSubdivideP(i/3);
				end = clock();
				std::cout << i/3 << " threads: " << end - start << " clicks\n";
				Keyboard(RESET, 0, 0);
			}
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

				// Set Mesh Size and Location
				Mesh->Center()->Normalize();

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
				Quaternion QD = Quaternion(0.0f, Game::Direction[0], Game::Direction[1], Game::Direction[2]);
				Quaternion QU = Quaternion(0.0f, Game::Up[0], Game::Up[1], Game::Up[2]);
				OpenMesh::Vec3f D = OpenMesh::Vec3f(Game::Direction[0], Game::Direction[1], Game::Direction[2]);
				OpenMesh::Vec3f U = OpenMesh::Vec3f(Game::Up[0], Game::Up[1], Game::Up[2]);
				float halfAngle = ((float)M_PI)*5.0f/360.f;
				OpenMesh::Vec3f Axis = D % U;
				QD = QD.Rotate(Quaternion(cosf(halfAngle), sinf(halfAngle)*Axis[0], sinf(halfAngle)*Axis[1], sinf(halfAngle)*Axis[2]));
				QU = QU.Rotate(Quaternion(cosf(halfAngle), sinf(halfAngle)*Axis[0], sinf(halfAngle)*Axis[1], sinf(halfAngle)*Axis[2]));
				Game::Direction = OpenMesh::Vec3f(QD.x(), QD.y(), QD.z());
				Game::Up = OpenMesh::Vec3f(QU.x(), QU.y(), QU.z());
				PolyMesh::Meshes[0]->RotateAboutCenter(5.0f, Axis[0], Axis[1], Axis[2]);
				break;
			}
		case GLUT_KEY_DOWN:
			{
				Quaternion QD = Quaternion(0.0f, Game::Direction[0], Game::Direction[1], Game::Direction[2]);
				Quaternion QU = Quaternion(0.0f, Game::Up[0], Game::Up[1], Game::Up[2]);
				OpenMesh::Vec3f D = OpenMesh::Vec3f(Game::Direction[0], Game::Direction[1], Game::Direction[2]);
				OpenMesh::Vec3f U = OpenMesh::Vec3f(Game::Up[0], Game::Up[1], Game::Up[2]);
				float halfAngle = ((float)M_PI)*5.0f/360.f;
				OpenMesh::Vec3f Axis = U % D;
				QD = QD.Rotate(Quaternion(cosf(halfAngle), sinf(halfAngle)*Axis[0], sinf(halfAngle)*Axis[1], sinf(halfAngle)*Axis[2]));
				QU = QU.Rotate(Quaternion(cosf(halfAngle), sinf(halfAngle)*Axis[0], sinf(halfAngle)*Axis[1], sinf(halfAngle)*Axis[2]));
				Game::Direction = OpenMesh::Vec3f(QD.x(), QD.y(), QD.z());
				Game::Up = OpenMesh::Vec3f(QU.x(), QU.y(), QU.z());
				PolyMesh::Meshes[0]->RotateAboutCenter(5.0f, Axis[0], Axis[1], Axis[2]);
				break;
			}
		case GLUT_KEY_LEFT:
			{
				Quaternion Q = Quaternion(0.0f, Game::Direction[0], Game::Direction[1], Game::Direction[2]);
				float halfAngle = ((float)M_PI)*5.0f/360.f;
				Q = Q.Rotate(Quaternion(cosf(halfAngle), sinf(halfAngle)*Game::Up[0], sinf(halfAngle)*Game::Up[1], sinf(halfAngle)*Game::Up[2]));
				Game::Direction = OpenMesh::Vec3f(Q.x(), Q.y(), Q.z());
				PolyMesh::Meshes[0]->RotateAboutCenter(5.0f, Game::Up[0], Game::Up[1], Game::Up[2]);
				break;
			}
		case GLUT_KEY_RIGHT:
			{
				Quaternion Q = Quaternion(0.0f, Game::Direction[0], Game::Direction[1], Game::Direction[2]);
				float halfAngle = -((float)M_PI)*5.0f/360.f;
				Q = Q.Rotate(Quaternion(cosf(halfAngle), sinf(halfAngle)*Game::Up[0], sinf(halfAngle)*Game::Up[1], sinf(halfAngle)*Game::Up[2]));
				Game::Direction = OpenMesh::Vec3f(Q.x(), Q.y(), Q.z());
				PolyMesh::Meshes[0]->RotateAboutCenter(-5.0f, Game::Up[0], Game::Up[1], Game::Up[2]);
				break;
			}
		default:
			break;
		}
	}

	void MouseWheel(int wheel, int direction, int x, int y)
	{
		switch (direction)
		{
		case 1:
			if (Game::Speed < 5.0f)
				Game::Speed++;
			break;
		case -1:
			if (Game::Speed > -5.0f)
				Game::Speed--;
			break;
		default:
			break;
		}
	}

	void Timer(int time)
	{
		PolyMesh::Time += (uint64_t) FRAME_PERIOD;

		/* Move Ship */
		OpenMesh::Vec3f Delta = Game::Direction.normalized();
		Delta = Delta * (Game::Speed / FRAME_RATE);
		PolyMesh::Meshes[0]->Translate(Delta[0], Delta[1], Delta[2]);
		//PolyMesh::Meshes[1]->Translate(Delta[0], Delta[1], Delta[2]);

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

	// Register Mousewheel Handler
	glutMouseWheelFunc(Window::MouseWheel);

	// Register Timer Handler
	glutTimerFunc((int) FRAME_PERIOD, Window::Timer, (int) FRAME_PERIOD);

	// Enable Normalization
	glEnable(GL_NORMALIZE);

	// Enable Depth Test
	glEnable(GL_DEPTH_TEST);

	// Create Camera
	Window::Camera = new Game::Camera();

	// Load Mesh
	PolyMesh *Mesh = new PolyMesh(OBJECT, GL_TRIANGLES, GL_SMOOTH);
	//Mesh->AttachShader(PHONG_SHADER);

	// Load and set up environment sphere
	PolyMesh *EnvSphere = new PolyMesh("assets\\obj\\sphere.obj", GL_TRIANGLES, GL_SMOOTH);
	EnvSphere->Center()->Normalize()->Scale(50,50,50)->Rotate(90,0,1,0);
	space_image = bitmap_image("assets\\bmp\\space.bmp");
	space_image.rgb_to_bgr();
	EnvSphere->ApplyTexture(space_image.data(), space_image.width(), space_image.height());

	// Apply Animation
	//AnimationRoutine *routine = new AnimationRoutine("D:\\SkyDrive\\Documents\\Workspace\\obj\\routine.ani");
	//Mesh->Animate(routine, 0, true);

	// Set Mesh Size and Location
	Mesh->Center()->Normalize();

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
	Game::Speed = 0.0f;
	Game::Direction = OpenMesh::Vec3f(0.0f, 0.0f, -1.0f);
	Game::Up = OpenMesh::Vec3f(0.0f, 1.0f, 0.0f);

	// Run Loop
	glutMainLoop();

	return 0;
}