#ifndef STORYHANDLER_H
#define STORYHANDLER_H

#include "iGraphics.h"
#include <stdio.h>

// Story Asset Handles
extern int initialStoryImages[12];
extern int level1IntroImage;
extern int level2IntroImage, level3IntroImage, level4IntroImage;
extern int level1StoryImages[3];
extern int level2StoryImages[3];
extern int level3StoryImages[3];
extern int level4StoryImages[3];
extern int cave2StoryImages[3];
extern int finalVictoryImages[2];
extern int finalStoryImages[11];
extern int levelSelectionBG;
extern int level1BtnImg, level2BtnImg, level3BtnImg, level4BtnImg;
extern int nextBtnImg, skipBtnImg, homeBtnImg, prevBtnImg, btnLvlCustomImg;

struct LevelButton {
  int x, y, w, h;
};
extern LevelButton lvl1Btn, lvl2Btn, lvl3Btn, lvl4Btn, lvlCustomBtn;

// Story State
extern int initialStoryIndex;
extern int level1StoryIndex;
extern int level2StoryIndex;
extern int level3StoryIndex;
extern int level4StoryIndex;
extern int cave2StoryIndex;
extern int finalVictoryIndex;
extern int finalStoryIndex;
extern int storyTimerCount;

// Animation globals (defined in iMain.cpp)
extern bool btnAnimActive;
extern int  btnAnimTimer;
extern int  btnAnimCode;    // 20=next,21=skip,22=prev,30-34=level btns (context 2)
extern int  btnAnimContext; // 0=MainMenu, 1=Custom, 2=Story

// Button dimensions and position (Lower Right Corner)
// Screen 1000x600
#define btnStoryW 180
#define btnStoryH 55
#define nextBtnX 800
#define nextBtnY 80
#define prevBtnX 20
#define prevBtnY 20
#define skipBtnX 800
#define skipBtnY 20
#define homeBtnX 20
#define homeBtnY 500
#define homeBtnW 80
#define homeBtnH 80

// Grow animation helper for story buttons (uses fixed x/y/w/h)
#define DRAW_STORY_BTN(bx, by, bw, bh, img, code) \
  do { \
    if (btnAnimActive && btnAnimContext == 2 && btnAnimCode == (code)) { \
      int _ew = (int)((bw) * 1.15f); \
      int _eh = (int)((bh) * 1.15f); \
      int _ex = (bx) - (_ew - (bw)) / 2; \
      int _ey = (by) - (_eh - (bh)) / 2; \
      iShowImage(_ex, _ey, _ew, _eh, (img)); \
    } else { \
      iShowImage((bx), (by), (bw), (bh), (img)); \
    } \
  } while(0)

// Grow animation helper for level-selection buttons (LevelButton struct)
#define DRAW_LEVEL_BTN(btn, img, code) \
  do { \
    if (btnAnimActive && btnAnimContext == 2 && btnAnimCode == (code)) { \
      int _ew = (int)((btn).w * 1.15f); \
      int _eh = (int)((btn).h * 1.15f); \
      int _ex = (btn).x - (_ew - (btn).w) / 2; \
      int _ey = (btn).y - (_eh - (btn).h) / 2; \
      iShowImage(_ex, _ey, _ew, _eh, (img)); \
    } else { \
      iShowImage((btn).x, (btn).y, (btn).w, (btn).h, (img)); \
    } \
  } while(0)

// Note: Function definitions are kept here for simplicity as per user's small
// header pattern, but relying on iGraphics.h being included in iMain.cpp before
// this file.

