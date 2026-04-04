#ifndef DRAGON_H
#define DRAGON_H

#include "iGraphics.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef SCREEN_W
#define SCREEN_W 1000
#endif

#include "GameState.h"

extern int charX, charY, charWidth, charHeight;
extern int currentLevel;
extern bool isGamePaused;
extern GameState gameState;
extern int gameRunTimeSeconds;
extern unsigned int keyPressed[512];

extern int dragonFlyImgs[3][3];
extern int dragonAttackImgs[3][2];
extern int dragonBackFlyImgs[3][3];
extern int dragonBackAttackImgs[3][2];

struct Dragon {
  float x, y;
  int type; // 1, 2, or 3
  bool active;
  bool isAttacking;
  bool facingRight; 

  int animCounter;
  float speed;
  int flyFrameIndex;    
  int attackFrameIndex; 

  // Vanishing mechanics
  bool isVanishing;
  int vanishingTimer;
  int holdATimer;
  int cooldownTimer;
};

extern Dragon dragons[5];

inline void loadDragonAssets() {
  char filename[100];
  for (int d = 0; d < 3; d++) {
    for (int i = 0; i < 3; i++) {
      sprintf_s(filename, sizeof(filename), "Dragons\\Dragon(%d)%d.png", d + 1, i + 1);
      dragonFlyImgs[d][i] = iLoadImage((char *)filename);
      sprintf_s(filename, sizeof(filename), "Dragons\\Dragon(%d)back%d.png", d + 1, i + 1);
      dragonBackFlyImgs[d][i] = iLoadImage((char *)filename);
    }
    for (int i = 0; i < 2; i++) {
      sprintf_s(filename, sizeof(filename), "Dragons\\Dragon(%d)attack%d.png", d + 1, i + 1);
      dragonAttackImgs[d][i] = iLoadImage((char *)filename);
      sprintf_s(filename, sizeof(filename), "Dragons\\Dragon(%d)backattack%d.png", d + 1, i + 1);
      dragonBackAttackImgs[d][i] = iLoadImage((char *)filename);
    }
  }
}

inline void initDragons() {
  for (int i = 0; i < 5; i++) {
    dragons[i].active = false;
    dragons[i].isVanishing = false;
    dragons[i].vanishingTimer = 0;
    dragons[i].holdATimer = 0;
    dragons[i].cooldownTimer = 0;
    dragons[i].flyFrameIndex = 0;
    dragons[i].attackFrameIndex = 0;
  }
}

inline void spawnDragon() {
  for (int i = 0; i < 5; i++) {
    if (!dragons[i].active) {
      dragons[i].active = true;
      dragons[i].type = (rand() % 3) + 1; // 1, 2, or 3

      if (dragons[i].type == 1) {
        dragons[i].x = SCREEN_W + 200; // Come from Right
        dragons[i].facingRight = false;
      } else {
        dragons[i].x = -350; // Come from Left (2 and 3)
        dragons[i].facingRight = true;
      }

      dragons[i].y = 450 + rand() % 100; // Come from higher up
      dragons[i].isAttacking = false;
      dragons[i].animCounter = 0;
      dragons[i].speed = 2.0f + (rand() % 4); // Chase speed
      dragons[i].isVanishing = false;
      dragons[i].vanishingTimer = 0;
      dragons[i].holdATimer = 0;
      dragons[i].cooldownTimer = 0;
      dragons[i].flyFrameIndex = 0;
      dragons[i].attackFrameIndex = 0;
      break;
    }
  }
}

