/**
* GRAFIKA KOMPUTEROWA
* 
* Sterowanie:
*	p - przelaczanie typu powierzchni: punkty
*	w - przelaczanie typu powierzchni: siatka
*	s - przelaczanie typu powierzchni: kolorowe trojkaty
*
**/

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>
#include <ctime>
#include <string>
#include <iostream>
using namespace std;

static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
// inicjalizacja po³o¿enia obserwatora

static GLfloat	fi = 0.0,		// k¹ty obrotu, elewacja i azymut
theta = 0.0;
static GLfloat	pix2angle_x,	// przelicznik pikseli na stopnie
pix2angle_y;
static GLint	status = 0;		// stan klawiszy myszy
								// 0 - nie naciœniêto ¿adnego klawisza
								// 1 - naciœniêty zosta³ lewy klawisz, 2 - prawy
static int	x_pos_old = 0,		// poprzednia pozycja kursora myszy
y_pos_old = 0;
static int	delta_x = 0,		// ró¿nica pomiêdzy pozycj¹ bie¿¹c¹
delta_y = 0;        // i poprzedni¹ kursora myszy

float viewerR = 30.0;	//Promien sfery obserwatora



//punkt w 3D
typedef float point3[3];

//d³ugoœæ boku kwadratu jednostkowego
float squareSize = 1.0;

//iloœæ punktów, dowolnie zmieniana
int N = 30; 

//Stala PI
const float PI = 3.14159265;

//Tablica na punkty
point3 **pointsTable;

//Tablica na kolory
point3 **colorsTable;

//typ wyœwietlania danych, defaultowo ustawiony na punkty
int model = 3;  // 1- punkty, 2- siatka, 3 - wype³nione trójk¹ty



//*****wyliczanie punktów X, Y, Z wzorami z instrukcji******

//Wspó³rzêdne punktu X - (u,v) w przestrzeni 3D
float xAxisPoint(float u, float v) {
return  (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * cos(v*PI);
}

//Wspó³rzêdne punktu Y - (u,v) w przestrzeni 3D
float yAxisPoint(float u) {
return  160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2) - 5; // -5 na koñcu, aby by³o przesuniête na œrodek osi
}

//Wspó³rzêdna punktu Z - (u,v) w przestrzeni 3D
float zAxisPoint(float u, float v) {
	return (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * sin(v*PI);
}


//Funkcja generujaca siatkê punktów
void createPointsGrid() {
	
	float stepXY = squareSize / N;

	//Przypisanie punktom wspolrzednych
	for (int i = 0; i<N + 1; i++) {
		for (int j = 0; j<N + 1; j++) {
			pointsTable[i][j][0] = xAxisPoint(j*stepXY, i*stepXY);
			pointsTable[i][j][1] = yAxisPoint(j*stepXY);
			pointsTable[i][j][2] = zAxisPoint(j*stepXY, i*stepXY);
		}
	}
}

void CreateEgg(void) {

	// Stworzenie siatki punktów 3d
	createPointsGrid();
	
	//kolor jajka
	glColor3f(1.000, 0.855, 0.725);

	//prze³¹czanie typu powierzchnii
	switch (model) {
		//PUNKTY
	case 1: {
		glBegin(GL_POINTS);
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				glVertex3fv(pointsTable[i][j]);
			}
		}
		glEnd();
	} break;
	case 2: {
		//SIATKA
		//³¹czymy punkty (pion, poziom, na skos prawo, na skos lewo)
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				//Pion
				glBegin(GL_LINES);
				glVertex3fv(pointsTable[i][j]);
				glVertex3fv(pointsTable[i][j + 1]);
				glEnd();
				//Poziom
				glBegin(GL_LINES);
				glVertex3fv(pointsTable[i][j]);
				glVertex3fv(pointsTable[i + 1][j]);
				glEnd();
				//Prawo
				glBegin(GL_LINES);
				glVertex3fv(pointsTable[i][j]);
				glVertex3fv(pointsTable[i + 1][j + 1]);
				glEnd();
				//Lewo
				glBegin(GL_LINES);
				glVertex3fv(pointsTable[i + 1][j]);
				glVertex3fv(pointsTable[i][j + 1]);
				glEnd();
			}
		}
	} break;
	case 3: {
			//KOLOROWE TROJKATY
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < N; j++) {
				//W jedna strone
				glBegin(GL_TRIANGLES);
				glColor3fv(colorsTable[i][j + 1]);
				glVertex3fv(pointsTable[i][j + 1]);
				glColor3fv(colorsTable[i + 1][j]);
				glVertex3fv(pointsTable[i + 1][j]);
				glColor3fv(colorsTable[i + 1][j + 1]);
				glVertex3fv(pointsTable[i + 1][j + 1]);
				glEnd();
				//W druga strone
				glBegin(GL_TRIANGLES);
				glColor3fv(colorsTable[i][j]);
				glVertex3fv(pointsTable[i][j]);
				glColor3fv(colorsTable[i + 1][j]);
				glVertex3fv(pointsTable[i + 1][j]);
				glColor3fv(colorsTable[i][j + 1]);
				glVertex3fv(pointsTable[i][j + 1]);
				glEnd();
			}
		}
	}
	}
}
	



