#ifndef OBSTACLEHANDLER_H
#define OBSTACLEHANDLER_H

#include "iGraphics.h"
#include <stdio.h>
#include <stdlib.h>

// Variables
extern int obs1, obs2, obs3;
extern int obstacleSpawnTimer;
extern bool isCaveActive;
extern int bombImg;
extern int explosionImgs[7];

// Screen dimensions needed for logic
#define SCREEN_W 1000
#define SCREEN_H 600
#ifndef SCROLL_SPD
#define SCROLL_SPD 3
#endif

// Shark assets
extern int shark11, shark12, shark13;
extern int shark21, shark22, shark23;
extern int obsBrokenBridge; // obs21.png
extern int sharksSpawnedInLevel;

struct Obstacle {
  int x, y;
  int type; // 1, 2, 3
  bool active;
};

struct Shark {
  float x, y;
  int type;      // 1: shark1 (1 life), 2: shark2 (2 lives)
  int jumpState; // 0: swimming, 1: jumping up, 2: falling
  float velocityY;
  bool active;
  bool isDying;
  float dyingVel;
  bool isRetreating;
};

struct Bomb {
  float x, y;
  bool active;
};

struct Explosion {
  float x, y;
  int frame;
  bool active;
  int timer;
};

extern Bomb bombs[3];
extern Explosion explosions[3];
extern bool isCharFrozen;
extern int freezeTimer;
extern int shakeOffsetX;
extern int shakeOffsetY;
extern bool isFallingSequence;
extern float fallY;
extern float fallVel;
extern int hitBridgeIndex;
extern void startScreenShake(int duration, int magnitude);

// Seasonal Obstacles
extern int obs221, obs222, obs223; // Summer, Rainy, Winter
extern int imgBroken;              // Fallen effect
extern int imgRLL, imgGLL;         // Left lever
extern int imgRBL, imgGBL;         // Right lever

extern int selectedWeather;
extern int charX, charY, charWidth, charHeight;
extern int lives;
extern bool isInvincible;
extern int invincibilityTimer;
extern int groundY;

struct BrokenBridge {
  float x, y;
  bool active;
  bool isBroken;       // Has it started falling?
  bool leverActivated; // Was it neutralized by a lever?
  bool isRLL;          // Type of lever: true for rll, false for rbl
  float fallenY;       // Current Y for the falling broken.png
  float fallenVel;     // Velocity for falling effect
  bool damaged;        // Has it already dealt damage?
};

extern Obstacle obstacles[3];
extern Shark sharks[2];
extern BrokenBridge bridges[2];

extern int sharksSpawnedInLevel; // Counter for total sharks in Level 2

inline void loadObstacleAssets() {
  obs1 = iLoadImage("obs img\\obs1.bmp");
  obs2 = iLoadImage("obs img\\obs2.bmp");
  obs3 = iLoadImage("obs img\\obs3.bmp");

  // Level 2 Shark Assets
  shark11 = iLoadImage("obs img\\shark11.png");
  shark12 = iLoadImage("obs img\\shark12.png");
  shark13 = iLoadImage("obs img\\shark13.png");

  shark21 = iLoadImage("obs img\\shark21.png");
  shark22 = iLoadImage("obs img\\shark22.png");
  shark23 = iLoadImage("obs img\\shark23.png");

  obsBrokenBridge = iLoadImage("obs img\\obs21.png");

  // Broken Bridge Assets
  obs221 = iLoadImage("obs img\\obs221.bmp");
  obs222 = iLoadImage("obs img\\obs222.bmp");
  obs223 = iLoadImage("obs img\\obs223.bmp");
  imgBroken = iLoadImage("obs img\\broken.png");
  imgRLL = iLoadImage("obs img\\rll.png");
  imgGLL = iLoadImage("obs img\\gll.png");
  imgRBL = iLoadImage("obs img\\rbl.png");
  imgGBL = iLoadImage("obs img\\gbl.png");

  // Bomb & Explosion Assets
  bombImg = iLoadImage("obs img\\bomb.png");
  char expPath[100];
  for (int i = 0; i < 7; i++) {
    sprintf_s(expPath, "obs img\\explosion\\explosion%d.png", i + 1);
    explosionImgs[i] = iLoadImage(expPath);
  }
}

