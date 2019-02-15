/*************************************************************************************/
// Szkielet programu do tworzenia modelu sceny 3-D z wizualizacją osi
// układu współrzędnych dla rzutowania perspektywicznego
/*************************************************************************************/


#include <stdlib.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <vector>
#include <cmath>
#include <functional>
#define M_PI 3.14159265358979323846

using namespace std;

typedef float point3[3];
static GLfloat viewer[] = { 0.0, 0.0, 10.0 };
static GLfloat	pix2angle_x;
static GLfloat	pix2angle_y;
// inicjalizacja położenia obserwatora
static GLfloat pom = 1.0;
static GLfloat thetax = 0.0; // kąt obrotu obiektu osi X
static GLfloat thetay = 0.0; // / kąt obrotu obiektu osi Y
static GLfloat theta_zoom = 10.0;// kat obrotu obiektu
static GLfloat pix2angle; // przelicznik pikseli na stopnie
static GLint status = 0; // stan klawiszy myszy
						 // 0 - nie nacisnieto zadnego klawisza
						 // 1 - nacisniety zostal lewy klawisz
static int x_pos_old = 0; // X poprzednia pozycja kursora myszy
static int y_pos_old = 0; // Y poprzednia pozycja kursora myszy
static int zoom = 0;// przyblizenie poprzednia pozycja kursora myszy
static int delta_x = 0; // roznica pomiedzy pozycja biezaca
static int delta_y = 0; // i poprzednia kursora myszy
static int delta_zoom = 0; // roznica pomiedzy pozycja biezaca
						   // i poprzednie kursora myszy

struct Punkty
{
	float x = 0;
	float y = 0;
	float z = 0;
	int R = 0;
};

int model = 1;
int toruses = 1;
const int N = 100;
const double R = 2.0;
const double r = 1.0;

//wektor punktow
vector<vector< Punkty > > tab(N + 1, vector< Punkty >(N + 1));

/*************************************************************************************/
// Funkcja określająca co ma być rysowane (zawsze wywoływana, gdy trzeba
// przerysować scenę)
/*************************************************************************************/

