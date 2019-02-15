#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <stdio.h>
#include <conio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#define N 100

typedef float point3[3];    //3D
typedef float point2[2];    //2D
point3 **tab;
point3 **tabx;
point2 **tabteks;

float bok = 1.0;            //bok kwadratu
float promien = 10.0;       //promien swiatla

static GLfloat viewer[] = { 0.0, 0.0, 10.0 };    //inicjalizacja po³o¿enia obserwatora
static GLfloat angle[] = { 0.0, 0.0, 0.0 };    //katy obrotu

static GLfloat theta = 0.0;    //azymut
static GLfloat fi = 0.0;    //elewacja
static GLfloat pix2angle_x = 0.0;        //przelicznik pikseli na stopnie
static GLfloat pix2angle_y = 0.0;

static GLint status = 0;    // stan klawiszy myszy
							// 0 - nie naciœniêto ¿adnego klawisza
							// 1 - naciœniêty zosta³ lewy klawisz, 2 - prawy

static int x_pos_old = 0;
static int y_pos_old = 0;
static int delta_x = 0;
static int delta_y = 0;

/*************************************************************************************/
// Funkcja wczytuje dane obrazu zapisanego w formacie TGA w pliku o nazwie 
// FileName, alokuje pamiêæ i zwraca wskaŸnik (pBits) do bufora w którym 
// umieszczone s¹ dane. 
// Ponadto udostêpnia szerokoœæ (ImWidth), wysokoœæ (ImHeight) obrazu
// tekstury oraz dane opisuj¹ce format obrazu wed³ug specyfikacji OpenGL 
// (ImComponents) i (ImFormat).
// Jest to bardzo uproszczona wersja funkcji wczytuj¹cej dane z pliku TGA.
// Dzia³a tylko dla obrazów wykorzystuj¹cych 8, 24, or 32 bitowy kolor.
// Nie obs³uguje plików w formacie TGA kodowanych z kompresj¹ RLE.
/*************************************************************************************/
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
	}
	TGAHEADER;
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
/*************************************************************************************/

void Mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		y_pos_old = y;            // przypisanie aktualnie odczytanej pozycji kursora
		status = 2;               // wciœniêty zosta³ prawy klawisz myszy
	}
	else
		status = 0;                // nie zosta³ wciœniêty ¿aden klawisz
}


void Motion(GLsizei x, GLsizei y)
{
	delta_x = x - x_pos_old;    //obliczenie ró¿nicy po³o¿enia kursora myszy
	x_pos_old = x;                //podstawienie bie¿acego po³o¿enia jako poprzednie


	delta_y = y - y_pos_old;    //obliczenie ró¿nicy po³o¿enia kursora myszy
	y_pos_old = y;                //podstawienie bie¿acego po³o¿enia jako poprzednie


	glutPostRedisplay();        //przerysowanie obrazu sceny
}


float vecnorm_x(float u, float v)
{
	float yu = (640.0*u*u*u - 960.0*u*u + 320.0*u);
	float yv = 0;
	float zu = (-450.0*u*u*u*u + 900.0*u*u*u - 810.0*u*u + 360.0*u - 45.0)*sin(M_PI*v);
	float zv = -M_PI*(90.0*u*u*u*u*u - 225.0*u*u*u*u + 270.0*u*u*u - 180.0*u*u + 45.0*u)*cos(M_PI*v);


	float x = (GLfloat)(yu*zv - zu*yv);
	return x;
}

float vecnorm_y(float u, float v)
{
	float xu = (-450.0*u*u*u*u + 900.0*u*u*u - 810.0*u*u + 360.0*u - 45.0)*cos(M_PI*v);
	float xv = M_PI*(90.0*u*u*u*u*u - 225.0*u*u*u*u + 270.0*u*u*u - 180.0*u*u + 45.0*u)*sin(M_PI*v);
	float zu = (-450.0*u*u*u*u + 900.0*u*u*u - 810.0*u*u + 360.0*u - 45.0)*sin(M_PI*v);
	float zv = -M_PI*(90.0*u*u*u*u*u - 225.0*u*u*u*u + 270.0*u*u*u - 180.0*u*u + 45.0*u)*cos(M_PI*v);

	float y = (GLfloat)(zu*xv - xu*zv);
	return y;
}


