#ifndef OWL_COMPANION_H_
#define OWL_COMPANION_H_

// =====================================================================
// owl.h  --  Level 4 Owl Companion Mechanic
// Requires: Boss.h, ObstacleHandler.h included BEFORE this header.
// Global owlImg, owlReady, owlCooldownTimer defined in iMain.cpp.
// =====================================================================

#include "iGraphics.h"
#include <stdio.h>

// Externals from iMain.cpp
extern int owlImg;
extern int owlBulletImg;
extern int owlFrames[9];
extern bool owlReady;
extern int owlCooldownTimer;
extern bool hasCompanion;
extern int currentLevel;
extern GameState gameState;
extern int groundY;

// Externals from Boss.h / iMain.cpp
extern BossLevel4 boss4Obj;
extern int level4Phase;

// ---- Owl Bullet ----
struct OwlBullet {
  float x, y;
  bool active;
};

// ---- Owl State ----
#define OWL_MAX_BULLETS 10
#define OWL_WIDTH 120 // Enlarged from 93
#define OWL_HEIGHT 80  // Enlarged from 62
#define OWL_STATION_X 250.0f
#define OWL_FLY_Y 200.0f // flying height (y from bottom)

struct OwlCompanion {
  float x, y;
  bool active;
  bool stationed; // false=flying in, true=at station X
  int life;       // 12 hits from enemies
  int hitFlashTimer;
  int bulletTimer; // counts down; fire when 0
  int frameIndex;
  int animCounter;
  OwlBullet bullets[OWL_MAX_BULLETS];

  // Per-NPC owlet hit counters (parallel to boss4Obj.restNpcs / restBigNpc)
  int npcOwlHits[3];
  int bigNpcOwlHits;
};

#ifdef _MSC_VER
#define OWL_SELECTANY __declspec(selectany)
#else
#define OWL_SELECTANY __attribute__((weak))
#endif

OWL_SELECTANY OwlCompanion owlCompanion;

// ---- Init ----
inline void initOwl() {
  owlCompanion.active = true;
  owlCompanion.stationed = false;
  owlCompanion.x = -120.0f;
  owlCompanion.y = (float)groundY; // Walk on the ground
  owlCompanion.life = 12;
  owlCompanion.hitFlashTimer = 0;
  owlCompanion.bulletTimer = 30; // fire first bullet 1 sec after stationing
  owlCompanion.frameIndex = 0;
  owlCompanion.animCounter = 0;
  for (int i = 0; i < OWL_MAX_BULLETS; i++)
    owlCompanion.bullets[i].active = false;
  for (int i = 0; i < 3; i++)
    owlCompanion.npcOwlHits[i] = 0;
  owlCompanion.bigNpcOwlHits = 0;

  // Start cooldown immediately so owl can't be re-used until back
  owlReady = false;
  owlCooldownTimer = 10;
}

// ---- Spawn a bullet ----
inline void owlFireBullet() {
  for (int i = 0; i < OWL_MAX_BULLETS; i++) {
    if (!owlCompanion.bullets[i].active) {
      owlCompanion.bullets[i].active = true;
      owlCompanion.bullets[i].x = owlCompanion.x + 55.0f; // Approx gunpoint
      owlCompanion.bullets[i].y = owlCompanion.y + 18.0f; // Align to gun
      break;
    }
  }
}

// ---- Helper: AABB hit check for a bullet ----
inline bool owlBulletAABB(const OwlBullet &b, float tx, float ty, float tw,
                          float th) {
  return b.active && b.x + 60.0f > tx && b.x < tx + tw && b.y + 20.0f > ty &&
         b.y < ty + th;
}

// ---- Helper: AABB hit check for owl itself ----
inline bool owlBodyAABB(float tx, float ty, float tw, float th) {
  return owlCompanion.x + OWL_WIDTH > tx && owlCompanion.x < tx + tw &&
         owlCompanion.y + OWL_HEIGHT > ty && owlCompanion.y < ty + th;
}