// Funkcja "bada" stan myszy i ustawia wartości odpowiednich zmiennych globalnych
void Mouse(int btn, int state, int x, int y)
{
	if (btn == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		x_pos_old = x; // przypisanie aktualnie odczytanej pozycji kursora
		y_pos_old = y; // jako pozycji poprzedniej
		status = 1; // wcisniety zostal lewy klawisz myszy
	}
	else if
		(
			btn == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		zoom = y;
		status = 2;
	}
	else
		status = 0; // nie zostal wcisniety zaden klawisz
}
/*************************************************************************************/
// Funkcja "monitoruje" położenie kursora myszy i ustawia wartości odpowiednich
// zmiennych globalnych
void Motion(GLsizei x, GLsizei y)
{
	delta_x = x - x_pos_old; // obliczenie roznicy polozenia kursora myszy
	x_pos_old = x;// podstawienie bieżącego położenia jako poprzednie
	delta_y = y - y_pos_old;// obliczenie roznicy polozenia kursora myszy
	y_pos_old = y;// podstawienie bieżącego położenia jako poprzednie
	delta_zoom = y - zoom;// obliczenie roznicy polozenia kursora myszy
	zoom = y;// podstawienie bieżącego położenia jako poprzednie
	glutPostRedisplay(); // przerysowanie obrazu sceny
}
// Funkcja rysująca osie układu wspolrzednych
/*************************************************************************************/
void Axes(void)//rysowanie osi wspołrzędnych
{
	point3 x_min = { -5.0, 0.0, 0.0 };
	point3 x_max = { 5.0, 0.0, 0.0 };
	// poczatek i koniec obrazu osi x
	point3 y_min = { 0.0, -7.0, 0.0 };
	point3 y_max = { 0.0, 7.0, 0.0 };
	// poczatek i koniec obrazu osi y
	point3 z_min = { 0.0, 0.0, -5.0 };
	point3 z_max = { 0.0, 0.0, 5.0 };
	// poczatek i koniec obrazu osi y
	glColor3f(1.0f, 0.0f, 0.0f); // kolor rysowania osi - czerwony
	glBegin(GL_LINES); // rysowanie osi x
	glVertex3fv(x_min);
	glVertex3fv(x_max);
	glEnd();
	glColor3f(0.0f, 1.0f, 0.0f); // kolor rysowania - zielony
	glBegin(GL_LINES); // rysowanie osi y
	glVertex3fv(y_min);
	glVertex3fv(y_max);
	glEnd();
	glColor3f(0.0f, 0.0f, 1.0f); // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z
	glVertex3fv(z_min);
	glVertex3fv(z_max);
	glEnd();
}
/*************************************************************************************/
//Rysowanie Torusa
void Torus(void)
{
	const float par = N / static_cast<float>(N * N);
	for (int i = 0; i < N + 1; ++i)
	{
		for (int j = 0; j < N + 1; ++j)
		{
			tab[i][j].x = (par * i);
			tab[i][j].y = (par * j);
		}
	} for (int i = 0; i<N + 1; i++)
	{
		for (int j = 0; j < N + 1; j++)
		{
			float u = tab[i][j].x;
			float v = tab[i][j].y;
			tab[i][j].x = (R + (r * cos(2 * M_PI*v))) * cos(2 * M_PI*u);
			tab[i][j].y = (R + (r * cos(2 * M_PI*v))) * sin(2 * M_PI*u);
			tab[i][j].z = (r * sin(2 * M_PI*v));
		}
	}
	switch (model)
	{
	case 1://rysowanie chmury punktow
	{
		glColor3f(0.0f, 1.0f, 0.0f);
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				float x = tab[i][j].x;
				float y = tab[i][j].y;
				float z = tab[i][j].z;
				glBegin(GL_POINTS);
				glVertex3f(x, y, z);
				glEnd();
			}
		}
		break;
	} case 2://rysowanie siatki
	{
		glColor3f(0.0f, 1.0f, 0.0f);
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				glBegin(GL_LINES);
				glVertex3f(tab[i][j].x, tab[i][j].y, tab[i][j].z);
				glVertex3f(tab[i + 1][j].x, tab[i + 1][j].y, tab[i + 1][j].z);
				glVertex3f(tab[i][j].x, tab[i][j].y, tab[i][j].z);
				glVertex3f(tab[i][j + 1].x, tab[i][j + 1].y, tab[i][j + 1].z);
				glVertex3f(tab[i][j].x, tab[i][j].y, tab[i][j].z);
				glVertex3f(tab[i + 1][j + 1].x, tab[i + 1][j + 1].y, tab[i + 1][j + 1].z);
				glEnd();
			}
		}
		break;
	} case 3://rysowanie trojkatow siatki
	{
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				glBegin(GL_TRIANGLES);
				glColor3ub(0, 205, 0);
				glVertex3f(tab[i][j].x, tab[i][j].y, tab[i][j].z);
				glColor3ub(0, 175, 0);
				glVertex3f(tab[i + 1][j].x, tab[i + 1][j].y, tab[i + 1][j].z);
				glColor3ub(0, 145, 0);
				glVertex3f(tab[i][j + 1].x, tab[i][j + 1].y, tab[i][j + 1].z);
				glEnd();
				glBegin(GL_TRIANGLES);
				glColor3ub(0, 205, 0);
				glVertex3f(tab[i + 1][j].x, tab[i + 1][j].y, tab[i + 1][j].z);
				glColor3ub(0, 175, 0);
				glVertex3f(tab[i + 1][j + 1].x, tab[i + 1][j + 1].y, tab[i + 1][j + 1].z);
				glColor3ub(0, 145, 0);
				glVertex3f(tab[i][j + 1].x, tab[i][j + 1].y, tab[i][j + 1].z);
				glEnd();
			}
		}
		break;
	}
	}

}
/*************************************************************************************/
//funkcja określająca co ma być rysowane (zawsze wywoływana, gdy trzeba
//przerysować scenę)
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszczącym
	glLoadIdentity();
	// Czyszczenie macierzy biezacej
	gluLookAt(viewer[0], viewer[1], viewer[2], 0.0, 0.0, 0.0, 0.0, pom, 0.0);
	// Zdefiniowanie położenia obserwatora
	// Narysowanie osi przy pomocy funkcji zdefiniowanej powyżej
	if (status == 1) // jezli lewy klawisz myszy wcisniety
	{
		thetax += delta_x * pix2angle / 30.0;// modyfikacja kąta obrotu o kat proporcjonalny
		thetay += delta_y * pix2angle / 30.0;// modyfikacja kąta obrotu o kat proporcjonalny
	}
	else if (status == 2)// jezli prawy klawisz myszy wcisniety
	{
		theta_zoom += delta_zoom / 10.0;// modyfikacja kąta obrotu o kat proporcjonalny
	}
	if (thetay > M_PI) thetay -= 2 * M_PI;//sprawdzanie czy thetay jest wieksze od Pi
	else if (thetay <= -M_PI) thetay += 2 * M_PI;
	if (thetay > M_PI / 2 || thetay < -M_PI / 2)
	{
		pom = -1.0;
	}
	else
	{
		pom = 1.0;
	}
	viewer[0] = theta_zoom*cos(thetax)*cos(thetay);
	viewer[1] = theta_zoom*sin(thetay);
	viewer[2] = theta_zoom*sin(thetax)*cos(thetay);

	glTranslated(5, -6, 0);//przesuniecie
	glRotated(45.0, 0.0, 1.0, 3);//obrucenie
	Torus();//inarysowanie pierwszego torusa
	glRotated(90.0, 0, 1.0, 0);
	glTranslated(0, 3, 0);
	Torus();
	glRotated(90.0, 0, 1.0, 0);
	glTranslated(0, 3, 0);
	Torus();
	glRotated(90.0, 0, 1.0, 0);
	glTranslated(0, 3, 0);
	Torus();

	glFlush();
	// Przekazanie poleceń rysujących do wykonania
	glutSwapBuffers();
}
/*************************************************************************************/
//przypisanie klawiszy do konkretnych dzialan
void keys(unsigned char key, int x, int y)
{
	if (key == 'p') model = 1;//punkty
	if (key == 'w') model = 2;//siatka
	if (key == 's') model = 3;//wypełnione trojkaty
	if (key == '1') toruses = 1;//punkty
	if (key == '2') toruses = 2;//siatka
	if (key == '3') toruses = 3;//wypełnione trojkaty
	if (key == '4') toruses = 3;//wypełnione trojkaty
	RenderScene();
}
/*************************************************************************************/
// Funkcja ustalająca stan renderowania
void MyInit(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	// Kolor czyszczący (wypełnienia okna) ustawiono na czarny
}
/*************************************************************************************/
// Funkcja ma za zadanie utrzymanie stałych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokość i szerokość okna) są
// przekazywane do funkcji za każdym razem gdy zmieni się rozmiar okna.

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	pix2angle = 360.0 / (float)horizontal;// przeliczenie pikseli na stopnie
	glMatrixMode(GL_PROJECTION);
	// Przerzuczenie macierzy biezacej na macierz projekcji
	glLoadIdentity();
	// Czyszcznie macierzy biezacej
	gluPerspective(100.0, 1.0, 1.0, 30.0);
	// Ustawienie parametrow dla rzutu perspektywicznego
	if (horizontal <= vertical)
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);
	else
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
	// Ustawienie wielkosci okna okna widoku (viewport) w zaleznosci
	// relacji pomiedzy wysokoscia i szerokoscia okna
	glMatrixMode(GL_MODELVIEW);
	// Przerzuczenie macierzy biezacej na macierz widoku modelu
	glLoadIdentity();
	// Czyszczenie macierzy biezacej
}

