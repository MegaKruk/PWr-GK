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
//#include <gl/glext.h>
#define N 20
typedef float point3[3];
static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
static GLfloat theta = 0.0;
static GLfloat theta1 = 0.0;
static GLfloat theta2 = 0.0;
static GLfloat theta3 = 0.0;
// k?t obrotu obiektu
static GLfloat pix2angle;     // przelicznik pikseli na stopnie
static GLfloat pix2angle1;
static GLint status = 0;      // stan klawiszy myszy 
static GLint status1 = 0;                              // 0 - nie naci?ni?to ?adnego klawisza
													   // 1 - naci?ni?ty zosta? lewy klawisz

static int x_pos_old = 0;       // poprzednia pozycja kursora myszy
static int y_pos_old = 0;
static int delta_x = 0;       // ró?nica pomi?dzy pozycj? bie??c?
static int delta_y = 0;                               // i poprzedni? kursora myszy 
static GLfloat x = 0, y = 0, z = 0;  //wybór osi obrotu
									 //static GLfloat viewer[]= {0.0, 0.0, 10.0};
static GLfloat zoom = 10.0;

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
/**************************
* Function Declarations
*
**************************/
void Jeden()
{
	glBegin(GL_TRIANGLES);

	glTexCoord2f(0.0f, 1.0f);
	glVertex2f(0.0f, 1.0f);
	glTexCoord2f(0.87f, -0.5f);
	glVertex2f(0.87f, -0.5f);
	glTexCoord2f(-0.87f, -0.5f);
	glVertex2f(-0.87f, -0.5f);

	glEnd();
}


