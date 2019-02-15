#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>
#include <ctime>
#include <string>
#include <iostream>

static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
// inicjalizacja położenia obserwatora

static GLfloat	fi = 0.0,		// kąty obrotu, elewacja i azymut
theta = 0.0;
static GLfloat	pix2angle_x,	// przelicznik pikseli na stopnie
pix2angle_y;
static GLint	status = 0;		// stan klawiszy myszy
								// 0 - nie naciśnięto żadnego klawisza
								// 1 - naciśnięty został lewy klawisz, 2 - prawy
static int	x_pos_old = 0,		// poprzednia pozycja kursora myszy
y_pos_old = 0;
static int	delta_x = 0,		// różnica pomiędzy pozycją bieżącą
delta_y = 0;        // i poprzednią kursora myszy

float viewerR = 30.0;	//Promien sfery obserwatora



//punkt w 3D
typedef float point3[3];

//długość boku kwadratu jednostkowego
float squareSize = 1.0;

//ilość punktów, dowolnie zmieniana
int N = 100;

//Stala PI
const float PI = 3.14159265;

//Tablica na punkty
point3 **pointsTable;

//Tablica na kolory
point3 **colorsTable;

//typ wyświetlania danych, defaultowo ustawiony na punkty
int model = 3;  // 1- punkty, 2- siatka, 3 - wypełnione trójkąty



//*****wyliczanie punktów X, Y, Z wzorami z instrukcji******

//Współrzędne punktu X - (u,v) w przestrzeni 3D
float xAxisPoint(float u, float v) {
return  (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * cos(v*PI);
}

//Współrzędne punktu Y - (u,v) w przestrzeni 3D
float yAxisPoint(float u) {
return  160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2) - 5; // -5 na końcu, aby było przesunięte na środek osi
}

//Współrzędna punktu Z - (u,v) w przestrzeni 3D
float zAxisPoint(float u, float v) {
	return (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * sin(v*PI);
}


//Funkcja generujaca siatkę punktów
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

	//przełączanie typu powierzchnii
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
		//łączymy punkty (pion, poziom, na skos prawo, na skos lewo)
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
	// początek i koniec obrazu osi x

	point3  y_min = { 0.0, -8.0, 0.0 };
	point3  y_max = { 0.0,  8.0, 0.0 };
	// początek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -8.0 };
	point3  z_max = { 0.0, 0.0,  8.0 };
	//  początek i koniec obrazu osi y


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
// Funkcja określająca co ma być rysowane (zawsze wywoływana gdy trzeba
// przerysować scenę)

void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszczącym
	glLoadIdentity();
	// Czyszczenie macierzy bieżącej



	if (status == 1) {	// jeśli lewy klawisz myszy wciśnięty
		theta += delta_x*pix2angle_x;		// modyfikacja kąta obrotu o kąt proporcjonalny
		fi += delta_y*pix2angle_y;		// do różnicy położeń kursora myszy

		if (theta >= 360.0)
			theta = 0.0;
		if (fi >= 360.0)
			fi = 0.0;
	}
	else if (status == 2) {	// jeśli prawy klawisz myszy wciśnięty
		viewerR += 0.1* delta_y;	// modyfikacja polozenia obserwatora(zoom)

		if (viewerR <= 5)	// ograniczenie zblizenia
			viewerR = 5;
		if (viewerR >= 57.6)	// ograniczenie oddalenia
			viewerR = 57.6;
	}


	//Wspolrzedne obserwatora - wzorki z ZSK
	viewer[0] = viewerR * cos(theta) * cos(fi);
	viewer[1] = viewerR * sin(fi);
	viewer[2] = viewerR * sin(theta) * cos(fi);


	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, cos(fi), 0.0);
	// Zdefiniowanie położenia obserwatora

	//Axes();
	// Narysowanie osi przy pomocy funkcji zdefiniowanej powyżej
	glRotatef(theta, 0.0, 1.0, 0.0);  //obrót obiektu o nowy kąt
	glRotatef(fi, 1.0, 0.0, 0.0);

	//Renderowanie jajka
	CreateEgg();

	glFlush();
	// Przekazanie poleceń rysujących do wykonania

	glutSwapBuffers();
}

