#ifndef LIFEHANDLER_H
#define LIFEHANDLER_H

#include "iGraphics.h"
#include <stdio.h>
#include <stdlib.h>

// Extern variables defined in iMain or Character.h
extern int lives;
extern int heartImg;

// Life UI Positioning
// Beside pause icon (Pause is at 20, 520, size 70x70)
// We'll place hearts starting from 100, 535
extern int gameRunTimeSeconds; // Defined in iMain.cpp
extern int charX, charY;       // From Character.h
extern int charWidth, charHeight;
extern int currentLevel;

extern int heartItemImg;
struct HeartItem {
  int x, y;
  bool active;
};
extern struct HeartItem heartItems[2];

extern int heartsSpawnedCount;

inline void loadLifeAssets() {
  heartImg = iLoadImage((char *)"scores and items\\heart1.bmp");
  heartItemImg = iLoadImage((char *)"scores and items\\heart2.bmp");

  // Initialize hearts
  for (int i = 0; i < 2; i++) {
    heartItems[i].active = false;
    heartItems[i].x = 0;
    heartItems[i].y = 0;
  }
}

extern int heart1SpawnTime;
extern int heart2SpawnTime;
extern bool heart1Queued;
extern bool heart2Queued;

inline void spawnHeartLevel4() {
  for (int i = 0; i < 2; i++) {
    if (!heartItems[i].active) {
      heartItems[i].active = true;
      heartItems[i].x = 100 + rand() % 800; // falling from above
      heartItems[i].y = 650;
      break;
    }
  }
}

inline void updateHeartLogic() {
  // Initialize spawn times once
  if (heart1SpawnTime == 0) {
    heart1SpawnTime = 20 + rand() % 16; // 20-35s
    heart2SpawnTime = 50 + rand() % 16; // 50-65s
  }

  // Spawn Heart 1 & 2 (Level 1,2,3 logic)
  if (currentLevel != 4) {
    if (gameRunTimeSeconds >= heart1SpawnTime && !heart1Queued && heartsSpawnedCount < 1) {
      for (int i = 0; i < 2; i++) {
        if (!heartItems[i].active) {
          heartItems[i].active = true;
          heartItems[i].x = 1000;
          heartItems[i].y = 50 + rand() % 131; // 50-180 (reachable)
          heart1Queued = true;
          heartsSpawnedCount++;
          break;
        }
      }
    }
    if (gameRunTimeSeconds >= heart2SpawnTime && !heart2Queued && heartsSpawnedCount < 2) {
      for (int i = 0; i < 2; i++) {
        if (!heartItems[i].active) {
          heartItems[i].active = true;
          heartItems[i].x = 1000;
          heartItems[i].y = 50 + rand() % 131; // 50-180 (reachable)
          heart2Queued = true;
          heartsSpawnedCount++;
          break;
        }
      }
    }
  }

  // Move hearts (ALL levels)
  for (int i = 0; i < 2; i++) {
    if (heartItems[i].active) {
      if (currentLevel == 4) {
        heartItems[i].y -= 4; // Move down
        if (heartItems[i].y < -50) {
          heartItems[i].active = false;
        }
      } else {
        heartItems[i].x -= 5; // Move left
        if (heartItems[i].x < -50) {
          heartItems[i].active = false;
        }
      }

      // Collision Detection
      if (charX + charWidth > heartItems[i].x &&
          charX < heartItems[i].x + 40 &&
          charY + charHeight > heartItems[i].y &&
          charY < heartItems[i].y + 40) {

        heartItems[i].active = false;
        lives++; 
      }
    }
  }
}

inline void drawLivesUI() {
  int startX = 100;
  int startY = 535; // Centered vertically relative to pause button
  int heartSize = 40;
  int spacing = 5;

  for (int i = 0; i < lives; i++) {
    if (heartImg > 0) {
      iShowImage(startX + i * (heartSize + spacing), startY, heartSize,
                 heartSize, heartImg);
    } else {
      // Fallback circle if image fails
      iSetColor(255, 0, 0);
      iFilledCircle(startX + i * (heartSize + spacing) + heartSize / 2.0,
                    startY + heartSize / 2.0, heartSize / 2.0);
    }
  }

  // Draw Spawned Heart Items
  for (int i = 0; i < 2; i++) {
    if (heartItems[i].active) {
      iShowImage(heartItems[i].x, heartItems[i].y, 40, 40, heartItemImg);
    }
  }
}

#endif