inline void loadStoryAssets() {
  static bool storyLoaded = false;
  if (storyLoaded) return;
  storyLoaded = true;

  char path[100];

  // Initial Story
  for (int i = 0; i < 12; i++) {
    sprintf_s(path, sizeof(path), "story\\initial story\\%d.png", i + 1);
    initialStoryImages[i] = iLoadImage(path);
  }

  // Level 1 Intro
  level1IntroImage = iLoadImage((char *)"story\\level 1\\frontpage.png");

  // Other Level Intros
  level2IntroImage = iLoadImage((char *)"story\\level 2\\frontpage2.png");
  level3IntroImage = iLoadImage((char *)"story\\level 3\\frontpage3.png");
  level4IntroImage = iLoadImage((char *)"story\\level 4\\frontpage4.png");

  // Level 1 Story
  for (int i = 0; i < 3; i++) {
    sprintf_s(path, sizeof(path), "story\\level 1\\%d.png", i + 1);
    level1StoryImages[i] = iLoadImage(path);
  }

  // Level 2 Story
  for (int i = 0; i < 3; i++) {
    sprintf_s(path, sizeof(path), "story\\level 2\\%d.png", i + 8);
    level2StoryImages[i] = iLoadImage(path);
  }

  // Level 3 Story
  for (int i = 0; i < 3; i++) {
    sprintf_s(path, sizeof(path), "story\\level 3\\%d.png", i + 14);
    level3StoryImages[i] = iLoadImage(path);
  }

  // Level 4 Story
  for (int i = 0; i < 3; i++) {
    sprintf_s(path, sizeof(path), "story\\level 4\\%d.png", i + 18);
    level4StoryImages[i] = iLoadImage(path);
  }

  // Cave 2 Story
  for (int i = 0; i < 3; i++) {
    sprintf_s(path, sizeof(path), "story\\cave2story\\%d.png", i + 11);
    cave2StoryImages[i] = iLoadImage(path);
  }

  // Final Victory Frames (level4\final victory\1.png, 2.png)
  for (int i = 0; i < 2; i++) {
    sprintf_s(path, sizeof(path), "level4\\final victory\\%d.png", i + 1);
    finalVictoryImages[i] = iLoadImage(path);
  }

  // Final Story Pages (story\final story\1.png .. 11.png)
  for (int i = 0; i < 11; i++) {
    sprintf_s(path, sizeof(path), "story\\final story\\%d.png", i + 1);
    finalStoryImages[i] = iLoadImage(path);
  }

  // Level Selection Assets
  levelSelectionBG = iLoadImage((char *)"story\\level btn bg.png");
  level1BtnImg = iLoadImage((char *)"story\\l1 btn.png");
  level2BtnImg = iLoadImage((char *)"story\\l2 btn.png");
  level3BtnImg = iLoadImage((char *)"story\\l3 btn.png");
  level4BtnImg = iLoadImage((char *)"story\\l4 btn.png");

  // Buttons
  nextBtnImg = iLoadImage((char *)"story\\next button.png");
  skipBtnImg = iLoadImage((char *)"story\\skip button.png");
  homeBtnImg = iLoadImage((char *)"story\\home icon button.png");
  prevBtnImg = iLoadImage((char *)"story\\previous button.png");
  btnLvlCustomImg = iLoadImage((char *)"story\\customization button.png");

  // Initialize Level Selection Buttons
  lvl1Btn.w = 330;
  lvl1Btn.h = 100;
  lvl1Btn.x = 300 - lvl1Btn.w / 2;
  lvl1Btn.y = 400 - lvl1Btn.h / 2;

  lvl2Btn.w = 330;
  lvl2Btn.h = 100;
  lvl2Btn.x = 300 - lvl2Btn.w / 2;
  lvl2Btn.y = 200 - lvl2Btn.h / 2;

  lvl3Btn.w = 330;
  lvl3Btn.h = 100;
  lvl3Btn.x = 700 - lvl3Btn.w / 2;
  lvl3Btn.y = 400 - lvl3Btn.h / 2;

  lvl4Btn.w = 330;
  lvl4Btn.h = 180;
  lvl4Btn.x = 700 - lvl4Btn.w / 2;
  lvl4Btn.y = 200 - lvl4Btn.h / 2;

  // Customization Button (Bottom Middle)
  lvlCustomBtn.w = 330; // Using similar width to level buttons
  lvlCustomBtn.h = 100; // Increased height
  lvlCustomBtn.x = 500 - lvlCustomBtn.w / 2;
  lvlCustomBtn.y = 20;
}