float vecnorm_z(float u, float v)
{
	float xu = (-450.0*u*u*u*u + 900.0*u*u*u - 810.0*u*u + 360.0*u - 45.0)*cos(M_PI*v);
	float xv = M_PI*(90.0*u*u*u*u*u - 225.0*u*u*u*u + 270.0*u*u*u - 180.0*u*u + 45.0*u)*sin(M_PI*v);
	float yu = (640.0*u*u*u - 960.0*u*u + 320.0*u);
	float yv = 0.0;

	float z = (GLfloat)(xu*yv - yu*xv);
	return z;
}


float parametr_x(float u, float v)
{
	float x = (-90.0*u*u*u*u*u + 225.0*u*u*u*u - 270.0*u*u*u + 180.0*u*u - 45.0*u)*cos(M_PI*v);
	return x;
}


float parametr_y(float u, float v)
{
	float y = (160.0*u*u*u*u - 320.0*u*u*u + 160.0*u*u) - 5.0;
	return y;
}


float parametr_z(float u, float v)
{
	float z = (-90.0*u*u*u*u*u + 225.0*u*u*u*u - 270.0*u*u*u + 180.0*u*u - 45.0*u)*sin(M_PI*v);
	return z;
}


void SiatkaPunktow()    //wygenerowanie siatki punktow
{
	for (int i = 0; i<N + 1; i++)    //przypisanie wspolrzednych do punktow
	{
		for (int k = 0; k<N + 1; k++)
		{
			tab[i][k][0] = k*float(bok / N);
			tab[i][k][1] = i*float(bok / N);
		}
	}


	for (int i = 0; i<N + 1; i++)    //przesztalcenie wspolrzednych w przestrzen 3D
	{
		for (int k = 0; k<N + 1; k++)
		{
			float v = tab[i][k][0];
			float u = tab[i][k][1];
			tab[i][k][0] = parametr_x(u, v);
			tab[i][k][1] = parametr_y(u, v);
			tab[i][k][2] = parametr_z(u, v);


			tabteks[i][k][0] = v;    //wspolrzedne tekstury
			tabteks[i][k][1] = u;

			float x = vecnorm_x(u, v);    //wyliczenie wspolrzednych wektora normalnego x
			float y = vecnorm_y(u, v);    //wyliczenie wspolrzednych wektora normalnego y
			float z = vecnorm_z(u, v);    //wyliczenie wspolrzednych wektora normalnego z


			if (i < N / 2)    //boki jajka - normalizacja wektorow normalnych
			{
				tabx[i][k][0] = x / (float)sqrt((x*x) + (y*y) + (z*z));
				tabx[i][k][1] = y / (float)sqrt((x*x) + (y*y) + (z*z));
				tabx[i][k][2] = z / (float)sqrt((x*x) + (y*y) + (z*z));
			}


			if (i > N / 2)    //boki jajka - normalizacja wektorow normalnych
			{
				tabx[i][k][0] = -1.0*x / (float)sqrt((x*x) + (y*y) + (z*z));
				tabx[i][k][1] = -1.0*y / (float)sqrt((x*x) + (y*y) + (z*z));
				tabx[i][k][2] = -1.0*z / (float)sqrt((x*x) + (y*y) + (z*z));
			}


			if (i == N / 2)    //gora jajka - normalizacja wektorow normalnych
			{
				tabx[i][k][0] = 0;
				tabx[i][k][1] = 1;
				tabx[i][k][2] = 0;
			}


			if (i == 0 || i == N) //dol jajka - normalizacja wektorow normalnych
			{
				tabx[i][k][0] = 0;
				tabx[i][k][1] = -1;
				tabx[i][k][2] = 0;
			}
		}
	}
}