inline void initObstacles() {
  for (int i = 0; i < 3; i++) {
    obstacles[i].active = false;
  }
  for (int i = 0; i < 2; i++) {
    sharks[i].active = false;
    sharks[i].jumpState = 0;
    sharks[i].isDying = false;
    sharks[i].dyingVel = 0;
    bridges[i].active = false;
    bridges[i].isBroken = false;
    bridges[i].leverActivated = false;
    bridges[i].fallenY = 0;
    bridges[i].fallenVel = 0;
    bridges[i].damaged = false;
    bridges[i].isRLL = (i == 0);
  }
  for (int i = 0; i < 3; i++) {
    bombs[i].active = false;
    explosions[i].active = false;
  }
  isCharFrozen = false;
  freezeTimer = 0;
  sharksSpawnedInLevel = 0;
}

// Obstacle Settings
extern int obsGapMin;
extern int obsGapMax;
extern int obsHighY;
extern int obsLowY;
extern int charX; // From iMain
extern int currentLevel;

inline void updateObstaclePhysics() {
  if (currentLevel == 2) {
    // ---- SHARK LOGIC for Level 2 ----
    for (int i = 0; i < 2; i++) {
      if (sharks[i].active) {
        if (sharks[i].isDying) {
          sharks[i].y += sharks[i].dyingVel;
          sharks[i].dyingVel -= 0.5f;
          if (sharks[i].y < -200)
            sharks[i].active = false;
          continue;
        }

        if (sharks[i].isRetreating) {
          sharks[i].x += (SCROLL_SPD * 4); // Swim back fast
          if (sharks[i].x > SCREEN_W + 300) {
            sharks[i].active = false;
            sharks[i].isRetreating = false;
          }
          continue;
        } else {
          sharks[i].x -= SCROLL_SPD + 1;
        }

        if (sharks[i].jumpState == 0) {
          sharks[i].y = (float)-10.0f;
          if (sharks[i].x - charX < 200 && sharks[i].x > charX) {
            sharks[i].jumpState = 1;
            sharks[i].velocityY = (sharks[i].type == 1) ? 10.0f : 13.0f;
          }
        } else {
          sharks[i].y += sharks[i].velocityY;
          sharks[i].velocityY -= 0.6f;
          if (sharks[i].velocityY > 0)
            sharks[i].jumpState = 1;
          else
            sharks[i].jumpState = 2;
          if (sharks[i].y < -10 && sharks[i].velocityY < 0) {
            sharks[i].y = -10;
            sharks[i].jumpState = 0;
            sharks[i].velocityY = 0;
          }
        }
        if (sharks[i].x < -500) {
          sharks[i].active = false;
        }
      }
    }

    // ---- BROKEN BRIDGE LOGIC ----
    for (int i = 0; i < 2; i++) {
      if (!bridges[i].active)
        continue;
      bridges[i].x -= SCROLL_SPD;
      float leverX = bridges[i].x - 220;
      float leverY = bridges[i].y - 15;
      int leverW = 60;
      int leverH = bridges[i].isRLL ? 80 : 450;
      if (!bridges[i].leverActivated) {
        if (charX + charWidth - 85 > leverX && charX + 85 < leverX + leverW) {
          float lampTopStart = leverY + (leverH * 0.8);
          float lampTopEnd = leverY + leverH;
          if (charY + charHeight > lampTopStart && charY < lampTopEnd) {
            bridges[i].leverActivated = true;
          }
        }
      }
      if (!bridges[i].leverActivated && !bridges[i].isBroken) {
        if (charX + charWidth - 85 > bridges[i].x &&
            charX + 85 < bridges[i].x + 430 && charY <= groundY + 10) {
          bridges[i].isBroken = true;
          bridges[i].fallenY = bridges[i].y - 40;
          bridges[i].fallenVel = -1;

          // Start Character Fall Sequence
          if (!isFallingSequence && !isInvincible) {
            isFallingSequence = true;
            isCharFrozen = true;
            fallY = (float)charY;
            fallVel = -2; // Initial fall velocity
            hitBridgeIndex = i;
            lives--;
            isInvincible = true;
            invincibilityTimer = 120; // Longer invincibility for the sequence
            bridges[i].damaged = true;
            startScreenShake(15, 8); // Medium shake for falling
          }
        }
      }
      if (bridges[i].isBroken && !bridges[i].leverActivated) {
        bridges[i].fallenY += bridges[i].fallenVel;
        bridges[i].fallenVel -= 0.5;
      }
      if (bridges[i].x < -600)
        bridges[i].active = false;
    }

    // ---- BOMB PHYSICS ----
    for (int i = 0; i < 3; i++) {
      if (bombs[i].active) {
        bombs[i].y -= 6;
        if (bombs[i].y < -50) {
          bombs[i].active = false;
        }
      }
    }

    // ---- EXPLOSION ANIMATION ----
    for (int i = 0; i < 3; i++) {
      if (explosions[i].active) {
        explosions[i].timer++;
        // Total 3 seconds = 100 ticks (approx) @ 30ms.
        // 100 / 7 frames = 14 ticks per frame
        if (explosions[i].timer >= 14) {
          explosions[i].timer = 0;
          explosions[i].frame++;
          if (explosions[i].frame >= 7) {
            explosions[i].active = false;
          }
        }
      }
    }

    // ---- BRIDGE COLLISION DAMAGE ----
    if (!isInvincible && !isFallingSequence) {
      for (int i = 0; i < 2; i++) {
        if (!bridges[i].active || bridges[i].damaged ||
            bridges[i].leverActivated)
          continue;
        if (charX + charWidth - 60 > bridges[i].x + 20 &&
            charX + 60 < bridges[i].x + 430 && charY <= groundY + 5) {
          lives--;
          isInvincible = true;
          invincibilityTimer = 120;
          bridges[i].damaged = true;
          isFallingSequence = true;
          isCharFrozen = true;
          fallY = (float)charY;
          fallVel = -2;
          hitBridgeIndex = i;
          startScreenShake(15, 8); // Medium shake for falling
        }
      }
    }

    // Spawning Logic
    static int level2SpawnDist = 0;
    level2SpawnDist += SCROLL_SPD; // Use global scroll speed

    if (level2SpawnDist > 800 &&
        !isCaveActive) { // Reduced distance for testing
      int choice = rand() % 2;
      if (choice == 0) {
        // Spawn Shark
        for (int i = 0; i < 2; i++) {
          if (!sharks[i].active) {
            sharks[i].active = true;
            sharks[i].x = (float)SCREEN_W + 50;
            sharks[i].y = -10;

            // Increment shark counter
            sharksSpawnedInLevel++;

            // Task: Shark #3 must be Shark 1 (small)
            if (sharksSpawnedInLevel == 3) {
              sharks[i].type = 1;
            } else {
              sharks[i].type = (rand() % 2) + 1;
            }

            sharks[i].jumpState = 0;
            sharks[i].velocityY = 0;
            sharks[i].isDying = false;
            level2SpawnDist = 0;
            break;
          }
        }
      } else {
        // Spawn Bridge
        for (int i = 0; i < 2; i++) {
          if (!bridges[i].active) {
            bridges[i].active = true;
            bridges[i].x = (float)SCREEN_W + 350;
            bridges[i].y = 90;
            bridges[i].isBroken = false;
            bridges[i].leverActivated = false;
            bridges[i].damaged = false;
            bridges[i].fallenY = 0;
            bridges[i].fallenVel = 0;
            bridges[i].isRLL = (rand() % 2 == 0);
            level2SpawnDist = 0;
            break;
          }
        }
      }
    }

    // ---- BOMB SPAWNING ----
    static int bombSpawnTimer = 0;
    bombSpawnTimer++;
    if (bombSpawnTimer > 150) {
      for (int i = 0; i < 3; i++) {
        if (!bombs[i].active) {
          bombs[i].active = true;
          bombs[i].x = (float)(charX + (rand() % 400));
          if (bombs[i].x > SCREEN_W - 50)
            bombs[i].x = (float)(SCREEN_W - 50);
          bombs[i].y = SCREEN_H + 50;
          bombSpawnTimer = 0;
          break;
        }
      }
    }
    return;
  }

  // ---- STANDARD OBSTACLE LOGIC (Level 1) ----
  int activeCount = 0;
  for (int i = 0; i < 3; i++) {
    if (obstacles[i].active) {
      obstacles[i].x -= SCROLL_SPD;
      activeCount++;
      if (obstacles[i].x < -150) {
        obstacles[i].active = false;
      }
    }
  }
  if (isCaveActive)
    return;
  static int distSinceLast = 0;
  static int nextSpawnGap = 300;
  distSinceLast += SCROLL_SPD;
  if (activeCount < 3 && !isCaveActive) {
    if (distSinceLast > nextSpawnGap) {
      for (int i = 0; i < 3; i++) {
        if (!obstacles[i].active) {
          obstacles[i].active = true;
          obstacles[i].x = SCREEN_W;
          obstacles[i].type = (rand() % 3) + 1;
          obstacles[i].y = (obstacles[i].type == 1) ? obsHighY : obsLowY;
          distSinceLast = 0;
          nextSpawnGap = obsGapMin + rand() % (obsGapMax - obsGapMin + 1);
          break;
        }
      }
    }
  }
}