inline void drawHomeButton() {
  if (btnAnimActive && btnAnimContext == 2 && btnAnimCode == 23) {
    int _ew = (int)(homeBtnW * 1.15f);
    int _eh = (int)(homeBtnH * 1.15f);
    int _ex = homeBtnX - (_ew - homeBtnW) / 2;
    int _ey = homeBtnY - (_eh - homeBtnH) / 2;
    iShowImage(_ex, _ey, _ew, _eh, homeBtnImg);
  } else {
    iShowImage(homeBtnX, homeBtnY, homeBtnW, homeBtnH, homeBtnImg);
  }
}

inline void drawLevelSelection() {
  iShowImage(0, 0, 1000, 600, levelSelectionBG);
  drawHomeButton();

  // Four buttons with grow animation
  DRAW_LEVEL_BTN(lvl1Btn, level1BtnImg, 30);
  DRAW_LEVEL_BTN(lvl2Btn, level2BtnImg, 31);
  DRAW_LEVEL_BTN(lvl3Btn, level3BtnImg, 32);
  DRAW_LEVEL_BTN(lvl4Btn, level4BtnImg, 33);

  // Draw Customization Button (with grow animation)
  DRAW_LEVEL_BTN(lvlCustomBtn, btnLvlCustomImg, 34);
}

inline void drawInitialStory() {
  if (initialStoryIndex < 12) {
    iShowImage(0, 0, 1000, 600, initialStoryImages[initialStoryIndex]);
    drawHomeButton();

    // Draw Buttons in lower right (with grow animation)
    DRAW_STORY_BTN(nextBtnX, nextBtnY, btnStoryW, btnStoryH, nextBtnImg, 20);
    DRAW_STORY_BTN(prevBtnX, prevBtnY, btnStoryW, btnStoryH, prevBtnImg, 22);
    DRAW_STORY_BTN(skipBtnX, skipBtnY, btnStoryW, btnStoryH, skipBtnImg, 21);
  }
}

inline void drawLevel1Intro() {
  iShowImage(0, 0, 1000, 600, level1IntroImage);
  drawHomeButton();

  // Draw Navigation Buttons (with grow animation)
  DRAW_STORY_BTN(nextBtnX, nextBtnY, btnStoryW, btnStoryH, nextBtnImg, 20);
  DRAW_STORY_BTN(prevBtnX, prevBtnY, btnStoryW, btnStoryH, prevBtnImg, 22);
  DRAW_STORY_BTN(skipBtnX, skipBtnY, btnStoryW, btnStoryH, skipBtnImg, 21);
}

inline void drawLevel2Intro() {
  iShowImage(0, 0, 1000, 600, level2IntroImage);
  drawHomeButton();

  // Draw Navigation Buttons (with grow animation)
  DRAW_STORY_BTN(nextBtnX, nextBtnY, btnStoryW, btnStoryH, nextBtnImg, 20);
  DRAW_STORY_BTN(prevBtnX, prevBtnY, btnStoryW, btnStoryH, prevBtnImg, 22);
  DRAW_STORY_BTN(skipBtnX, skipBtnY, btnStoryW, btnStoryH, skipBtnImg, 21);
}

inline void drawLevel3Intro() {
  iShowImage(0, 0, 1000, 600, level3IntroImage);
  drawHomeButton();

  // Draw Navigation Buttons (with grow animation)
  DRAW_STORY_BTN(nextBtnX, nextBtnY, btnStoryW, btnStoryH, nextBtnImg, 20);
  DRAW_STORY_BTN(prevBtnX, prevBtnY, btnStoryW, btnStoryH, prevBtnImg, 22);
  DRAW_STORY_BTN(skipBtnX, skipBtnY, btnStoryW, btnStoryH, skipBtnImg, 21);
}