void Axes(void)
{
	point3  x_min = { -8.0, 0.0, 0.0 };
	point3  x_max = { 8.0, 0.0, 0.0 };
	// pocz¹tek i koniec obrazu osi x

	point3  y_min = { 0.0, -8.0, 0.0 };
	point3  y_max = { 0.0,  8.0, 0.0 };
	// pocz¹tek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -8.0 };
	point3  z_max = { 0.0, 0.0,  8.0 };
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
/*************************************************************************************/
// Funkcja okreœlaj¹ca co ma byæ rysowane (zawsze wywo³ywana gdy trzeba 
// przerysowaæ scenê)

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym
	glLoadIdentity();
	// Czyszczenie macierzy bie¿¹cej

	

	if (status == 1) {	// jeœli lewy klawisz myszy wciœniêty
		theta += delta_x*pix2angle_x;		// modyfikacja k¹ta obrotu o k¹t proporcjonalny
		fi += delta_y*pix2angle_y;		// do ró¿nicy po³o¿eñ kursora myszy

		if (theta >= 360.0)
			theta = 0.0;
		if (fi >= 360.0)
			fi = 0.0;
	}
	else if (status == 2) {	// jeœli prawy klawisz myszy wciœniêty
		viewerR += 0.1* delta_y;	// modyfikacja polozenia obserwatora(zoom)
		cout << viewerR;
		cout << "\n";
		if (viewerR <= 9)	// ograniczenie zblizenia
			viewerR = 9;
		if (viewerR >= 57.6)	// ograniczenie oddalenia
			viewerR = 57.6;
	}


	//Wspolrzedne obserwatora - wzorki z ZSK
	viewer[0] = viewerR * cos(theta) * cos(fi);
	viewer[1] = viewerR * sin(fi);
	viewer[2] = viewerR * sin(theta) * cos(fi);

	
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, cos(fi), 0.0);
	// Zdefiniowanie po³o¿enia obserwatora

	Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej powy¿ej
	glRotatef(theta, 0.0, 1.0, 0.0);  //obrót obiektu o nowy k¹t
	glRotatef(fi, 1.0, 0.0, 0.0);

	//Renderowanie jajka
	CreateEgg();

	glFlush();
	// Przekazanie poleceñ rysuj¹cych do wykonania

	glutSwapBuffers();
}

//Zmiana wyœwietlania za pomoc¹ klawiszy
void Keys(unsigned char key, int x, int y)
{
	if (key == 'p') model = 1;
	if (key == 'w') model  = 2;
	if (key == 's') model  = 3;
	RenderScene();
}



/*************************************************************************************/
// Funkcja ustalaj¹ca stan renderowania

