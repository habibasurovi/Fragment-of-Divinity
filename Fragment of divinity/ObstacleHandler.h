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
extern int explosionImgs[5];

// Screen dimensions needed for logic
#ifndef SCREEN_W
#define SCREEN_W 1000
#endif
#ifndef SCREEN_H
#define SCREEN_H 600
#endif
#ifndef SCROLL_SPD
#define SCROLL_SPD 3
#endif

extern int sharksSpawnedInLevel;
extern int currentLevel;

// Level 2 Assets (Restored)
extern int shark11, shark12, shark13;
extern int shark21, shark22, shark23;
extern int obsBrokenBridge;

// Level 3 Assets
extern int worm11, worm12, worm13, worm14;
extern int worm21, worm22, worm23, worm24;
extern int l3HoleImg;
extern int handImgs[4];
extern int shortPillerImg, longPillerImg;
extern int flameImgs[4];
extern int fireImgs[4];

extern bool isL3HoleRemoved;
extern bool flamesHitGround[2];
extern bool isFlameFalling[2];
extern float flameYPos[2];
extern float flameVel[2];
extern int flameFrame;
extern int fireFrame;
extern int handFrame;
extern int wormFrame;
extern float handOffset;
extern float handDir;

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
extern bool isFallingSequence;
extern float fallY;
extern float fallVel;
extern int hitBridgeIndex;

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
extern int level2SpawnDist;
extern int bombSpawnTimer;
extern int distSinceLast;
extern int nextSpawnGap;

// Counter for total sharks in Level 2

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
  for (int i = 0; i < 5; i++) {
    sprintf_s(expPath, "obs img\\explosion\\ezgif-frame-00%d.png", i + 1);
    explosionImgs[i] = iLoadImage(expPath);
  }

  // Level 3 Assets
  worm11 = iLoadImage("level3\\worm11.png");
  worm12 = iLoadImage("level3\\worm12.png");
  worm13 = iLoadImage("level3\\worm13.png");
  worm14 = iLoadImage(
      "level3\\worm13.png"); // Using 13 if 14 missing as per folder list

  worm21 = iLoadImage("level3\\worm21.png");
  worm22 = iLoadImage("level3\\worm22.png");
  worm23 = iLoadImage("level3\\worm23.png");
  worm24 = iLoadImage("level3\\worm23.png"); // Using 23 if 24 missing

  l3HoleImg = iLoadImage("level3\\L3hole.png");
  shortPillerImg = iLoadImage("level3\\shortpiller.png");
  longPillerImg = iLoadImage("level3\\longpiller.png");

  for (int i = 0; i < 4; i++) {
    char path[100];
    sprintf_s(path, "level3\\hand%d.png", i + 1);
    handImgs[i] = iLoadImage(path);
    sprintf_s(path, "level3\\flame%d.png", i + 1);
    flameImgs[i] = iLoadImage(path);
    sprintf_s(path, "level3\\fire%d.png", i + 1);
    fireImgs[i] = iLoadImage(path);
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

  // Level 3 Init
  isL3HoleRemoved = false;
  flameFrame = 0;
  fireFrame = 0;
  handFrame = 0;
  wormFrame = 0;
  handOffset = 0;
  handDir = 1;
  for (int i = 0; i < 2; i++) {
    flamesHitGround[i] = false;
    isFlameFalling[i] = false;
    flameYPos[i] = 0;
    flameVel[i] = 0;
  }
}