void Trojkat()
{
	glBegin(GL_QUADS);

	glTexCoord2f(-0.5f, 0.5f);
	glVertex2f(-0.5f, 0.5f);


	//  glColor3f (1.0f, 0.0f, 0.0f); 
	glTexCoord2f(0.5f, 0.5f);
	glVertex2f(0.5f, 0.5f);

	// glColor3f (1.0f, 0.0f, 0.0f);  
	glTexCoord2f(0.5f, -0.5f);
	glVertex2f(0.5f, -0.5f);

	// glColor3f (1.0f, 0.0f, 0.0f);   
	glTexCoord2f(-0.5f, -0.5f);
	glVertex2f(-0.5f, -0.5f);

	glEnd();

	glBegin(GL_TRIANGLES);
	glTexCoord3f(-0.5f, -0.5f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.0f);

	glTexCoord3f(0.5f, -0.5f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);

	glTexCoord3f(0.0f, 0.0f, 0.75f);
	glVertex3f(0.0f, 0.0f, 0.75f);


	glEnd();



	glBegin(GL_TRIANGLES);


	glTexCoord3f(0.5f, -0.5f, 0.0f);
	glVertex3f(0.5f, -0.5f, 0.0f);
	glTexCoord3f(0.5f, 0.5f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);
	glTexCoord3f(0.0f, 0.0f, 0.75f);
	glVertex3f(0.0f, 0.0f, 0.75f);

	glEnd();

	glBegin(GL_TRIANGLES);


	glTexCoord3f(0.5f, 0.5f, 0.0f);
	glVertex3f(0.5f, 0.5f, 0.0f);
	glTexCoord3f(-0.5f, 0.5f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glTexCoord3f(0.0f, 0.0f, 0.75f);
	glVertex3f(0.0f, 0.0f, 0.75f);

	glEnd();

	glBegin(GL_TRIANGLES);


	glTexCoord3f(-0.5f, 0.5f, 0.0f);
	glVertex3f(-0.5f, 0.5f, 0.0f);
	glTexCoord3f(-0.5f, -0.5f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.0f);
	glTexCoord3f(0.0f, 0.0f, 0.75f);
	glVertex3f(0.0f, 0.0f, 0.75f);

	glEnd();

};



void Maltroj(Kolor color[][20])
{
	//int N = 20;
	float x;
	x = 0.533 / N;
	float tabx[N][N];
	float taby[N][N];
	//  float theta=0.0;
	for (int i = 0; i<N; i++)
	{
		for (int k = 0; k<N; k++)
		{
			tabx[i][k] = 0.0 + (x*k);
			taby[i][k] = 0.0 + (x*i);
			//  glBegin(GL_POINTS); // rysowanie osi x
			//  glVertex2f(tabx[i][k],taby[i][k]);
		}
	}

	float pktx[N][N], pkty[N][N], pktz[N][N];
	float u;
	float v;
	for (int m = 0; m<N; m++)
	{
		for (int l = 0; l<N; l++)
		{
			u = tabx[m][l];
			v = taby[m][l];


			pktx[m][l] = ((-9 * pow(u, 5)) + (22.5*pow(u, 4)) - (27 * pow(u, 3)) + (18 * pow(u, 2)) - (4.5*u))*(cos(v*pi));
			pkty[m][l] = ((16 * pow(u, 4)) - (32 * pow(u, 3)) + (16 * pow(u, 2)));
			pktz[m][l] = ((-9 * pow(u, 5)) + (22.5*pow(u, 4)) - (27 * pow(u, 3)) + (18 * pow(u, 2)) - (4.5*u))*(sin(v*pi));

		}
	}



	for (int m = 0; m<N - 1; m++)
	{
		for (int l = 0; l<N - 1; l++)


		{
			glBegin(GL_TRIANGLES);
			glColor3f(color[m][l].getR(), color[m][l].getG(), color[m][l].getB());
			glVertex3f(pktx[m][l], pkty[m][l] - 0.5, pktz[m][l]);
			glColor3f(color[m + 1][l].getR(), color[m + 1][l].getG(), color[m + 1][l].getB());
			glVertex3f(pktx[m + 1][l], pkty[m + 1][l] - 0.5, pktz[m + 1][l]);
			glColor3f(color[m + 1][l + 1].getR(), color[m + 1][l + 1].getG(), color[m + 1][l + 1].getB());
			glVertex3f(pktx[m + 1][l + 1], pkty[m + 1][l + 1] - 0.5, pktz[m + 1][l + 1]);

			glBegin(GL_TRIANGLES);
			glColor3f(color[m][l].getR(), color[m][l].getG(), color[m][l].getB());
			glVertex3f(pktx[m][l], pkty[m][l] - 0.5, pktz[m][l]);
			glColor3f(color[m + 1][l + 1].getR(), color[m + 1][l + 1].getG(), color[m + 1][l + 1].getB());
			glVertex3f(pktx[m + 1][l + 1], pkty[m + 1][l + 1] - 0.5, pktz[m + 1][l + 1]);
			glColor3f(color[m][l + 1].getR(), color[m][l + 1].getG(), color[m][l + 1].getB());
			glVertex3f(pktx[m][l + 1], pkty[m][l + 1] - 0.5, pktz[m][l + 1]);

			glBegin(GL_TRIANGLES);
			glColor3f(color[m][l].getR(), color[m][l].getG(), color[m][l].getB());
			glVertex3f(pktx[m][l], pkty[m][l] - 0.5, -1 * pktz[m][l]);
			glColor3f(color[m + 1][l + 1].getR(), color[m + 1][l + 1].getG(), color[m + 1][l + 1].getB());
			glVertex3f(pktx[m + 1][l + 1], pkty[m + 1][l + 1] - 0.5, -1 * pktz[m + 1][l + 1]);
			glColor3f(color[m + 1][l].getR(), color[m + 1][l].getG(), color[m + 1][l].getB());
			glVertex3f(pktx[m + 1][l], pkty[m + 1][l] - 0.5, -1 * pktz[m + 1][l]);

			glBegin(GL_TRIANGLES);
			glColor3f(color[m][l].getR(), color[m][l].getG(), color[m][l].getB());
			glVertex3f(pktx[m][l], pkty[m][l] - 0.5, -1 * pktz[m][l]);
			glColor3f(color[m][l + 1].getR(), color[m][l + 1].getG(), color[m][l + 1].getB());
			glVertex3f(pktx[m][l + 1], pkty[m][l + 1] - 0.5, -1 * pktz[m][l + 1]);
			glColor3f(color[m + 1][l + 1].getR(), color[m + 1][l + 1].getG(), color[m + 1][l + 1].getB());
			glVertex3f(pktx[m + 1][l + 1], pkty[m + 1][l + 1] - 0.5, -1 * pktz[m + 1][l + 1]);

			glBegin(GL_TRIANGLES);
			glColor3f(color[m][l].getR(), color[m][l].getG(), color[m][l].getB());
			glVertex3f(-1 * pktx[m][l], pkty[m][l] - 0.5, pktz[m][l]);
			glColor3f(color[m + 1][l + 1].getR(), color[m + 1][l + 1].getG(), color[m + 1][l + 1].getB());
			glVertex3f(-1 * pktx[m + 1][l + 1], pkty[m + 1][l + 1] - 0.5, pktz[m + 1][l + 1]);
			glColor3f(color[m + 1][l].getR(), color[m + 1][l].getG(), color[m + 1][l].getB());
			glVertex3f(-1 * pktx[m + 1][l], pkty[m + 1][l] - 0.5, pktz[m + 1][l]);

			glBegin(GL_TRIANGLES);
			glColor3f(color[m][l].getR(), color[m][l].getG(), color[m][l].getB());
			glVertex3f(-1 * pktx[m][l], pkty[m][l] - 0.5, pktz[m][l]);
			glColor3f(color[m][l + 1].getR(), color[m][l + 1].getG(), color[m][l + 1].getB());
			glVertex3f(-1 * pktx[m][l + 1], pkty[m][l + 1] - 0.5, pktz[m][l + 1]);
			glColor3f(color[m + 1][l + 1].getR(), color[m + 1][l + 1].getG(), color[m + 1][l + 1].getB());
			glVertex3f(-1 * pktx[m + 1][l + 1], pkty[m + 1][l + 1] - 0.5, pktz[m + 1][l + 1]);

			glBegin(GL_TRIANGLES);
			glColor3f(color[m][l].getR(), color[m][l].getG(), color[m][l].getB());
			glVertex3f(-1 * pktx[m][l], pkty[m][l] - 0.5, -1 * pktz[m][l]);
			glColor3f(color[m + 1][l].getR(), color[m + 1][l].getG(), color[m + 1][l].getB());
			glVertex3f(-1 * pktx[m + 1][l], pkty[m + 1][l] - 0.5, -1 * pktz[m + 1][l]);
			glColor3f(color[m + 1][l + 1].getR(), color[m + 1][l + 1].getG(), color[m + 1][l + 1].getB());
			glVertex3f(-1 * pktx[m + 1][l + 1], pkty[m + 1][l + 1] - 0.5, -1 * pktz[m + 1][l + 1]);

			glBegin(GL_TRIANGLES);
			glColor3f(color[m][l].getR(), color[m][l].getG(), color[m][l].getB());
			glVertex3f(-1 * pktx[m][l], pkty[m][l] - 0.5, -1 * pktz[m][l]);
			glColor3f(color[m + 1][l + 1].getR(), color[m + 1][l + 1].getG(), color[m + 1][l + 1].getB());
			glVertex3f(-1 * pktx[m + 1][l + 1], pkty[m + 1][l + 1] - 0.5, -1 * pktz[m + 1][l + 1]);
			glColor3f(color[m][l + 1].getR(), color[m][l + 1].getG(), color[m][l + 1].getB());
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

GLbyte *LoadTGAImage(const char *FileName, GLint *ImWidth, GLint *ImHeight, GLint *ImComponents, GLenum *ImFormat)
{
	/*************************************************************************************/
	// Struktura dla nag³ówka pliku  TGA 

#pragma pack(1)            
	typedef struct
	{
		GLbyte    idlength;
		GLbyte    colormaptype;
		GLbyte    datatypecode;
		unsigned short    colormapstart;
		unsigned short    colormaplength;
		unsigned char     colormapdepth;
		unsigned short    x_orgin;
		unsigned short    y_orgin;
		unsigned short    width;
		unsigned short    height;
		GLbyte    bitsperpixel;
		GLbyte    descriptor;
	}TGAHEADER;
#pragma pack(8)


	FILE *pFile;
	TGAHEADER tgaHeader;
	unsigned long lImageSize;
	short sDepth;
	GLbyte    *pbitsperpixel = NULL;

	/*************************************************************************************/
	// Wartoœci domyœlne zwracane w przypadku b³êdu 

	*ImWidth = 0;
	*ImHeight = 0;
	*ImFormat = GL_BGR_EXT;
	*ImComponents = GL_RGB8;
#pragma warning (disable : 4996)

	pFile = fopen(FileName, "rb");
	if (pFile == NULL)
		return NULL;
	/*************************************************************************************/

	// Przeczytanie nag³ówka pliku 

	fread(&tgaHeader, sizeof(TGAHEADER), 1, pFile);

	/*************************************************************************************/
	// Odczytanie szerokoœci, wysokoœci i g³êbi obrazu 

	*ImWidth = tgaHeader.width;
	*ImHeight = tgaHeader.height;
	sDepth = tgaHeader.bitsperpixel / 8;

	/*************************************************************************************/

	// Sprawdzenie, czy g³êbia spe³nia za³o¿one warunki (8, 24, lub 32 bity) 

	if (tgaHeader.bitsperpixel != 8 && tgaHeader.bitsperpixel != 24 && tgaHeader.bitsperpixel != 32)
		return NULL;

	/*************************************************************************************/
	// Obliczenie rozmiaru bufora w pamiêci

	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

	/*************************************************************************************/
	// Alokacja pamiêci dla danych obrazu

	pbitsperpixel = (GLbyte*)malloc(lImageSize * sizeof(GLbyte));

	if (pbitsperpixel == NULL)
		return NULL;

	if (fread(pbitsperpixel, lImageSize, 1, pFile) != 1)
	{
		free(pbitsperpixel);
		return NULL;
	}

	/*************************************************************************************/
	// Ustawienie formatu OpenGL

	switch (sDepth)
	{
	case 3:
		*ImFormat = GL_BGR_EXT;
		*ImComponents = GL_RGB8;
		break;
	case 4:
		*ImFormat = GL_BGRA_EXT;
		*ImComponents = GL_RGBA8;
		break;
	case 1:
		*ImFormat = GL_LUMINANCE;
		*ImComponents = GL_LUMINANCE8;
		break;
	};

	fclose(pFile);

	return pbitsperpixel;
}

void Mouse(int btn, int state, int x, int y)
{


	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		x_pos_old = x;
		y_pos_old = y;
		// przypisanie aktualnie odczytanej pozycji kursora 
		// jako pozycji poprzedniej
		status = 1;         // wci?ni?ty zosta? lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
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
	x_pos_old = x;
	y_pos_old = y;           // podstawienie bie?acego po?o?enia jako poprzednie

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
	//Axes();
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
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
		if (delta_y>0) zoom += 1;
		if (delta_y<0) zoom -= 1;
	}

	viewer[0] = zoom*cos(theta)*cos(theta1);
	viewer[1] = zoom*sin(theta1);
	viewer[2] = zoom*sin(theta)*cos(theta1);

	// Ustawienie koloru rysowania na bia?y



	Trojkat();
	//Jeden();
	//Maltroj(color);


	//glutWireTeapot(3.0);
	// Narysowanie czajnika 
	glFlush();
	// Przekazanie polece? rysuj?cych do wykonania

	glutSwapBuffers();
	//
}
/*************************************************************************************/
// Funkcja ustalaj?ca stan renderowania

void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Zmienne dla obrazu tekstury 

	GLbyte *pBytes;
	GLint ImWidth, ImHeight, ImComponents;
	GLenum ImFormat;

	// ..................................

	//       Pozosta³a czêœæ funkcji MyInit()
	// ..................................

	/*************************************************************************************/
	// Teksturowanie bêdzie prowadzone tyko po jednej stronie œciany 

	glEnable(GL_CULL_FACE);

	/*************************************************************************************/
	//  Przeczytanie obrazu tekstury z pliku o nazwie tekstura.tga

	pBytes = LoadTGAImage("D1_t.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);

	/*************************************************************************************/
	// Zdefiniowanie tekstury 2-D 

	glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);

	/*************************************************************************************/
	// Zwolnienie pamiêci

	free(pBytes);

	/*************************************************************************************/
	// W³¹czenie mechanizmu teksturowania

	glEnable(GL_TEXTURE_2D);

	/*************************************************************************************/
	// Ustalenie trybu teksturowania

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	/*************************************************************************************/
	// Okreœlenie sposobu nak³adania tekstur

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// ..................................

	//       Pozosta³a czêœæ funkcji MyInit()
	// ..................................

}
// Kolor czyszcz?cy (wype?nienia okna) ustawiono na czarny

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

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(600, 600);
	glutInit(&argc, argv);

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

}

/*************************************************************************************/