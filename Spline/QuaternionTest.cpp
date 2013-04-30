#include <Spline.hh>
#include <Quaternion.h>

#include <string>

#include <GL/glut.h>


namespace QuaternionTestWindow
{
	std::string Title = "CS 248 Assignment 2";
	int Width = 600, Height = 600;
	Spline<Quaternion> *spline;

	void Reshape(int width, int height)
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(-1,1,-1,1,-0.5,0.5);
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
			Quaternion Q = (*spline)(seg);
			float factor = (spline->end() - (float)seg)/(spline->end() - spline->begin());
			glVertex3f(factor*Q.x(), factor*Q.y(), factor*Q.z());
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

int main (int argc, char **argv)
{
	// Initialize GL
	glutInit(&argc, argv);

	// Initialize Display Mode
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);

	// Initialize QuaternionTestWindow Size Parameters
	glutInitWindowSize(QuaternionTestWindow::Width, QuaternionTestWindow::Height);

	// Create QuaternionTestWindow
	glutCreateWindow(QuaternionTestWindow::Title.c_str());

	// Register Reshape Handler
	glutReshapeFunc(QuaternionTestWindow::Reshape);

	// Register Display Handler
	glutDisplayFunc(QuaternionTestWindow::Display);

	// Register Keyboard Handler
	glutKeyboardFunc(QuaternionTestWindow::Keyboard);

	// Create spline
	QuaternionTestWindow::spline = new Spline<Quaternion>;

	// Add control points
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,1,0.0,0.0),0);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,1,0),100);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,-1,0,0),200);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,-1,0),300);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,1,0.0,0.0),400);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,1,0),500);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,-1,0,0),600);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,-1,0),700);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,1,0.0,0.0),800);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,1,0),900);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,-1,0,0),1000);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,-1,0),1100);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,1,0.0,0.0),1200);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,1,0),1300);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,-1,0,0),1400);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,-1,0),1500);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,1,0.0,0.0),1600);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,1,0),1700);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,-1,0,0),1800);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,-1,0),1900);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,1,0.0,0.0),2000);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,1,0),2100);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,-1,0,0),2200);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,-1,0),2300);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,1,0.0,0.0),2400);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,1,0),2500);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,-1,0,0),2600);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,-1,0),2700);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,1,0.0,0.0),2800);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,1,0),2900);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,-1,0,0),3000);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,-1,0),3100);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,1,0.0,0.0),3200);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,1,0),3300);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,-1,0,0),3400);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,-1,0),3500);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,1,0.0,0.0),3600);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,1,0),3700);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,-1,0,0),3800);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,-1,0),3900);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,1,0.0,0.0),4000);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,1,0),4100);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,-1,0,0),4200);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,-1,0),4300);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,1,0.0,0.0),4400);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,1,0),4500);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,-1,0,0),4600);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,-1,0),4700);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,1,0.0,0.0),4800);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,1,0),4900);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,-1,0,0),5000);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,-1,0),5100);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,1,0.0,0.0),5200);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,1,0),5300);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,-1,0,0),5400);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,-1,0),5500);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,1,0.0,0.0),5600);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,1,0),5700);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,-1,0,0),5800);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,-1,0),5900);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,1,0.0,0.0),6000);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,1,0),6100);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,-1,0,0),6200);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,-1,0),6300);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,1,0.0,0.0),6400);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,1,0),6500);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,-1,0,0),6600);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,-1,0),6700);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,1,0.0,0.0),6800);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,1,0),6900);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,-1,0,0),7000);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,-1,0),7100);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,1,0.0,0.0),7200);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,1,0),7300);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,-1,0,0),7400);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,-1,0),7500);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,1,0.0,0.0),7600);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,1,0),7700);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,-1,0,0),7800);
	QuaternionTestWindow::spline->AddControlPoint(Quaternion(0.0,0,-1,0),7900);

	// Run Loop
	glutMainLoop();

	return 0;
}