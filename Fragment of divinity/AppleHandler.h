#ifndef APPLEHANDLER_H
#define APPLEHANDLER_H

#include "Character.h"
#include "ObstacleHandler.h"
#include "iGraphics.h"
#include <stdlib.h>
#include <stdio.h>

// Variables
struct Apple {
  int x, y;
  bool active;
};
extern Apple apples[20]; // Buffer for multiple apples

extern int applesCollected;
extern int appleImg;
extern int appleSpawnTimer;
extern bool isCaveActive;
extern bool isLevelTransitioning;
extern int transitionTimer;

#define APPLE_Y 120
#define APPLE_W 40
#define APPLE_H 40
#define APPLE_GAP 80

inline void loadAppleAssets() {
  appleImg = iLoadImage("scores and items\\apple.png");
}

inline void initApples() {
  for (int i = 0; i < 20; i++) {
    apples[i].active = false;
  }
  applesCollected = 0;
}

inline void updateApplePhysics() {
  // 1. Move existing apples
  int activeCount = 0;
  int rightmostX = -100;

  for (int i = 0; i < 20; i++) {
    if (apples[i].active) {
      apples[i].x -= SCROLL_SPD; // Move with background speed
      activeCount++;

      if (apples[i].x > rightmostX)
        rightmostX = apples[i].x;

      if (apples[i].x < -50) {
        apples[i].active = false;
      }
    }
  }

  // 2. Spawn Logic
  static int distSinceLastApple = 0;
  static int nextAppleGap = 300; // Initial gap

  distSinceLastApple += SCROLL_SPD;

  // Check if cave is approaching.
  // caveX starts offscreen right (e.g. 1000) and moves left.
  // If caveX is within a "danger zone" (e.g., < 1300), don't spawn.
  // We want to ensure no apples spawn if they would overlap the cave entry.
  bool safeToSpawn = !isCaveActive;
  if (isCaveActive || transitionTimer > 0 || isLevelTransitioning) {
      safeToSpawn = false;
  }
  
  if (distSinceLastApple > nextAppleGap && safeToSpawn) {
    

    int groupSize =
        3 + rand() % 4; // 3 to 6 apples (reduced slightly to fit gaps)
    int startX = SCREEN_W + 300;

	int groupWidth = (groupSize * APPLE_GAP);

    // Determine Y position based on Obstacles
    int spawnY = 120; // Default: middle reachable
    bool overlapHigh = false;
    bool overlapLow = false;

    // Check active obstacles for POTENTIAL overlap
   

    for (int i = 0; i < 3; i++) {
        if (obstacles[i].active) {
            int obsLeft = obstacles[i].x;
            int obsRight = obstacles[i].x + 120;
            if (obsRight + 50 > startX && obsLeft < startX + groupWidth + 50) {
                if (obstacles[i].type == 1) overlapHigh = true;
                else overlapLow = true;
            }
        }
    }

    // Level 2 obstacles
    for (int i = 0; i < 2; i++) {
        // Sharks
        if (sharks[i].active) {
            if (sharks[i].x + 100 > startX && sharks[i].x < startX + groupWidth + 100) {
                overlapLow = true; // Sharks are mostly low/mid
                overlapHigh = true; // They also jump
            }
        }
        // Bridges
        if (bridges[i].active) {
            if (bridges[i].x + 500 > startX && bridges[i].x < startX + groupWidth + 100) {
                overlapLow = true; // Bridges are at y=90
            }
        }
    }

    // Decision Logic with Range Constraints (100-200px difference)
    if (overlapHigh && overlapLow) {
      // Blocked both ways? Delay.
      distSinceLastApple = 0;
      nextAppleGap = 220;
      return;
    } else if (overlapHigh) {
      // Obstacle is High (e.g., Bird). We must spawn Low.
      // Constraint: Obstacle ~250. We need 100-200 diff.
      // Target Y: 50 to 150.
      spawnY = 50 + rand() % 50; // 50-100 range
    } else if (overlapLow) {
      // Obstacle is Low (e.g., Stone). We must spawn High.
      // Constraint: Obstacle ~50. We need 100-200 diff.
      // Target Y: 150 to 250.
      spawnY = 140 + rand() % 41; // 140-180 range (reachable)
    } else {
      // No immediate overlap, but we should still respect general spacing
      spawnY = 50 + rand() % 131; // 50-180 full range
    }

    // Spawn the group
    int spawnedCount = 0;
    for (int i = 0; i < 20; i++) {
      if (!apples[i].active) {
        apples[i].active = true;
        apples[i].x = startX + (spawnedCount * APPLE_GAP);
        apples[i].y = spawnY;

        spawnedCount++;
        if (spawnedCount >= groupSize)
          break;
      }
    }

    distSinceLastApple = 0;
    // Frequency: 220px to 500px gap
    nextAppleGap = 220 + rand() % (500 - 220 + 1);
  }
}

inline void checkAppleCollision() {
  // Character Hitbox (approximate based on refined logic)
  int cLeft = charX + 50;
  int cRight = charX + charWidth - 50;
  int cTop = charY + charHeight - 20;
  int cBottom = charY;

  for (int i = 0; i < 20; i++) {
    if (apples[i].active) {
      // AABB Collision
      if (cRight > apples[i].x && cLeft < apples[i].x + APPLE_W &&
          cTop > apples[i].y && cBottom < apples[i].y + APPLE_H) {

        apples[i].active = false;
        applesCollected++;
      }
    }
  }
}

inline void drawApples() {
  for (int i = 0; i < 20; i++) {
    if (apples[i].active) {
      iShowImage(apples[i].x, apples[i].y, APPLE_W, APPLE_H,
                 appleImg); // Render in world
    }
  }
}

inline void drawAppleScoreUI() {
  // Top Right Corner
  int uiX = SCREEN_W - 150;
  int uiY = SCREEN_H - 60;

  // Icon
  iShowImage(uiX, uiY, 50, 50, appleImg);

  // Colon
  iSetColor(255, 255, 255);
  iText(uiX + 50, uiY + 10, ":", GLUT_BITMAP_TIMES_ROMAN_24);

  // Score
  char scoreText[10];
  sprintf_s(scoreText, "%d", applesCollected);
  iText(uiX + 70, uiY + 10, scoreText, GLUT_BITMAP_TIMES_ROMAN_24);
}

#endif
