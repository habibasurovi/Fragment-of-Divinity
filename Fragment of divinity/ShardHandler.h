#ifndef SHARDHANDLER_H
#define SHARDHANDLER_H

#include "Character.h"
#include "ObstacleHandler.h"
#include "dragon.h"
#include "iGraphics.h"
#include <stdio.h>
#include <stdlib.h>

// Globals
extern float fallingShardX, fallingShardY;
extern bool isFallingShardActive;
extern int shardSpawnTimer;
extern bool isHaloActive;
extern int haloTimer;
// extern int haloImg;
extern int shardImg;
extern int currentLevel;
// extern int screenWidth, screenHeight; // Commented out to avoid #define
// conflict in VS 2013
extern bool hasClaimedShard;
SELECTANY int shardCooldownTimer = 0;
extern int haloFrames[5];
SELECTANY int haloFrameIndex = 0;
SELECTANY int haloAnimCounter = 0;

inline void initShard() {
  isFallingShardActive = false;
  shardSpawnTimer = 0;
  isHaloActive = false;
  haloTimer = 0;
}

inline void updateShardLogic() {
  if (shardCooldownTimer > 0)
    shardCooldownTimer--;
  if ((currentLevel != 2 && currentLevel != 3 && currentLevel != 4) ||
      !hasClaimedShard)
    return;

  // 1. Spawning
  shardSpawnTimer++;
  // 15-20 sec gap at 30fps -> 450 to 600 frames
  int spawnThreshold = 450 + rand() % 151;

  if (shardSpawnTimer >= spawnThreshold && !isFallingShardActive &&
      !isHaloActive) {
    isFallingShardActive = true;
    fallingShardX = (float)(100 + rand() % 800);
    fallingShardY = 650; // Above screen
    shardSpawnTimer = 0;
  }

  // 2. Physics & Collision
  if (isFallingShardActive) {
    fallingShardY -= 5; // Fall speed

    // Character collision
    // Hitbox: character is roughly charWidth x charHeight
    if (charX + charWidth - 30 > fallingShardX &&
        charX + 30 < fallingShardX + 60 &&
        charY + charHeight - 30 > fallingShardY &&
        charY + 10 < fallingShardY + 60) {

      isFallingShardActive = false;
      isHaloActive = true;
      haloTimer = 150; // 5 seconds at 30fps
      isInvincible = true;
    }

    if (fallingShardY < -100) {
      isFallingShardActive = false;
    }
  }

  // 3. Halo Logic & Obstacle Destruction
  if (isHaloActive) {
    haloTimer--;
    isInvincible = true; // Stay protected

    // Check collision with ALL obstacles and deactivate them if touched

    // a. Standard Obstacles
    for (int i = 0; i < 3; i++) {
      if (obstacles[i].active) {
        // Using a slightly larger hitbox for the halo (approx 150x150)
        if (charX + charWidth > obstacles[i].x &&
            charX < obstacles[i].x + 120 &&
            charY + charHeight > obstacles[i].y &&
            charY < obstacles[i].y + 120) {
          obstacles[i].active = false;
          // Note: "blink and vanish" - since user said "immediately",
          // deactivating is the most immediate way.
        }
      }
    }

    // b. Sharks
    for (int i = 0; i < 2; i++) {
      if (sharks[i].active) {
        int sRange = (sharks[i].type == 1) ? 70 : 90;
        if (charX + charWidth > sharks[i].x && charX < sharks[i].x + sRange &&
            charY + charHeight > sharks[i].y && charY < sharks[i].y + sRange) {
          sharks[i].active = false;
        }
      }
    }

    // c. Bombs
    for (int i = 0; i < 3; i++) {
      if (bombs[i].active) {
        if (charX + charWidth > bombs[i].x && charX < bombs[i].x + 40 &&
            charY + charHeight > bombs[i].y && charY < bombs[i].y + 40) {
          bombs[i].active = false;
        }
      }
    }

    // d. Dragons (if any)
    for (int i = 0; i < 2; i++) {
      if (dragons[i].active) {
        if (charX + charWidth > dragons[i].x && charX < dragons[i].x + 150 &&
            charY + charHeight > dragons[i].y && charY < dragons[i].y + 150) {
          dragons[i].active = false;
        }
      }
    }

    // --- LEVEL 3 SPECIFIC DESTRUCTION ---
    if (currentLevel == 3) {
      // e. Level 3 Walking NPCs
      for (int i = 0; i < 3; i++) {
        if (npcList[i].active) {
          if (charX + charWidth > npcList[i].x && charX < npcList[i].x + 120 &&
              charY + charHeight > npcList[i].y && charY < npcList[i].y + 120) {
            npcList[i].active = false;
          }
        }
      }
      // f. Level 3 Green Fires
      for (int i = 0; i < 3; i++) {
        if (fireList[i].active) {
          if (charX + charWidth > fireList[i].x && charX < fireList[i].x + 80 &&
              charY + charHeight > fireList[i].y &&
              charY < fireList[i].y + 80) {
            fireList[i].active = false;
          }
        }
      }
      // g. Level 3 Skulls
      for (int i = 0; i < 3; i++) {
        if (skulls[i].active) {
          if (charX + charWidth > skulls[i].x && charX < skulls[i].x + 80 &&
              charY + charHeight > skulls[i].y && charY < skulls[i].y + 80) {
            skulls[i].active = false;
          }
        }
      }
    }

    if (isHaloActive) {
      haloAnimCounter++;
      if (haloAnimCounter >= 10) {
        haloAnimCounter = 0;
        haloFrameIndex = (haloFrameIndex + 1) % 5;
      }
    }
    if (haloTimer <= 0) {
      isHaloActive = false;
      isInvincible = false;
      haloFrameIndex = 0;
      haloAnimCounter = 0;
    }
  }
}

inline void drawShardElements() {
  if ((currentLevel != 2 && currentLevel != 3 && currentLevel != 4) ||
      !hasClaimedShard)
    return;

  if (isFallingShardActive) {
    iShowImage((int)fallingShardX, (int)fallingShardY, 60, 60, shardImg);
  }

  if (isHaloActive) {
    // Center the halo on the character
    int haloWidth = 180;
    int haloHeight = 200;
    int hX = charX + (charWidth / 2) - (haloWidth / 2);
    int hY = charY; // As requested, same as character's Y
    iShowImage(hX, hY, haloWidth, haloHeight, haloFrames[haloFrameIndex]);
  }
}

#endif
