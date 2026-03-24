#ifndef CUSTOMIZATIONMENU_H
#define CUSTOMIZATIONMENU_H

#include "iGraphics.h"

// Assets
extern int customMenuBG;
extern int btnWeather, btnCharCustom;


// Variables to store selection
extern int selectedWeather;
extern int selectedCharacter;

// Sub-menu State: 0 = Main Custom, 1 = Character Custom, 2 = Weather Custom
extern int customSubState;
extern int imgCharSubBG, imgWeatherSubBG;
extern int btnCharCstm, btnWeatherCstm;

// Animation globals (defined in iMain.cpp)
extern bool btnAnimActive;
extern int  btnAnimTimer;
extern int  btnAnimCode;    // 10=CharCstm, 11=WeatherCstm (context 1)
extern int  btnAnimContext; // 0=MainMenu, 1=Custom, 2=Story


struct CustomButton {
  int x, y;
  int w, h;
};
extern CustomButton bCharCstm, bWeatherCstm, bBack;

// Selection Buttons (Red "SELECT" buttons)
struct SelectButton {
  int x, y;
  int w, h;
};
extern SelectButton selChar[3], selWeather[3];

inline void loadCustomizationAssets() {
  // LOAD IMAGES
  customMenuBG = iLoadImage((char *)"images fod\\menu\\Custom menu\\custm_bg.png");
  imgCharSubBG = iLoadImage((char *)"images fod\\menu\\Custom menu\\char_custom.png");
  imgWeatherSubBG = iLoadImage((char *)"images fod\\menu\\Custom menu\\weather.bmp");

  btnCharCstm = iLoadImage((char *)"images fod\\menu\\Custom menu\\char_cstm_btn.png");
  btnWeatherCstm =
      iLoadImage((char *)"images fod\\menu\\Custom menu\\weather_cstm_btn.png");

  // BUTTON POSITIONS

  // Main Custom Menu Buttons (Scaled)
  bCharCstm.x = 166;
  bCharCstm.y = 300;
  bCharCstm.w = 660;
  bCharCstm.h = 210;
  bWeatherCstm.x = 166;
  bWeatherCstm.y = 60;
  bWeatherCstm.w = 660;
  bWeatherCstm.h = 210;

  // Back Button (Generic position, can be used in all screens)
  bBack.x = 30;
  bBack.y = 535;
  bBack.w = 120;
  bBack.h = 45;

  // Character Select Buttons (Red Rectangles)
  // Assuming 3 frames across the screen. 1000 width / 3 = 333 per frame.
  int frameWidth = 320;
  int btnW = 80, btnH = 30;

  for (int i = 0; i < 3; i++) {
    selChar[i].w = btnW;
    selChar[i].h = btnH;
    // selChar[i].x = (i * frameWidth) + (frameWidth - btnW)/2;
    selChar[i].y = 32; // Scaled up near bottom
  }
  selChar[0].x = (0 * frameWidth) + (frameWidth + 30 - btnW) / 2; // aryn
  selChar[1].x = (1 * frameWidth) + (frameWidth - btnW) / 2;      // kael
  selChar[2].x =
      (2 * (frameWidth - 10)) + (frameWidth - 10 - btnW) / 2; // liora

  // Weather Select Buttons (Middle of frames)
  for (int i = 0; i < 3; i++) {
    selWeather[i].w = btnW;
    selWeather[i].h = btnH;
    selWeather[i].x = (i * frameWidth) + (frameWidth - btnW) / 2;
    selWeather[i].y = 275; // Scaled Middle (approx 46% of 600)
  }
}

// Helper to draw Select Button
inline void drawSelectBtn(int x, int y, int w, int h, bool isSelected) {

  iSetColor(0, 0, 0);
  iText(x + 10, y + 8, isSelected ? (char*)"SELECTED" : (char*)" SELECT ",
        (void*)GLUT_BITMAP_HELVETICA_18);
}

#define DRAW_CSTM_BTN(btn, img, code) \
  do { \
    if (btnAnimActive && btnAnimContext == 1 && btnAnimCode == (code)) { \
      int _ew = (int)((btn).w * 1.15f); \
      int _eh = (int)((btn).h * 1.15f); \
      int _ex = (btn).x - (_ew - (btn).w) / 2; \
      int _ey = (btn).y - (_eh - (btn).h) / 2; \
      iShowImage(_ex, _ey, _ew, _eh, (img)); \
    } else { \
      iShowImage((btn).x, (btn).y, (btn).w, (btn).h, (img)); \
    } \
  } while(0)