/*void ChangeSize(GLsizei horizontal, GLsizei vertical) // !!! utrata możliwości obracania azymutem
{

	pix2angle_x = 360.0 *0.1 / (float)horizontal;
	pix2angle_y = 360.0 * 0.1 / (float)horizontal;

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
}*/
/*************************************************************************************/
// Główny punkt wejścia programu. Program działa w trybie konsoli
int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitWindowSize(800, 500);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutCreateWindow("Torus");
	glutMouseFunc(Mouse);
	// Ustala funkcje zwrotna odpowiedzialna za badanie stanu myszy
	glutMotionFunc(Motion);
	// Ustala funkcję zwrotną odpowiedzialną za badanie ruchu myszy
	glEnable(GL_DEPTH_TEST);
	// Włączenie mechanizmu usuwania niewidocznych elementów sceny
	glutKeyboardFunc(keys);
	//Funkcja ta rejestruje funkcję zwrotną wywoływaną przez biblitekę GLUT
	glutDisplayFunc(RenderScene);
	// Określenie, że funkcja RenderScene będzie funkcją zwrotną
	// (callback function). Będzie ona wywoływana za każdym razem
	// gdy zajdzie potrzeba przerysowania okna
	glutReshapeFunc(ChangeSize);
	MyInit();
	// Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną
	// za zmiany rozmiaru okna
	MyInit();
	// Funkcja MyInit() (zdefiniowana powyżej) wykonuje wszelkie
	// inicjalizacje konieczne przed przystąpieniem do renderowania
	glEnable(GL_DEPTH_TEST);
	// Włączenie mechanizmu usuwania niewidocznych elementów sceny
	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}

/*************************************************************************************/
