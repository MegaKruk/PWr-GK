#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <gl/glu.h>
//#include <gl/openglut.h>
#define pi 3.141592
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>
#include <stdlib.h>

typedef float point3[3];
//static GLfloat viewer[]= {0.0, 0.0, 10.0};
static GLfloat theta = 0.0;
static GLfloat theta1 = 0.0;
static GLfloat theta2 = 0.0;
static GLfloat theta3 = 0.0;
// k?t obrotu obiektu
static GLfloat pix2angle;     // przelicznik pikseli na stopnie
static GLfloat pix2angle1;
static GLint status = 0;      // stan klawiszy myszy
static GLint status1 = 0;                              // 0 - nie naci?ni?to ?adnego klawisza
#define N 25										   // 1 - naci?ni?ty zosta? lewy klawisz
int stan =1;
static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old = 0;
static int delta_x = 0;       // ró?nica pomi?dzy pozycj? bie??c?
static int delta_y = 0;                               // i poprzedni? kursora myszy
static GLfloat x = 0, y = 0, z = 0;  //wybór osi obrotu
static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
static GLfloat zoom = 10.0;
static int delta_x1 = 0;       // ró?nica pomi?dzy pozycj? bie??c?
static int delta_y1 = 0;
static GLfloat viewer1[] = { 0.0, 0.0, 10.0 };

// inicjalizacja po?o?enia obserwatora
/*************************************************************************************/
// Funkcja rysuj?ca osie uk?adu wspó?rz?dnych

class Kolor {
public:
	float r, g, b;
	Kolor() {
		r = 0.0;
		g = 0.0;
		b = 0.0;
		r = (1.0*(rand() % 100)) / 100;
		g = (1.0*(rand() % 100)) / 100;
		b = (1.0*(rand() % 100)) / 100;
	}
	float getR() { return r; }
	float getG() { return g; }
	float getB() { return b; }
};
Kolor color[20][20];


