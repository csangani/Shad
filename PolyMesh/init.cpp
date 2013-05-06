#include <PolyMesh/init.h>
#include <PolyMesh/bitmap_image.h>
#include <PolyMesh/PolyMesh.h>

#include <GL/freeglut.h>

namespace Window
{
	std::string Title = "CS 248 Assignment 2";
	int Width = 600, Height = 480;

	void Reshape(int width, int height)
	{
		Width = width;
		Height = height;

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		/*gluPerspective(45,((float)width)/height,0.1f,100.f);
		gluLookAt(3,3,3,0,0,0,0,1,0);
		*/
		glOrtho(-1,1,-1,1,-1,1);
		
		glViewport(0,0,width,height);

		glutPostRedisplay();
	}

	void _display(PolyMesh *mesh)
	{
		mesh->Draw();
	}

	void Display(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		std::for_each(PolyMesh::Meshes.begin(), PolyMesh::Meshes.end(), _display);

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
			PolyMesh::Meshes[0]->LoopSubdivideP();
			glutPostRedisplay();
			break;
		case SUBDIVIDE:
			PolyMesh::Meshes[0]->LoopSubdivide();
			glutPostRedisplay();
			break;
		case SHADEMODE:
			PolyMesh::Meshes[0]->ShadeMode = PolyMesh::Meshes[0]->ShadeMode == GL_SMOOTH ? GL_FLAT : GL_SMOOTH;
			break;
		case DRAWMODE:
			PolyMesh::Meshes[0]->DrawMode = PolyMesh::Meshes[0]->DrawMode == GL_LINE_LOOP ? GL_TRIANGLES : GL_LINE_LOOP;
			break;
		case RESET:
			{
				// Load Mesh
				PolyMesh *Mesh = new PolyMesh(OBJECT, GL_TRIANGLES, GL_SMOOTH);

				// Set Mesh Size and Location
				Mesh->Center()->Normalize();

				// Set Mesh and Plane Material Parameters
				Mesh->MaterialSpecular = Specular;

				Mesh->MaterialDiffuse = Diffuse;

				Mesh->MaterialAmbient = Ambient;

				Mesh->MaterialShininess = Shininess;

				// Apply Texture to Mesh
				image = bitmap_image("D:\\SkyDrive\\Documents\\Workspace\\earth.bmp");
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

	void Timer(int time)
	{
		PolyMesh::Time += (uint64_t) FRAME_PERIOD;

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

	// Register Reshape Handler
	glutReshapeFunc(Window::Reshape);

	// Register Display Handler
	glutDisplayFunc(Window::Display);

	// Register Keyboard Handler
	glutKeyboardFunc(Window::Keyboard);

	// Register Timer Handler
	glutTimerFunc((int) FRAME_PERIOD, Window::Timer, (int) FRAME_PERIOD);

	// Load Mesh
	PolyMesh *Mesh = new PolyMesh(OBJECT, GL_TRIANGLES, GL_SMOOTH);

	// Load and set up environment sphere
	PolyMesh *EnvSphere = new PolyMesh("D:\\SkyDrive\\Documents\\Workspace\\sphere.obj", GL_TRIANGLES, GL_SMOOTH);
	EnvSphere->Center()->Normalize()->Scale(12,12,12)->Rotate(90,0,1,0);
	space_image = bitmap_image("D:\\SkyDrive\\Documents\\Workspace\\space.bmp");
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
	image = bitmap_image("D:\\SkyDrive\\Documents\\Workspace\\Earth.bmp");
	image.rgb_to_bgr();
	Mesh->ApplyTexture(image.data(), image.width(), image.height());

	// Setup Lights
	GLfloat LightPosition[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

	GLfloat LightDirection[] = { -1.0f, -1.0f, -1.0f };
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, LightDirection);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 60.0f);

	GLfloat LightAmbient[] = { 0.6f ,0.6f ,0.6f , 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);

	GLfloat LightDiffuse[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);

	GLfloat LightSpecular[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, LightSpecular);

	glEnable(GL_LIGHT0);

	Mesh->EnableLighting();
	
	// Enable Normalization
	glEnable(GL_NORMALIZE);

	// Enable Depth Test
	glEnable(GL_DEPTH_TEST);

	// Run Loop
	glutMainLoop();

	return 0;
}