void MyInit(void)
{
	// Kolor czyszc¹cy (wype³nienia okna) ustawiono na czarny
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
}
/*************************************************************************************/
// Funkcja ma za zadanie utrzymanie sta³ych proporcji rysowanych 
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokoœæ i szerokoœæ okna) s¹ 
// przekazywane do funkcji za ka¿dym razem gdy zmieni siê rozmiar okna.

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{

	pix2angle_x = 360.0 *0.1/ (float)horizontal;
	pix2angle_y = 360.0 * 0.1/ (float)horizontal;

	glViewport(0, 0, horizontal, vertical);

	glMatrixMode(GL_PROJECTION);
	// Prze³¹czenie macierzy bie¿¹cej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bie¿¹cej 

	GLfloat aspect = (GLfloat)horizontal / (GLfloat)vertical;
	gluPerspective(70, aspect, 1.0, 60.0);
	// Ustawienie parametrów dla rzutu perspektywicznego



	if (horizontal <= vertical)
		glViewport(1.0, 1.0, -1.0 / aspect, 1.0 / aspect);

	else
		glViewport(-1.0*aspect, 1.0 * aspect, -1.0, 1.0);

	// Ustawienie wielkoœci okna okna widoku (viewport) w zale¿noœci
	// relacji pomiêdzy wysokoœci¹ i szerokoœci¹ okna

	glMatrixMode(GL_MODELVIEW);
	// Prze³¹czenie macierzy bie¿¹cej na macierz widoku modelu  

	glLoadIdentity();
	// Czyszczenie macierzy bie¿¹cej 
}
// Funkcja "bada" stan myszy i ustawia wartosci odpowiednich zmiennych globalnych
void Mouse(int btn, int state, int x, int y) {

	
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		x_pos_old = x;        // przypisanie aktualnie odczytanej pozycji kursora
		y_pos_old = y;        // jako pozycji poprzedniej
		status = 1;         // wciœniêty zosta³ lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		y_pos_old = y;		// przypisanie aktualnie odczytanej pozycji kursora
							// jako pozycji poprzedniej
		status = 2;			//wciœniêty zosta³ prawy klawisz myszy
	}
	else
		status = 0;         // nie zosta³ wciœniêty ¿aden klawisz
}

// Funkcja "monitoruje" polozenie kursora myszy i ustawia wartosci odpowiednich 
// zmiennych globalnych
void Motion(GLsizei x, GLsizei y) {
	delta_x = x - x_pos_old;    // obliczenie ró¿nicy po³o¿enia kursora myszy
	x_pos_old = x;	// podstawienie bie¿acego po³o¿enia jako poprzednie

	delta_y = y - y_pos_old;	// obliczenie ró¿nicy po³o¿enia kursora myszy
	y_pos_old = y;	// podstawienie bie¿acego po³o¿enia jako poprzednie

	glutPostRedisplay();	// przerysowanie obrazu sceny
}


//Alokacja tablicy punktów
void AllocatePoints()
{
	pointsTable = new point3*[N + 1];
	for (int i = 0; i<N + 1; i++) {
		pointsTable[i] = new point3[N + 1];
	}
}

//Alokacja tablicy kolorów
void AllocateColorTable()
{
	colorsTable = new point3*[N + 1];
	for (int i = 0; i < N + 1; i++) {
		colorsTable[i] = new point3[N + 1];
	}
}

void RandomizeColors()
{
	for (int i = 0; i < N + 1; i++) {
		for (int j = 0; j < N + 1; j++) {
			colorsTable[i][j][0] = ((float)(rand() % 10) + 1) / 10;
			colorsTable[i][j][1] = ((float)(rand() % 10) + 1) / 10;
			colorsTable[i][j][2] = ((float)(rand() % 10) + 1) / 10;
		}
	}
}

void main(void)
{

	AllocatePoints();
	AllocateColorTable();
	RandomizeColors();

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(800, 600);

	glutCreateWindow("Rzutowanie perspektywyczne, interakcja, jajko");

	glutDisplayFunc(RenderScene);
	// Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹
	// (callback function).  Bedzie ona wywo³ywana za ka¿dym razem 
	// gdy zajdzie potrzba przeryswania okna

	// Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹
	// zazmiany rozmiaru okna  
	glutReshapeFunc(ChangeSize);

	MyInit();
	// Funkcja MyInit() (zdefiniowana powy¿ej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przyst¹pieniem do renderowania 

	glEnable(GL_DEPTH_TEST);
	// W³¹czenie mechanizmu usuwania powierzchni niewidocznych

	glutMouseFunc(Mouse);
	// Ustala funkcjê zwrotn¹ odpowiedzialn¹ za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcjê zwrotn¹ odpowiedzialn¹ za badanie ruchu myszy

	//Rejestracja funkcji zwrotnej dla klawiatury
	glutKeyboardFunc(Keys);

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}
/*************************************************************************************/
