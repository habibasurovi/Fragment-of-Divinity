#ifndef I_GRAPHICS_H
#define I_GRAPHICS_H

//
//  Original Author: S. M. Shahriar Nirjon
//
//  Last Modified by: Mr. Mohammad Imrul Jubair [Assistant Professor (AUST CSE)]
//  Last Updated: 16 December 2017
//
//  Version: 4.0
//

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "glut32.lib")
#pragma comment(lib, "glaux.lib")
#include "glaux.h"
#include "glut.h"
#include <math.h>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

extern int iScreenHeight, iScreenWidth;
extern int iMouseX, iMouseY;
extern int ifft;
extern void (*iAnimFunction[10])(void);
extern int iAnimCount;
extern int iAnimDelays[10];
extern int iAnimPause[10];

extern unsigned int keyPressed[512];
extern unsigned int specialKeyPressed[512];

void iDraw();
void fixedUpdate();
void iMouseMove(int, int);
void iPassiveMouseMove(int, int);
void iMouse(int button, int state, int x, int y);
void iKeyboard(unsigned char);
void iSpecialKeyboard(unsigned char);

static void __stdcall iA0(HWND h, UINT m, UINT_PTR i, DWORD t) {
  if (!iAnimPause[0])
    iAnimFunction[0]();
}
static void __stdcall iA1(HWND h, UINT m, UINT_PTR i, DWORD t) {
  if (!iAnimPause[1])
    iAnimFunction[1]();
}
static void __stdcall iA2(HWND h, UINT m, UINT_PTR i, DWORD t) {
  if (!iAnimPause[2])
    iAnimFunction[2]();
}
static void __stdcall iA3(HWND h, UINT m, UINT_PTR i, DWORD t) {
  if (!iAnimPause[3])
    iAnimFunction[3]();
}
static void __stdcall iA4(HWND h, UINT m, UINT_PTR i, DWORD t) {
  if (!iAnimPause[4])
    iAnimFunction[4]();
}
static void __stdcall iA5(HWND h, UINT m, UINT_PTR i, DWORD t) {
  if (!iAnimPause[5])
    iAnimFunction[5]();
}
static void __stdcall iA6(HWND h, UINT m, UINT_PTR i, DWORD t) {
  if (!iAnimPause[6])
    iAnimFunction[6]();
}
static void __stdcall iA7(HWND h, UINT m, UINT_PTR i, DWORD t) {
  if (!iAnimPause[7])
    iAnimFunction[7]();
}
static void __stdcall iA8(HWND h, UINT m, UINT_PTR i, DWORD t) {
  if (!iAnimPause[8])
    iAnimFunction[8]();
}
static void __stdcall iA9(HWND h, UINT m, UINT_PTR i, DWORD t) {
  if (!iAnimPause[9])
    iAnimFunction[9]();
}
static void __stdcall keypressHandler(HWND h, UINT m, UINT_PTR i, DWORD t) {
  fixedUpdate();
}

inline int isKeyPressed(unsigned char key) { return keyPressed[key]; }

inline int isSpecialKeyPressed(unsigned char key) {
  return specialKeyPressed[key];
}

inline int iSetTimer(int msec, void (*f)(void)) {
  int i = iAnimCount;

  if (iAnimCount >= 10) {
    printf("Error: Maximum number of already timer used.\n");
    return -1;
  }

  iAnimFunction[i] = f;
  iAnimDelays[i] = msec;
  iAnimPause[i] = 0;

  if (iAnimCount == 0)
    SetTimer(0, 0, msec, iA0);
  if (iAnimCount == 1)
    SetTimer(0, 0, msec, iA1);
  if (iAnimCount == 2)
    SetTimer(0, 0, msec, iA2);
  if (iAnimCount == 3)
    SetTimer(0, 0, msec, iA3);
  if (iAnimCount == 4)
    SetTimer(0, 0, msec, iA4);

  if (iAnimCount == 5)
    SetTimer(0, 0, msec, iA5);
  if (iAnimCount == 6)
    SetTimer(0, 0, msec, iA6);
  if (iAnimCount == 7)
    SetTimer(0, 0, msec, iA7);
  if (iAnimCount == 8)
    SetTimer(0, 0, msec, iA8);
  if (iAnimCount == 9)
    SetTimer(0, 0, msec, iA9);
  iAnimCount++;

  return iAnimCount - 1;
}

inline void iPauseTimer(int index) {
  if (index >= 0 && index < iAnimCount) {
    iAnimPause[index] = 1;
  }
}

inline void iResumeTimer(int index) {
  if (index >= 0 && index < iAnimCount) {
    iAnimPause[index] = 0;
  }
}

