#ifndef GUNBARHANDLER_H
#define GUNBARHANDLER_H

#include "iGraphics.h"
#include <stdio.h>

// ---- External Dependencies ----
extern int currentLevel;
extern bool hasClaimedGun; // true if gun was earned in Level 2 IQ
extern bool isGamePaused;

// ---- Asset ----
int gunBarImg;
int emptyGunBarImg;

// ---- Position (below pause button: bPause.y=520, bPause.h=70) ----
#define GUN_BAR_X 35
#define GUN_BAR_Y 360 // 520 - 100 - 60px gap = 360
#define GUN_BAR_W 120
#define GUN_BAR_H 100
#define GUN_COOLDOWN_FRAMES 300 // 10 seconds @ 30fps

// ---- State ----
bool gunBarAvailable = false;             // Ready to use?
int gunBarCooldown = GUN_COOLDOWN_FRAMES; // Frames remaining
bool gunBarEquipped = false;              // Equip animation active?
int gunBarAnimTimer = 0;                  // Equip animation countdown

// ---- Load ----
inline void loadGunBarAssets() {
  gunBarImg = iLoadImage("scores and items\\gunbar.png");
  emptyGunBarImg = iLoadImage("scores and items\\emptygunbar.png");
}

// ---- Init (call on level reset) ----
inline void initGunBar() {
  gunBarAvailable = false;
  gunBarCooldown = GUN_COOLDOWN_FRAMES;
  gunBarEquipped = false;
  gunBarAnimTimer = 0;
}

// ---- Update (call every frame from masterGameLoop) ----
inline void updateGunBar() {
  if (currentLevel != 3 || !hasClaimedGun)
    return;
  if (isGamePaused)
    return;

  if (gunBarEquipped) {
    // Play equip animation countdown
    gunBarAnimTimer--;
    if (gunBarAnimTimer <= 0) {
      // Animation done -> start cooldown again
      gunBarEquipped = false;
      gunBarAvailable = false;
      gunBarCooldown = GUN_COOLDOWN_FRAMES;
    }
  } else if (!gunBarAvailable) {
    // Counting down cooldown
    gunBarCooldown--;
    if (gunBarCooldown <= 0) {
      gunBarCooldown = 0;
      gunBarAvailable = true;
    }
  }
}

// ---- Draw (call from iDraw during GAME state) ----
inline void drawGunBar() {
  if (currentLevel != 3 || !hasClaimedGun)
    return;

  int bx = GUN_BAR_X;
  int by = GUN_BAR_Y;
  int bw = GUN_BAR_W;
  int bh = GUN_BAR_H;

  if (gunBarEquipped) {
    // Equip animation: scale up ~15% centered on bar
    int ew = (int)(bw * 1.15f);
    int eh = (int)(bh * 1.15f);
    int ex = bx - (ew - bw) / 2;
    int ey = by - (eh - bh) / 2;
    iSetColor(255, 255, 255);
    iShowImage(ex, ey, ew, eh, gunBarImg);
  } else {
    if (!gunBarAvailable) {
      // Draw empty bar image (cooldown state)
      iSetColor(255, 255, 255);
      iShowImage(bx, by, bw, bh, emptyGunBarImg);

      // Draw cooldown seconds remaining (centered text)
      int secondsLeft = (gunBarCooldown + 29) / 30; // round up
      char timeStr[8];
      sprintf_s(timeStr, "%ds", secondsLeft);
      iSetColor(0, 0, 0); // Black text
      // Center text: each char ~9px wide, font height ~18px
      int textLen = (int)strlen(timeStr);
      int tx = bx + (bw - textLen * 9) / 2;
      int ty = by + (bh - 18) / 2;
      iText(tx, ty, timeStr, GLUT_BITMAP_HELVETICA_18);
    } else {
      // Draw base bar image (available state)
      iSetColor(255, 255, 255);
      iShowImage(bx, by, bw, bh, gunBarImg);
    }
  }
}

// ---- Click Handler (call from iMouse GAME block) ----
// Returns true if the gun was just equipped
inline bool handleGunBarClick(int mx, int my) {
  if (currentLevel != 3 || !hasClaimedGun)
    return false;
  if (!gunBarAvailable || gunBarEquipped)
    return false;

  if (mx >= GUN_BAR_X && mx <= GUN_BAR_X + GUN_BAR_W && my >= GUN_BAR_Y &&
      my <= GUN_BAR_Y + GUN_BAR_H) {
    gunBarEquipped = true;
    gunBarAnimTimer = 15; // ~0.5s animation at 30fps
    gunBarAvailable = false;
    return true;
  }
  return false;
}

#endif
