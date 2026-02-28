#ifndef CHARACTERCUSTOMIZATION_H
#define CHARACTERCUSTOMIZATION_H

#include "iGraphics.h"
#include <stdio.h>

// Defined in CustomizationMenu.h
extern int selectedCharacter;

// Variables to store image assets for each character
extern int arynWalk[10];
extern int kaelWalk[10];
extern int leoraWalk[10];
extern int arynWalkBack[10];
extern int kaelWalkBack[10];
extern int leoraWalkBack[10];

extern int arynJump[3];
extern int kaelJump[3];
extern int leoraJump[3];
extern int arynJumpBack[3];
extern int kaelJumpBack[3];
extern int leoraJumpBack[3];

extern int arynAttack[7];
extern int kaeroAttack[6];
extern int leoraAttack[6];
extern int arynAttackBack[7];
extern int kaeroAttackBack[6];
extern int leoraAttackBack[6];

extern int arynFall;
extern int kaeroFall;
extern int leoraFall;

// Load all character assets at startup
inline void loadCharacterCustomizationAssets() {
  char filename[100];

  // Load Aryn Images
  for (int i = 0; i < 10; i++) {
    sprintf_s(filename, "aryn\\aryn%d.png", i + 1);
    arynWalk[i] = iLoadImage(filename);
    sprintf_s(filename, "aryn\\arynback%d.png", i + 1);
    arynWalkBack[i] = iLoadImage(filename);
  }
  for (int i = 0; i < 3; i++) {
    sprintf_s(filename, "aryn\\arynjump%d.png", i + 1);
    arynJump[i] = iLoadImage(filename);
    sprintf_s(filename, "aryn\\arynjumpback%d.png", i + 1);
    arynJumpBack[i] = iLoadImage(filename);
  }
  for (int i = 0; i < 7; i++) {
    sprintf_s(filename, "aryn\\arynattack%d.png", i + 1);
    arynAttack[i] = iLoadImage(filename);
    sprintf_s(filename, "aryn\\arynattackback%d.png", i + 1);
    arynAttackBack[i] = iLoadImage(filename);
  }

  sprintf_s(filename, "aryn\\arynfall.png");
  arynFall = iLoadImage(filename);

  // Load Default Character Images (Frame/Kaero)
  for (int i = 0; i < 10; i++) {
    sprintf_s(filename, "Character\\Frame%d.png", i + 1);
    kaelWalk[i] = iLoadImage(filename);
    sprintf_s(filename, "Character\\Frameback%d.png", i + 1);
    kaelWalkBack[i] = iLoadImage(filename);
  }
  for (int i = 0; i < 3; i++) {
    sprintf_s(filename, "Character\\kaerojump%d.png", i + 1);
    kaelJump[i] = iLoadImage(filename);
    sprintf_s(filename, "Character\\kaerojumpback%d.png", i + 1);
    kaelJumpBack[i] = iLoadImage(filename);
  }
  for (int i = 0; i < 6; i++) {
    sprintf_s(filename, "Character\\kaeroattack%d.png", i + 1);
    kaeroAttack[i] = iLoadImage(filename);
    sprintf_s(filename, "Character\\kaeroattackback%d.png", i + 1);
    kaeroAttackBack[i] = iLoadImage(filename);
  }

  sprintf_s(filename, "Character\\kaerofall.png");
  kaeroFall = iLoadImage(filename);

  // Load Leora Images
  for (int i = 0; i < 10; i++) {
    sprintf_s(filename, "Leora\\leora%d.png", i + 1);
    leoraWalk[i] = iLoadImage(filename);
    sprintf_s(filename, "Leora\\leoraback%d.png", i + 1);
    leoraWalkBack[i] = iLoadImage(filename);
  }
  for (int i = 0; i < 3; i++) {
    sprintf_s(filename, "Leora\\leorajump%d.png", i + 1);
    leoraJump[i] = iLoadImage(filename);
    sprintf_s(filename, "Leora\\leorajumpback%d.png", i + 1);
    leoraJumpBack[i] = iLoadImage(filename);
  }
  for (int i = 0; i < 6; i++) {
    sprintf_s(filename, "Leora\\leoraattack%d.png", i + 1);
    leoraAttack[i] = iLoadImage(filename);
    sprintf_s(filename, "Leora\\leoraattackback%d.png", i + 1);
    leoraAttackBack[i] = iLoadImage(filename);
  }
  sprintf_s(filename, "Leora\\leorafall.png");
  leoraFall = iLoadImage(filename);
}

inline int getSelectedCharacterFallImage() {
  if (selectedCharacter == 0)
    return arynFall;
  else if (selectedCharacter == 1)
    return kaeroFall;
  else if (selectedCharacter == 2)
    return leoraFall;
  return kaeroFall;
}

// Helper to get the correct frame based on selection
// selectedCharacter is defined in CustomizationMenu.h
// 0: Aryn, 1: Kael, 2: Leora
inline int getSelectedCharacterImage(int frameIndex, bool isBack) {
  if (frameIndex < 0 || frameIndex >= 10)
    return -1; // Safety check

  if (selectedCharacter == 0) {
    return isBack ? arynWalkBack[frameIndex] : arynWalk[frameIndex];
  } else if (selectedCharacter == 1) {
    return isBack ? kaelWalkBack[frameIndex] : kaelWalk[frameIndex];
  } else if (selectedCharacter == 2) {
    return isBack ? leoraWalkBack[frameIndex] : leoraWalk[frameIndex];
  }
  return isBack ? kaelWalkBack[frameIndex] : kaelWalk[frameIndex]; // Fallback
}

inline int getSelectedCharacterJumpImage(int jumpIndex, bool isBack) {
  if (jumpIndex < 0 || jumpIndex >= 3)
    return -1;

  if (selectedCharacter == 0) {
    return isBack ? arynJumpBack[jumpIndex] : arynJump[jumpIndex];
  } else if (selectedCharacter == 1) {
    return isBack ? kaelJumpBack[jumpIndex] : kaelJump[jumpIndex];
  } else if (selectedCharacter == 2) {
    return isBack ? leoraJumpBack[jumpIndex] : leoraJump[jumpIndex];
  }
  return isBack ? kaelJumpBack[jumpIndex] : kaelJump[jumpIndex];
}

inline int getSelectedCharacterAttackImage(int attackIndex, bool isBack) {
  if (selectedCharacter == 0) {
    if (attackIndex < 0 || attackIndex >= 7)
      return -1;
    return isBack ? arynAttackBack[attackIndex] : arynAttack[attackIndex];
  } else if (selectedCharacter == 1) {
    if (attackIndex < 0 || attackIndex >= 6)
      return -1;
    return isBack ? kaeroAttackBack[attackIndex] : kaeroAttack[attackIndex];
  } else if (selectedCharacter == 2) {
    if (attackIndex < 0 || attackIndex >= 6)
      return -1;
    return isBack ? leoraAttackBack[attackIndex] : leoraAttack[attackIndex];
  }
  return -1;
}

#endif
