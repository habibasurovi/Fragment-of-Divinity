#ifndef GAMEOVERHANDLER_H
#define GAMEOVERHANDLER_H

#include "iGraphics.h"
#include "LevelCompleteHandler.h"

// Extern variables defined in iMain.cpp
extern int imgGameOverBG;
extern int imgSettingsBtn;

extern ButtonRect btnRestartGO;
extern ButtonRect btnHomeGO;
extern ButtonRect btnSettingsGO;

inline void loadGameOverAssets() {
    imgGameOverBG = iLoadImage((char *)"background img\\game over.png");
    imgSettingsBtn = iLoadImage((char *)"background img\\settings.png");
    
    // Adjusted Y-coordinates and dimensions specifically for Game Over menu
    btnHomeGO.x = LC_FRAME_X + 110;
    btnHomeGO.y = LC_FRAME_Y + 210; // Increased by 50
    btnHomeGO.w = LC_BTN_W;
    btnHomeGO.h = LC_BTN_H;

    btnRestartGO.x = (1000 - LC_BTN_W) / 2;
    btnRestartGO.y = LC_FRAME_Y + 130; // Increased by 50
    btnRestartGO.w = LC_BTN_W;
    btnRestartGO.h = LC_BTN_H;

    btnSettingsGO.x = LC_FRAME_X + LC_FRAME_W - LC_BTN_W - 140; // Shifted left to accommodate extra width
    btnSettingsGO.y = LC_FRAME_Y + 210; // Increased by 50
    btnSettingsGO.w = LC_BTN_W + 40; // Increased width
    btnSettingsGO.h = LC_BTN_H;
}

inline void drawGameOverMenu() {
    // Draw the Game Over background same size/location as Level Complete
    iShowImage(LC_FRAME_X, LC_FRAME_Y, LC_FRAME_W, LC_FRAME_H, imgGameOverBG);
    
    // Draw the 3 buttons
    iShowImage(btnHomeGO.x, btnHomeGO.y, btnHomeGO.w, btnHomeGO.h, imgHomeLevel);
    iShowImage(btnRestartGO.x, btnRestartGO.y, btnRestartGO.w, btnRestartGO.h, imgRestartLevel);
    iShowImage(btnSettingsGO.x, btnSettingsGO.y, btnSettingsGO.w, btnSettingsGO.h, imgSettingsBtn);
}

// Actions: 1: Restart, 2: Home, 3: Settings, 0: None
inline int handleGameOverMouse(int mx, int my) {
    if (mx >= btnRestartGO.x && mx <= btnRestartGO.x + btnRestartGO.w &&
        my >= btnRestartGO.y && my <= btnRestartGO.y + btnRestartGO.h) {
        return 1;
    }
    if (mx >= btnHomeGO.x && mx <= btnHomeGO.x + btnHomeGO.w &&
        my >= btnHomeGO.y && my <= btnHomeGO.y + btnHomeGO.h) {
        return 2;
    }
    if (mx >= btnSettingsGO.x && mx <= btnSettingsGO.x + btnSettingsGO.w &&
        my >= btnSettingsGO.y && my <= btnSettingsGO.y + btnSettingsGO.h) {
        return 3;
    }
    return 0;
}

#endif
