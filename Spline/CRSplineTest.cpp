#include <Spline.hh>
#include <CatmullRom.h>

#include <string>

#include <GL/glut.h>


namespace SplineTestWindow
{
	std::string Title = "CS 248 Assignment 2";
	int Width = 600, Height = 600;
	Spline<CatmullRom> *spline;

	void Reshape(int width, int height)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(0,1,0,1,-0.5,0.5);
		glViewport(height < width ? (width-height)/2 : 0, width < height ? (height-width)/2 : 0, std::min(width,height), std::min(width,height));

		Width = width;
		Height = height;

		glutPostRedisplay();
	}

	void Display(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor3f(1.0f,0.0f,1.0f);
		glBegin(GL_POINTS);
		for (uint64_t seg = spline->begin(); seg != spline->end(); seg++)
		{
			CatmullRom CR = (*spline)(seg);
			glVertex3f(CR.Vec3f()[0],CR.Vec3f()[1],CR.Vec3f()[2]);
		}
		glEnd();
		glutSwapBuffers();
	}

	void Keyboard(unsigned char key, int x, int y) {
		switch (key)
		{
		case 27: 
			exit(0);	
			break;
		default:
			break;
		}
	}

}

int main_ (int argc, char **argv)
{
	// Initialize GL
	glutInit(&argc, argv);

	// Initialize Display Mode
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	// Initialize SplineTestWindow Size Parameters
	glutInitWindowSize(SplineTestWindow::Width, SplineTestWindow::Height);

	// Create SplineTestWindow
	glutCreateWindow(SplineTestWindow::Title.c_str());

	// Register Reshape Handler
	glutReshapeFunc(SplineTestWindow::Reshape);

	// Register Display Handler
	glutDisplayFunc(SplineTestWindow::Display);

	// Register Keyboard Handler
	glutKeyboardFunc(SplineTestWindow::Keyboard);

	// Create spline
	SplineTestWindow::spline = new Spline<CatmullRom>;

	// Add control points
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.0,0.5,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.0,0),100);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.975,0.5,0),200);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.975,0),300);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.025,0.5,0),400);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.025,0),500);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.950,0.5,0),600);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.950,0),700);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.05,0.5,0),800);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.05,0),900);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.925,0.5,0),1000);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.925,0),1100);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.075,0.5,0),1200);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.075,0),1300);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.900,0.5,0),1400);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.900,0),1500);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.1,0.5,0),1600);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.1,0),1605);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.875,0.5,0),1800);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.875,0),1900);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.125,0.5,0),2000);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.125,0),2100);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.850,0.5,0),2200);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.850,0),2300);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.15,0.5,0),2400);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.15,0),2500);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.825,0.5,0),2600);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.825,0),2700);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.175,0.5,0),2800);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.175,0),2900);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.800,0.5,0),3000);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.800,0),3010);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.2,0.5,0),3200);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.2,0),3300);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.775,0.5,0),3400);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.775,0),5600);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.225,0.5,0),5700);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.225,0),5800);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.750,0.5,0),5900);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.750,0),6000);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.25,0.5,0),7000);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.25,0),7100);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.725,0.5,0),7200);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.725,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.275,0.5,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.275,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.700,0.5,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.700,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.3,0.5,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.3,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.675,0.5,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.675,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.325,0.5,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.325,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.650,0.5,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.650,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.35,0.5,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.35,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.625,0.5,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.625,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.375,0.5,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.375,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.600,0.5,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.600,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.4,0.5,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.4,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.575,0.5,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.575,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.425,0.5,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.425,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.550,0.5,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.550,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.45,0.5,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.45,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.525,0.5,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.525,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.475,0.5,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.475,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.500,0.5,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.500,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.5,0),0);
	SplineTestWindow::spline->AddControlPoint(OpenMesh::Vec3f(0.5,0.5,0),0);

	// Run Loop
	glutMainLoop();

	return 0;
}