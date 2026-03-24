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

extern int arynAttack[4];
extern int kaeroAttack[6];
extern int leoraAttack[3];
extern int arynAttackBack[4];
extern int kaeroAttackBack[6];
extern int leoraAttackBack[3];

extern int arynFall;
extern int kaeroFall;
extern int leoraFall;

extern int arynJumpAttack[2];
extern int kaeroJumpAttack[2];
extern int leoraJumpAttack[2];

// Static (idle replacement) images for Level 4
extern int arynStatic[3];
extern int arynStaticBack[3];
extern int kaeroStatic[3];
extern int kaeroStaticBack[3];
extern int leoraStatic[3];
extern int leoraStaticBack[3];

inline void loadCharacterCustomizationAssets() {
  static bool characterAssetsLoaded = false;
  if (characterAssetsLoaded) return;
  characterAssetsLoaded = true;

  char filename[100];

  // Load Aryn Images
  for (int i = 0; i < 10; i++) {
    sprintf_s(filename, sizeof(filename), "aryn\\aryn%d.png", i + 1);
    arynWalk[i] = iLoadImage((char *)filename);
    sprintf_s(filename, sizeof(filename), "aryn\\arynback%d.png", i + 1);
    arynWalkBack[i] = iLoadImage((char *)filename);
  }
  for (int i = 0; i < 3; i++) {
    sprintf_s(filename, sizeof(filename), "aryn\\arynjump%d.png", i + 1);
    arynJump[i] = iLoadImage((char *)filename);
    sprintf_s(filename, sizeof(filename), "aryn\\arynjumpback%d.png", i + 1);
    arynJumpBack[i] = iLoadImage((char *)filename);
  }
  for (int i = 0; i < 4; i++) {
    sprintf_s(filename, sizeof(filename), "aryn\\arynattack%d.png", i + 1);
    arynAttack[i] = iLoadImage((char *)filename);
    sprintf_s(filename, sizeof(filename), "aryn\\arynattackback%d.png", i + 1);
    arynAttackBack[i] = iLoadImage((char *)filename);
  }

  strcpy_s(filename, sizeof(filename), "aryn\\arynfall.png");
  arynFall = iLoadImage((char *)filename);

  // Load Default Character Images (Frame/Kaero)
  for (int i = 0; i < 10; i++) {
    sprintf_s(filename, sizeof(filename), "Character\\Frame%d.png", i + 1);
    kaelWalk[i] = iLoadImage((char *)filename);
    sprintf_s(filename, sizeof(filename), "Character\\Frameback%d.png", i + 1);
    kaelWalkBack[i] = iLoadImage((char *)filename);
  }
  for (int i = 0; i < 3; i++) {
    sprintf_s(filename, sizeof(filename), "Character\\kaerojump%d.png", i + 1);
    kaelJump[i] = iLoadImage((char *)filename);
    sprintf_s(filename, sizeof(filename), "Character\\kaerojumpback%d.png", i + 1);
    kaelJumpBack[i] = iLoadImage((char *)filename);
  }
  for (int i = 0; i < 6; i++) {
    sprintf_s(filename, sizeof(filename), "Character\\kaeroattack%d.png", i + 1);
    kaeroAttack[i] = iLoadImage((char *)filename);
    sprintf_s(filename, sizeof(filename), "Character\\kaeroattackback%d.png", i + 1);
    kaeroAttackBack[i] = iLoadImage((char *)filename);
  }

  strcpy_s(filename, sizeof(filename), "Character\\kaerofall.png");
  kaeroFall = iLoadImage((char *)filename);

  // Load Leora Images
  for (int i = 0; i < 10; i++) {
    sprintf_s(filename, sizeof(filename), "Leora\\leora%d.png", i + 1);
    leoraWalk[i] = iLoadImage((char *)filename);
    sprintf_s(filename, sizeof(filename), "Leora\\leoraback%d.png", i + 1);
    leoraWalkBack[i] = iLoadImage((char *)filename);
  }
  for (int i = 0; i < 3; i++) {
    sprintf_s(filename, sizeof(filename), "Leora\\leorajump%d.png", i + 1);
    leoraJump[i] = iLoadImage((char *)filename);
    sprintf_s(filename, sizeof(filename), "Leora\\leorajumpback%d.png", i + 1);
    leoraJumpBack[i] = iLoadImage((char *)filename);
  }
  for (int i = 0; i < 3; i++) {
    sprintf_s(filename, sizeof(filename), "Leora\\leoraattack%d.png", i + 1);
    leoraAttack[i] = iLoadImage((char *)filename);
    sprintf_s(filename, sizeof(filename), "Leora\\leoraattackback%d.png", i + 1);
    leoraAttackBack[i] = iLoadImage((char *)filename);
  }
  strcpy_s(filename, sizeof(filename), "Leora\\leorafall.png");
  leoraFall = iLoadImage((char *)filename);

  // Load Static images for Level 4 idle
  for (int i = 0; i < 3; i++) {
    sprintf_s(filename, sizeof(filename), "aryn\\arynstatic%d.png", i + 1);
    arynStatic[i] = iLoadImage((char *)filename);
    sprintf_s(filename, sizeof(filename), "aryn\\arynstaticback%d.png", i + 1);
    arynStaticBack[i] = iLoadImage((char *)filename);
  }
  for (int i = 0; i < 2; i++) {
    sprintf_s(filename, sizeof(filename), "Character\\kaerostatic%d.png", i + 1);
    kaeroStatic[i] = iLoadImage((char *)filename);
    sprintf_s(filename, sizeof(filename), "Character\\kaerostaticback%d.png", i + 1);
    kaeroStaticBack[i] = iLoadImage((char *)filename);
  }
  for (int i = 0; i < 2; i++) {
    sprintf_s(filename, sizeof(filename), "Leora\\leorastatic%d.png", i + 1);
    leoraStatic[i] = iLoadImage((char *)filename);
    sprintf_s(filename, sizeof(filename), "Leora\\leorastaticback%d.png", i + 1);
    leoraStaticBack[i] = iLoadImage((char *)filename);
  }

  // Load JumpAttack images
  for (int i = 0; i < 2; i++) {
    sprintf_s(filename, sizeof(filename), "aryn\\arynjumpattack%d.png", i + 1);
    arynJumpAttack[i] = iLoadImage((char *)filename);
    sprintf_s(filename, sizeof(filename), "Character\\kaerojumpattack%d.png", i + 1);
    kaeroJumpAttack[i] = iLoadImage((char *)filename);
    sprintf_s(filename, sizeof(filename), "Leora\\leorajumpattack%d.png", i + 1);
    leoraJumpAttack[i] = iLoadImage((char *)filename);
  }
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
    if (attackIndex < 0 || attackIndex >= 4)
      return -1;
    return isBack ? arynAttackBack[attackIndex] : arynAttack[attackIndex];
  } else if (selectedCharacter == 1) {
    if (attackIndex < 0 || attackIndex >= 6)
      return -1;
    return isBack ? kaeroAttackBack[attackIndex] : kaeroAttack[attackIndex];
  } else if (selectedCharacter == 2) {
    if (attackIndex < 0 || attackIndex >= 3)
      return -1;
    return isBack ? leoraAttackBack[attackIndex] : leoraAttack[attackIndex];
  }
  return -1;
}

inline int getSelectedCharacterJumpAttackImage(int index) {
  if (index < 0 || index >= 2) return -1;
  if (selectedCharacter == 0) return arynJumpAttack[index];
  if (selectedCharacter == 1) return kaeroJumpAttack[index];
  if (selectedCharacter == 2) return leoraJumpAttack[index];
  return kaeroJumpAttack[index];
}

// Returns the level-4 static idle image
inline int getSelectedCharacterStaticImage(int frameIndex, bool isBack) {
  if (selectedCharacter == 0) {
    return isBack ? arynStaticBack[frameIndex % 3] : arynStatic[frameIndex % 3];
  } else if (selectedCharacter == 1) {
    return isBack ? kaeroStaticBack[frameIndex % 2] : kaeroStatic[frameIndex % 2];
  } else if (selectedCharacter == 2) {
    return isBack ? leoraStaticBack[frameIndex % 2] : leoraStatic[frameIndex % 2];
  }
  return isBack ? kaeroStaticBack[frameIndex % 2] : kaeroStatic[frameIndex % 2];
}

#endif
