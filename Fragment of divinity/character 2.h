#ifndef CHARACTER2_H
#define CHARACTER2_H

#include "iGraphics.h"
#include <stdio.h>

extern int selectedCharacter;

// Variables to store run image assets
extern int arynRun[9];
extern int kaeroRun[8];
extern int leoraRun[7];
extern int arynRunBack[9];
extern int kaeroRunBack[8];
extern int leoraRunBack[7];

// Load run assets
inline void loadCharacter2Assets() {
  char filename[100];

  // Load Aryn Run (aryn(run)1.png to aryn(run)9.png) in aryn folder
  for (int i = 0; i < 9; i++) {
    sprintf_s(filename, "aryn\\aryn(run)%d.png", i + 1);
    arynRun[i] = iLoadImage(filename);
    sprintf_s(filename, "aryn\\arynback(1)%d.png", i + 1);
    arynRunBack[i] = iLoadImage(filename);
  }

  // Load Kaero Run (kaero(run)1.png to kaero(run)8.png) in Character folder
  for (int i = 0; i < 8; i++) {
    sprintf_s(filename, "Character\\kaero(run)%d.png", i + 1);
    kaeroRun[i] = iLoadImage(filename);
    sprintf_s(filename, "Character\\kaeroback(1)%d.png", i + 1);
    kaeroRunBack[i] = iLoadImage(filename);
  }

  // Load Leora Run (leora(run)1.png to leora(run)7.png) in Leora folder
  for (int i = 0; i < 7; i++) {
    sprintf_s(filename, "Leora\\leora(run)%d.png", i + 1);
    leoraRun[i] = iLoadImage(filename);
    sprintf_s(filename, "Leora\\leoraback(1)%d.png", i + 1);
    leoraRunBack[i] = iLoadImage(filename);
  }
}

// Helper to get run frame
inline int getCharacterRunImage(int frameIndex, bool isBack) {
  if (selectedCharacter == 0) {
    if (frameIndex < 0 || frameIndex >= 9)
      return -1;
    return isBack ? arynRunBack[frameIndex] : arynRun[frameIndex];
  } else if (selectedCharacter == 1) {
    if (frameIndex < 0 || frameIndex >= 8)
      return -1;
    return isBack ? kaeroRunBack[frameIndex] : kaeroRun[frameIndex];
  } else if (selectedCharacter == 2) {
    if (frameIndex < 0 || frameIndex >= 7)
      return -1;
    return isBack ? leoraRunBack[frameIndex] : leoraRun[frameIndex];
  }
  return -1;
}

// Logic for right arrow key run
extern bool isRightArrowPressed;
extern bool isLeftArrowPressed;
extern int runFrameIndex;
extern int runAnimCounter;
extern int runAnimSpeed;

inline void updateRunAnimation() {
  int maxFrames = 1;
  if (selectedCharacter == 0)
    maxFrames = 9;
  else if (selectedCharacter == 1)
    maxFrames = 8;
  else if (selectedCharacter == 2)
    maxFrames = 7;

  if (isRightArrowPressed || isLeftArrowPressed) {
    runAnimCounter++;
    if (runAnimCounter >= runAnimSpeed) {
      runAnimCounter = 0;
      runFrameIndex++;
      if (runFrameIndex >= maxFrames) {
        runFrameIndex = 0;
      }
    }
  }
}

#endif