inline void updateDragonPhysics() {
  if (isGamePaused || gameState != GAME || (currentLevel != 2))
    return;

  static int spawnTicks = 0;
  bool anyActive = false;
  for (int i = 0; i < 5; i++) {
    if (dragons[i].active)
      anyActive = true;
  }

  if (!anyActive && gameRunTimeSeconds < (currentLevel == 3 ? 56 : 41)) {
    spawnTicks++;
    if (spawnTicks >= 333) { // 10 seconds
      spawnDragon();
      spawnTicks = 0;
    }
  } else {
    spawnTicks = 0;
  }

  for (int i = 0; i < 5; i++) {
    if (dragons[i].active) {
      if (dragons[i].isVanishing) {
        dragons[i].vanishingTimer--;
        if (dragons[i].vanishingTimer <= 0) {
          dragons[i].active = false;
          dragons[i].isVanishing = false;
        }
        continue;
      }

      if (dragons[i].cooldownTimer > 0) dragons[i].cooldownTimer--;

      float targetX = (float)(charX + (float)charWidth / 2.0f);
      float targetY = (float)(charY + (float)charHeight / 2.0f);
      float dxCenter = targetX - (dragons[i].x + 100.0f);
      float dyCenter = targetY - (dragons[i].y + 100.0f);
      float trueDist = (float)sqrt((double)(dxCenter * dxCenter + dyCenter * dyCenter));
      
      dragons[i].facingRight = (dxCenter > 0);

      if (!dragons[i].isAttacking) {
          if (dragons[i].cooldownTimer > 0) {
              // Hover above player
              float targetAltitude = charY + 250.0f;
              float dyUp = targetAltitude - (dragons[i].y + 100.0f);
              if (trueDist > 0.0f) {
                  dragons[i].x += (dxCenter / trueDist) * dragons[i].speed;
                  float moveY = dragons[i].speed * 0.8f;
                  if (fabs(dyUp) <= moveY) dragons[i].y += dyUp;
                  else dragons[i].y += (dyUp > 0 ? 1 : -1) * moveY;
              }
          } else {
              // Swoop down directly towards character
              if (trueDist > 0.0f) {
                  dragons[i].x += (dxCenter / trueDist) * dragons[i].speed;
                  dragons[i].y += (dyCenter / trueDist) * dragons[i].speed;
              }
              if (trueDist < 120.0f) {
                  dragons[i].isAttacking = true;
                  dragons[i].attackFrameIndex = 0;
              }
          }
      } else {
          // Track slowly while attacking
          float atkSpeed = 1.5f;
          if (trueDist > 0.0f) {
              float actSpeed = trueDist < atkSpeed ? trueDist : atkSpeed;
              dragons[i].x += (dxCenter / trueDist) * actSpeed;
              dragons[i].y += (dyCenter / trueDist) * actSpeed;
          }
          
          if (keyPressed[' ']) {
              dragons[i].isVanishing = true;
              dragons[i].vanishingTimer = 60; 
              dragons[i].holdATimer = 0;
          }
          
          if (trueDist > 250.0f) {
              dragons[i].isAttacking = false;
              dragons[i].cooldownTimer = 120;
          }
      }

      dragons[i].animCounter++;
      if (dragons[i].animCounter >= 4) { 
        dragons[i].animCounter = 0;
        if (dragons[i].isAttacking) {
          dragons[i].attackFrameIndex++;
          if (dragons[i].attackFrameIndex >= 2) {
              dragons[i].isAttacking = false;
              dragons[i].cooldownTimer = 90;
              dragons[i].attackFrameIndex = 0;
          }
        } else {
          dragons[i].flyFrameIndex = (dragons[i].flyFrameIndex + 1) % 3;
        }
      }
    }
  }
}

inline void drawDragons() {
  if (gameState != GAME || (currentLevel != 2))
    return;
  for (int i = 0; i < 5; i++) {
    if (dragons[i].active) {
      if (dragons[i].isVanishing && (dragons[i].vanishingTimer / 5) % 2 == 0) {
        continue;
      }

      int img = -1;
      int dIndex = dragons[i].type - 1; // 0, 1, or 2
      if (dragons[i].isAttacking) {
          if (dragons[i].facingRight) {
              img = dragonAttackImgs[dIndex][dragons[i].attackFrameIndex];
          } else {
              img = dragonBackAttackImgs[dIndex][dragons[i].attackFrameIndex];
          }
      } else {
          if (dragons[i].facingRight) {
              img = dragonFlyImgs[dIndex][dragons[i].flyFrameIndex];
          } else {
              img = dragonBackFlyImgs[dIndex][dragons[i].flyFrameIndex];
          }
      }

      if (img != -1) {
        iShowImage((int)dragons[i].x, (int)dragons[i].y, 200, 200, img);
      }
    }
  }
}

#endif

