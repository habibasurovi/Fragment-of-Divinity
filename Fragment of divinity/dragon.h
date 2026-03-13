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
extern unsigned int keyPressed[512];

struct Dragon {
  float x, y;
  int type; // 1, 2, or 3
  int frameIndex;
  bool active;
  bool isAttacking;
  int animCounter;
  float speed;

  // Vanishing mechanics
  bool isVanishing;
  int vanishingTimer;
  int holdATimer;
};

extern Dragon dragons[5];
extern int dragon1Imgs[9];
extern int dragon2Imgs[10];
extern int dragon3Imgs[6];

inline void loadDragonAssets() {
  char filename[100];
  for (int i = 0; i < 9; i++) {
    sprintf_s(filename, "Dragons\\Dragon(1)%d.png", i + 1);
    dragon1Imgs[i] = iLoadImage(filename);
  }
  for (int i = 0; i < 10; i++) {
    sprintf_s(filename, "Dragons\\Dragon(2)%d.png", i + 1);
    dragon2Imgs[i] = iLoadImage(filename);
  }
  for (int i = 0; i < 6; i++) {
    sprintf_s(filename, "Dragons\\Dragon(3)%d.png", i + 1);
    dragon3Imgs[i] = iLoadImage(filename);
  }
}

inline void initDragons() {
  for (int i = 0; i < 5; i++) {
    dragons[i].active = false;
    dragons[i].isVanishing = false;
    dragons[i].vanishingTimer = 0;
    dragons[i].holdATimer = 0;
  }
}

inline void spawnDragon() {
  for (int i = 0; i < 5; i++) {
    if (!dragons[i].active) {
      dragons[i].active = true;
      dragons[i].type = (rand() % 3) + 1;

      if (dragons[i].type == 1) {
        dragons[i].x = SCREEN_W + 200; // Come from Right
      } else {
        dragons[i].x = -350; // Come from Left (2 and 3)
      }

      dragons[i].y = 450 + rand() % 100; // Come from higher up
      dragons[i].frameIndex = 0;
      dragons[i].isAttacking = false;
      dragons[i].animCounter = 0;
      dragons[i].speed = 2.0f + (rand() % 4); // Chase speed
      dragons[i].isVanishing = false;
      dragons[i].vanishingTimer = 0;
      dragons[i].holdATimer = 0;
      break;
    }
  }
}

inline void updateDragonPhysics() {
  if (isGamePaused || gameState != GAME || (currentLevel != 2))
    return;

  // Only one dragon at a time, coming after 10 seconds (333 ticks at 30ms)
  static int spawnTicks = 0;
  bool anyActive = false;
  for (int i = 0; i < 5; i++) {
    if (dragons[i].active)
      anyActive = true;
  }

  if (!anyActive) {
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

      // Chase character center
      float targetX = (float)(charX + (float)charWidth / 2.0f);
      float targetY = (float)(charY + (float)charHeight / 2.0f);

      // Dragon center is x+150, y+150 (for 300x300 size)
      float dx = targetX - (dragons[i].x + 150.0f);
      float dy = targetY - (dragons[i].y + 150.0f);
      float dist = (float)sqrt((double)(dx * dx + dy * dy));

      // Chasing Logic: Move toward character
      if (dist > 10.0f) {
        // Vertical chase (descend to match character)
        float absDy = (dy > 0) ? dy : -dy;
        if (absDy > 5.0f) {
          dragons[i].y += (dy / dist) * dragons[i].speed;
        }

        // Horizontal chase
        float absDx = (dx > 0) ? dx : -dx;
        if (absDx > 120.0f) { // Maintain a slight distance before attacking
          dragons[i].x += (dx / dist) * dragons[i].speed;
        }
      }

      // Attack proximity check
      if (dist < 200.0f) {
        dragons[i].isAttacking = true;

        // Space button causes the dragon to vanish
        // Space button causes the dragon to vanish
        if (keyPressed[' ']) {
          dragons[i].isVanishing = true;
          dragons[i].vanishingTimer = 60; // 2 seconds of blinking
          dragons[i].holdATimer = 0;
        }
      } else {
        dragons[i].isAttacking = false;
        dragons[i].holdATimer = 0;
      }

      // Animation
      dragons[i].animCounter++;
      if (dragons[i].animCounter >= 4) { // Animation speed
        dragons[i].animCounter = 0;
        if (dragons[i].isAttacking) {
          if (dragons[i].type == 1) {
            dragons[i].frameIndex++;
            if (dragons[i].frameIndex < 3 || dragons[i].frameIndex >= 9)
              dragons[i].frameIndex = 3;
          } else if (dragons[i].type == 3) {
            dragons[i].frameIndex++;
            if (dragons[i].frameIndex < 3 || dragons[i].frameIndex >= 6)
              dragons[i].frameIndex = 3;
          } else {
            // Dragon 2 or non-attacking
            dragons[i].frameIndex = (dragons[i].frameIndex + 1) % 3;
          }
        } else {
          // Constant flying frames (1-3)
          dragons[i].frameIndex = (dragons[i].frameIndex + 1) % 3;
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
      // Blinking effect when vanishing
      if (dragons[i].isVanishing && (dragons[i].vanishingTimer / 5) % 2 == 0) {
        continue;
      }

      int img = -1;
      if (dragons[i].type == 1)
        img = dragon1Imgs[dragons[i].frameIndex];
      else if (dragons[i].type == 2)
        img = dragon2Imgs[dragons[i].frameIndex];
      else if (dragons[i].type == 3)
        img = dragon3Imgs[dragons[i].frameIndex];

      if (img != -1) {
        // Increased size (previous 250*250, now 300*300)
        iShowImage((int)dragons[i].x, (int)dragons[i].y, 300, 300, img);
      }
    }
  }
}

#endif
