/*************************************************************************************/
//  Program symuluj�cy ruch planet w przestrzeni 3D
/*************************************************************************************/

#define _USE_MATH_DEFINES
#include <windows.h>
#include <gl/gl.h>
#include <gl/GLU.h>
#include <gl/glut.h>
#include <cmath>
#include <iostream>
#include <random>
#include <ctime>
#include <sstream>

typedef GLfloat point3[3];
typedef GLfloat point2[2];
typedef GLfloat color3[3];
typedef GLfloat angle3[3];

#pragma once

class Planet
{
private:
	float minorAxis, majorAxis;
	float area, p, e, angle, ellipseStep;
	angle3 angles, spinSpeed, planeAngles;
	point2 position;

public:
	Planet(float firstAxis, float secondAxis, float startingAngleVelocity, angle3 spinSpeed, float startingAngle = 0.0f)
	{
		if (firstAxis > secondAxis)
		{
			majorAxis = firstAxis;
			minorAxis = secondAxis;
		}
		else
		{
			majorAxis = secondAxis;
			minorAxis = firstAxis;
		}

		ellipseStep = startingAngleVelocity;
		p = pow(minorAxis, 2.0) / majorAxis;
		e = sqrt(1.0 - (pow(minorAxis, 2.0) / pow(majorAxis, 2.0)));
		angle = startingAngle;
		this->spinSpeed[0] = spinSpeed[0];
		this->spinSpeed[1] = spinSpeed[1];
		this->spinSpeed[2] = spinSpeed[2];

		angles[0] = spinSpeed[0];
		angles[1] = spinSpeed[1];
		angles[2] = 0.0;

		planeAngles[0] = 10.0;
		planeAngles[1] = 0.0;
		planeAngles[2] = 10.0;

		position[0] = 30.0;
		position[1] = 21.0;

		float r = p / (1.0 + e * cos(ellipseStep * M_PI / 180.0));
		area = (pow(r, 2.0) * (ellipseStep * M_PI / 180.0)) / 2.0;
	}

	void nextStep()
	{
		if (majorAxis != 0.0 && minorAxis != 0.0)
		{
			float r = p / (1.0 + e * cos(angle * M_PI / 180.0));
			angle += ellipseStep;
			while (angle >= 360.0)
				angle -= 360.0;

			ellipseStep = (area * 2.0) / pow(r, 2.0);

			position[0] = r * cos(angle * M_PI / 180.0);
			position[1] = r * sin(angle * M_PI / 180.0);
		}

		angles[2] += spinSpeed[2];
		while (angles[2] >= 360.0)
			angles[2] -= 360.0;
	}

	void prepareMatrix(bool path = false)
	{
		rotatePlane();
		if (path)
			drawPath();
		translate();
		rotate();
	}

	void setPlane(angle3 angles)
	{
		planeAngles[0] = angles[0];
		planeAngles[1] = angles[1];
		planeAngles[2] = angles[2];
	}

	void translate()
	{
		glTranslatef(position[0], 0.0, position[1]);
	}

	void rotate()
	{
		glRotatef(angles[0], 1.0, 0.0, 0.0);
		glRotatef(angles[1], 0.0, 1.0, 0.0);
		glRotatef(angles[2], 0.0, 0.0, 1.0);
	}

	void rotatePlane()
	{
		glRotatef(planeAngles[0], 1.0, 0.0, 0.0);
		glRotatef(planeAngles[1], 0.0, 1.0, 0.0);
		glRotatef(planeAngles[2], 0.0, 0.0, 1.0);
	}

