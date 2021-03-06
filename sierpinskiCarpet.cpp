/*************************************************************************************/

// Trochę bardziej skomplikowany program wykorzystujący funkcje biblioteki OpenGL

/*************************************************************************************/

#include "pch.h"

#include <windows.h>

#include <gl/gl.h>

#include <gl/glut.h>
#include <vector>

/*************************************************************************************/


// Funkcaja określająca, co ma być rysowane 
// (zawsze wywoływana, gdy trzeba przerysować scenę)


typedef float Point[2];

float getRand()
{
	return float(rand() % 1000) / 1000;
}

void drawRectangle(Point a, Point b, Point c, Point d)
{
	glBegin(GL_POLYGON);
	glColor3f(getRand(), getRand(), getRand());
	glVertex2fv(a);
	glColor3f(getRand(), getRand(), getRand());
	glVertex2fv(b);
	glColor3f(getRand(), getRand(), getRand());
	glVertex2fv(c);
	glColor3f(getRand(), getRand(), getRand());
	glVertex2fv(d);
	glEnd();
}

void drawSierpinskiCarpet(Point ld, Point rd, int level) {

	float width = (rd[0] - ld[0]) / 3;
	width += (getRand() * 2 - 1) / 5;

	float left, right;

	for (int i = 0; i < 3; i++) {

		for (int j = 0; j < 3; j++) {

			left = ld[0] + (width * i);
			right = ld[1] + (width * j);
			Point newLu = { left, right };
			right += width;
			Point newLd = { left, right };
			left += width;
			Point newRu = { left, right };
			right -= width;
			Point newRd = { left, right };

			if ((i != 1) || (j != 1)) {
				if (level == 1)
				{
					drawRectangle(newLu, newLd, newRu, newRd);
				}
				else
				{
					drawSierpinskiCarpet(newLd, newRd, level - 1);
				}
			}
		}
	}

}
void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	Point ld = { -50, -50 };
	Point rd = { 50, -50 };

	int level = 3;

	drawSierpinskiCarpet(ld, rd, level);
	glFlush();
}
// Funkcja ustalająca stan renderowania



void MyInit(void)

{

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	// Kolor okna wnętrza okna - ustawiono na szary

}

/*************************************************************************************/

// Funkcja służąca do kontroli zachowania proporcji rysowanych obiektów
// niezależnie od rozmiarów okna graficznego



void ChangeSize(GLsizei horizontal, GLsizei vertical)

// Parametry horizontal i vertical (szerokość i wysokość okna) są
// przekazywane do funkcji za każdym razem, gdy zmieni się rozmiar okna

{

	GLfloat AspectRatio;

	// Deklaracja zmiennej AspectRatio określającej proporcję wymiarów okna



	if (vertical == 0)
		// Zabezpieczenie pzred dzieleniem przez 0

		vertical = 1;


	glViewport(0, 0, horizontal, vertical);
	// Ustawienie wielkościokna okna urządzenia (Viewport)
	// W tym przypadku od (0,0) do (horizontal, vertical)


	glMatrixMode(GL_PROJECTION);
	// Określenie układu współrzędnych obserwatora

	glLoadIdentity();
	// Określenie przestrzeni ograniczającej

	AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
	// Wyznaczenie współczynnika proporcji okna

	// Gdy okno na ekranie nie jest kwadratem wymagane jest 
	// określenie okna obserwatora. 
	// Pozwala to zachować właściwe proporcje rysowanego obiektu
	// Do określenia okna obserwatora służy funkcja glOrtho(...)



	if (horizontal <= vertical)

		glOrtho(-100.0, 100.0, -100.0 / AspectRatio, 100.0 / AspectRatio, 1.0, -1.0);

	else

		glOrtho(-100.0*AspectRatio, 100.0*AspectRatio, -100.0, 100.0, 1.0, -1.0);

	glMatrixMode(GL_MODELVIEW);
	// Określenie układu współrzędnych    

	glLoadIdentity();

}

/*************************************************************************************/

// Główny punkt wejścia programu. Program działa w trybie konsoli



void main(void)

{

	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// Ustawienie trybu wyświetlania
	// GLUT_SINGLE - pojedynczy bufor wyświetlania
	// GLUT_RGBA - model kolorów RGB



	glutCreateWindow("Drugi program w OpenGL");
	// Utworzenie okna i określenie treści napisu w nagłówku okna


	glutDisplayFunc(RenderScene);
	// Określenie, że funkcja RenderScene będzie funkcją zwrotną (callback)
	// Biblioteka GLUT będzie wywoływała tą funkcję za każdym razem, gdy
	// trzeba będzie przerysować okno


	glutReshapeFunc(ChangeSize);
	// Dla aktualnego okna ustala funkcję zwrotną odpowiedzialną za
	// zmiany rozmiaru okna

	MyInit();
	// Funkcja MyInit (zdefiniowana powyżej) wykonuje wszelkie  
	// inicjalizacje konieczneprzed przystąpieniem do renderowania


	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT

}

/*************************************************************************************/