//
// Puts a BMP image on screen
//
// parameters:
//  x - x coordinate
//  y - y coordinate
//  filename - name of the BMP file
//  ignoreColor - A specified color that should not be rendered. If you have an
//                image strip that should be rendered on top of another back
//                ground image, then the background of the image strip should
//                not get rendered. Use the background color of the image strip
//                in ignoreColor parameter. Then the strip's background does
//                not get rendered.
//
//                To disable this feature, put -1 in this parameter
//
inline void iShowBMP2(int x, int y, const char filename[], int ignoreColor) {
  AUX_RGBImageRec *TextureImage;
  TextureImage = auxDIBImageLoad((char *)filename);

  int i, j;
  int width = TextureImage->sizeX;
  int height = TextureImage->sizeY;
  int nPixels = width * height;
  int *rgPixels = new int[nPixels];

  for (i = 0, j = 0; i < nPixels; i++, j += 3) {
    int rgb = 0;
    for (int k = 2; k >= 0; k--) {
      rgb = ((rgb << 8) | TextureImage->data[j + k]);
    }

    rgPixels[i] = (rgb == ignoreColor) ? 0 : 255;
    rgPixels[i] = ((rgPixels[i] << 24) | rgb);
  }

  glRasterPos2f(x, y);
  glDrawPixels(width, height, GL_RGBA, GL_UNSIGNED_BYTE, rgPixels);

  delete[] rgPixels;
  free(TextureImage->data);
  free(TextureImage);
}

inline void iShowBMP(int x, int y, const char filename[]) {
  iShowBMP2(x, y, filename, -1 /* ignoreColor */);
}

inline unsigned int iLoadImage(const char filename[]) {
  int width, height, bpp;

  unsigned int texture;

  BYTE *data(0);
  data = stbi_load(filename, &width, &height, &bpp, 4);
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);

  stbi_image_free(data);

  return texture;
}