inline void drawLevel4Intro() {
  iShowImage(0, 0, 1000, 600, level4IntroImage);
  drawHomeButton();

  // Draw Navigation Buttons (with grow animation)
  DRAW_STORY_BTN(nextBtnX, nextBtnY, btnStoryW, btnStoryH, nextBtnImg, 20);
  DRAW_STORY_BTN(prevBtnX, prevBtnY, btnStoryW, btnStoryH, prevBtnImg, 22);
  DRAW_STORY_BTN(skipBtnX, skipBtnY, btnStoryW, btnStoryH, skipBtnImg, 21);
}

inline void drawLevelTwoStory() {
  if (level2StoryIndex < 3) {
    iShowImage(0, 0, 1000, 600, level2StoryImages[level2StoryIndex]);
    DRAW_STORY_BTN(nextBtnX, nextBtnY, btnStoryW, btnStoryH, nextBtnImg, 20);
    DRAW_STORY_BTN(prevBtnX, prevBtnY, btnStoryW, btnStoryH, prevBtnImg, 22);
    DRAW_STORY_BTN(skipBtnX, skipBtnY, btnStoryW, btnStoryH, skipBtnImg, 21);
  }
}

inline void drawLevelThreeStory() {
  if (level3StoryIndex < 3) {
    iShowImage(0, 0, 1000, 600, level3StoryImages[level3StoryIndex]);
    DRAW_STORY_BTN(nextBtnX, nextBtnY, btnStoryW, btnStoryH, nextBtnImg, 20);
    DRAW_STORY_BTN(prevBtnX, prevBtnY, btnStoryW, btnStoryH, prevBtnImg, 22);
    DRAW_STORY_BTN(skipBtnX, skipBtnY, btnStoryW, btnStoryH, skipBtnImg, 21);
  }
}

inline void drawLevelFourStory() {
  if (level4StoryIndex < 3) {
    iShowImage(0, 0, 1000, 600, level4StoryImages[level4StoryIndex]);
    DRAW_STORY_BTN(nextBtnX, nextBtnY, btnStoryW, btnStoryH, nextBtnImg, 20);
    DRAW_STORY_BTN(prevBtnX, prevBtnY, btnStoryW, btnStoryH, prevBtnImg, 22);
    DRAW_STORY_BTN(skipBtnX, skipBtnY, btnStoryW, btnStoryH, skipBtnImg, 21);
  }
}

inline void drawCaveTwoStory() {
  if (cave2StoryIndex < 3) {
    iShowImage(0, 0, 1000, 600, cave2StoryImages[cave2StoryIndex]);
    DRAW_STORY_BTN(nextBtnX, nextBtnY, btnStoryW, btnStoryH, nextBtnImg, 20);
    DRAW_STORY_BTN(prevBtnX, prevBtnY, btnStoryW, btnStoryH, prevBtnImg, 22);
    DRAW_STORY_BTN(skipBtnX, skipBtnY, btnStoryW, btnStoryH, skipBtnImg, 21);
  }
}

inline void drawLevel1Story() {
  if (level1StoryIndex < 3) {
    iShowImage(0, 0, 1000, 600, level1StoryImages[level1StoryIndex]);
    DRAW_STORY_BTN(nextBtnX, nextBtnY, btnStoryW, btnStoryH, nextBtnImg, 20);
    DRAW_STORY_BTN(prevBtnX, prevBtnY, btnStoryW, btnStoryH, prevBtnImg, 22);
    DRAW_STORY_BTN(skipBtnX, skipBtnY, btnStoryW, btnStoryH, skipBtnImg, 21);
  }
}

inline int handleStoryMouse(int mx, int my) {
  if (mx >= nextBtnX && mx <= nextBtnX + btnStoryW && my >= nextBtnY &&
      my <= nextBtnY + btnStoryH) {
    return 1; // Next
  }
  if (mx >= prevBtnX && mx <= prevBtnX + btnStoryW && my >= prevBtnY &&
      my <= prevBtnY + btnStoryH) {
    return 3; // Previous
  }
  if (mx >= skipBtnX && mx <= skipBtnX + btnStoryW && my >= skipBtnY &&
      my <= skipBtnY + btnStoryH) {
    return 2; // Skip
  }
  return 0;
}