inline void drawObstacles() {
  if (currentLevel == 2) {
    for (int i = 0; i < 2; i++) {
      if (sharks[i].active) {
        int img = 0;
        if (sharks[i].type == 1) {
          if (sharks[i].jumpState == 0)
            img = shark11;
          else if (sharks[i].jumpState == 1)
            img = shark12;
          else
            img = shark13;
          iShowImage((int)sharks[i].x + shakeOffsetX, (int)sharks[i].y + shakeOffsetY, 70, 70, img);
        } else {
          if (sharks[i].jumpState == 0)
            img = shark21;
          else if (sharks[i].jumpState == 1)
            img = shark22;
          else
            img = shark23;
          iShowImage((int)sharks[i].x + shakeOffsetX, (int)sharks[i].y + shakeOffsetY, 90, 90, img);
        }
      }
    }

    for (int i = 0; i < 2; i++) {
      if (!bridges[i].active)
        continue;
      int lImg = bridges[i].isRLL
                     ? (bridges[i].leverActivated ? imgGLL : imgRLL)
                     : (bridges[i].leverActivated ? imgGBL : imgRBL);
      float leverX = bridges[i].x - 220;
      float leverY = bridges[i].y - 15;
      int leverW = 60;
      int leverH = bridges[i].isRLL ? 80 : 450;
      iShowImage((int)leverX + shakeOffsetX, (int)leverY + shakeOffsetY, leverW, leverH, lImg);
      if (!bridges[i].leverActivated) {
        int seasonImg = obs221;
        if (selectedWeather == 1)
          seasonImg = obs222;
        else if (selectedWeather == 2)
          seasonImg = obs223;

        // Always show seasonImg (water/hole) when lever is red/inactive
        iShowImage((int)bridges[i].x + shakeOffsetX, (int)bridges[i].y - 15 + shakeOffsetY, 450, 60,
                   seasonImg);

        if (!bridges[i].isBroken) {
          iShowImage((int)bridges[i].x - 50 + shakeOffsetX, (int)bridges[i].y - 40 + shakeOffsetY, 550, 60,
                     imgBroken);
        } else {
          iShowImage((int)bridges[i].x - 50 + shakeOffsetX, (int)bridges[i].fallenY + shakeOffsetY, 550, 60,
                     imgBroken);
        }
      }
    }
    for (int i = 0; i < 3; i++) {
      if (bombs[i].active) {
        iShowImage((int)bombs[i].x + shakeOffsetX, (int)bombs[i].y + shakeOffsetY, 70, 70, bombImg);
      }
    }
    return;
  }

  // Level 1 logic
  for (int i = 0; i < 3; i++) {
    if (obstacles[i].active) {
      if (obstacles[i].type == 1)
        iShowImage(obstacles[i].x + shakeOffsetX, obstacles[i].y + shakeOffsetY, 120, 120, obs1);
      else if (obstacles[i].type == 2)
        iShowImage(obstacles[i].x + shakeOffsetX, obstacles[i].y + shakeOffsetY, 120, 80, obs2);
      else
        iShowImage(obstacles[i].x + shakeOffsetX, obstacles[i].y + shakeOffsetY, 120, 120, obs3);
    }
  }
}

inline void drawExplosions() {
  if (currentLevel == 2) {
    for (int i = 0; i < 3; i++) {
      if (explosions[i].active) {
        // Center the 200x200 explosion on the trigger point (character center)
        iShowImage((int)explosions[i].x - 100 + shakeOffsetX, (int)explosions[i].y - 100 + shakeOffsetY, 200, 200,
                   explosionImgs[explosions[i].frame]);
      }
    }
  }
}

#endif