//Zmiana wyświetlania za pomocą klawiszy
void Keys(unsigned char key, int x, int y)
{
	if (key == 'p') model = 1;
	if (key == 'w') model  = 2;
	if (key == 's') model  = 3;
	RenderScene();
}



/*************************************************************************************/
// Funkcja ustalająca stan renderowania

void MyInit(void)
{
	// Kolor czyszcący (wypełnienia okna) ustawiono na czarny
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

}
/*************************************************************************************/
// Funkcja ma za zadanie utrzymanie stałych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokość i szerokość okna) są
// przekazywane do funkcji za każdym razem gdy zmieni się rozmiar okna.

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{

	pix2angle_x = 360.0 *0.1/ (float)horizontal;
	pix2angle_y = 360.0 * 0.1/ (float)horizontal;

	glViewport(0, 0, horizontal, vertical);

	glMatrixMode(GL_PROJECTION);
	// Przełączenie macierzy bieżącej na macierz projekcji

	glLoadIdentity();
	// Czyszcznie macierzy bieżącej

	GLfloat aspect = (GLfloat)horizontal / (GLfloat)vertical;
	gluPerspective(70, aspect, 1.0, 60.0);
	// Ustawienie parametrów dla rzutu perspektywicznego

	if (horizontal <= vertical)
		glViewport(1.0, 1.0, -1.0 / aspect, 1.0 / aspect);

	else
		glViewport(-1.0*aspect, 1.0 * aspect, -1.0, 1.0);

	// Ustawienie wielkości okna okna widoku (viewport) w zależności
	// relacji pomiędzy wysokością i szerokością okna

	glMatrixMode(GL_MODELVIEW);
	// Przełączenie macierzy bieżącej na macierz widoku modelu

	glLoadIdentity();
	// Czyszczenie macierzy bieżącej
}
// Funkcja "bada" stan myszy i ustawia wartosci odpowiednich zmiennych globalnych
void Mouse(int btn, int state, int x, int y) {


	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		x_pos_old = x;        // przypisanie aktualnie odczytanej pozycji kursora
		y_pos_old = y;        // jako pozycji poprzedniej
		status = 1;         // wciśnięty został lewy klawisz myszy
	}
	else if (btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		y_pos_old = y;		// przypisanie aktualnie odczytanej pozycji kursora
							// jako pozycji poprzedniej
		status = 2;			//wciśnięty został prawy klawisz myszy
	}
	else
		status = 0;         // nie został wciśnięty żaden klawisz
}

// Funkcja "monitoruje" polozenie kursora myszy i ustawia wartosci odpowiednich
// zmiennych globalnych
void Motion(GLsizei x, GLsizei y) {
	delta_x = x - x_pos_old;    // obliczenie różnicy położenia kursora myszy
	x_pos_old = x;	// podstawienie bieżacego położenia jako poprzednie

	delta_y = y - y_pos_old;	// obliczenie różnicy położenia kursora myszy
	y_pos_old = y;	// podstawienie bieżacego położenia jako poprzednie

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

int main(int argc, char ** argv)
{
    glutInit(&argc, argv);

	AllocatePoints();
	AllocateColorTable();
	RandomizeColors();

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(800, 600);

	glutCreateWindow("Rzutowanie perspektywyczne, interakcja, jajko");

	glutDisplayFunc(RenderScene);
	// Określenie, że funkcja RenderScene będzie funkcją zwrotną
	// (callback function).  Bedzie ona wywoływana za każdym razem
	// gdy zajdzie potrzba przeryswania okna

	// Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną
	// zazmiany rozmiaru okna
	glutReshapeFunc(ChangeSize);

	MyInit();
	// Funkcja MyInit() (zdefiniowana powyżej) wykonuje wszelkie
	// inicjalizacje konieczne  przed przystąpieniem do renderowania

	glEnable(GL_DEPTH_TEST);
	// Włączenie mechanizmu usuwania powierzchni niewidocznych

	glutMouseFunc(Mouse);
	// Ustala funkcję zwrotną odpowiedzialną za badanie stanu myszy

	glutMotionFunc(Motion);
	// Ustala funkcję zwrotną odpowiedzialną za badanie ruchu myszy

	//Rejestracja funkcji zwrotnej dla klawiatury
	glutKeyboardFunc(Keys);

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}
/*************************************************************************************/