inline int handleLevelSelectionMouse(int mx, int my) {
  // L1
  if (mx >= lvl1Btn.x && mx <= lvl1Btn.x + lvl1Btn.w && my >= lvl1Btn.y &&
      my <= lvl1Btn.y + lvl1Btn.h)
    return 1;
  // L2
  if (mx >= lvl2Btn.x && mx <= lvl2Btn.x + lvl2Btn.w && my >= lvl2Btn.y &&
      my <= lvl2Btn.y + lvl2Btn.h)
    return 2;
  // L3
  if (mx >= lvl3Btn.x && mx <= lvl3Btn.x + lvl3Btn.w && my >= lvl3Btn.y &&
      my <= lvl3Btn.y + lvl3Btn.h)
    return 3;
  // L4
  if (mx >= lvl4Btn.x && mx <= lvl4Btn.x + lvl4Btn.w && my >= lvl4Btn.y &&
      my <= lvl4Btn.y + lvl4Btn.h)
    return 4;

  // Customization Button
  if (mx >= lvlCustomBtn.x && mx <= lvlCustomBtn.x + lvlCustomBtn.w &&
      my >= lvlCustomBtn.y && my <= lvlCustomBtn.y + lvlCustomBtn.h)
    return 5; // Customization

  return 0;
}

inline int handleHomeButtonMouse(int mx, int my) {
  if (mx >= homeBtnX && mx <= homeBtnX + homeBtnW && my >= homeBtnY &&
      my <= homeBtnY + homeBtnH) {
    return 1;
  }
  return 0;
}

// ---- FINAL VICTORY: next button hitbox (cave-style: 750,50,220,70) ----
inline int handleFinalVictoryClick(int mx, int my) {
  if (mx >= 750 && mx <= 970 && my >= 50 && my <= 120)
    return 1;
  return 0;
}

// Draw final victory frame (level4 bg + victory image centered + animated next btn)
// Requires bg41/bg42 to be valid (loaded via loadBossAssets)
inline void drawFinalVictory() {
  extern int bg41, bg42, bGX;
  // Draw the frozen level-4 background at its current scroll position
  iShowImage(bGX,        0, 1000, 600, bg41);
  iShowImage(bGX + 1000, 0, 1000, 600, bg42);
  iShowImage(bGX + 2000, 0, 1000, 600, bg42);
  iShowImage(bGX + 3000, 0, 1000, 600, bg42);
  // Victory frame centered (700x450) on top of background
  if (finalVictoryIndex >= 0 && finalVictoryIndex < 2) {
    iSetColor(255, 255, 255);
    iShowImage(150, 75, 700, 450, finalVictoryImages[finalVictoryIndex]);
  }
  // Animated story-style Next button at bottom-right (code 20 for grow anim)
  extern int imgNextLevel;
  iSetColor(255, 255, 255);
  DRAW_STORY_BTN(750, 50, 220, 70, imgNextLevel, 20);
}

// Draw final story page with prev/next/skip buttons
inline void drawFinalStory() {
  if (finalStoryIndex >= 0 && finalStoryIndex < 11) {
    iSetColor(255, 255, 255);
    iShowImage(0, 0, 1000, 600, finalStoryImages[finalStoryIndex]);
    DRAW_STORY_BTN(nextBtnX, nextBtnY, btnStoryW, btnStoryH, nextBtnImg, 20);
    DRAW_STORY_BTN(prevBtnX, prevBtnY, btnStoryW, btnStoryH, prevBtnImg, 22);
    // On last page, no skip (next goes to main menu)
    if (finalStoryIndex < 10) {
      DRAW_STORY_BTN(skipBtnX, skipBtnY, btnStoryW, btnStoryH, skipBtnImg, 21);
    }
  }
}

#endif