void CretateEgg(Kolor color[][20])
{
	//int N = 25;
	float x;
	x = 0.533 / N;
	float tabx[N][N];
	float taby[N][N];
	//  float theta=0.0;
	for (int i = 0; i<N; i++)
	{
		for (int k = 0; k<N; k++)
		{
			tabx[i][k] = 0.0000000000000000000000000000000000001 + (x*k);
			taby[i][k] = 0.00000000000000000000000000000000000001 + (x*i);
			//  glBegin(GL_POINTS); // rysowanie osi x
			//  glVertex2f(tabx[i][k],taby[i][k]);
		}
	}

	float pktx[N][N], pkty[N][N], pktz[N][N];
	float u;
	float v;
	float x_x[N][N], x_v[N][N], y_x[N][N], y_v[N][N], z_x[N][N], z_v[N][N];
	float normalnyx[N][N], normalnyy[N][N], normalnyz[N][N];
	float dlugosc;
	for (int m = 0; m<N; m++)
	{
		for (int l = 0; l<N; l++)
		{
			u = tabx[m][l];
			v = taby[m][l];


			pktx[m][l] = ((-9 * pow(u, 5)) + (22.5*pow(u, 4)) - (27 * pow(u, 3)) + (18 * pow(u, 2)) - (4.5*u))*(cos(v*pi));
			pkty[m][l] = ((16 * pow(u, 4)) - (32 * pow(u, 3)) + (16 * pow(u, 2)));
			pktz[m][l] = ((-9 * pow(u, 5)) + (22.5*pow(u, 4)) - (27 * pow(u, 3)) + (18 * pow(u, 2)) - (4.5*u))*(sin(v*pi));

			x_x[m][l] = ((-450 * pow(u, 4)) + (900 * pow(u, 3)) - (810 * pow(u, 2)) + (360 * pow(u, 1)) - (45))*(cos(v*pi));
			x_v[m][l] = ((90 * pow(u, 5)) + (-225 * pow(u, 4)) - (-270 * pow(u, 3)) + (-180 * pow(u, 2)) - (-45 * u))*((sin(v*pi)))*pi;
			y_x[m][l] = ((640 * pow(u, 3)) - (960 * pow(u, 2)) + (320 * pow(u, 1)));
			y_v[m][l] = 0;
			z_x[m][l] = ((-450 * pow(u, 4)) + (900 * pow(u, 3)) - (810 * pow(u, 2)) + (360 * pow(u, 1)) - (45))*(sin(v*pi));
			z_v[m][l] = ((-90 * pow(u, 5)) + (225 * pow(u, 4)) - (270 * pow(u, 3)) + (180 * pow(u, 2)) - (45 * u))*(cos(v*pi))*pi;


			normalnyx[m][l] = y_x[m][l] * z_v[m][l] - z_x[m][l] * y_v[m][l];
			normalnyy[m][l] = z_x[m][l] * x_v[m][l] - x_x[m][l] * z_v[m][l];
			normalnyz[m][l] = x_x[m][l] * y_v[m][l] - y_x[m][l] * x_v[m][l];

			dlugosc = 1.0*sqrt((pow(normalnyx[m][l], 2)) + (pow(normalnyy[m][l], 2)) + pow(normalnyz[m][l], 2));

			normalnyx[m][l] = 1.0*normalnyx[m][l] / dlugosc;
			normalnyy[m][l] = 1.0*normalnyy[m][l] / dlugosc + 0.5;
			normalnyz[m][l] = 1.0*normalnyz[m][l] / dlugosc;



		}
	}

	for (int m = 0; m<N - 1; m++)
	{
		for (int l = 0; l<N - 1; l++)



		{

			glBegin(GL_TRIANGLES);
			//   glColor3f(color[m][l].getR(),color[m][l].getG(),color[m][l].getB());
			glNormal3f(normalnyx[m][l], normalnyy[m][l] - 0.5, normalnyz[m][l]);
			glVertex3f(pktx[m][l], pkty[m][l] - 0.5, pktz[m][l]);
			//  glColor3f(color[m+1][l].getR(),color[m+1][l].getG(),color[m+1][l].getB());
			glNormal3f(normalnyx[m + 1][l], normalnyy[m + 1][l] - 0.5, normalnyz[m + 1][l]);
			glVertex3f(pktx[m + 1][l], pkty[m + 1][l] - 0.5, pktz[m + 1][l]);
			//   glColor3f(color[m+1][l+1].getR(),color[m+1][l+1].getG(),color[m+1][l+1].getB());
			glNormal3f(normalnyx[m + 1][l + 1], normalnyy[m + 1][l + 1] - 0.5, normalnyz[m + 1][l + 1]);
			glVertex3f(pktx[m + 1][l + 1], pkty[m + 1][l + 1] - 0.5, pktz[m + 1][l + 1]);

			glBegin(GL_TRIANGLES);
			//   glColor3f(color[m][l].getR(),color[m][l].getG(),color[m][l].getB());
			glNormal3f(normalnyx[m][l], normalnyy[m][l] - 0.5, normalnyz[m][l]);
			glVertex3f(pktx[m][l], pkty[m][l] - 0.5, pktz[m][l]);
			//    glColor3f(color[m+1][l+1].getR(),color[m+1][l+1].getG(),color[m+1][l+1].getB());
			glNormal3f(normalnyx[m + 1][l + 1], normalnyy[m + 1][l + 1] - 0.5, normalnyz[m + 1][l + 1]);
			glVertex3f(pktx[m + 1][l + 1], pkty[m + 1][l + 1] - 0.5, pktz[m + 1][l + 1]);
			//    glColor3f(color[m][l+1].getR(),color[m][l+1].getG(),color[m][l+1].getB());
			glNormal3f(normalnyx[m][l + 1], normalnyy[m][l + 1] - 0.5, normalnyz[m][l + 1]);
			glVertex3f(pktx[m][l + 1], pkty[m][l + 1] - 0.5, pktz[m][l + 1]);

			glBegin(GL_TRIANGLES);
			//  glColor3f(color[m][l].getR(),color[m][l].getG(),color[m][l].getB());
			glNormal3f(normalnyx[m][l], normalnyy[m][l] - 0.5, -1 * normalnyz[m][l]);
			glVertex3f(pktx[m][l], pkty[m][l] - 0.5, -1 * pktz[m][l]);
			//   glColor3f(color[m+1][l+1].getR(),color[m+1][l+1].getG(),color[m+1][l+1].getB());
			glNormal3f(normalnyx[m + 1][l + 1], normalnyy[m + 1][l + 1] - 0.5, -1 * normalnyz[m + 1][l + 1]);
			glVertex3f(pktx[m + 1][l + 1], pkty[m + 1][l + 1] - 0.5, -1 * pktz[m + 1][l + 1]);
			//   glColor3f(color[m+1][l].getR(),color[m+1][l].getG(),color[m+1][l].getB());
			glNormal3f(normalnyx[m + 1][l], normalnyy[m + 1][l] - 0.5, -1 * normalnyz[m + 1][l]);
			glVertex3f(pktx[m + 1][l], pkty[m + 1][l] - 0.5, -1 * pktz[m + 1][l]);

			glBegin(GL_TRIANGLES);
			//   glColor3f(color[m][l].getR(),color[m][l].getG(),color[m][l].getB());
			glNormal3f(normalnyx[m][l], normalnyy[m][l] - 0.5, -1 * normalnyz[m][l]);
			glVertex3f(pktx[m][l], pkty[m][l] - 0.5, -1 * pktz[m][l]);
			//   glColor3f(color[m][l+1].getR(),color[m][l+1].getG(),color[m][l+1].getB());
			glNormal3f(normalnyx[m][l + 1], normalnyy[m][l + 1] - 0.5, -1 * normalnyz[m][l + 1]);
			glVertex3f(pktx[m][l + 1], pkty[m][l + 1] - 0.5, -1 * pktz[m][l + 1]);
			//    glColor3f(color[m+1][l+1].getR(),color[m+1][l+1].getG(),color[m+1][l+1].getB());
			glNormal3f(normalnyx[m + 1][l + 1], normalnyy[m + 1][l + 1] - 0.5, -1 * normalnyz[m + 1][l + 1]);
			glVertex3f(pktx[m + 1][l + 1], pkty[m + 1][l + 1] - 0.5, -1 * pktz[m + 1][l + 1]);

			glBegin(GL_TRIANGLES);
			//   glColor3f(color[m][l].getR(),color[m][l].getG(),color[m][l].getB());
			glNormal3f(-1 * normalnyx[m][l], normalnyy[m][l] - 0.5, normalnyz[m][l]);
			glVertex3f(-1 * pktx[m][l], pkty[m][l] - 0.5, pktz[m][l]);
			//    glColor3f(color[m+1][l+1].getR(),color[m+1][l+1].getG(),color[m+1][l+1].getB());
			glNormal3f(-1 * normalnyx[m + 1][l + 1], normalnyy[m + 1][l + 1] - 0.5, normalnyz[m + 1][l + 1]);
			glVertex3f(-1 * pktx[m + 1][l + 1], pkty[m + 1][l + 1] - 0.5, pktz[m + 1][l + 1]);
			//    glColor3f(color[m+1][l].getR(),color[m+1][l].getG(),color[m+1][l].getB());
			glNormal3f(-1 * normalnyx[m + 1][l], normalnyy[m + 1][l] - 0.5, normalnyz[m + 1][l]);
			glVertex3f(-1 * pktx[m + 1][l], pkty[m + 1][l] - 0.5, pktz[m + 1][l]);

			glBegin(GL_TRIANGLES);
			//     glColor3f(color[m][l].getR(),color[m][l].getG(),color[m][l].getB());
			glNormal3f(-1 * normalnyx[m][l], normalnyy[m][l] - 0.5, normalnyz[m][l]);
			glVertex3f(-1 * pktx[m][l], pkty[m][l] - 0.5, pktz[m][l]);
			//    glColor3f(color[m][l+1].getR(),color[m][l+1].getG(),color[m][l+1].getB());
			glNormal3f(-1 * normalnyx[m][l + 1], normalnyy[m][l + 1] - 0.5, normalnyz[m][l + 1]);
			glVertex3f(-1 * pktx[m][l + 1], pkty[m][l + 1] - 0.5, pktz[m][l + 1]);
			//    glColor3f(color[m+1][l+1].getR(),color[m+1][l+1].getG(),color[m+1][l+1].getB());
			glNormal3f(-1 * normalnyx[m + 1][l + 1], normalnyy[m + 1][l + 1] - 0.5, normalnyz[m + 1][l + 1]);
			glVertex3f(-1 * pktx[m + 1][l + 1], pkty[m + 1][l + 1] - 0.5, pktz[m + 1][l + 1]);

			glBegin(GL_TRIANGLES);
			//    glColor3f(color[m][l].getR(),color[m][l].getG(),color[m][l].getB());
			glNormal3f(-1 * normalnyx[m][l], normalnyy[m][l] - 0.5, -1 * normalnyz[m][l]);
			glVertex3f(-1 * pktx[m][l], pkty[m][l] - 0.5, -1 * pktz[m][l]);
			//    glColor3f(color[m+1][l].getR(),color[m+1][l].getG(),color[m+1][l].getB());
			glNormal3f(-1 * normalnyx[m + 1][l], normalnyy[m + 1][l] - 0.5, -1 * normalnyz[m + 1][l]);
			glVertex3f(-1 * pktx[m + 1][l], pkty[m + 1][l] - 0.5, -1 * pktz[m + 1][l]);
			//    glColor3f(color[m+1][l+1].getR(),color[m+1][l+1].getG(),color[m+1][l+1].getB());
			glNormal3f(-1 * normalnyx[m + 1][l + 1], normalnyy[m + 1][l + 1] - 0.5, -1 * normalnyz[m + 1][l + 1]);
			glVertex3f(-1 * pktx[m + 1][l + 1], pkty[m + 1][l + 1] - 0.5, -1 * pktz[m + 1][l + 1]);

			glBegin(GL_TRIANGLES);
			//    glColor3f(color[m][l].getR(),color[m][l].getG(),color[m][l].getB());
			glNormal3f(-1 * normalnyx[m][l], normalnyy[m][l] - 0.5, -1 * normalnyz[m][l]);
			glVertex3f(-1 * pktx[m][l], pkty[m][l] - 0.5, -1 * pktz[m][l]);
			//   glColor3f(color[m+1][l+1].getR(),color[m+1][l+1].getG(),color[m+1][l+1].getB());
			glNormal3f(-1 * normalnyx[m + 1][l + 1], normalnyy[m + 1][l + 1] - 0.5, -1 * normalnyz[m + 1][l + 1]);
			glVertex3f(-1 * pktx[m + 1][l + 1], pkty[m + 1][l + 1] - 0.5, -1 * pktz[m + 1][l + 1]);
			//  glColor3f(color[m][l+1].getR(),color[m][l+1].getG(),color[m][l+1].getB());
			glNormal3f(-1 * normalnyx[m][l + 1], normalnyy[m][l + 1] - 0.5, -1 * normalnyz[m][l + 1]);
			glVertex3f(-1 * pktx[m][l + 1], pkty[m][l + 1] - 0.5, -1 * pktz[m][l + 1]);
			glEnd();

		}
	}
};