	void drawPath()
	{
		if (majorAxis != 0.0 && minorAxis != 0.0)
		{
			point2 pos;
			float r;

			for (int eps = 1; eps <= 360; eps++)
			{
				float eps1 = (eps - 1) * M_PI / 180.0, eps2 = eps * M_PI / 180.0;
				glBegin(GL_LINES);

				r = p / (1.0 + e * cos(eps1));
				pos[0] = r * cos(eps1);
				pos[1] = r * sin(eps1);
				glColor3f(1.0, 1.0, 1.0);
				glVertex3d(pos[0], 0.0, pos[1]);
				//printf("x = %g, y = %g\n", pos[0], pos[1]);

				r = p / (1.0 + e * cos(eps2));
				pos[0] = r * cos(eps2);
				pos[1] = r * sin(eps2);
				glColor3f(1.0, 1.0, 1.0);
				glVertex3d(pos[0], 0.0, pos[1]);
				//printf("x = %g, y = %g\n\n", pos[0], pos[1]);

				glEnd();
			}
		}
	}
};

using namespace std;
float dimensions = 250.0f;

clock_t animationTime;
angle3 planeAngles = { 0.0, 0.0, 0.0 };

angle3 solRotation = { 90.0, 0.0, 0.05 };
angle3 solPlane = { 90.0, 0.0, 0.0 };
color3 solColor = { 0.8, 0.8, 0.0 };

angle3 earthRotation = { 120.0, 30.0, 0.05 };
angle3 earthPlane = { 0.0, 0.0, 0.0 };
color3 earthColor = { 0.0, 0.0, 0.8 };

angle3 moonRotation = { 90.0, 0.0, 0.05 };
angle3 moonPlane = { 0.0, 0.0, 15.0 };
color3 moonColor = { 1.0, 1.0, 1.0 };

angle3 marsRotation = { 120.0, 30.0, 0.05 };
angle3 marsPlane = { 0.0, 120.0, 30.0 };
color3 marsColor = { 1.0, 0.0, 0.0 };

angle3 mercuryRotation = { 120.0, 180.0, 0.05 };
angle3 mercuryPlane = { 20.0, 40.0, 10.0 };
color3 mercuryColor = { 1.0, 0.2, 0.0 };

angle3 venusRotation = { 120.0, 30.0, 0.05 };
angle3 venusPlane = { 34.0, 43.0, 22.0 };
color3 venusColor = { 1.0, 0.0, 0.3 };



Planet sol(0.0, 0.0, 0.0, solRotation);
Planet earth(160.0, 160.0, 80.0, earthRotation);
Planet moon(20.0, 20.0, 150.0, moonRotation);
Planet mars(200.0, 200.0, 70.0, marsRotation);
Planet mercury(80.0, 80.0, 60.0, mercuryRotation);
Planet venus(100.0, 100.0, 50.0, venusRotation);

bool animate = true, drawPath = true;
double frameDurationSec = 1.0 / 60.0;

// Funkcja okre�laj�ca co ma by� rysowane (zawsze wywo�ywana gdy trzeba
// przerysowa� scen�)
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz�cym

	glLoadIdentity();
	// Czyszczenie macierzy bie��cej

	// Obracanie p�aszczyzny uk�adu
	glRotatef(planeAngles[0], 1.0, 0.0, 0.0);
	glRotatef(planeAngles[1], 0.0, 1.0, 0.0);
	glRotatef(planeAngles[2], 0.0, 0.0, 1.0);



	// Ziemia START
	glPushMatrix();
	earth.rotatePlane();
	if (drawPath)
		earth.drawPath();
	earth.translate();

	// Ksi�yc START
	glPushMatrix();
	moon.prepareMatrix(drawPath);
	glColor3fv(moonColor);
	glutWireSphere(2.0, 10, 8);
	glPopMatrix();
	// Ksi�yc KONIEC

	earth.rotate();
	glColor3fv(earthColor);
	glutWireSphere(10.0, 20, 16);
	glPopMatrix();
	// Ziemia KONIEC

	// Mars START
	glPushMatrix();
	mars.prepareMatrix(drawPath);
	glColor3fv(marsColor);
	glutWireSphere(10.0, 20, 16);
	glPopMatrix();
	// Mars KONIEC

	// merkury START
	glPushMatrix();
	mercury.prepareMatrix(drawPath);
	glColor3fv(mercuryColor);
	glutWireSphere(5.0, 20, 16);
	glPopMatrix();
	// merkury KONIEC

	// wenus START
	glPushMatrix();
	venus.prepareMatrix(drawPath);
	glColor3fv(venusColor);
	glutWireSphere(13.0, 20, 16);
	glPopMatrix();
	// wenus KONIEC
	// S�o�ce START

	glPushMatrix();
	sol.prepareMatrix(drawPath);
	glColor3fv(solColor);
	glutWireSphere(15.0, 20, 16);
	glPopMatrix();
	// S�o�ce KONIEC

	glutSwapBuffers();
}

