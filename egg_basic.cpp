/*************************************************************************************/

//  Szkielet programu do tworzenia modelu sceny 3-D z wizualizacj¹ osi 
//  uk³adu wspó³rzednych

/*************************************************************************************/
#define _USE_MATH_DEFINES

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>


typedef float point3[3];
struct Point
{
  float x, y, z;
};
static GLfloat theta[] = { 0.0, 0.0, 0.0 }; // trzy k¹ty obrotu
static int N = 20;
static Point** colors;
int model = 1;

float getRandom()
{
  return float(rand() % 1000) / 1000;
}


/*************************************************************************************/

// Funkcja rysuj¹ca osie uk³adu wspó³rzêdnych

Point getBezierCurve(float u, float v)
{
  float x, y, z;

  x = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * cos(M_PI * v);
  y = 160 * pow(u, 4) - 320 * pow(u, 3) + 160 * pow(u, 2);
  z = (-90 * pow(u, 5) + 225 * pow(u, 4) - 270 * pow(u, 3) + 180 * pow(u, 2) - 45 * u) * sin(M_PI * v);

  return { x, y, z }; //coordinates
}

Point** getBezierValues(int n)
{
  float interval = 1.0f / n;
  Point** bezierValues = new Point*[n + 1];

  for (size_t i = 0; i < n + 1; i++)
  {
    bezierValues[i] = new Point[n + 1];
  }
  
  for (size_t i = 0; i <= n; i++)
  {
    for (size_t j = 0; j <= n; j++)
    {
      float u = ((float)i) / n;
      float v = ((float)j) / n;

      bezierValues[i][j] = getBezierCurve(u, v);
    }
  }

  return bezierValues;
}

Point** getColors(int n)
{
  Point** colors = new Point*[n + 1];

  for (size_t i = 0; i < n + 1; i++)
  {
    colors[i] = new Point[n + 1];
  }

  for (size_t i = 0; i <= n; i++)
  {
    for (size_t j = 0; j <= n; j++)
    {
      colors[i][j] = {getRandom(), getRandom(), getRandom()};
    }
  }

  for (size_t i = 0; i < n/2; i++)
  {
    colors[i][n] = colors[n - i][0];
    colors[i][0] = colors[n - i][n];
  }

  return colors;
}

void drawEggFromPoints(int n, Point** bezierValues)
{
  glBegin(GL_POINTS);

  for (size_t i = 0; i <= n; i++)
  {
    for (size_t j = 0; j <= n; j++)
    {
      glVertex3f(bezierValues[i][j].x, bezierValues[i][j].y - 5, bezierValues[i][j].z);
    }
  }

  glEnd();
}


void drawEggFromLines(int n, Point** bezierValues)
{
  glBegin(GL_LINES);

  for (size_t i = 0; i < n; i++)
  {
    for (size_t j = 0; j < n; j++)
    {
      glVertex3f(bezierValues[i][j].x, bezierValues[i][j].y - 5, bezierValues[i][j].z);
      glVertex3f(bezierValues[i + 1][j].x, bezierValues[i + 1][j].y - 5, bezierValues[i + 1][j].z);
      glVertex3f(bezierValues[i][j].x, bezierValues[i][j].y - 5, bezierValues[i][j].z);
      glVertex3f(bezierValues[i][j + 1].x, bezierValues[i][j + 1].y - 5, bezierValues[i][j + 1].z);
      glVertex3f(bezierValues[i][j].x, bezierValues[i][j].y - 5, bezierValues[i][j].z);
      glVertex3f(bezierValues[i + 1][j + 1].x, bezierValues[i + 1][j + 1].y - 5, bezierValues[i + 1][j + 1].z);
    }
  }

  glEnd();
}

void drawEggFromTriangles(int n, Point** bezierValues, Point** colors)
{
  glBegin(GL_TRIANGLES);

  for (size_t i = 0; i < n; i++)
  {
    for (size_t j = 0; j < n; j++)
    {
      glColor3f(colors[i][j].x, colors[i][j].y, colors[i][j].z);
      glVertex3f(bezierValues[i][j].x, bezierValues[i][j].y - 5, bezierValues[i][j].z);
      glColor3f(colors[i + 1][j].x, colors[i + 1][j].y, colors[i + 1][j].z);
      glVertex3f(bezierValues[i + 1][j].x, bezierValues[i + 1][j].y - 5, bezierValues[i + 1][j].z);
      glColor3f(colors[i + 1][j + 1].x, colors[i + 1][j + 1].y, colors[i + 1][j + 1].z);
      glVertex3f(bezierValues[i + 1][j + 1].x, bezierValues[i + 1][j + 1].y - 5, bezierValues[i + 1][j + 1].z);
      
      glColor3f(colors[i][j + 1].x, colors[i][j + 1].y, colors[i][j + 1].z);
      glVertex3f(bezierValues[i][j + 1].x, bezierValues[i][j + 1].y - 5, bezierValues[i][j + 1].z);
      glColor3f(colors[i][j].x, colors[i][j].y, colors[i][j].z);
      glVertex3f(bezierValues[i][j].x, bezierValues[i][j].y - 5, bezierValues[i][j].z);
      glColor3f(colors[i + 1][j + 1].x, colors[i + 1][j + 1].y, colors[i + 1][j + 1].z);
      glVertex3f(bezierValues[i + 1][j + 1].x, bezierValues[i + 1][j + 1].y - 5, bezierValues[i + 1][j + 1].z);
    }
  }

  glEnd();
}