void Axes(void)
{
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz?tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0,  5.0, 0.0 };
	// pocz?tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0,  5.0 };
	//  pocz?tek i koniec obrazu osi y

	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
	glBegin(GL_LINES); // rysowanie osi x
	glVertex3fv(x_min);
	glVertex3fv(x_max);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
	glBegin(GL_LINES);  // rysowanie osi y
	glVertex3fv(y_min);
	glVertex3fv(y_max);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z
	glVertex3fv(z_min);
	glVertex3fv(z_max);
	glEnd();
}

void Mouse(int btn, int state, int x, int y)
{


	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		glEnable(GL_LIGHT0);
		x_pos_old = x;
		y_pos_old = y;
		// przypisanie aktualnie odczytanej pozycji kursora
		// jako pozycji poprzedniej
		status = 1;         // wci?ni?ty zosta? lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		glEnable(GL_LIGHT1);
		x_pos_old = x;
		y_pos_old = y;
		// przypisanie aktualnie odczytanej pozycji kursora
		// jako pozycji poprzedniej
		status = 2;         // wci?ni?ty zosta? lewy klawisz myszy
	}
	else
		status = 0;         // nie zosta? wci?ni?ty ?aden klawisz
}
void Motion(GLsizei x, GLsizei y)
{

	delta_x = x - x_pos_old;    // obliczenie ró?nicy po?o?enia kursora myszy
	delta_y = y - y_pos_old;
	delta_x1 = x - x_pos_old;    // obliczenie ró?nicy po?o?enia kursora myszy
	delta_y1 = y - y_pos_old;
	x_pos_old = x;
	y_pos_old = y;           // podstawienie bie?acego po?o?enia jako poprzedniegl

	glutPostRedisplay();    // przerysowanie obrazu sceny
}

