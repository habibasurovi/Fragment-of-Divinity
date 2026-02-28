#ifndef BACKGROUNDHANDLER_H
#define BACKGROUNDHANDLER_H

#include "iGraphics.h"

// Variables
extern int selectedWeather;
extern int currentLevel; // 1 for Level 1, 2 for Level 2

extern int bG1, bG2, bG3, bG4;
extern int sbG1, sbG2, sbG3, sbG4;
extern int wbG1, wbG2, wbG3, wbG4;

// Level 2 background assets
extern int s2bG1, s2bG2, s2bG3, s2bG4;
extern int w2bG1, w2bG2, w2bG3, w2bG4;
extern int r2bG1, r2bG2, r2bG3, r2bG4;

extern int bGX; // background X position
extern int shakeOffsetX;
extern int shakeOffsetY;

// Screen dimensions needed for logic (Redefining or assuming these match iMain)
// Ideally pass these or define in a common header. For now, matching iMain.
#define SCREEN_W 1000
#define SCREEN_H 600
#ifndef SCROLL_SPD
#define SCROLL_SPD 3
#endif

inline void loadBackgroundAssets() {
  // Rainy (Default/Existing) - Level 1
  bG1 = iLoadImage("background img\\bG1.bmp");
  bG2 = iLoadImage("background img\\bG2.bmp");
  bG3 = iLoadImage("background img\\bG3.bmp");
  bG4 = iLoadImage("background img\\bG4.bmp");

  // Summer - Level 1
  sbG1 = iLoadImage("background img\\sbG1.bmp");
  sbG2 = iLoadImage("background img\\sbG2.bmp");
  sbG3 = iLoadImage("background img\\sbG3.bmp");
  sbG4 = iLoadImage("background img\\sbG4.bmp");

  // Winter - Level 1
  wbG1 = iLoadImage("background img\\wbG1.bmp");
  wbG2 = iLoadImage("background img\\wbG2.bmp");
  wbG3 = iLoadImage("background img\\wbG3.bmp");
  wbG4 = iLoadImage("background img\\wbG4.bmp");

  // Level 2 Assets
  s2bG1 = iLoadImage("background img\\summer21.bmp");
  s2bG2 = iLoadImage("background img\\summer22.bmp");
  s2bG3 = iLoadImage("background img\\summer23.bmp");
  s2bG4 = iLoadImage("background img\\summer24.bmp");

  w2bG1 = iLoadImage("background img\\winte21.bmp"); // Filename on disk
  w2bG2 = iLoadImage("background img\\winter22.bmp");
  w2bG3 = iLoadImage("background img\\winter23.bmp");
  w2bG4 = iLoadImage("background img\\winte24.bmp"); // Filename on disk

  r2bG1 = iLoadImage("background img\\rainy21.bmp");
  r2bG2 = iLoadImage("background img\\rainy22.bmp");
  r2bG3 = iLoadImage("background img\\rainy23.bmp");
  r2bG4 = iLoadImage("background img\\rainy24.bmp");
}

inline void autoScrollRecursive() {
  bGX -= SCROLL_SPD; // move background left
  // wrap for infinite scrolling
  if (bGX <= -3 * SCREEN_W)
    bGX = 0;
}

inline void drawBackground() {
  int img1, img2, img3, img4;

  if (currentLevel == 2) {
    if (selectedWeather == 0) { // Summer
      img1 = s2bG1;
      img2 = s2bG2;
      img3 = s2bG3;
      img4 = s2bG4;
    } else if (selectedWeather == 1) { // Rainy
      img1 = r2bG1;
      img2 = r2bG2;
      img3 = r2bG3;
      img4 = r2bG4;
    } else { // Winter
      img1 = w2bG1;
      img2 = w2bG2;
      img3 = w2bG3;
      img4 = w2bG4;
    }
  } else {
    if (selectedWeather == 0) { // Summer
      img1 = sbG1;
      img2 = sbG2;
      img3 = sbG3;
      img4 = sbG4;
    } else if (selectedWeather == 1) { // Rainy
      img1 = bG1;
      img2 = bG2;
      img3 = bG3;
      img4 = bG4;
    } else { // Winter
      img1 = wbG1;
      img2 = wbG2;
      img3 = wbG3;
      img4 = wbG4;
    }
  }

  iShowImage(bGX + shakeOffsetX, 0 + shakeOffsetY, SCREEN_W, SCREEN_H, img1);
  iShowImage(bGX + SCREEN_W + shakeOffsetX, 0 + shakeOffsetY, SCREEN_W, SCREEN_H, img2);
  iShowImage(bGX + 2 * SCREEN_W + shakeOffsetX, 0 + shakeOffsetY, SCREEN_W, SCREEN_H, img3);
  iShowImage(bGX + 3 * SCREEN_W + shakeOffsetX, 0 + shakeOffsetY, SCREEN_W, SCREEN_H, img4);
}

#endif
