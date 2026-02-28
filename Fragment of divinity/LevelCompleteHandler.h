#ifndef LEVELCOMPLETEHANDLER_H
#define LEVELCOMPLETEHANDLER_H

#include "iGraphics.h"

// Struct for button positions
struct ButtonRect {
  int x, y, w, h;
};

// Extern variables defined in iMain.cpp
extern int levelCompleteBG;
extern int caveIQBG;
extern int imgNextLevel, imgRestartLevel, imgHomeLevel;
extern int applesCollected;
extern int appleImg;

extern ButtonRect btnNextLevel, btnRestartLevel, btnHomeLevel;
extern ButtonRect btnNextIQ;
extern ButtonRect btnNextIQ_Level2;

// Adjustable Dimensions
#define LC_FRAME_X 100
#define LC_FRAME_Y 50
#define LC_FRAME_W 800
#define LC_FRAME_H 500

#define LC_BTN_W 220
#define LC_BTN_H 70
#define LC_BTN_GAP 30
#define LC_BTN_Y 120

// Initialization in LevelCompleteHandler.h
inline void loadLevelCompleteAssets() {
  levelCompleteBG = iLoadImage((char *)"background img\\lcf.png");
  imgNextLevel = iLoadImage((char *)"background img\\next.png");
  imgRestartLevel = iLoadImage((char *)"background img\\restart.png");
  imgHomeLevel = iLoadImage((char *)"background img\\home.png");
  caveIQBG = iLoadImage((char *)"background img\\cavebg.png");

  // Triangular layout:
  // Home: Bottom center
  // Restart: Left, close to border, higher than home
  // Next: Right, close to border, higher than home

  // Home Button (Left, Higher)
  btnHomeLevel.x = LC_FRAME_X + 110;
  btnHomeLevel.y = LC_FRAME_Y + 160;
  btnHomeLevel.w = LC_BTN_W;
  btnHomeLevel.h = LC_BTN_H;

  // Restart Button (Bottom Center)
  btnRestartLevel.x = (1000 - LC_BTN_W) / 2;
  btnRestartLevel.y = LC_FRAME_Y + 80;
  btnRestartLevel.w = LC_BTN_W;
  btnRestartLevel.h = LC_BTN_H;

  // Next Button (Right, Higher)
  btnNextLevel.x = LC_FRAME_X + LC_FRAME_W - LC_BTN_W - 110;
  btnNextLevel.y = LC_FRAME_Y + 160;
  btnNextLevel.w = LC_BTN_W;
  btnNextLevel.h = LC_BTN_H;

  // IQ Screen Buttons (Side by Side)
  // Total Width = 220 + 30 + 220 = 470
  int startX = (1000 - 470) / 2;

  // Home (Left)
  btnNextIQ.x = startX;
  btnNextIQ.y = 40;
  btnNextIQ.w = LC_BTN_W;
  btnNextIQ.h = LC_BTN_H;

  // Next (Bottom Right)
  // Next (Bottom Right)
  btnNextIQ_Level2.x = 750; // Shifted left to fit 220 width (750+220=970)
  btnNextIQ_Level2.y = 50;
  btnNextIQ_Level2.w = LC_BTN_W; // 220
  btnNextIQ_Level2.h = LC_BTN_H; // 70
}

inline void drawLevelComplete() {
  iShowImage(100, 50, 800, 500, levelCompleteBG);
  iShowImage(btnHomeLevel.x, btnHomeLevel.y, btnHomeLevel.w, btnHomeLevel.h,
             imgHomeLevel);
  iShowImage(btnRestartLevel.x, btnRestartLevel.y, btnRestartLevel.w,
             btnRestartLevel.h, imgRestartLevel);
  iShowImage(btnNextLevel.x, btnNextLevel.y, btnNextLevel.w, btnNextLevel.h,
             imgNextLevel);

  // Draw Apple Score (beside "food collected :" part on lcf.bmp)
  iShowImage(620, 300, 30, 30, appleImg);
  iSetColor(255, 230, 100); // Golden color to match theme
  char scoreText[10];
  sprintf_s(scoreText, "%d", applesCollected);
  iText(640, 300, scoreText, GLUT_BITMAP_HELVETICA_18);
}

inline void drawNextLevelIQ() {
  iShowImage(0, 0, 1000, 600, caveIQBG);
  // Buttons drawn in iMain.cpp loop for flexibility, or we can add here
}

// Actions: 1: Next Level, 2: Restart, 3: Home, 0: None
inline int handleLevelCompleteMouse(int mx, int my) {
  if (mx >= btnNextLevel.x && mx <= btnNextLevel.x + btnNextLevel.w &&
      my >= btnNextLevel.y && my <= btnNextLevel.y + btnNextLevel.h) {
    return 1; // Next Level (IQ screen)
  }
  if (mx >= btnRestartLevel.x && mx <= btnRestartLevel.x + btnRestartLevel.w &&
      my >= btnRestartLevel.y && my <= btnRestartLevel.y + btnRestartLevel.h) {
    return 2; // Restart
  }
  if (mx >= btnHomeLevel.x && mx <= btnHomeLevel.x + btnHomeLevel.w &&
      my >= btnHomeLevel.y && my <= btnHomeLevel.y + btnHomeLevel.h) {
    return 3; // Home
  }
  return 0;
}

inline int handleNextLevelIQMouse(int mx, int my) {
  // Home Button Logic Removed per User Request
  /*
  if (mx >= btnNextIQ.x && mx <= btnNextIQ.x + btnNextIQ.w &&
      my >= btnNextIQ.y && my <= btnNextIQ.y + btnNextIQ.h) {
    return 3; // Home/Menu
  }
  */
  if (mx >= btnNextIQ_Level2.x &&
      mx <= btnNextIQ_Level2.x + btnNextIQ_Level2.w &&
      my >= btnNextIQ_Level2.y &&
      my <= btnNextIQ_Level2.y + btnNextIQ_Level2.h) {
    return 4; // Continue to Level 2
  }
  return 0;
}

#endif
