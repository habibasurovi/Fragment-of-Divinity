#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "iGraphics.h"

// Flag to track if the game is paused
extern bool isGamePaused;

// Asset Variables
extern int pauseBtnImage;
extern int pauseFrameImage;
extern int btnContinue, btnRestart, btnSettingsPause, btnExitPause;

// Button Dimensions & Positions
struct PauseButton {
  int x, y;
  int w, h;
};
extern PauseButton bPause, bContinue, bRestart, bSettingsP, bExitP;

// Frame Dimensions
extern int frameX, frameY, frameW, frameH;

inline void loadPauseMenuAssets() {
  // Load Images
  pauseBtnImage = iLoadImage((char *)"images fod\\menu\\buttons internal\\pause.png");
  pauseFrameImage = iLoadImage((char *)"images fod\\menu\\buttons internal\\frame.png");
  btnContinue = iLoadImage((char *)"images fod\\menu\\buttons internal\\continue.png");
  btnRestart = iLoadImage((char *)"images fod\\menu\\buttons internal\\restart.png");
  btnSettingsPause =
      iLoadImage((char *)"images fod\\menu\\buttons internal\\settings.png");
  btnExitPause = iLoadImage((char *)"images fod\\menu\\buttons internal\\exit.png");

  // Initialize Pause Button (Top Left Corner)
  bPause.x = 20;
  bPause.y = 520; // Screen height is 600, so 530 is near top
  bPause.w = 70;  // Assuming size, will adjust if needed
  bPause.h = 70;

  // Initialize Frame (Centered)
  frameW = 400;
  frameH = 500;
  frameX = (1000 - frameW) / 2; // (ScreenWidth - FrameWidth) / 2
  frameY = (600 - frameH) / 2;  // (ScreenHeight - FrameHeight) / 2

  // Initialize Menu Buttons (Inside Frame)
  int btnW = 200;
  int btnH = 50;
  int startY = frameY + frameH - 180; // Lowered start position
  int gap = 70;                       // Slightly reduced gap

  // Continue
  bContinue.w = btnW;
  bContinue.h = btnH;
  bContinue.x = frameX + (frameW - btnW) / 2;
  bContinue.y = startY;

  // Restart
  bRestart.w = btnW;
  bRestart.h = btnH;
  bRestart.x = frameX + (frameW - btnW) / 2;
  bRestart.y = startY - gap;

  // Settings
  bSettingsP.w = btnW;
  bSettingsP.h = btnH;
  bSettingsP.x = frameX + (frameW - btnW) / 2;
  bSettingsP.y = startY - gap * 2;

  // Exit
  bExitP.w = btnW;
  bExitP.h = btnH;
  bExitP.x = frameX + (frameW - btnW) / 2;
  bExitP.y = startY - gap * 3;
}

inline void drawPauseMenu() {
  // Always draw Pause button in Game state
  iShowImage(bPause.x, bPause.y, bPause.w, bPause.h, pauseBtnImage);

  if (isGamePaused) {
    // Draw Frame
    iShowImage(frameX, frameY, frameW, frameH, pauseFrameImage);

    // Draw Buttons
    iShowImage(bContinue.x, bContinue.y, bContinue.w, bContinue.h, btnContinue);
    iShowImage(bRestart.x, bRestart.y, bRestart.w, bRestart.h, btnRestart);
    iShowImage(bSettingsP.x, bSettingsP.y, bSettingsP.w, bSettingsP.h,
               btnSettingsPause);
    iShowImage(bExitP.x, bExitP.y, bExitP.w, bExitP.h, btnExitPause);
  }
}

// Returns:
// 0: No Action
// 1: Resume Game
// 2: Restart Game
// 3: Go to Settings
// 4: Exit to Main Menu
inline int handlePauseMenuMouse(int mx, int my) {
  // Check Pause Button Click
  if (mx >= bPause.x && mx <= bPause.x + bPause.w && my >= bPause.y &&
      my <= bPause.y + bPause.h) {
    isGamePaused = !isGamePaused; // Toggle Pause
    return 0;
  }

  if (isGamePaused) {
    // Continue
    if (mx >= bContinue.x && mx <= bContinue.x + bContinue.w &&
        my >= bContinue.y && my <= bContinue.y + bContinue.h) {
      isGamePaused = false;
      return 1; // Resume
    }
    // Restart
    if (mx >= bRestart.x && mx <= bRestart.x + bRestart.w && my >= bRestart.y &&
        my <= bRestart.y + bRestart.h) {
      isGamePaused = false;
      return 2; // Restart
    }
    // Settings
    if (mx >= bSettingsP.x && mx <= bSettingsP.x + bSettingsP.w &&
        my >= bSettingsP.y && my <= bSettingsP.y + bSettingsP.h) {
     
      return 3;
    }
    // Exit
    if (mx >= bExitP.x && mx <= bExitP.x + bExitP.w && my >= bExitP.y &&
        my <= bExitP.y + bExitP.h) {
      isGamePaused = false;
      return 4; // Exit to Main Menu
    }
  }

  return 0;
}

#endif