inline void iShowImage(int x, int y, int width, int height,
                       unsigned int texture) {

  glEnable(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  glBegin(GL_QUADS);

  glTexCoord2f(0, 0);
  glVertex2f(x, y);

  glTexCoord2f(1, 0);
  glVertex2f(x + width, y);

  glTexCoord2f(1, -1);
  glVertex2f(x + width, y + height);

  glTexCoord2f(0, -1);
  glVertex2f(x, y + height);

  glEnd();

  glDisable(GL_TEXTURE_2D);
}

inline void iShowImageHFlip(int x, int y, int width, int height,
                            unsigned int texture) {

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  glBegin(GL_QUADS);
  glTexCoord2f(1, 0);
  glVertex2f(x, y);
  glTexCoord2f(0, 0);
  glVertex2f(x + width, y);
  glTexCoord2f(0, -1);
  glVertex2f(x + width, y + height);
  glTexCoord2f(1, -1);
  glVertex2f(x, y + height);
  glEnd();

  glDisable(GL_TEXTURE_2D);
}

// Vertical flip (upside-down)
inline void iShowImageVFlip(int x, int y, int width, int height,
                            unsigned int texture) {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glBegin(GL_QUADS);
  // Swap top/bottom tex coords to flip vertically
  glTexCoord2f(0, -1); glVertex2f(x, y);
  glTexCoord2f(1, -1); glVertex2f(x + width, y);
  glTexCoord2f(1,  0); glVertex2f(x + width, y + height);
  glTexCoord2f(0,  0); glVertex2f(x, y + height);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

// Horizontal + Vertical flip (mirrored and upside-down)
inline void iShowImageHVFlip(int x, int y, int width, int height,
                             unsigned int texture) {
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
  glBegin(GL_QUADS);
  glTexCoord2f(1, -1); glVertex2f(x, y);
  glTexCoord2f(0, -1); glVertex2f(x + width, y);
  glTexCoord2f(0,  0); glVertex2f(x + width, y + height);
  glTexCoord2f(1,  0); glVertex2f(x, y + height);
  glEnd();
  glDisable(GL_TEXTURE_2D);
}

inline void iGetPixelColor(int cursorX, int cursorY, int rgb[]) {
  GLubyte pixel[3];
  glReadPixels(cursorX, cursorY, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, (void *)pixel);

  rgb[0] = pixel[0];
  rgb[1] = pixel[1];
  rgb[2] = pixel[2];

  // printf("%d %d %d\n",pixel[0],pixel[1],pixel[2]);
}

inline void iText(GLdouble x, GLdouble y, const char *str,
                  void *font = GLUT_BITMAP_8_BY_13) {
  glRasterPos3d(x, y, 0);
  int i;
  for (i = 0; str[i]; i++) {
    glutBitmapCharacter(
        font, str[i]); //,GLUT_BITMAP_8_BY_13, GLUT_BITMAP_TIMES_ROMAN_24
  }
}

inline void iPoint(double x, double y, int size = 0) {
  int i, j;
  glBegin(GL_POINTS);
  glVertex2f(x, y);
  for (i = x - size; i < x + size; i++) {
    for (j = y - size; j < y + size; j++) {
      glVertex2f(i, j);
    }
  }
  glEnd();
}

inline void iLine(double x1, double y1, double x2, double y2) {
  glBegin(GL_LINE_STRIP);
  glVertex2f(x1, y1);
  glVertex2f(x2, y2);
  glEnd();
}

inline void iFilledPolygon(double x[], double y[], int n) {
  int i;
  if (n < 3)
    return;
  glBegin(GL_POLYGON);
  for (i = 0; i < n; i++) {
    glVertex2f(x[i], y[i]);
  }
  glEnd();
}

inline void iPolygon(double x[], double y[], int n) {
  int i;
  if (n < 3)
    return;
  glBegin(GL_LINE_STRIP);
  for (i = 0; i < n; i++) {
    glVertex2f(x[i], y[i]);
  }
  glVertex2f(x[0], y[0]);
  glEnd();
}

inline void iRectangle(double left, double bottom, double dx, double dy) {
  double x1, y1, x2, y2;

  x1 = left;
  y1 = bottom;
  x2 = x1 + dx;
  y2 = y1 + dy;

  iLine(x1, y1, x2, y1);
  iLine(x2, y1, x2, y2);
  iLine(x2, y2, x1, y2);
  iLine(x1, y2, x1, y1);
}

inline void iFilledRectangle(double left, double bottom, double dx, double dy) {
  double xx[4], yy[4];
  double x1, y1, x2, y2;

  x1 = left;
  y1 = bottom;
  x2 = x1 + dx;
  y2 = y1 + dy;

  xx[0] = x1;
  yy[0] = y1;
  xx[1] = x2;
  yy[1] = y1;
  xx[2] = x2;
  yy[2] = y2;
  xx[3] = x1;
  yy[3] = y2;

  iFilledPolygon(xx, yy, 4);
}

inline void iFilledCircle(double x, double y, double r, int slices = 100) {
  double t, PI = acos(-1.0), dt, x1, y1, xp, yp;
  dt = 2 * PI / slices;
  xp = x + r;
  yp = y;
  glBegin(GL_POLYGON);
  for (t = 0; t <= 2 * PI; t += dt) {
    x1 = x + r * cos(t);
    y1 = y + r * sin(t);

    glVertex2f(xp, yp);
    xp = x1;
    yp = y1;
  }
  glEnd();
}

inline void iCircle(double x, double y, double r, int slices = 100) {
  double t, PI = acos(-1.0), dt, x1, y1, xp, yp;
  dt = 2 * PI / slices;
  xp = x + r;
  yp = y;
  for (t = 0; t <= 2 * PI; t += dt) {
    x1 = x + r * cos(t);
    y1 = y + r * sin(t);
    iLine(xp, yp, x1, y1);
    xp = x1;
    yp = y1;
  }
}

inline void iEllipse(double x, double y, double a, double b, int slices = 100) {
  double t, PI = acos(-1.0), dt, x1, y1, xp, yp;
  dt = 2 * PI / slices;
  xp = x + a;
  yp = y;
  for (t = 0; t <= 2 * PI; t += dt) {
    x1 = x + a * cos(t);
    y1 = y + b * sin(t);
    iLine(xp, yp, x1, y1);
    xp = x1;
    yp = y1;
  }
}

inline void iFilledEllipse(double x, double y, double a, double b,
                           int slices = 100) {
  double t, PI = acos(-1.0), dt, x1, y1, xp, yp;
  dt = 2 * PI / slices;
  xp = x + a;
  yp = y;
  glBegin(GL_POLYGON);
  for (t = 0; t <= 2 * PI; t += dt) {
    x1 = x + a * cos(t);
    y1 = y + b * sin(t);
    glVertex2f(xp, yp);
    xp = x1;
    yp = y1;
  }
  glEnd();
}

// Rotates the co-ordinate system
// Parameters:
//  (x, y) - The pivot point for rotation
//  degree - degree of rotation
//
// After calling iRotate(), evrey subsequent rendering will
// happen in rotated fashion. To stop rotation of subsequent rendering,
// call iUnRotate(). Typical call pattern would be:
//      iRotate();
//      Render your objects, that you want rendered as rotated
//      iUnRotate();
//
inline void iRotate(double x, double y, double degree) {
  glPushMatrix();

  glTranslatef(x, y, 0.0);

  glRotatef(degree, 0, 0, 1.0);

  glTranslatef(-x, -y, 0.0);
}

inline void iUnRotate() { glPopMatrix(); }

inline void iSetColor(double r, double g, double b) {
  double mmx;
  mmx = r;
  if (g > mmx)
    mmx = g;
  if (b > mmx)
    mmx = b;
  mmx = 255;
  if (mmx > 0) {
    r /= mmx;
    g /= mmx;
    b /= mmx;
  }
  glColor3f(r, g, b);
}

inline void iDelay(int sec) {
  int t1, t2;
  t1 = time(0);
  while (1) {
    t2 = time(0);
    if (t2 - t1 >= sec)
      break;
  }
}

inline void iDelayMS(int msec) {
  clock_t end;
  end = clock() + msec * CLOCKS_PER_SEC / 1000;
  while (end > clock())
    ;
}

inline void iClear() {
  glClear(GL_COLOR_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glClearColor(0, 0, 0, 0);
  glFlush();
}

inline void displayFF(void) {

  iDraw();
  glutSwapBuffers();
}

inline void animFF(void) {
  if (ifft == 0) {
    ifft = 1;
    iClear();
  }
  glutPostRedisplay();
}

inline void keyboardHandlerUp1FF(unsigned char key, int x, int y) {
  keyPressed[key] = 0;
  glutPostRedisplay();
}
inline void keyboardHandlerUp2FF(int key, int x, int y) {
  specialKeyPressed[key] = 0;
  glutPostRedisplay();
}

inline void keyboardHandler1FF(unsigned char key, int x, int y) {
  iKeyboard(key);
  keyPressed[key] = 1;
  glutPostRedisplay();
}
inline void keyboardHandler2FF(int key, int x, int y) {
  iSpecialKeyboard((unsigned char)key);
  specialKeyPressed[key] = 1;
  glutPostRedisplay();
}

inline void mouseMoveHandlerFF(int mx, int my) {
  iMouseX = mx;
  iMouseY = iScreenHeight - my;
  iMouseMove(iMouseX, iMouseY);

  glFlush();
}

inline void mousePassiveMoveHandlerFF(int mx, int my) {
  iMouseX = mx;
  iMouseY = iScreenHeight - my;
  iPassiveMouseMove(iMouseX, iMouseY);

  glFlush();
}

inline void mouseHandlerFF(int button, int state, int x, int y) {
  iMouseX = x;
  iMouseY = iScreenHeight - y;

  iMouse(button, state, iMouseX, iMouseY);

  glFlush();
}

inline void iInitialize(int width = 500, int height = 500,
                        const char *title = "iGraphics",
                        int keyboardSamplingRate = 16) {
  SetTimer(0, 0, keyboardSamplingRate, keypressHandler);

  iScreenHeight = height;
  iScreenWidth = width;

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA);
  glutInitWindowSize(width, height);
  glutInitWindowPosition(10, 10);
  glutCreateWindow(title);
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
  // glOrtho(-100.0 , 100.0 , -100.0 , 100.0 , -1.0 , 1.0) ;
  // SetTimer(0, 0, 10, timer_proc);
}

inline void iStart() {
  iClear();

  glutDisplayFunc(displayFF);
  glutKeyboardFunc(keyboardHandler1FF); // normal
  glutSpecialFunc(keyboardHandler2FF);  // special keys
  glutKeyboardUpFunc(keyboardHandlerUp1FF);
  glutSpecialUpFunc(keyboardHandlerUp2FF);
  glutMouseFunc(mouseHandlerFF);
  glutMotionFunc(mouseMoveHandlerFF);
  glutPassiveMotionFunc(mousePassiveMoveHandlerFF);
  glutIdleFunc(animFF);

  //
  // Setup Alpha channel testing.
  // If alpha value is greater than 0, then those
  // pixels will be rendered. Otherwise, they would not be rendered
  //
  glAlphaFunc(GL_GREATER, 0.0f);
  glEnable(GL_ALPHA_TEST);

  glutMainLoop();
}

inline void iDrawFadeOverlay(float fadeAlpha) {
  if (fadeAlpha <= 0.0f) return;
  if (fadeAlpha > 1.0f) fadeAlpha = 1.0f;

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glColor4f(0.0f, 0.0f, 0.0f, fadeAlpha);
  glBegin(GL_QUADS);
    glVertex2f(0, 0);
    glVertex2f(1000, 0);
    glVertex2f(1000, 600);
    glVertex2f(0, 600);
  glEnd();
  glDisable(GL_BLEND);
  glColor3f(1.0f, 1.0f, 1.0f);
}

#endif