void Egg()
{
	SiatkaPunktow();    //wywolanie siatki punktow

	glColor3f(1.0, 1.0, 1.0);

	for (int i = 0; i < N; i++)
	{
		for (int k = 0; k < N; k++)
		{
			glBegin(GL_TRIANGLES);
			glNormal3fv(tabx[i][k + 1]);
			glTexCoord2f(tabteks[i][k + 1][0], tabteks[i][k + 1][1]);
			glVertex3fv(tab[i][k + 1]);

			glNormal3fv(tabx[i + 1][k]);
			glTexCoord2f(tabteks[i + 1][k][0], tabteks[i + 1][k][1]);
			glVertex3fv(tab[i + 1][k]);

			glNormal3fv(tabx[i + 1][k + 1]);
			glTexCoord2f(tabteks[i + 1][k + 1][0], tabteks[i + 1][k + 1][1]);
			glVertex3fv(tab[i + 1][k + 1]);
			glEnd();

			glBegin(GL_TRIANGLES);
			glNormal3fv((tabx[i][k]));
			glTexCoord2f(tabteks[i][k][0], tabteks[i][k][1]);
			glVertex3fv(tab[i][k]);

			glNormal3fv(tabx[i + 1][k]);
			glTexCoord2f(tabteks[i + 1][k][0], tabteks[i + 1][k][1]);
			glVertex3fv(tab[i + 1][k]);

			glNormal3fv(tabx[i][k + 1]);
			glTexCoord2f(tabteks[i][k + 1][0], tabteks[i][k + 1][1]);
			glVertex3fv(tab[i][k + 1]);

			glEnd();
		}
	}
}


void Axes(void)
{
	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// pocz¹tek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0, 5.0, 0.0 };
	// pocz¹tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0, 5.0 };
	//  pocz¹tek i koniec obrazu osi y

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


void spinEgg()
{
	angle[0] -= 0.5;

	if (angle[0] > 360.0)
		angle[0] -= 360.0;
	angle[1] -= 0.5;

	if (angle[1] > 360.0)
		angle[1] -= 360.0;
	angle[2] -= 0.5;

	if (angle[2] > 360.0)
		angle[2] -= 360.0;

	glutPostRedisplay(); //odœwie¿enie zawartoœci aktualnego okna
}


void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym
	glLoadIdentity();
	// Czyszczenie macierzy bie¿¹cej

	if (status == 2)
	{    //jesli prawy klawisz myszy wcisniety
		promien += 0.1* delta_y;
		if (promien <= 7)    //granica wyswietlania jajka na ekranie
			promien = 7;
		if (promien >= 25)
			promien = 25;
	}

	viewer[0] = promien * cos(theta) * cos(fi);    //wspolrzedne obserwatora
	viewer[1] = promien * sin(fi);
	viewer[2] = promien * sin(theta) * cos(fi);

	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, cos(fi), 0.0);
	// Zdefiniowanie po³o¿enia obserwatora

	glRotatef(angle[0], 1.0, 0.0, 0.0);    //rotacje
	glRotatef(angle[1], 0.0, 1.0, 0.0);
	glRotatef(angle[2], 0.0, 0.0, 1.0);

	//Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej powy?ej

	Egg();
	// Narysowanie jajka

	glFlush();
	// Przekazanie polece? rysuj?cych do wykonania

	glutSwapBuffers();
	//
}


void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszc¹cy (wype³nienia okna) ustawiono na czarny

	// Zmienne dla obrazu tekstury 
	GLbyte *pBytes;
	GLint ImWidth;
	GLint ImHeight;
	GLint ImComponents;
	GLenum ImFormat;

	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	// wspó³czynniki ka =[kar,kag,kab] dla œwiat³a otoczenia

	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// wspó³czynniki kd =[kdr,kdg,kdb] œwiat³a rozproszonego

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// wspó³czynniki ks =[ksr,ksg,ksb] dla œwiat³a odbitego                

	GLfloat mat_shininess = { 100.0 };
	// wspó³czynnik n opisuj¹cy po³ysk powierzchni

	// Definicja Ÿród³a œwiat³a

	GLfloat light_position[] = { 0.0, 0.0, 10.0, 1.0 };
	// po³o¿enie Ÿród³a

	GLfloat light_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a otoczenia
	// Ia = [Iar,Iag,Iab]

	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
	// odbicie dyfuzyjne Id = [Idr,Idg,Idb]

	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	// sk³adowe intensywnoœci œwiecenia Ÿród³a œwiat³a powoduj¹cego
	// odbicie kierunkowe Is = [Isr,Isg,Isb]

	GLfloat att_constant = { 1.0 };
	// sk³adowa sta³a ds dla modelu zmian oœwietlenia w funkcji 
	// odleg³oœci od Ÿród³a

	GLdouble att_linear = { 0.05 };
	// sk³adowa liniowa dl dla modelu zmian oœwietlenia w funkcji 
	// odleg³oœci od Ÿród³a

	GLdouble att_quadratic = { 0.001 };
	// sk³adowa kwadratowa dq dla modelu zmian oœwietlenia w funkcji
	// odleg³oœci od Ÿród³a

	// Ustawienie patrametrów materia³u 
	//-------------------------------------------------------
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	// Ustawienie patrametrów materia³u 
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	// Ustawienie parametrów Ÿród³a
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, att_constant);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, att_linear);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, att_quadratic);

	/*************************************************************************************/
	// Ustawienie opcji systemu oœwietlania sceny 

	glShadeModel(GL_SMOOTH); // w³aczenie ³agodnego cieniowania
	glEnable(GL_LIGHTING);   // w³aczenie systemu oœwietlenia sceny 
	glEnable(GL_LIGHT0);     // w³¹czenie Ÿród³a o numerze 0
	glEnable(GL_DEPTH_TEST); // w³¹czenie mechanizmu z-bufora 
							 /*************************************************************************************/

							 // Przeczytanie obrazu tekstury z pliku o nazwie tekstura.tga
	pBytes = LoadTGAImage("P4_t.tga", &ImWidth, &ImHeight, &ImComponents, &ImFormat);

	// Zdefiniowanie tekstury 2-D 
	glTexImage2D(GL_TEXTURE_2D, 0, ImComponents, ImWidth, ImHeight, 0, ImFormat, GL_UNSIGNED_BYTE, pBytes);

	// Zwolnienie pamiêci   
	free(pBytes);

	// W³¹czenie mechanizmu teksturowania
	glEnable(GL_TEXTURE_2D);

	// Ustalenie trybu teksturowania
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// Okreœlenie sposobu nak³adania tekstur
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