// Obstacle Settings
extern int obsGapMin;
extern int obsGapMax;
extern int obsHighY;
extern int obsLowY;
// From iMain

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

        float moveSpeed = (float)(SCROLL_SPD + 1);
        if (sharks[i].isRetreating) {
          sharks[i].x += moveSpeed; // Move back along the x-axis
          if (sharks[i].x > SCREEN_W + 300) {
            sharks[i].active = false;
            sharks[i].isRetreating = false;
          }
        } else {
          sharks[i].x -= moveSpeed;
        }

        if (sharks[i].jumpState == 0) {
          sharks[i].y = (float)-10.0f;
          if (!sharks[i].isRetreating && sharks[i].x - charX < 200 &&
              sharks[i].x > charX) {
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
          }
        }
      }
      if (bridges[i].isBroken && !bridges[i].leverActivated) {
        bridges[i].fallenY += bridges[i].fallenVel;
        bridges[i].fallenVel -= 0.5f;
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
        if (explosions[i].timer >=
            18) { // 3 seconds total (5 frames * 18 = 90 frames)
          explosions[i].timer = 0;
          explosions[i].frame++;
          if (explosions[i].frame >= 5) {
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
        }
      }
    }

    // Spawning Logic
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

  if (currentLevel == 3) {
    // ---- WORM LOGIC (Reusing Shark logic) ----
    for (int i = 0; i < 2; i++) {
      if (sharks[i].active) {
        if (sharks[i].isDying) {
          sharks[i].y += sharks[i].dyingVel;
          sharks[i].dyingVel -= 0.5f;
          if (sharks[i].y < -200)
            sharks[i].active = false;
          continue;
        }

        float moveSpeed = (float)(SCROLL_SPD + 1);
        if (sharks[i].isRetreating) {
          sharks[i].x += moveSpeed; // Move back (return)
          if (sharks[i].x > SCREEN_W + 300) {
            sharks[i].active = false;
            sharks[i].isRetreating = false;
          }
        } else {
          sharks[i].x -= moveSpeed;
        }

        if (sharks[i].jumpState == 0) {
          sharks[i].y = (float)-10.0f;
          if (!sharks[i].isRetreating && sharks[i].x - charX < 200 &&
              sharks[i].x > charX) {
            sharks[i].jumpState = 1;
            sharks[i].velocityY = 10.0f; // All worms match shark1 logic
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

    // ---- HOLE & HAND LOGIC ----
    handFrame = (handFrame + 1) % 40;
    handOffset += 0.5f * handDir;
    if (handOffset > 10 || handOffset < -10)
      handDir *= -1;

    for (int i = 0; i < 2; i++) {
      if (!bridges[i].active)
        continue;
      bridges[i].x -= SCROLL_SPD;

      // Decrease distance between pillar and hole (adjust from -220 to -80)
      float pillerX = bridges[i].x - 80; // Pillar is 80px from hole edge
      float pillerY = bridges[i].y - 30;
      int pillerW = 80;
      int pillerH = bridges[i].isRLL ? 120 : 350;

      // Flame Logic
      if (!isFlameFalling[i] && !flamesHitGround[i]) {
        // Check for collision with flame to trigger fall
        if (charX + charWidth - 30 > pillerX &&
            charX + 30 < pillerX + pillerW) {
          float flameTopStart = pillerY + pillerH - 50;
          float flameTopEnd = pillerY + pillerH + 50;
          if (charY + charHeight > flameTopStart && charY < flameTopEnd) {
            isFlameFalling[i] = true;
            // Place exactly at pillar top
            flameYPos[i] = (float)pillerY + pillerH;
            flameVel[i] = -2;
          }
        }
      }

      if (isFlameFalling[i]) {
        // Adjust falling trajectory to fall towards hole (bridges[i].x + 100
        // approx center of hole) flameX will be based on where it fell from.
        flameYPos[i] += flameVel[i];
        flameVel[i] -= 0.5f;
        // As it falls, we move it right so it falls *into* the hole area

        if (flameYPos[i] <= groundY) {
          flameYPos[i] = (float)groundY;
          isFlameFalling[i] = false;
          flamesHitGround[i] = true;
          isL3HoleRemoved = true;
        }
      }

      // Hole & Hand Collision
      if (!bridges[i].isBroken) {
        // Character must be further into the hole to fall
        if (charX + charWidth - 110 > bridges[i].x + 80 &&
            charX + 80 < bridges[i].x + 400 && charY <= groundY + 50) {

          bridges[i].isBroken = true;
          if (!isFallingSequence && !isInvincible) {
            isFallingSequence = true;
            isCharFrozen = true;
            fallY = (float)charY;
            fallVel = -1; // Gentle pull effect
            hitBridgeIndex = i;

            // Damage logic: double if hand is still there, single if flame hit ground
            if (!flamesHitGround[i]) {
              lives -= 2; // Double damage from hand
            } else {
              lives--; // Single damage from hole
            }

            isInvincible = true;
            invincibilityTimer = 120;
          }
        }
      }

      if (bridges[i].x < -600)
        bridges[i].active = false;
    }

    flameFrame = (flameFrame + 1) % 40;
    fireFrame = (fireFrame + 1) % 40;
    wormFrame = (wormFrame + 1) % 40;

    // ---- EXPLOSION ANIMATION ----
    for (int i = 0; i < 3; i++) {
      if (explosions[i].active) {
        explosions[i].timer++;
        if (explosions[i].timer >= 18) {
          explosions[i].timer = 0;
          explosions[i].frame++;
          if (explosions[i].frame >= 5)
            explosions[i].active = false;
        }
      }
    }

    // Spawning Level 3
    level2SpawnDist += SCROLL_SPD;
    if (level2SpawnDist > 1000) {
      int choice = rand() % 2;
      if (choice == 0) {
        // Worm
        for (int i = 0; i < 2; i++) {
          if (!sharks[i].active) {
            sharks[i].active = true;
            sharks[i].x = (float)SCREEN_W + 50;
            sharks[i].y = -10;
            sharks[i].type = (rand() % 2) + 1;
            sharks[i].jumpState = 0;
            // Recoded worm2 to work like level 2 shark1 (10.0f velocity)
            sharks[i].velocityY = 0;
            sharks[i].isDying = false;
            sharks[i].isRetreating = false;
            level2SpawnDist = 0;
            break;
          }
        }
      } else {
        // Hole
        for (int i = 0; i < 2; i++) {
          if (!bridges[i].active) {
            bridges[i].active = true;
            bridges[i].x = (float)SCREEN_W + 400;
            bridges[i].y = 90;
            bridges[i].isBroken = false;
            bridges[i].isRLL = (rand() % 2 == 0);
            isFlameFalling[i] = false;
            flamesHitGround[i] = false;
            isL3HoleRemoved = false;
            level2SpawnDist = 0;
            break;
          }
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
  static int distSinceLast_L1 = 0;
  static int nextSpawnGap_L1 = 300;
  distSinceLast_L1 += SCROLL_SPD;
  if (activeCount < 3 && !isCaveActive) {
    if (distSinceLast_L1 > nextSpawnGap_L1) {
      for (int i = 0; i < 3; i++) {
        if (!obstacles[i].active) {
          obstacles[i].active = true;
          obstacles[i].x = SCREEN_W;
          obstacles[i].type = (rand() % 3) + 1;
          obstacles[i].y = (obstacles[i].type == 1) ? obsHighY : obsLowY;
          distSinceLast_L1 = 0;
          nextSpawnGap_L1 = obsGapMin + rand() % (obsGapMax - obsGapMin + 1);
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
        int sW, sH;
        if (sharks[i].type == 1) {
          sW = 70;
          sH = 70;
          if (sharks[i].jumpState == 0)
            img = shark11;
          else if (sharks[i].jumpState == 1)
            img = shark12;
          else
            img = shark13;
        } else {
          sW = 90;
          sH = 90;
          if (sharks[i].jumpState == 0)
            img = shark21;
          else if (sharks[i].jumpState == 1)
            img = shark22;
          else
            img = shark23;
        }

        if (sharks[i].isRetreating) {
          iShowImageHFlip((int)sharks[i].x, (int)sharks[i].y, sW, sH, img);
        } else {
          iShowImage((int)sharks[i].x, (int)sharks[i].y, sW, sH, img);
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
      iShowImage((int)leverX, (int)leverY, leverW, leverH, lImg);
      if (!bridges[i].leverActivated) {
        int seasonImg = obs221;
        if (selectedWeather == 1)
          seasonImg = obs222;
        else if (selectedWeather == 2)
          seasonImg = obs223;

        // Always show seasonImg (water/hole) when lever is red/inactive
        iShowImage((int)bridges[i].x, (int)bridges[i].y - 15, 450, 60,
                   seasonImg);

        if (!bridges[i].isBroken) {
          iShowImage((int)bridges[i].x - 50, (int)bridges[i].y - 40, 550, 60,
                     imgBroken);
        } else {
          iShowImage((int)bridges[i].x - 50, (int)bridges[i].fallenY, 550, 60,
                     imgBroken);
        }
      }
    }
    for (int i = 0; i < 3; i++) {
      if (bombs[i].active) {
        iShowImage((int)bombs[i].x, (int)bombs[i].y, 50, 50, bombImg);
      }
    }
    return;
  }

  if (currentLevel == 3) {
    // Worms (Enhanced with 4-frame cycle for smoother movement)
    int wF = wormFrame / 10; // 0, 1, 2, 3
    for (int i = 0; i < 2; i++) {
      if (sharks[i].active) {
        int img = 0;
        if (sharks[i].type == 1) {
          if (sharks[i].jumpState == 1)
            img = worm12;
          else if (sharks[i].jumpState == 2)
            img = worm13;
          else {
            // Cycle 11, 12, 13, 14 (where 14 is mapped to 13 or 12)
            if (wF == 0)
              img = worm11;
            else if (wF == 1)
              img = worm12;
            else if (wF == 2)
              img = worm13;
            else
              img = worm14;
          }
        } else {
          if (sharks[i].jumpState == 1)
            img = worm22;
          else if (sharks[i].jumpState == 2)
            img = worm23;
          else {
            // Cycle 21, 22, 23, 24
            if (wF == 0)
              img = worm21;
            else if (wF == 1)
              img = worm22;
            else if (wF == 2)
              img = worm23;
            else
              img = worm24;
          }
        }
        int wW = 150;
        int wH = 120;
        
        // Rotated/Flipped when retreating so they face opposite directon
        bool shouldFlip = sharks[i].isRetreating;

        if (shouldFlip) {
          iShowImageHFlip((int)sharks[i].x, (int)sharks[i].y, wW, wH, img);
        } else {
          iShowImage((int)sharks[i].x, (int)sharks[i].y, wW, wH, img);
        }
      }
    }

    // Holes and Pillars
    for (int i = 0; i < 2; i++) {
      if (!bridges[i].active)
        continue;

      // Pillars
      int pImg = bridges[i].isRLL ? shortPillerImg : longPillerImg;
      int pW = 80;
      int pH = bridges[i].isRLL ? 120 : 350;
      iShowImage((int)bridges[i].x - 80, (int)bridges[i].y - 30, pW, pH, pImg);

      // Flame/Fire on Pillars
      if (!flamesHitGround[i]) {
        if (!isFlameFalling[i]) {
          // Exactly on pillar top
          float fY = (float)bridges[i].y - 30 + pH;
          // Reverted size to 50x60, centered on pillar (pX - 80, pW 80 -> center at -40)
          iShowImage((int)bridges[i].x - 40 - 25, (int)fY, 50, 60,
                     flameImgs[flameFrame / 10]);
        } else {
          // While falling, interpolate X from pillar to hole area
          float startY = (float)bridges[i].y - 30 + pH;
          float totalDistY = startY - (float)groundY;
          if (totalDistY < 1)
            totalDistY = 1;
          float progress = 1.0f - ((flameYPos[i] - (float)groundY) / totalDistY);
          if (progress < 0)
            progress = 0;
          if (progress > 1)
            progress = 1;

          float startX = (float)bridges[i].x - 65;
          float endX = (float)bridges[i].x + 200; // Aim toward hole area
          float currentX = startX + (endX - startX) * progress;

          iShowImage((int)currentX, (int)flameYPos[i], 50, 60,
                     flameImgs[flameFrame / 10]);
        }
      } else {
        // Show fire IN FRONT OF THE HOLE
        // Coordinates offset from hole as requested (x+90 from last coordinate)
        iShowImage((int)bridges[i].x + 90, (int)bridges[i].y - 30, 300, 80,
                   fireImgs[fireFrame / 10]);
      }

      // Hole and Hands
      if (!isL3HoleRemoved) {
        // Render Hole
        iShowImage((int)bridges[i].x - 50, (int)bridges[i].y - 70, 550, 100,
                   l3HoleImg);
        
        // Render Hand on top of hole if flame didn't hit ground
        if (!flamesHitGround[i]) {
            // Hand Y lowered by 10 as requested (now at y - 70)
            // Size 300x160, centered over hole
            iShowImage((int)bridges[i].x + 75, (int)bridges[i].y - 70, 300, 160,
                       handImgs[handFrame / 10]);
        }
      } else {
        // If hole is "removed" by flame, keep rendering hole
        iShowImage((int)bridges[i].x - 50, (int)bridges[i].y - 70, 550, 100,
                   l3HoleImg);
      }
    }
    return;
  }

  // Level 1 logic
  for (int i = 0; i < 3; i++) {
    if (obstacles[i].active) {
      if (obstacles[i].type == 1)
        iShowImage(obstacles[i].x, obstacles[i].y, 120, 120, obs1);
      else if (obstacles[i].type == 2)
        iShowImage(obstacles[i].x, obstacles[i].y, 120, 80, obs2);
      else
        iShowImage(obstacles[i].x, obstacles[i].y, 120, 120, obs3);
    }
  }
}

inline void drawExplosions() {
  if (currentLevel == 2) {
    for (int i = 0; i < 3; i++) {
      if (explosions[i].active) {
        iShowImage((int)explosions[i].x - 65, (int)explosions[i].y, 130, 70,
                   explosionImgs[explosions[i].frame]);
      }
    }
  }
}

#endif