void drawEgg(int n)
{
  Point** bezierValues = getBezierValues(n);

  glRotatef(theta[0], 1.0, 0.0, 0.0);
  glRotatef(theta[1], 0.0, 1.0, 0.0);
  glRotatef(theta[2], 0.0, 0.0, 1.0);

  if (model == 1)
  {
    drawEggFromPoints(n, bezierValues);
  }
  else if (model == 2)
  {
    drawEggFromLines(n, bezierValues);
  }
  else
  {
    drawEggFromTriangles(n, bezierValues, colors);
  }
}

void spinEgg()
{
  theta[0] -= 0.5;
  if (theta[0] > 360.0) theta[0] -= 360.0;

  theta[1] -= 0.5;
  if (theta[1] > 360.0) theta[1] -= 360.0;

  theta[2] -= 0.5;
  if (theta[2] > 360.0) theta[2] -= 360.0;

  glutPostRedisplay(); //odœwie¿enie zawartoœci aktualnego okna
}

void Axes(void)
{

  point3  x_min = { -5.0, 0.0, 0.0 };
  point3  x_max = { 5.0, 0.0, 0.0 };
  // pocz¹tek i koniec obrazu osi x

  point3  y_min = { 0.0, -5.0, 0.0 };
  point3  y_max = { 0.0,  5.0, 0.0 };
  // pocz¹tek i koniec obrazu osi y

  point3  z_min = { 0.0, 0.0, -5.0 };
  point3  z_max = { 0.0, 0.0,  5.0 };
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

  Axes();
  // Narysowanie osi przy pomocy funkcji zdefiniowanej wy¿ej

  glColor3f(1.0f, 1.0f, 1.0f); // Ustawienie koloru rysowania na bia³y

  //glRotated(60.0, 1.0, 1.0, 1.0);  // Obrót o 60 stopni

  //glutWireTeapot(3.0); // Narysowanie obrazu czajnika do herbaty
  
  drawEgg(N);

  glutIdleFunc(spinEgg);

  glFlush();
  // Przekazanie poleceñ rysuj¹cych do wykonania


  glutSwapBuffers();
  //

}

/*************************************************************************************/

// Funkcja ustalaj¹ca stan renderowania


void MyInit(void)
{

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  // Kolor czyszc¹cy (wype³nienia okna) ustawiono na czarny

}

/*************************************************************************************/

// Funkcja ma za zadanie utrzymanie sta³ych proporcji rysowanych
// w przypadku zmiany rozmiarów okna.
// Parametry vertical i horizontal (wysokoœæ i szerokoœæ okna) s¹
// przekazywane do funkcji za ka¿dym razem gdy zmieni siê rozmiar okna.



void ChangeSize(GLsizei horizontal, GLsizei vertical)
{

  GLfloat AspectRatio;
  // Deklaracja zmiennej AspectRatio  okreœlaj¹cej proporcjê
  // wymiarów okna 

  if (vertical == 0)  // Zabezpieczenie przed dzieleniem przez 0

    vertical = 1;

  glViewport(0, 0, horizontal, vertical);
  // Ustawienie wielkoœciokna okna widoku (viewport)
  // W tym przypadku od (0,0) do (horizontal, vertical)  

  glMatrixMode(GL_PROJECTION);
  // Prze³¹czenie macierzy bie¿¹cej na macierz projekcji 

  glLoadIdentity();
  // Czyszcznie macierzy bie¿¹cej            

  AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;
  // Wyznaczenie wspó³czynnika  proporcji okna
  // Gdy okno nie jest kwadratem wymagane jest okreœlenie tak zwanej
  // przestrzeni ograniczaj¹cej pozwalaj¹cej zachowaæ w³aœciwe
  // proporcje rysowanego obiektu.
  // Do okreslenia przestrzeni ograniczj¹cej s³u¿y funkcja
  // glOrtho(...)            

  if (horizontal <= vertical)

    glOrtho(-7.5, 7.5, -7.5 / AspectRatio, 7.5 / AspectRatio, 10.0, -10.0);

  else

    glOrtho(-7.5*AspectRatio, 7.5*AspectRatio, -7.5, 7.5, 10.0, -10.0);

  glMatrixMode(GL_MODELVIEW);
  // Prze³¹czenie macierzy bie¿¹cej na macierz widoku modelu                                    

  glLoadIdentity();
  // Czyszcenie macierzy bie¿¹cej

}

/*************************************************************************************/

// G³ówny punkt wejœcia programu. Program dzia³a w trybie konsoli

void keys(unsigned char key, int x, int y)
{
  if (key == 'p') model = 1;
  if (key == 'w') model = 2;
  if (key == 's') model = 3;

  RenderScene(); // przerysowanie obrazu sceny
}

void main(void)
{
  colors = getColors(N);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  glutInitWindowSize(500, 500);

  glutCreateWindow("Uk³ad wspó³rzêdnych 3-D");

  glutDisplayFunc(RenderScene);
  // Okreœlenie, ¿e funkcja RenderScene bêdzie funkcj¹ zwrotn¹
  // (callback function).  Bedzie ona wywo³ywana za ka¿dym razem
  // gdy zajdzie potrzba przeryswania okna 

  glutReshapeFunc(ChangeSize);
  // Dla aktualnego okna ustala funkcjê zwrotn¹ odpowiedzialn¹
  // zazmiany rozmiaru okna      

  glutKeyboardFunc(keys);

  MyInit();
  // Funkcja MyInit() (zdefiniowana powy¿ej) wykonuje wszelkie
  // inicjalizacje konieczne  przed przyst¹pieniem do renderowania 

  glEnable(GL_DEPTH_TEST);
  // W³¹czenie mechanizmu usuwania powierzchni niewidocznych

  glutMainLoop();
  // Funkcja uruchamia szkielet biblioteki GLUT

}

/*************************************************************************************/
