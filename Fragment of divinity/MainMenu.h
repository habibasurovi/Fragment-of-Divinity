
#ifndef MAINMENU_H
#define MAINMENU_H

#include "iGraphics.h"

// Asset Variables
extern int mainMenuBG;
extern int btnStart, btnSettings, btnCustomization, btnExit, btnHighscore;

// Button Dimensions
struct Button {
  int x, y;
  int w, h;
};
extern Button bStart, bSettings, bCustomization, bExit, bHighscore;

// Animation globals (defined in iMain.cpp)
extern bool  btnAnimActive;
extern int   btnAnimTimer;    // counts down from 1 (fires on globalTimerLogic)
extern int   btnAnimCode;     // which button is growing
extern int   btnAnimContext;  // 0=MainMenu, 1=Custom, 2=Story

inline void loadMainMenuAssets() {
 
  mainMenuBG = iLoadImage("images fod\\menu\\main menu\\demo1.jpg");

  // Loading Buttons
  btnStart =
      iLoadImage("images fod\\menu\\main menu\\buttons\\start.png");
  btnSettings =
      iLoadImage("images fod\\menu\\main menu\\buttons\\settings.png");
  btnCustomization = iLoadImage(
      "images fod\\menu\\main menu\\buttons\\customization.png");
  btnExit = iLoadImage("images fod\\menu\\main menu\\buttons\\exit.png");
  btnHighscore = iLoadImage("images fod\\menu\\main menu\\buttons\\highscrore.png");

  // Initializing Button Positions (Scaled for 1000x600)
  // Original X~200 -> 330. Original Y~250 -> 375.
  // Sizes 220x45 -> 360x65 approx

  int bw = 460; // Increased width
  int bh = 63;  // Balanced height
  int bx = 270; // Centered (1000 - 460) / 2

  bStart.w = bw;
  bStart.h = bh;
  bStart.x = bx;
  bStart.y = 110 + 75*4;

  bCustomization.w = bw;
  bCustomization.h = bh;
  bCustomization.x = bx;
  bCustomization.y = 110 + 75*3;

  bSettings.w = bw;
  bSettings.h = bh;
  bSettings.x = bx;
  bSettings.y = 110 +75*2;

  bHighscore.w = bw;
  bHighscore.h = bh;
  bHighscore.x = bx;
  bHighscore.y = 110 + 75;

  bExit.w = bw;
  bExit.h = bh;
  bExit.x = bx;
  bExit.y = 110;
}

// Helper: draw a button, enlarged by ~15% if it is the currently-animated one
#define DRAW_MAIN_BTN(btn, img, code) \
  do { \
    if (btnAnimActive && btnAnimContext == 0 && btnAnimCode == (code)) { \
      int _ew = (int)((btn).w * 1.15f); \
      int _eh = (int)((btn).h * 1.15f); \
      int _ex = (btn).x - (_ew - (btn).w) / 2; \
      int _ey = (btn).y - (_eh - (btn).h) / 2; \
      iShowImage(_ex, _ey, _ew, _eh, (img)); \
    } else { \
      iShowImage((btn).x, (btn).y, (btn).w, (btn).h, (img)); \
    } \
  } while(0)

inline void drawMainMenu() {
  iShowImage(0, 0, 1000, 600, mainMenuBG);

  DRAW_MAIN_BTN(bStart,         btnStart,          1);
  DRAW_MAIN_BTN(bCustomization, btnCustomization,  3);
  DRAW_MAIN_BTN(bSettings,      btnSettings,       2);
  DRAW_MAIN_BTN(bHighscore,     btnHighscore,      5);
  DRAW_MAIN_BTN(bExit,          btnExit,           4);
}

// Returns the state to switch to, or -1 if no change
// 1: PLAY (GAME), 2: SETTINGS (placeholder), 3: CUSTOMIZATION, 4: EXIT
inline int handleMainMenuMouse(int mx, int my) {
  if (mx >= bStart.x && mx <= bStart.x + bStart.w && my >= bStart.y &&
      my <= bStart.y + bStart.h) {
    return 1; // Start Game
  }
  if (mx >= bCustomization.x && mx <= bCustomization.x + bCustomization.w &&
      my >= bCustomization.y && my <= bCustomization.y + bCustomization.h) {
    return 3;
  }
  if (mx >= bSettings.x && mx <= bSettings.x + bSettings.w &&
      my >= bSettings.y && my <= bSettings.y + bSettings.h) {
    return 2;
  }
  if (mx >= bHighscore.x && mx <= bHighscore.x + bHighscore.w &&
      my >= bHighscore.y && my <= bHighscore.y + bHighscore.h) {
    return 5; // HIGHSCORES state
  }
  if (mx >= bExit.x && mx <= bExit.x + bExit.w && my >= bExit.y &&
      my <= bExit.y + bExit.h) {
    return 4; // Exit (animation plays first, then exit(0) fires in firePendingAction)
  }
  return -1;
}

#endif