inline void drawCustomizationMenu() {
  if (customSubState == 0) { // Main Custom Menu
    iShowImage(0, 0, 1000, 600, customMenuBG);

    // Draw Buttons to enter sub-menus (with grow animation)
    DRAW_CSTM_BTN(bCharCstm,    btnCharCstm,    10);
    DRAW_CSTM_BTN(bWeatherCstm, btnWeatherCstm, 11);

    // Back to Main Menu
    iSetColor(255, 255, 255);
    iText(bBack.x, bBack.y, (char*)"BACK", (void*)GLUT_BITMAP_HELVETICA_18);
  } else if (customSubState == 1) { // Character Customization
    iShowImage(0, 0, 1000, 600, imgCharSubBG);

    // Draw Select Buttons
    for (int i = 0; i < 3; i++) {
      drawSelectBtn(selChar[i].x, selChar[i].y, selChar[i].w, selChar[i].h,
                    (selectedCharacter == i));
    }

    // Back to Custom Main
    iSetColor(255, 255, 255);
    iText(bBack.x, bBack.y, (char*)"BACK", (void*)GLUT_BITMAP_HELVETICA_18);
  } else if (customSubState == 2) { // Weather Customization
    iShowImage(0, 0, 1000, 600, imgWeatherSubBG);

    // Draw Select Buttons
    for (int i = 0; i < 3; i++) {
      drawSelectBtn(selWeather[i].x, selWeather[i].y, selWeather[i].w,
                    selWeather[i].h, (selectedWeather == i));
    }

    // Back to Custom Main
    iSetColor(0, 0, 0); // Black text for contrast on weather? or White?
    iText(bBack.x, bBack.y, (char*)"BACK", (void*)GLUT_BITMAP_HELVETICA_18);
  }
}

// Return 1 to Start Game, -1 to stay
// Return 1 to Start Game (not used here anymore), -1 to stay, 0 to go back to
// Main Menu
inline int handleCustomizationMouse(int mx, int my) {

  if (customSubState == 0) { // Main Custom Screen
    // Character Customization Button
    if (mx >= bCharCstm.x && mx <= bCharCstm.x + bCharCstm.w &&
        my >= bCharCstm.y && my <= bCharCstm.y + bCharCstm.h) {
      customSubState = 1;
    }
    // Weather Customization Button
    else if (mx >= bWeatherCstm.x && mx <= bWeatherCstm.x + bWeatherCstm.w &&
             my >= bWeatherCstm.y && my <= bWeatherCstm.y + bWeatherCstm.h) {
      customSubState = 2;
    }
    // Back Button -> Return to Main Menu
    else if (mx >= bBack.x && mx <= bBack.x + bBack.w && my >= bBack.y &&
             my <= bBack.y + bBack.h) {
     
      return 0; // Signal to iMain to switch to MENU
    }
  } else if (customSubState == 1) { // Character Sub-menu
    for (int i = 0; i < 3; i++) {
      if (mx >= selChar[i].x && mx <= selChar[i].x + selChar[i].w &&
          my >= selChar[i].y && my <= selChar[i].y + selChar[i].h) {
        selectedCharacter = i;
        return 2; // Signal click
      }
    }
    // Back Button -> Return to Custom Main
    if (mx >= bBack.x && mx <= bBack.x + bBack.w && my >= bBack.y &&
        my <= bBack.y + bBack.h) {
      customSubState = 0;
      return 2; // Signal click
    }
  } else if (customSubState == 2) { // Weather Sub-menu
    for (int i = 0; i < 3; i++) {
      if (mx >= selWeather[i].x && mx <= selWeather[i].x + selWeather[i].w &&
          my >= selWeather[i].y && my <= selWeather[i].y + selWeather[i].h) {
        selectedWeather = i;
        return 2; // Signal click
      }
    }
    // Back Button -> Return to Custom Main
    if (mx >= bBack.x && mx <= bBack.x + bBack.w && my >= bBack.y &&
        my <= bBack.y + bBack.h) {
      customSubState = 0;
      return 2; // Signal click
    }
  }

  return -1;
}

#endif