// ---- Hit owl (reduces life) ----
inline void owlTakeHit() {
  if (owlCompanion.hitFlashTimer > 0)
    return; // invincibility frames
  owlCompanion.life--;
  owlCompanion.hitFlashTimer = 20;
  if (owlCompanion.life <= 0) {
    owlCompanion.active = false;
  }
}

// ---- Update (called every 30ms masterGameLoop tick) ----
inline void updateOwl() {
  if (!owlCompanion.active)
    return;

  // --- Fly in & Animate ---
  if (!owlCompanion.stationed) {
    owlCompanion.x += 8.0f;
    owlCompanion.animCounter++;
    if (owlCompanion.animCounter >= 5) { // Animation speed
      owlCompanion.animCounter = 0;
      owlCompanion.frameIndex = (owlCompanion.frameIndex + 1) % 9;
    }
    if (owlCompanion.x >= OWL_STATION_X) {
      owlCompanion.x = OWL_STATION_X;
      owlCompanion.stationed = true;
      owlCompanion.frameIndex = 0; // Show frame_000 when stationed
    }
  }

  // --- Fire bullets every 30 ticks (~1 sec) while stationed ---
  if (owlCompanion.stationed) {
    owlCompanion.bulletTimer--;
    if (owlCompanion.bulletTimer <= 0) {
      owlCompanion.bulletTimer = 60; // 2 seconds (was 30)
      owlFireBullet();
    }
  }

  // --- Move bullets rightward ---
  for (int i = 0; i < OWL_MAX_BULLETS; i++) {
    if (owlCompanion.bullets[i].active) {
      owlCompanion.bullets[i].x += 18.0f;
      if (owlCompanion.bullets[i].x > 1060.0f)
        owlCompanion.bullets[i].active = false;
    }
  }

  // --- Hit flash decay ---
  if (owlCompanion.hitFlashTimer > 0)
    owlCompanion.hitFlashTimer--;
}