/*************************************************************************************/
// Funkcja ustalaj�ca stan renderowania
void MyInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	earth.setPlane(earthPlane);
	moon.setPlane(moonPlane);
	mars.setPlane(marsPlane);
	mercury.setPlane(mercuryPlane);
	venus.setPlane(venusPlane);
	venus.setPlane(venusPlane);
	animationTime = clock();
}

/*************************************************************************************/
// Funkcja ma za zadanie utrzymanie sta�ych proporcji rysowanych
// w przypadku zmiany rozmiar�w okna.
// Parametry vertical i horizontal (wysoko�� i szeroko�� okna) s�
// przekazywane do funkcji za ka�dym razem gdy zmieni si� rozmiar okna.
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	GLfloat AspectRatio;
	if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0
		vertical = 1;

	glViewport(0, 0, horizontal, vertical);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;

	if (horizontal <= vertical)
		glOrtho(-dimensions, dimensions, -dimensions / AspectRatio, dimensions / AspectRatio, dimensions, -dimensions);

	else
		glOrtho(-dimensions * AspectRatio, dimensions * AspectRatio, -dimensions, dimensions, dimensions, -dimensions);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	const float angleStep = 10.0;
	const float fpsChange = 0.01f;
	int number = -1;
	bool add;

	switch (key)
	{
	case ' ':
		animate = !animate;
		break;
	case 'w':
		add = true;
		number = 0;
		break;
	case 's':
		add = false;
		number = 0;
		break;
	case 'a':
		add = false;
		number = 1;
		break;
	case 'd':
		add = true;
		number = 1;
		break;
	case 'q':
		add = true;
		number = 2;
		break;
	case 'e':
		add = false;
		number = 2;
		break;
	case 'r':
		planeAngles[0] = planeAngles[1] = planeAngles[2] = 0.0;
		break;
	case 'p':
		drawPath = !drawPath;
		break;
	default:
		number = -1;
		break;
	}

	// Zmiana nachylenia p�aszczyzny w stosunku do obserwatora
	if (number >= 0 && number < 3)
	{
		if (add)
		{
			if (planeAngles[number] < 360.0)
				planeAngles[number] += angleStep;
			else
				planeAngles[number] = 0.0;
		}
		else
		{
			if (planeAngles[number] > angleStep)
				planeAngles[number] -= angleStep;
			else
				planeAngles[number] = 360.0;
		}
		printf("Plane angles - x = %g, y = %g, z = %g\n", planeAngles[0], planeAngles[1], planeAngles[2]);
	}

	glutPostRedisplay(); //od�wie�enie zawarto�ci aktualnego okna
}

void animation()
{
	double timeInSec = (double(clock()) - double(animationTime)) / double(CLOCKS_PER_SEC);
	const GLfloat step = 0.1f;

	if (animate)
	{
		if (timeInSec > frameDurationSec)
		{
			sol.nextStep();
			earth.nextStep();
			moon.nextStep();
			mars.nextStep();
			venus.nextStep();
			mercury.nextStep();
			animationTime = clock();
			glutPostRedisplay();
		}
	}
}

/*************************************************************************************/
// G��wny punkt wej�cia programu. Program dzia�a w trybie konsoli
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	stringstream title;
	title << "Solar system";
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow(title.str().c_str());

	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutIdleFunc(animation);

	glutKeyboardFunc(keyboard);

	MyInit();

	glEnable(GL_DEPTH_TEST);
	glutMainLoop();

}
/*************************************************************************************/