/*************************************************************************************/
// Funkcja okre?laj?ca co ma by? rysowane (zawsze wywo?ywana, gdy trzeba
// przerysowa? scen?)

void RenderScene(void)
{
	// do ró?nicy po?o?e? kursora myszy



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz?cym

	glLoadIdentity();
	// Czyszczenie macierzy bie??cej

	gluLookAt(0, 0, -10, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//Axes();
	// Zdefiniowanie po?o?enia obserwatora
	//Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej powy?ej
	if (status == 1)                    // je?li lewy klawisz myszy wci?ni?ty
	{
		if (delta_x != 0) {
			theta += delta_x*pix2angle*0.01;// modyfikacja k?ta obrotu o k?t proporcjonalny
		}
		if (delta_y != 0) {
			theta1 += delta_y*pix2angle1*0.01;// modyfikacja k?ta obrotu o k?t proporcjonalny
		}

	}                                 // do ró?nicy po?o?e? kursora myszy
	if (status == 2) {
		if (delta_x1 != 0) {
			theta2 += delta_x1*pix2angle*0.01;// modyfikacja k?ta obrotu o k?t proporcjonalny
		}
		if (delta_y1 != 0) {
			theta3 += delta_y1*pix2angle1*0.01;// modyfikacja k?ta obrotu o k?t proporcjonalny
		}
	}

	viewer[0] = zoom*cos(theta)*cos(theta1);
	viewer[1] = zoom*sin(theta1);
	viewer[2] = zoom*sin(theta)*cos(theta1);

	viewer1[0] = zoom*cos(theta2)*cos(theta3);
	viewer1[1] = zoom*sin(theta3);
	viewer1[2] = zoom*sin(theta2)*cos(theta3);


	GLfloat light_position[] = { viewer[0], viewer[1], viewer[2], 0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	GLfloat light_position1[] = { viewer1[0], viewer1[1], viewer1[2], 0 };
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	// Ustawienie koloru rysowania na bia?y
	CretateEgg(color);
	//glutSolidTeapot(3.0);
	// Narysowanie czajnika
	glFlush();
	// Przekazanie polece? rysuj?cych do wykonania

	glutSwapBuffers();
	//
}
/*************************************************************************************/
// Funkcja ustalaj?ca stan renderowania
void keys(unsigned char key, int x, int y)
{
	if (key == 'q') stan = 1;
	if (key == 'w')stan = 2;


	RenderScene(); // przerysowanie obrazu sceny
}
void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	/*************************************************************************************/
	//  Definicja materia³u z jakiego zrobiony jest czajnik
	//  i definicja Ÿród³a œwiat³a
	/*************************************************************************************/

	/*************************************************************************************/
	// Definicja materia³u z jakiego zrobiony jest czajnik

	GLfloat mat_ambient[] = { 0.15, 0.4, 0.4, 100.0 };

	// wspó³czynniki ka =[kar,kag,kab] dla œwiat³a otoczenia

	GLfloat mat_diffuse[] = { 1.0, 0.0, 1.0, 100.0 };

	// wspó³czynniki kd =[kdr,kdg,kdb] œwiat³a rozproszonego

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 10.0 };

	// wspó³czynniki ks =[ksr,ksg,ksb] dla œwiat³a odbitego

	GLfloat mat_shininess = { 50.0 };
	// wspó³czynnik n opisuj¹cy po³ysk powierzchni

	/*************************************************************************************/
	// Definicja Ÿród³a œwiat³a

	GLfloat light_position[] = { 0, 0, 0, 100.0 };
	GLfloat light_position1[] = { 0, 0, 0, 100.0 };
	// po³o¿enie Ÿród³a

	GLfloat light_ambient[] = { 1.0, 1.0, 0.0, 1.0 };
	GLfloat light_ambient1[] = { 1.0, 1.0, 0.0, 1.0 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a otoczenia
	// Ia = [Iar,Iag,Iab]

	GLfloat light_diffuse[] = { 0.0, 0.0, 1.0, 10.0 };
	GLfloat light_diffuse1[] = { 1.0, 1.0, 0.0, 10.0 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
	// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 10.0 };
	GLfloat light_specular1[] = { 1.0, 1.0, 1.0, 10.0 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
	// odbicie kierunkowe Is = [Isr,Isg,Isb]

	GLfloat att_constant = { 1.0 };
	// sk³adowa sta³a ds dla modelu zmian oœwietlenia w funkcji
	// odleg³oœci od Ÿród³a

	GLfloat att_linear = {(GLfloat) 0.05 };
	// sk³adowa liniowa dl dla modelu zmian oœwietlenia w funkcji
	// odleg³oœci od Ÿród³a

	GLfloat att_quadratic = { (GLfloat) 0.001 };
	// sk³adowa kwadratowa dq dla modelu zmian oœwietlenia w funkcji
	// odleg³oœci od Ÿród³a

	/*************************************************************************************/

	// Ustawienie parametrów materia³u i Ÿród³a œwiat³a

	/*************************************************************************************/
	// Ustawienie patrametrów materia³u

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	/*************************************************************************************/
	// Ustawienie parametrów Ÿród³a

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);


	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, att_quadratic);

	/*************************************************************************************/
	// Ustawienie opcji systemu oœwietlania sceny

	glShadeModel(GL_SMOOTH); // w³aczenie ³agodnego cieniowania
	glEnable(GL_LIGHTING);   // w³aczenie systemu oœwietlenia sceny
	//if (stan ==1)
	glEnable(GL_LIGHT0);     // w³¹czenie Ÿród³a o numerze 0
	//if (stan == 2)
	glEnable(GL_LIGHT1);
	glEnable(GL_DEPTH_TEST); // w³¹czenie mechanizmu z-bufora

							 /*************************************************************************************/


							 // Kolor czyszcz?cy (wype?nienia okna) ustawiono na czarny
}
/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie sta?ych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysoko?? i szeroko?? okna) s?
// przekazywane do funkcji za ka?dym razem gdy zmieni si? rozmiar okna.
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;
	pix2angle1 = 360.0 / (float)vertical; // przeliczenie pikseli na stopnie
	glMatrixMode(GL_PROJECTION);
	// Prze??czenie macierzy bie??cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie??cej

	gluPerspective(10, 1.0, 1.0, 30.0);
	// Ustawienie parametrów dla rzutu perspektywicznego


	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);

	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielko?ci okna okna widoku (viewport) w zale?no?ci
	// relacji pomi?dzy wysoko?ci? i szeroko?ci? okna

	glMatrixMode(GL_MODELVIEW);
	// Prze??czenie macierzy bie??cej na macierz widoku modelu

	glLoadIdentity();
	// Czyszczenie macierzy bie??cej



}

/*************************************************************************************/
// G?ówny punkt wej?cia programu. Program dzia?a w trybie konsoli

int main(int argc, char** argv)
{
    glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(600, 600);

	glutCreateWindow("Rzutowanie perspektywiczne");

	glutDisplayFunc(RenderScene);
	// Okre?lenie, ?e funkcja RenderScene b?dzie funkcj? zwrotn?
	// (callback function).  B?dzie ona wywo?ywana za ka?dym razem
	// gdy zajdzie potrzeba przerysowania okna

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcj? zwrotn? odpowiedzialn?
	// za zmiany rozmiaru okna
	glutMouseFunc(Mouse);
	// Ustala funkcj? zwrotn? odpowiedzialn? za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcj? zwrotn? odpowiedzialn? za badanie ruchu myszy
	MyInit();
	// Funkcja MyInit() (zdefiniowana powy?ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst?pieniem do renderowania
	glEnable(GL_DEPTH_TEST);
	// W??czenie mechanizmu usuwania niewidocznych elementów sceny


	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
	glutKeyboardFunc(keys);

}

/*************************************************************************************/