// ---- Collision checks (called every masterGameLoop tick) ----
inline void updateOwlCollisions() {
  if (!owlCompanion.active)
    return;

  // ====== BULLETS HIT ENEMIES ======
  for (int i = 0; i < OWL_MAX_BULLETS; i++) {
    if (!owlCompanion.bullets[i].active)
      continue;
    bool hit = false;

    // -- Boss --
    if (!hit && boss4Obj.active && level4Phase == 3) {
      if (owlBulletAABB(owlCompanion.bullets[i], boss4Obj.x, boss4Obj.y,
                        BOSS_WIDTH, BOSS_HEIGHT)) {
        owlCompanion.bullets[i].active = false;
        boss4Obj.life -= 1; // 1 health unit per bullet
        boss4Obj.isHit = true;
        boss4Obj.hitTimer = 12;
        if (boss4Obj.life <= 0) {
          boss4Obj.life = 0;
          level4Phase = 4; // trigger die sequence
        }
        hit = true;
      }
    }

    // -- Rest-phase WalkingNPCs --
    if (!hit) {
      for (int j = 0; j < 3; j++) {
        if (boss4Obj.restNpcs[j].active) {
          if (owlBulletAABB(owlCompanion.bullets[i], boss4Obj.restNpcs[j].x,
                            boss4Obj.restNpcs[j].y, 80.0f, 120.0f)) {
            owlCompanion.bullets[i].active = false;
            owlCompanion.npcOwlHits[j]++;
            boss4Obj.restNpcs[j].hitFlashTimer = 10;
            if (owlCompanion.npcOwlHits[j] >= 4) {
              boss4Obj.restNpcs[j].active = false;
              owlCompanion.npcOwlHits[j] = 0;
            }
            hit = true;
            break;
          }
        }
      }
    }

    // -- BigNPC --
    if (!hit && boss4Obj.restBigNpc.active) {
      if (owlBulletAABB(owlCompanion.bullets[i], boss4Obj.restBigNpc.x,
                        boss4Obj.restBigNpc.y, 200.0f, 200.0f)) {
        owlCompanion.bullets[i].active = false;
        owlCompanion.bigNpcOwlHits++;
        boss4Obj.restBigNpc.hitFlashTimer = 10;
        if (owlCompanion.bigNpcOwlHits >= 10) {
          boss4Obj.restBigNpc.active = false;
          owlCompanion.bigNpcOwlHits = 0;
        }
        hit = true;
      }
    }

    // -- Spider/Enemy (bossEnemies) - 1 hit kill --
    if (!hit) {
      for (int j = 0; j < 5; j++) {
        if (boss4Obj.bossEnemies[j].active) {
          if (owlBulletAABB(owlCompanion.bullets[i], boss4Obj.bossEnemies[j].x,
                            boss4Obj.bossEnemies[j].y, 100.0f, 60.0f)) {
            owlCompanion.bullets[i].active = false;
            boss4Obj.bossEnemies[j].active = false;
            hit = true;
            break;
          }
        }
      }
    }
  }

  // ====== ENEMIES HIT OWL ======

  // Boss fireball
  if (boss4Obj.fbActive) {
    if (owlBodyAABB(boss4Obj.fbX, boss4Obj.fbY, 40.0f, 40.0f)) {
      boss4Obj.fbActive = false;
      owlTakeHit();
    }
  }

  // Top obstacles (dropping rocks/highobs)
  for (int i = 0; i < 10; i++) {
    if (boss4Obj.topObstacles[i].active) {
      if (owlBodyAABB(boss4Obj.topObstacles[i].x, boss4Obj.topObstacles[i].y,
                      90.0f, 90.0f)) {
        boss4Obj.topObstacles[i].active = false;
        owlTakeHit();
        if (!owlCompanion.active)
          return;
      }
    }
  }

  // Spider enemies
  for (int i = 0; i < 5; i++) {
    if (boss4Obj.bossEnemies[i].active) {
      if (owlBodyAABB(boss4Obj.bossEnemies[i].x, boss4Obj.bossEnemies[i].y,
                      100.0f, 60.0f)) {
        owlTakeHit();
        if (!owlCompanion.active)
          return;
      }
    }
  }

  // Rest-phase WalkingNPCs
  for (int i = 0; i < 3; i++) {
    if (boss4Obj.restNpcs[i].active) {
      if (owlBodyAABB(boss4Obj.restNpcs[i].x, boss4Obj.restNpcs[i].y, 80.0f,
                      120.0f)) {
        owlTakeHit();
        if (!owlCompanion.active)
          return;
      }
    }
  }

  // BigNPC
  if (boss4Obj.restBigNpc.active) {
    if (owlBodyAABB(boss4Obj.restBigNpc.x, boss4Obj.restBigNpc.y, 200.0f,
                    200.0f)) {
      owlTakeHit();
    }
  }
}

// ---- Draw ----
inline void drawOwl() {
  if (!owlCompanion.active)
    return;

  // Flash red when hit
  if (owlCompanion.hitFlashTimer > 0 &&
      (owlCompanion.hitFlashTimer / 3) % 2 == 1) {
    iSetColor(255, 80, 80);
  } else {
    iSetColor(255, 255, 255);
  }
  iShowImage((int)owlCompanion.x, (int)owlCompanion.y, OWL_WIDTH, OWL_HEIGHT,
             owlFrames[owlCompanion.frameIndex]);

  // Draw bullets (3:1 ratio: 60x20)
  for (int i = 0; i < OWL_MAX_BULLETS; i++) {
    if (owlCompanion.bullets[i].active) {
      iShowImage((int)owlCompanion.bullets[i].x, (int)owlCompanion.bullets[i].y,
                 60, 20, owlBulletImg);
    }
  }

  // Health bar beneath the hearts (Player hearts at startX=100, Y=535)
  // Background
  iSetColor(40, 40, 40);
  iFilledRectangle(100, 515, 200, 8);
  // Current Life
  iSetColor(80, 220, 80);
  iFilledRectangle(100, 515, (int)(200.0f * (owlCompanion.life / 12.0f)), 8);
  iSetColor(255, 255, 255);
  iRectangle(100, 515, 200, 8);
}

#endif // OWL_COMPANION_H_
