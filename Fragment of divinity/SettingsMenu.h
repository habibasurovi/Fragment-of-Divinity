#ifndef SETTINGSMENU_H
#define SETTINGSMENU_H

#include "iGraphics.h"

// Variables
// Variables
extern bool isMusicOn;
extern bool isSoundOn;
extern int previousState;

// Assets
extern int settingsBG, settingsFrame, crossImage;
extern int btnMusic, btnSound, btnExitSettings;

// Frame Dimensions (Adjustable)
// Frame Dimensions (Adjustable)
extern int sFrameW;
extern int sFrameH;
extern int sFrameX;
extern int sFrameY;

// Button Dimensions
// Button Dimensions
struct SettingsButton {
  int x, y;
  int w, h;
};
extern SettingsButton bMusic, bSound, bBackSettings;

inline void loadSettingsAssets() {
  settingsBG = iLoadImage((char *)"images fod\\menu\\settings menu\\settings bg.png");
  settingsFrame =
      iLoadImage((char *)"images fod\\menu\\settings menu\\settings farme.png");
  btnMusic = iLoadImage((char *)"images fod\\menu\\settings menu\\music button.png");
  btnSound = iLoadImage((char *)"images fod\\menu\\settings menu\\sound.png");
  btnExitSettings = iLoadImage((char *)"images fod\\menu\\settings menu\\exit.png");
  crossImage = iLoadImage((char *)"images fod\\menu\\settings menu\\cross.png");

  // Unified Button Size
  int btnW = 320;
  int btnH = 80;
  int gap = 90;

  int centerX = 1000 / 2;
  // Start drawing from top-ish of the frame
  int startY = sFrameY + sFrameH - 325;

  // Music Button
  bMusic.w = btnW;
  bMusic.h = btnH;
  bMusic.x = centerX - (btnW / 2);
  bMusic.y = startY;

  // Sound Button
  bSound.w = btnW;
  bSound.h = btnH;
  bSound.x = centerX - (btnW / 2);
  bSound.y = startY - gap;

  // Exit Button
  bBackSettings.w = btnW;
  bBackSettings.h = btnH;
  bBackSettings.x = centerX - (btnW / 2);
  bBackSettings.y = startY - gap * 2;
}

inline void drawSettingsMenu() {
  // Draw Background (Full Screen)
  iShowImage(0, 0, 1000, 600, settingsBG);

  // Draw Frame (Centered, Adjustable Size)
  iShowImage(sFrameX, sFrameY, sFrameW, sFrameH, settingsFrame);

  // Draw Buttons
  iShowImage(bMusic.x, bMusic.y, bMusic.w, bMusic.h, btnMusic);
  // Draw Cross if Music is Off
  if (!isMusicOn) {
    
    int iconSize = bMusic.h - 10;
    iShowImage(bMusic.x + 10, bMusic.y + 5, iconSize, iconSize, crossImage);
  }

  iShowImage(bSound.x, bSound.y, bSound.w, bSound.h, btnSound);
  // Draw Cross if Sound is Off
  if (!isSoundOn) {
    // Assume icon is on the Left side
    int iconSize = bSound.h - 10;
    iShowImage(bSound.x + 10, bSound.y + 5, iconSize, iconSize, crossImage);
  }

  iShowImage(bBackSettings.x, bBackSettings.y, bBackSettings.w, bBackSettings.h,
             btnExitSettings);
}

// Return: -1 (Stay), 0 (Main Menu), 1 (Game/Pause)
inline int handleSettingsMenuMouse(int mx, int my) {

  // Toggle Music
  if (mx >= bMusic.x && mx <= bMusic.x + bMusic.w && my >= bMusic.y &&
      my <= bMusic.y + bMusic.h) {
    isMusicOn = !isMusicOn;
  }

  // Toggle Sound
  if (mx >= bSound.x && mx <= bSound.x + bSound.w && my >= bSound.y &&
      my <= bSound.y + bSound.h) {
    isSoundOn = !isSoundOn;
  }

  // Exit
  if (mx >= bBackSettings.x && mx <= bBackSettings.x + bBackSettings.w &&
      my >= bBackSettings.y && my <= bBackSettings.y + bBackSettings.h) {
    return previousState;
  }

  return -1;
}

#endif