// Funkcja ma za zadanie utrzymanie sta³ych proporcji rysowanych 
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokoœæ i szerokoœæ okna) s¹ 
// przekazywane do funkcji za ka¿dym razem gdy zmieni siê rozmiar okna.
void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle_x = 360.0*0.1 / (float)horizontal;
	pix2angle_y = 360.0*0.1 / (float)vertical;

	glMatrixMode(GL_PROJECTION);
	// Prze³¹czenie macierzy bie¿¹cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie¿¹cej

	// Ustawienie parametrów dla rzutu perspektywicznego
	gluPerspective(70, (float)horizontal / (float)vertical, 1.0, 30.0);    //ustawienie wielkosci okna, aby nie "obcinalo" obiektu po prawej i lewej stronie

	glViewport(0, 0, (float)horizontal, (float)vertical);
	// Ustawienie wielkoœci okna okna widoku (viewport) w zale¿noœci
	// relacji pomiêdzy wysokoœci¹ i szerokoœci¹ okna

	glMatrixMode(GL_MODELVIEW);
	// Prze³¹czenie macierzy bie¿¹cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie¿¹cej
}


int main(int argc, char** argv)
{
	tab = new point3*[N + 1];    //alokacja tablicy punktow
	for (int i = 0; i<N + 1; i++)
	{
		tab[i] = new point3[N + 1];
	}

	tabx = new point3*[N + 1];    //alokacja tablicy wektorow normalnych
	for (int i = 0; i < N + 1; i++)
	{
		tabx[i] = new point3[N + 1];
	}

	tabteks = new point2*[N + 1]; // alokacja tablicy tesktur
	for (int i = 0; i < N + 1; i++)
	{
		tabteks[i] = new point2[N + 1];
	}

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInit(&argc, argv);

	glutInitWindowSize(800, 600);

	glutCreateWindow("OpenGL");

	glutDisplayFunc(RenderScene);
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹
	// (callback function).  Bedzie ona wywo³ywana za ka¿dym razem 
	// gdy zajdzie potrzba przeryswania okna

	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹
	// zazmiany rozmiaru okna  

	glutMouseFunc(Mouse);
	// Ustala funkcjê zwrotn¹ odpowiedzialn¹ za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcjê zwrotn¹ odpowiedzialn¹ za badanie ruchu myszy

	glutIdleFunc(spinEgg);
	// Ustala funkcjê zwrotn¹ odpowiedzialn¹ za obracanie sie jajka

	MyInit();
	// Funkcja MyInit() (zdefiniowana powy¿ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst¹pieniem do renderowania

	glEnable(GL_DEPTH_TEST);
	// W³¹czenie mechanizmu usuwania powierzchni niewidocznych

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}