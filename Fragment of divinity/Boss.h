#ifndef BOSS_HEADER_H
#define BOSS_HEADER_H

#define _CRT_SECURE_NO_WARNINGS
#include "GameState.h"
#include "ObstacleHandler.h"
#include "iGraphics.h"
#include <math.h>
#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

extern int charAttackDamagelessTimer;

// --- BOSS GLOBAL VARIABLES (Using selectany for header-only consolidation) ---
#ifdef _MSC_VER
#define SELECTANY __declspec(selectany)
#else
#define SELECTANY __attribute__((weak))
#endif

SELECTANY int bossImgs[4];
SELECTANY int bossFlameImgs[11];
SELECTANY int bossWalkImgs[8];
SELECTANY int bossStandImgs[6];
SELECTANY int bossDieImgs[21];
SELECTANY int bossVanishImgs[8];
SELECTANY int bossAttackImgs[11];
SELECTANY int bossFireMotionImgs[10];
SELECTANY int fireballImgs[10];
SELECTANY int bossPreJumpImgs[6];
SELECTANY int bossJumpImgs[8];
SELECTANY int bossSmashImgs[10];
SELECTANY int bossPullImgs[5];
SELECTANY int bossPullDanceImgs[7];
SELECTANY int bossPushImgs[8];
SELECTANY int bossPushActiveImgs[8];
SELECTANY int bossHighImgs[9];
SELECTANY int bossThunderImgs[4];
SELECTANY int bossRestSkillImgs[7];
SELECTANY int bossShoutImgs[11];
SELECTANY int bossHitgroundImgs[11];
SELECTANY int bossHoleImgs[10];

// Bird images for Skill 8 (Hitground) bird attack
SELECTANY int artikunoImgs[6];
SELECTANY int volplexImgs[6];
SELECTANY int bossBackImgs[10];

SELECTANY bool level4BossEverCompletedFirstSequence = false;

SELECTANY int bigNpcWalkImgs[9];
SELECTANY int bigNpcAttackImgs[9];
SELECTANY int bg41, bg42;
SELECTANY int bossLifeFrameImg;

SELECTANY int spiderImgs[4];
SELECTANY int worm11, worm12, worm13, worm14;
SELECTANY int worm21, worm22, worm23, worm24;
SELECTANY int walkNpcHitAccumulator = 0; // 3 walking-NPC hits = 1 life lost
SELECTANY int highobs1Imgs[4];
SELECTANY int highobs2Imgs[4];
SELECTANY int batImgs[6];

struct BatNPC {
  float x, y;
  float velX, velY;
  bool active;
  int frame;
  int frameTimer;
  int life;
  int hitFlashTimer;
  bool facingLeft;
};

struct BossEnemyObj {
  float x, y;
  bool active;
  int type; // 0 for worm1, 1 for worm2
  int frame;
  int frameTimer;
};

struct BossHole {
  float x, y;
  bool active;
  int state; // 1=appearing, 2=staying, 3=disappearing
  int frame;
  int frameTimer;
  int stateTimer;   // For 3 sec stay
  bool birdSpawned; // Has a bird been spawned from this hole?
};

// Bird spawned by Hitground holes
struct HoleBird {
  float x, y; // Current position (center of bird)
  bool active;
  int type; // 0 = artikuno, 1 = volplex
  int frame;
  int frameTimer;
  int state; // 0=rising, 1=hovering, 2=diving, 3=dying, 4=fleeing
  int stateTimer;
  float scale;       // 0.0 to 1.0 (grows while rising)
  float velX, velY;  // Velocity for dive/fly-away
  int holeIndex;     // Which hole spawned this bird
  float startX;      // Hole center X (spawn origin)
  int life;          // NEW: Requires 2 hits to die
  int hitFlashTimer; // NEW: Blink when hit
};

struct BossTopObstacle {
  float x, y;
  float speedY;
  bool active;
  int frame;
  int frameTimer;
  int type; // 0 = highobs1, 1 = highobs2
};

// Big NPC that spawns during boss rest phase
struct BigNPC {
  float x, y;
  bool active;
  int life;  // 6 hits to kill
  int state; // 0: walking/following, 1: attacking
  int animFrame;
  int animCounter;
  int attackTimer;
  bool hasAttacked;
  int hitFlashTimer;
  int stunTimer; // 1-second stun after being hit (30 frames)
};

// Boss State Structure
struct BossLevel4 {
  float x, y;
  int frameIndex;
  int flameFrameIndex;
  bool active;
  int animCounter;
  int flameAnimCounter;
  int life; // Max 100, reduces by 5 per hit (1/20)
  int maxLife;
  bool isHit;
  int hitTimer;
  int dieFrameIndex;
  int dieAnimCounter;
  int victoryDelay; // Delay before showing victory screen after death
  int previousLife;
  int consecutiveHits;
  int knockbackState;
  int knockbackTimer;
  int knockbackFrameIndex;
  float bossInitialX;

  // Vanish Skill State
  int skillState; // 0: Idle, 1: Vanishing, 2: Hidden (Wait 2s), 3: Appearing
                  // (at char), 4: Attacking, 5: Gap (Before next)
  int skillTimer;
  int skillFrameIndex;
  int skillCount;
  float originalX, originalY;
  bool facingLeft;

  // Fire Attack Skill State
  int fireSkillState; // 0: Idle, 1: Fire Animation, 2: Fireball Traveling, 3:
                      // Gap
  int fireSkillTimer;
  int fireFrameIndex;
  int fireAttackCount;
  int fireAttackTarget;
  float fbX, fbY;
  float fbVelX, fbVelY;
  bool fbActive;
  int fbFrameIndex;
  bool charOnFire;
  int charFireTimer;

  // Jump Attack Skill State
  int jumpSkillState; // 0: Idle, 1: Pre-Jump, 2: Jump/Motion (1.7s), 3: Landing
                      // Effect (attack11-16 + flame)
  int jumpSkillTimer;
  int jumpFrameIndex;
  int jumpLandingFrame;
  float jumpStartX, jumpStartY;
  float jumpTargetX, jumpTargetY;
  bool showLandingFlame;
  int landingFlameTimer;
  float shakeX, shakeY;
  int jumpSkillCount; // Counter for 3-jump cycle

  // Pull Attack Skill State
  int pullSkillState; // 0: Idle, 1: High Shake, 2: Initial Pull Prep, 3:
                      // Pulling Phase
  int pullSkillTimer;
  int pullFrameIndex;
  int pullDanceFrame;
  float pullParticlesX[15];
  float pullParticlesY[15];
  float pullParticlesSpd[15];
  int pullSkillCount;
  int pullTargetCount;

  // Push Attack Skill State
  int pushSkillState; // 0: Idle, 1: 3 small jumps, 2: Prep motion, 3: Push
                      // execution, 4: Return
  int pushSkillTimer;
  int pushFrameIndex;
  int pushJumpCount;
  int pushCount;
  int pushTargetCount;
  float pushStartY;
  float pushTargetX;
  int pushDir;

  // Enemy Attack Skill State
  int enemySkillState; // 0: Idle, 1: Small jump, 2: High shake, 3: Prep motion,
                       // 4: Spawn, 5: Return
  int enemySkillTimer;
  int enemyFrameIndex;
  int enemySpawnTimer;
  float enemyStartY;
  BossEnemyObj bossEnemies[5];
  BatNPC bats[10];
  int batSpawnTimer;

  // Top Attack Skill State
  int topSkillState; // 0: Idle, 1: High motion, 2: Thunder motion (15s)
  int topSkillTimer;
  int topFrameIndex;
  int topSpawnTimer;
  BossTopObstacle topObstacles[10];

  // Rest Attack Skill State
  int restSkillState; // 0: Idle, 1: First motion (rest11-rest14), 2: Second
                      // motion (rest15-rest17)
  int restSkillTimer;
  int restFrameIndex;

  // Rest Phase NPCs (walking NPCs + Big NPC that protect the boss)
  WalkingNPC restNpcs[3];
  GreenFire restFires[3];
  BigNPC restBigNpc;
  int restNpcSpawnTimer;
  bool restBigNpcSpawned;

  // Hitground Skill State (Skill 8)
  int hitgroundSkillState; // 0=Idle, 1=Shout, 2=Hit start, 3=Hit loop
  int hitgroundSkillTimer;
  int hitgroundFrameIndex;
  int hitgroundLoopTimer;
  int hitgroundHoleCount;
  int hitgroundHoleTimer;
  BossHole bossHoles[4];
  HoleBird holeBirds[4];  // One bird per hole slot
  int birdDiveCooldown;   // Global cooldown (ticks) enforcing 2-sec gap between
                          // any two bird dives
  int birdHoverTicks[4];  // Per-bird random hover duration (5-7 sec)
  int hitgroundWaitTimer; // Timer for the 12 second wait phase

  // Skill Scheduler
  int currentSkill;     // -1: Choosing, 0-8: Active
  bool skillUsed[9];    // Track which skills have been used in current cycle
  int skillsCompleted;  // Count of skills finished in current cycle
  int skillGapTimer;    // Delay between skills
  int skillSequence[9]; // Shuffled array of indices 0-8
  int sequenceIndex;    // Current position in shuffled sequence
  int safeguardTimer;   // Timeout to prevent "stuck" skills

  // Smooth Flame Scaling
  float largeFlameScale; // 0.0 to 1.0
  int largeFlamePhase;   // 0: Small, 1: Growing, 2: Large, 3: Shrinking
  int recoilDir;         // 1: Move right, -1: Move left
};

SELECTANY BossLevel4 boss4Obj;
SELECTANY int level4Phase = 1;
SELECTANY int level4ScrollCount = 0;

// Positioning and Size Constants
#define BOSS_WIDTH 150
#define BOSS_HEIGHT 187
#define BOSS_FLAME_W 120 // Decreased from 150
#define BOSS_FLAME_H 80  // Decreased from 100
#define BOSS_FLAME_OFFSET_Y 25
#define BOSS_FLAME_OFFSET_X 30 // Adjusted to center narrower flame

/**
 * Loads the Level 4 Boss assets including the background images,
 * boss animation frames, and flame effects.
 */
inline void loadBossAssets() {
  static bool bossAssetsLoaded = false;
  if (bossAssetsLoaded)
    return;
  bossAssetsLoaded = true;

  char path[100];

  // Load Layered Level 4 Backgrounds
  bg41 = iLoadImage((char *)"Boss\\bg41.png");
  bg42 = iLoadImage((char *)"Boss\\bg42.png");

  // Load Boss rest state animation (rest11 to rest14)
  for (int i = 0; i < 4; i++) {
    sprintf_s(path, sizeof(path), "Boss\\rest1%d.png", i + 1);
    bossImgs[i] = iLoadImage((char *)path);
  }

  for (int i = 0; i < 8; i++) {
    sprintf_s(path, sizeof(path), "Boss\\walking (%d).png", i + 1);
    bossWalkImgs[i] = iLoadImage((char *)path);
  }

  for (int i = 0; i < 6; i++) {
    sprintf_s(path, sizeof(path), "Boss\\stand (%d).png", i + 1);
    bossStandImgs[i] = iLoadImage((char *)path);
  }

  // Load Flame effect animation (flame (1) to flame (11))
  for (int i = 0; i < 11; i++) {
    sprintf_s(path, sizeof(path), "Boss\\flame (%d).png", i + 1);
    bossFlameImgs[i] = iLoadImage((char *)path);
  }

  for (int i = 0; i < 21; i++) {
    if (i == 20) {
      sprintf_s(path, sizeof(path), "Boss\\die(%d).png", i + 1);
    } else {
      sprintf_s(path, sizeof(path), "Boss\\die (%d).png", i + 1);
    }
    bossDieImgs[i] = iLoadImage((char *)path);
  }

  for (int i = 0; i < 8; i++) {
    sprintf_s(path, sizeof(path), "Boss\\vanish (%d).png", i + 1);
    bossVanishImgs[i] = iLoadImage((char *)path);
  }

  for (int i = 0; i < 11; i++) {
    sprintf_s(path, sizeof(path), "Boss\\effect1%d.png", i + 1);
    bossAttackImgs[i] = iLoadImage((char *)path);
  }

  // Skill 2: Fire Attack Assets
  for (int i = 0; i < 10; i++) {
    sprintf_s(path, sizeof(path), "Boss\\fire (%d).png", i + 1);
    bossFireMotionImgs[i] = iLoadImage((char *)path);
    sprintf_s(path, sizeof(path), "Boss\\fireball (%d).png", i + 1);
    fireballImgs[i] = iLoadImage((char *)path);
  }

  for (int i = 0; i < 6; i++) {
    sprintf_s(path, sizeof(path), "Boss\\attack1%d.png", i + 1);
    bossPreJumpImgs[i] = iLoadImage((char *)path);
  }

  for (int i = 0; i < 8; i++) {
    sprintf_s(path, sizeof(path), "Boss\\jump (%d).png", i + 1);
    bossJumpImgs[i] = iLoadImage((char *)path);
  }

  for (int i = 0; i < 10; i++) {
    sprintf_s(path, sizeof(path), "Boss\\smash (%d).png", i + 1);
    bossSmashImgs[i] = iLoadImage((char *)path);
  }

  for (int i = 0; i < 5; i++) {
    sprintf_s(path, sizeof(path), "Boss\\pull (%d).png", i + 1);
    bossPullImgs[i] = iLoadImage((char *)path);
  }

  for (int i = 0; i < 7; i++) {
    sprintf_s(path, sizeof(path), "Boss\\pull (%d).png", i + 6);
    bossPullDanceImgs[i] = iLoadImage((char *)path);
  }

  for (int i = 0; i < 8; i++) {
    sprintf_s(path, sizeof(path), "Boss\\push%d.png", i + 11);
    bossPushImgs[i] = iLoadImage((char *)path);
  }

  for (int i = 0; i < 8; i++) {
    sprintf_s(path, sizeof(path), "Boss\\pushactive (%d).png", i + 2);
    bossPushActiveImgs[i] = iLoadImage((char *)path);
  }

  for (int i = 0; i < 9; i++) {
    sprintf_s(path, sizeof(path), "Boss\\high (%d).png", i + 1);
    bossHighImgs[i] = iLoadImage((char *)path);
  }

  for (int i = 0; i < 4; i++) {
    sprintf_s(path, sizeof(path), "Boss\\thunder (%d).png", i + 1);
    bossThunderImgs[i] = iLoadImage((char *)path);
  }

  for (int i = 0; i < 7; i++) {
    sprintf_s(path, sizeof(path), "Boss\\rest1%d.png", i + 1);
    bossRestSkillImgs[i] = iLoadImage((char *)path);
  }

  bossLifeFrameImg = iLoadImage((char *)"Boss\\showbosslife.png");

  // Big NPC Assets for Level 4 rest phase
  for (int i = 0; i < 9; i++) {
    sprintf_s(path, sizeof(path), "level4\\bignpc walk\\frame_00%d.png", i);
    bigNpcWalkImgs[i] = iLoadImage((char *)path);
    sprintf_s(path, sizeof(path), "level4\\bignpc attack\\frame_00%d.png", i);
    bigNpcAttackImgs[i] = iLoadImage((char *)path);
  }

  // Level 4 Spiders
  for (int i = 0; i < 4; i++) {
    sprintf_s(path, sizeof(path), "level4\\spider\\frame_00%d.png", i);
    spiderImgs[i] = iLoadImage((char *)path);
  }

  // Level 4 High Obstacles
  for (int i = 0; i < 4; i++) {
    sprintf_s(path, sizeof(path), "level4\\highobs1\\frame_00%d.png", i);
    highobs1Imgs[i] = iLoadImage((char *)path);
    sprintf_s(path, sizeof(path), "level4\\highobs2\\frame_00%d.png", i);
    highobs2Imgs[i] = iLoadImage((char *)path);
  }

  // Level 4 Bats
  for (int i = 0; i < 6; i++) {
    sprintf_s(path, sizeof(path), "level4\\bat\\frame_00%d.png", i);
    batImgs[i] = iLoadImage((char *)path);
  }

  // Skill 7 (Hitground) Assets
  for (int i = 0; i < 11; i++) {
    sprintf_s(path, sizeof(path), "Boss\\shout (%d).png", i + 1);
    bossShoutImgs[i] = iLoadImage((char *)path);
  }
  for (int i = 0; i < 11; i++) {
    sprintf_s(path, sizeof(path), "Boss\\hitground (%d).png", i + 1);
    bossHitgroundImgs[i] = iLoadImage((char *)path);
    // Fallback for potentially missing hitground (4).png (index 3)
    if (i == 3) {
      FILE *f;
      if (fopen_s(&f, path, "rb") == 0) {
        fclose(f);
      } else {
        bossHitgroundImgs[i] =
            bossHitgroundImgs[i - 1]; // fallback to hitground (3)
      }
    }
  }
  for (int i = 0; i < 10; i++) {
    sprintf_s(path, sizeof(path), "Boss\\hole (%d).png", i + 1);
    bossHoleImgs[i] = iLoadImage((char *)path);
  }

  // Bird images for Skill 8 (Hitground)
  for (int i = 0; i < 6; i++) {
    sprintf_s(path, sizeof(path), "vol&arti\\artikuno (%d).png", i + 1);
    artikunoImgs[i] = iLoadImage((char *)path);
    sprintf_s(path, sizeof(path), "vol&arti\\volplex (%d).png", i + 1);
    volplexImgs[i] = iLoadImage((char *)path);
  }

  for (int i = 0; i < 10; i++) {
    sprintf_s(path, sizeof(path), "Boss\\back (%d).png", i + 1);
    bossBackImgs[i] = iLoadImage((char *)path);
  }
}

/**
 * Shuffles the boss's skill sequence to ensure random but unique cycles.
 */
inline void shuffleBossSkills() {
  if (boss4Obj.skillsCompleted >= 9) {
    level4BossEverCompletedFirstSequence = true;
  }

  for (int i = 0; i < 9; i++)
    boss4Obj.skillSequence[i] = i;

  if (level4BossEverCompletedFirstSequence || boss4Obj.skillsCompleted >= 9) {
    for (int i = 8; i > 0; i--) {
      int j = rand() % (i + 1);
      int temp = boss4Obj.skillSequence[i];
      boss4Obj.skillSequence[i] = boss4Obj.skillSequence[j];
      boss4Obj.skillSequence[j] = temp;
    }
    // Hard Requirement: First attack must NOT be Rest (Index 7)
    if (boss4Obj.skillSequence[0] == 7) {
      // Swap with index 1 (which we know isn't 7 because indices are unique)
      int temp = boss4Obj.skillSequence[0];
      boss4Obj.skillSequence[0] = boss4Obj.skillSequence[1];
      boss4Obj.skillSequence[1] = temp;
    }
  } else {
    // Default sequential phase: Attack 1 to 9 (Index 0 to 8)
    for (int i = 0; i < 9; i++)
      boss4Obj.skillSequence[i] = i;
  }
  boss4Obj.sequenceIndex = 0;
}

/**
 * Initializes the Boss data at the start of Level 4 or when resetting.
 * Renamed to avoid collisions with other headers.
 */
inline void initFinalBoss() {
  memset(&boss4Obj, 0, sizeof(boss4Obj));
  boss4Obj.active = false;
  boss4Obj.x = 600.0f; // Positioned on the right side
  boss4Obj.bossInitialX = 600.0f;
  boss4Obj.y = 83.0f; // Anchored to Level 4 ground
  boss4Obj.life = 170;
  boss4Obj.maxLife = 170;
  boss4Obj.facingLeft = true;
  boss4Obj.largeFlameScale = 0.0f;
  boss4Obj.largeFlamePhase = 0;
  boss4Obj.previousLife = 170;
  boss4Obj.consecutiveHits = 0;
  boss4Obj.knockbackState = 0;
  // Pull Attack Initialized to avoid 0 speed junk
  for (int i = 0; i < 15; i++) {
    boss4Obj.pullParticlesX[i] = (float)(rand() % 1000);
    boss4Obj.pullParticlesY[i] = (float)(rand() % 500 + 50);
    boss4Obj.pullParticlesSpd[i] = (float)(rand() % 10 + 10);
  }
  boss4Obj.pullTargetCount = 2 + rand() % 2;

  // Push Attack target count
  boss4Obj.pushTargetCount = 2 + rand() % 2;

  // Fire Attack target count
  boss4Obj.fireAttackTarget = 2 + rand() % 4;

  // Scheduler Init
  boss4Obj.currentSkill = -1;
  boss4Obj.skillGapTimer = 10;
  boss4Obj.safeguardTimer = 0;
  shuffleBossSkills();

  // Reset Rest Phase NPCs
  for (int i = 0; i < 3; i++) {
    boss4Obj.restNpcs[i].active = false;
    boss4Obj.restNpcs[i].hitFlashTimer = 0;
    boss4Obj.restFires[i].active = false;
  }
  boss4Obj.restBigNpc.active = false;
  boss4Obj.restBigNpc.hitFlashTimer = 0;
  boss4Obj.restNpcSpawnTimer = 0;
  boss4Obj.restBigNpcSpawned = false;

  // Initialize Bats
  boss4Obj.batSpawnTimer = 0;
  for (int i = 0; i < 10; i++) {
    boss4Obj.bats[i].active = false;
  }
}

/**
 * Updates boss and flame animations.
 * Renamed to avoid collisions with other headers.
 */
inline void updateFinalBossLogic() {
  if (!boss4Obj.active)
    return;

  // Knockback State Interceptor (Overrides Boss Updates)
  extern int charX, charWidth;
  if (boss4Obj.life < boss4Obj.previousLife) {
    // Determine recoil direction based on character position
    float bossCenterX = boss4Obj.x + (BOSS_WIDTH / 2.0f);
    float charCenterX = (float)charX + (charWidth / 2.0f);
    boss4Obj.recoilDir = (charCenterX < bossCenterX) ? 1 : -1;

    boss4Obj.x +=
        5.0f * boss4Obj.recoilDir; // Boss goes back 5 axis dynamic direction
    boss4Obj.consecutiveHits++;
    if (boss4Obj.consecutiveHits >= 3) {
      boss4Obj.knockbackState = 1;
      boss4Obj.knockbackTimer = 0;
      boss4Obj.knockbackFrameIndex = 0;
      boss4Obj.consecutiveHits = 0; // reset
    }
  }
  boss4Obj.previousLife = boss4Obj.life;

  if (boss4Obj.knockbackState == 1) {
    boss4Obj.knockbackTimer++;
    boss4Obj.x += 0.5f * boss4Obj.recoilDir; // Pull boss backward an additional
                                             // 30 axis over 60 ticks

    // Smooth 10 frame animation covering 60 ticks (2 seconds)
    boss4Obj.knockbackFrameIndex = boss4Obj.knockbackTimer / 6;
    if (boss4Obj.knockbackFrameIndex > 9)
      boss4Obj.knockbackFrameIndex = 9;

    if (boss4Obj.knockbackTimer >= 60) {
      boss4Obj.knockbackState = 0;
    }
  }

  // Boss returns to regular place if shifted away
  if (boss4Obj.active && boss4Obj.knockbackState == 0) {
    if (boss4Obj.x > boss4Obj.bossInitialX + 5.0f) {
      boss4Obj.x -= 1.5f; // Gradual return speed left
    } else if (boss4Obj.x < boss4Obj.bossInitialX - 5.0f) {
      boss4Obj.x += 1.5f; // Gradual return speed right
    }
  }

  extern int level4Phase;

  // Boss breathing/resting animation speed
  boss4Obj.animCounter++;

  int animLimit = 10;
  if (level4Phase == 2)
    animLimit = 3; // Faster walk anim

  if (boss4Obj.animCounter >= animLimit) {
    boss4Obj.animCounter = 0;
    if (level4Phase == 2) {
      boss4Obj.frameIndex = (boss4Obj.frameIndex + 1) % 8;
      boss4Obj.facingLeft = true;
    } else if (level4Phase == 3) {
      boss4Obj.frameIndex =
          (boss4Obj.frameIndex + 1) % 6; // bossStandImgs has 6
    } else {
      boss4Obj.frameIndex = (boss4Obj.frameIndex + 1) % 4; // bossImgs has 4
    }
  }

  // Flame flicker animation speed (using all 11 frames)
  boss4Obj.flameAnimCounter++;
  if (boss4Obj.flameAnimCounter >= 5) { // Adjust speed as needed
    boss4Obj.flameAnimCounter = 0;
    boss4Obj.flameFrameIndex =
        (boss4Obj.flameFrameIndex + 1) % 11; // Loop all 11 frames
  }

  if (boss4Obj.isHit) {
    boss4Obj.hitTimer--;
    if (boss4Obj.hitTimer <= 0)
      boss4Obj.isHit = false;
  }

  if (level4Phase == 4) {
    boss4Obj.dieAnimCounter++;
    if (boss4Obj.dieAnimCounter >= 4) { // Faster animation for dying sequence
      boss4Obj.dieAnimCounter = 0;
      // Bound at 20 (image 21)
      if (boss4Obj.dieFrameIndex < 20) {
        boss4Obj.dieFrameIndex++;
      } else {
        boss4Obj.dieFrameIndex = 20; // Stay on final frame

        // Use struct member for victory delay to ensure clean state
        boss4Obj.victoryDelay++;
        if (boss4Obj.victoryDelay >= 30) {
          level4Phase = 5; // Victory screen state
          boss4Obj.victoryDelay = 0;
        }
      }
    }
    return; // Only dying animation plays in Phase 4
  }

  if (level4Phase == 3) {
    // Increment safeguard only when a skill is active
    if (boss4Obj.currentSkill != -1) {
      boss4Obj.safeguardTimer++;
      // If any skill takes more than 35 seconds, FORCE it to stop
      if (boss4Obj.safeguardTimer > 1150 && boss4Obj.currentSkill != 8) {
        boss4Obj.currentSkill = -1;
        boss4Obj.skillGapTimer = 30;
        boss4Obj.safeguardTimer = 0;
      }
    }

    // Only allow picking a new skill if level4Phase == 3 (true fight) AND the
    // current one is truly finished (-1)
    if (level4Phase == 3 && boss4Obj.currentSkill == -1) {
      boss4Obj.skillGapTimer--;
      if (boss4Obj.skillGapTimer <= 0) {
        // Reset safeguard for the new skill
        boss4Obj.safeguardTimer = 0;

        // If we finished the current shuffled sequence, shuffle again
        if (boss4Obj.sequenceIndex >= 9) {
          shuffleBossSkills();
        }

        int pick = boss4Obj.skillSequence[boss4Obj.sequenceIndex++];
        boss4Obj.currentSkill = pick;

        // --- NUCLEAR RESET: Ensure NO other skill states are active ---
        boss4Obj.skillState = 0;
        boss4Obj.skillTimer = 0;
        boss4Obj.skillFrameIndex = 0;
        boss4Obj.jumpSkillState = 0;
        boss4Obj.pullSkillState = 0;
        boss4Obj.pushSkillState = 0;
        boss4Obj.enemySkillState = 0;
        boss4Obj.topSkillState = 0;
        boss4Obj.fireSkillState = 0;
        boss4Obj.restSkillState = 0;
        boss4Obj.fbActive = false;
        boss4Obj.showLandingFlame = false;
        boss4Obj.shakeX = 0;
        boss4Obj.shakeY = 0;

        // --- Activate specific state for the chosen skill ---
        if (pick == 0) {
          boss4Obj.skillState = 1;
          boss4Obj.skillCount = 0;
          boss4Obj.skillFrameIndex = 0;
        } else if (pick == 1) {
          boss4Obj.jumpSkillState = 1;
          boss4Obj.jumpSkillTimer = 0;
          boss4Obj.jumpSkillCount = 0;
        } else if (pick == 2) {
          boss4Obj.pullSkillState = 1;
          boss4Obj.pullSkillTimer = 0;
          boss4Obj.pullSkillCount = 0;
        } else if (pick == 3) {
          boss4Obj.pushSkillState = 1;
          boss4Obj.pushSkillTimer = 0;
          boss4Obj.pushJumpCount = 0;
          boss4Obj.pushCount = 0;
          boss4Obj.pushStartY = boss4Obj.y;
        } else if (pick == 4) {
          boss4Obj.enemySkillState = 1;
          boss4Obj.enemySkillTimer = 0;
          boss4Obj.enemyStartY = boss4Obj.y;
        } else if (pick == 5) {
          boss4Obj.topSkillState = 1;
          boss4Obj.topSkillTimer = 0;
        } else if (pick == 6) {
          boss4Obj.fireSkillState = 1;
          boss4Obj.fireAttackCount = 0;
          boss4Obj.fireSkillTimer = 0;
          boss4Obj.fireAttackTarget = 2 + rand() % 4;
        } else if (pick == 7) {
          boss4Obj.restSkillState = 1;
          boss4Obj.restSkillTimer = 0;
          boss4Obj.restFrameIndex = 0;
        } else if (pick == 8) {
          boss4Obj.hitgroundSkillState = 1;
          boss4Obj.hitgroundSkillTimer = 0;
          boss4Obj.hitgroundFrameIndex = 0;
          boss4Obj.hitgroundHoleCount = 0;
        }
      }
    }
  }

  // --- SKILL 0: VANISH ATTACK LOGIC ---
  if (level4Phase == 3 && boss4Obj.currentSkill == 0) {
    extern int charX, charY, lives, charWidth, charHeight;
    extern bool isInvincible;
    extern int invincibilityTimer;
    extern GameState gameState;

    if (boss4Obj.skillState == 1) { // Vanishing
      boss4Obj.skillTimer++;
      if (boss4Obj.skillTimer % 3 == 0) { // Faster vanishing (was 5)
        boss4Obj.skillFrameIndex++;
        if (boss4Obj.skillFrameIndex >= 8) {
          boss4Obj.skillState = 2; // Hidden
          boss4Obj.skillTimer = 0;
          boss4Obj.x = -2000; // Move far off
        }
      }
    } else if (boss4Obj.skillState == 2) { // Hidden
      boss4Obj.skillTimer++;
      if (boss4Obj.skillTimer > 30) { // Slightly faster hidden phase (was 50)
        boss4Obj.skillState = 3;      // Appearing
        boss4Obj.skillTimer = 0;
        boss4Obj.skillFrameIndex = 7; // Start from last frame backwards
        boss4Obj.x = (float)charX - 50;
        boss4Obj.y = 83.0f;
        boss4Obj.largeFlamePhase = 1; // Start GROWING flame when appearing
        // Face the character when reappearing
        boss4Obj.facingLeft = (charX <= (int)boss4Obj.x);
      }
    } else if (boss4Obj.skillState == 3) { // Appearing
      boss4Obj.skillTimer++;
      // Keep facing the character while materialising
      boss4Obj.facingLeft = (charX <= (int)boss4Obj.x);
      if (boss4Obj.skillTimer % 2 == 0) { // Much faster appearing
        boss4Obj.skillFrameIndex--;
        if (boss4Obj.skillFrameIndex < 0) {
          boss4Obj.skillFrameIndex = 0;
          boss4Obj.skillState = 4; // Smash Attack
          boss4Obj.skillTimer = 0;
        }
      }
    } else if (boss4Obj.skillState == 4) { // Smash Attack at character site
      boss4Obj.skillTimer++;
      // Face the character throughout the smash
      boss4Obj.facingLeft = (charX <= (int)boss4Obj.x);

      // Smash Animation using skillFrameIndex (0-9)
      if (boss4Obj.skillTimer % 3 == 0) { // Faster smash attack
        boss4Obj.skillFrameIndex++;
      }

      // Damage collision check (similar to Jump Attack smash)
      float smashCenterX = boss4Obj.x + BOSS_WIDTH / 2.0f;
      float totalWidth = BOSS_WIDTH + 40.0f;
      if (charX + charWidth - 40 > (smashCenterX - totalWidth / 2.0f) &&
          charX + 40 < (smashCenterX + totalWidth / 2.0f) &&
          charY + charHeight > boss4Obj.y && charY < boss4Obj.y + BOSS_HEIGHT) {
        if (!isInvincible && charAttackDamagelessTimer <= 0) {
          lives--;
          if (lives <= 0) {
            lives = 0;
            gameState = GAME_OVER;
          } else {
            isInvincible = true;
            invincibilityTimer = 60;
          }
        }
      }

      if (boss4Obj.skillFrameIndex >= 10) {
        boss4Obj.skillCount++;
        if (boss4Obj.skillCount < 3) {
          boss4Obj.skillState = 1; // Repeat Vanish (Vanishes from current spot)
          boss4Obj.skillFrameIndex = 0;
          boss4Obj.skillTimer = 0;
          boss4Obj.largeFlameScale =
              0.0f; // Reset scale for next appearance smooth growth
          boss4Obj.largeFlamePhase =
              0; // Reset phase for next appearance growth transition
        } else {
          boss4Obj.skillState = 5; // Final Return to base
          boss4Obj.skillTimer = 0;
          boss4Obj.skillFrameIndex = 0;
          boss4Obj.largeFlamePhase = 3; // SHRINK flame when finishing attack
        }
      }
    } else if (boss4Obj.skillState == 5) { // Return
      boss4Obj.skillTimer++;
      float returnSpeed = 6.0f;
      if (boss4Obj.x < 750) {
        boss4Obj.x += returnSpeed;
        boss4Obj.facingLeft = false;
        if (boss4Obj.x > 750)
          boss4Obj.x = 750;
      } else if (boss4Obj.x > 750) {
        boss4Obj.x -= returnSpeed;
        boss4Obj.facingLeft = true;
        if (boss4Obj.x < 750)
          boss4Obj.x = 750;
      }

      if (boss4Obj.skillTimer % 6 == 0) {
        boss4Obj.frameIndex = (boss4Obj.frameIndex + 1) % 8;
      }

      if (abs(boss4Obj.x - 750) < 13) {
        boss4Obj.x = 750;
        boss4Obj.skillState = 0;
        boss4Obj.currentSkill = -1;
        boss4Obj.skillsCompleted++;
        boss4Obj.skillGapTimer = 60;
        boss4Obj.facingLeft = true;
      }
    }
  }

  // --- SKILL 1: JUMP ATTACK SKILL LOGIC ---
  if (level4Phase == 3 && boss4Obj.currentSkill == 1) {
    extern int charX, charY, lives;
    extern bool isInvincible;
    extern int invincibilityTimer;
    extern GameState gameState;

    if (boss4Obj.jumpSkillState == 1) {
      // State 1: Earthquake Shake & Rise Flame
      boss4Obj.jumpSkillTimer++;
      boss4Obj.shakeX = (float)(rand() % 10 - 5);
      boss4Obj.shakeY = (float)(rand() % 10 - 5);

      if (boss4Obj.jumpSkillTimer >= 3) { // Faster prep
        boss4Obj.jumpSkillTimer = 0;
        boss4Obj.jumpFrameIndex++;
        if (boss4Obj.jumpFrameIndex >= 6) {
          boss4Obj.jumpSkillState = 2; // Jump motion
          boss4Obj.jumpFrameIndex = 0;
          boss4Obj.jumpStartX = boss4Obj.x;
          boss4Obj.jumpStartY = boss4Obj.y;
          boss4Obj.jumpTargetX = (float)charX - 50;
          boss4Obj.jumpTargetY = 83.0f;
          boss4Obj.jumpSkillTimer = 0;
          boss4Obj.shakeX = 0;
          boss4Obj.shakeY = 0;
        }
      }
    } else if (boss4Obj.jumpSkillState == 2) {
      // State 2: Parabolic Jump
      boss4Obj.jumpSkillTimer++;
      float dur = 50.0f; // Smooth jump
      float t = boss4Obj.jumpSkillTimer / dur;

      // Face the character at all times while airborne
      boss4Obj.facingLeft = (charX <= (int)boss4Obj.x);

      if (t <= 1.0f) {
        boss4Obj.x = boss4Obj.jumpStartX +
                     (boss4Obj.jumpTargetX - boss4Obj.jumpStartX) * t;
        boss4Obj.y = boss4Obj.jumpStartY + (float)sin(3.14159 * t) * 250.0f;
        if (boss4Obj.y < boss4Obj.jumpTargetY && t > 0.5f)
          boss4Obj.y = boss4Obj.jumpTargetY;

        boss4Obj.jumpFrameIndex = (int)(t * 8);
        if (boss4Obj.jumpFrameIndex > 7)
          boss4Obj.jumpFrameIndex = 7;
      } else {
        boss4Obj.y = boss4Obj.jumpTargetY;
        boss4Obj.jumpSkillState = 3; // Smash motion
        boss4Obj.jumpSkillTimer = 0;
        boss4Obj.jumpLandingFrame = 0;
        boss4Obj.landingFlameTimer = 40; // Faster landing (was 60)
        boss4Obj.shakeX = 15;
        boss4Obj.shakeY = 15;
        boss4Obj.largeFlamePhase = 1; // GROW flame on landing
      }
    } else if (boss4Obj.jumpSkillState == 3) {
      // State 3: SMASH Motion
      boss4Obj.jumpSkillTimer++;

      if (boss4Obj.jumpSkillTimer >= 3) { // Faster smash anim
        boss4Obj.jumpSkillTimer = 0;
        boss4Obj.jumpLandingFrame++;
        if (boss4Obj.jumpLandingFrame >= 10)
          boss4Obj.jumpLandingFrame = 9;
      }

      boss4Obj.shakeX *= 0.85f;
      boss4Obj.shakeY *= 0.85f;

      boss4Obj.landingFlameTimer--;

      float smashCenterX = boss4Obj.x + BOSS_WIDTH / 2.0f;
      float totalWidth = BOSS_WIDTH + 40.0f; // Matches boss body more closely

      if (charX + 60 > (smashCenterX - totalWidth / 2.0f) &&
          charX < (smashCenterX + totalWidth / 2.0f)) {
        if (!isInvincible && charAttackDamagelessTimer <= 0) {
          lives--;
          if (lives <= 0) {
            lives = 0;
            gameState = GAME_OVER;
          } else {
            isInvincible = true;
            invincibilityTimer = 60;
          }
        }
      }

      if (boss4Obj.landingFlameTimer <= 0) {
        boss4Obj.shakeX = 0;
        boss4Obj.shakeY = 0;
        boss4Obj.jumpSkillCount++;

        if (boss4Obj.jumpSkillCount >= 3) {
          boss4Obj.jumpSkillState = 4; // Return state
        } else {
          boss4Obj.jumpSkillState = 1; // Next jump
          boss4Obj.jumpFrameIndex = 0;
          boss4Obj.jumpSkillTimer = 0;
          boss4Obj.largeFlamePhase = 3; // SHRINK flame before next jump
        }
      }
    } else if (boss4Obj.jumpSkillState == 4) {
      // State 4: Return
      boss4Obj.jumpSkillTimer++;
      float returnSpeed = 6.0f;
      if (boss4Obj.x < 750) {
        boss4Obj.x += returnSpeed;
        boss4Obj.facingLeft = false;
        if (boss4Obj.x > 750)
          boss4Obj.x = 750;
      } else if (boss4Obj.x > 750) {
        boss4Obj.x -= returnSpeed;
        boss4Obj.facingLeft = true;
        if (boss4Obj.x < 750)
          boss4Obj.x = 750;
      }

      if (boss4Obj.jumpSkillTimer >= 5) {
        boss4Obj.jumpSkillTimer = 0;
        boss4Obj.frameIndex = (boss4Obj.frameIndex + 1) % 8;
      }

      if (boss4Obj.x == 750) {
        boss4Obj.jumpSkillState = 0;
        boss4Obj.jumpSkillCount = 0;
        boss4Obj.jumpSkillTimer = 0;
        boss4Obj.facingLeft = true;
        boss4Obj.currentSkill = -1;
        boss4Obj.skillsCompleted++;
        boss4Obj.skillGapTimer = 60;
      }
    }
  }

  // --- SKILL 2: PULL ATTACK SKILL LOGIC ---
  if (level4Phase == 3 && boss4Obj.currentSkill == 2) {
    extern int charX, charY, lives;
    extern bool isInvincible;
    extern int invincibilityTimer;
    extern GameState gameState;

    if (boss4Obj.pullSkillState == 1) {
      // State 1: High Shake
      boss4Obj.pullSkillTimer++;
      boss4Obj.shakeX = (float)(rand() % 30 - 15);
      boss4Obj.shakeY = (float)(rand() % 30 - 15);

      if (boss4Obj.pullSkillTimer > 33) {
        boss4Obj.pullSkillState = 2; // Prep
        boss4Obj.pullSkillTimer = 0;
        boss4Obj.pullFrameIndex = 0;
        boss4Obj.shakeX = 0;
        boss4Obj.shakeY = 0;
      }
    } else if (boss4Obj.pullSkillState == 2) {
      // State 2: Pull Prep
      boss4Obj.pullSkillTimer++;
      if (boss4Obj.pullSkillTimer >= 6) {
        boss4Obj.pullSkillTimer = 0;
        boss4Obj.pullFrameIndex++;
        if (boss4Obj.pullFrameIndex >= 5) {
          boss4Obj.pullSkillState = 3; // Pulling
          boss4Obj.pullSkillTimer = 0;
          boss4Obj.pullDanceFrame = 0;
        }
      }
    } else if (boss4Obj.pullSkillState == 3) {
      // State 3: Pulling Phase
      boss4Obj.pullSkillTimer++;
      if (boss4Obj.pullSkillTimer % 5 == 0) {
        boss4Obj.pullDanceFrame = (boss4Obj.pullDanceFrame + 1) % 7;
      }

      if (charX < boss4Obj.x + 50) {
        charX += 12;
      }

      for (int i = 0; i < 15; i++) {
        boss4Obj.pullParticlesX[i] += boss4Obj.pullParticlesSpd[i];
        if (boss4Obj.pullParticlesX[i] > 1000) {
          boss4Obj.pullParticlesX[i] = -50;
          boss4Obj.pullParticlesY[i] = (float)(rand() % 500 + 50);
        }
      }

      if (charX + 40 > boss4Obj.x && charX < boss4Obj.x + BOSS_WIDTH - 40 &&
          charY + 120 > boss4Obj.y && charY < boss4Obj.y + BOSS_HEIGHT) {
        if (!isInvincible && charAttackDamagelessTimer <= 0) {
          lives--;
          if (lives <= 0) {
            lives = 0;
            gameState = GAME_OVER;
          } else {
            isInvincible = true;
            invincibilityTimer = 60;
          }
        }
      }

      if (boss4Obj.pullSkillTimer > 200) {
        boss4Obj.pullSkillState = 0;
        boss4Obj.pullSkillTimer = 0;
        boss4Obj.pullSkillCount++;

        if (boss4Obj.pullSkillCount >= boss4Obj.pullTargetCount) {
          boss4Obj.pullSkillCount = 0;
          boss4Obj.pullTargetCount = 2 + rand() % 2;
          boss4Obj.currentSkill = -1;
          boss4Obj.skillsCompleted++;
          boss4Obj.skillGapTimer = 60;
        } else {
          boss4Obj.pullSkillState = 1; // Repeat cycle
        }
      }
    }
  }

  // --- SKILL 3: PUSH ATTACK SKILL LOGIC ---
  if (level4Phase == 3 && boss4Obj.currentSkill == 3) {
    extern int charX, charY, lives;
    extern bool isInvincible;
    extern int invincibilityTimer;
    extern GameState gameState;

    if (boss4Obj.pushSkillState == 1) {
      // State 1: 3 small jumps
      boss4Obj.pushSkillTimer++;
      int cycle = boss4Obj.pushSkillTimer % 10;
      if (cycle < 5)
        boss4Obj.y += 4.0f;
      else
        boss4Obj.y -= 4.0f;

      if (boss4Obj.pushSkillTimer > 0 && boss4Obj.pushSkillTimer % 10 == 0) {
        boss4Obj.pushJumpCount++;
      }

      if (boss4Obj.pushJumpCount >= 3) {
        boss4Obj.pushSkillState = 2; // Prep
        boss4Obj.pushSkillTimer = 0;
        boss4Obj.pushFrameIndex = 0;
        boss4Obj.y = boss4Obj.pushStartY;
      }
    } else if (boss4Obj.pushSkillState == 2) {
      // State 2: Prep motion
      boss4Obj.pushSkillTimer++;
      if (boss4Obj.pushSkillTimer >= 5) {
        boss4Obj.pushSkillTimer = 0;
        boss4Obj.pushFrameIndex++;
        if (boss4Obj.pushFrameIndex >= 6) {
          boss4Obj.pushSkillState = 3; // Executing
          boss4Obj.pushSkillTimer = 0;
          boss4Obj.pushFrameIndex = 6;

          if (charX > boss4Obj.x) {
            boss4Obj.pushDir = 1;
            boss4Obj.pushTargetX = charX + 30.0f;
            boss4Obj.facingLeft = false;
          } else {
            boss4Obj.pushDir = -1;
            boss4Obj.pushTargetX = charX - 30.0f;
            boss4Obj.facingLeft = true;
          }
        }
      }
    } else if (boss4Obj.pushSkillState == 3) {
      // State 3: Pushing
      boss4Obj.pushSkillTimer++;
      if (boss4Obj.pushSkillTimer % 4 == 0) {
        boss4Obj.pushFrameIndex = (boss4Obj.pushFrameIndex == 6) ? 7 : 6;
      }
      boss4Obj.x += boss4Obj.pushDir * 18.0f;

      if (charY < boss4Obj.pushStartY + BOSS_HEIGHT - 30) {
        if (boss4Obj.pushDir == 1) {
          if (charX < boss4Obj.x + BOSS_WIDTH && charX + 50 > boss4Obj.x)
            charX = boss4Obj.x + BOSS_WIDTH;
        } else {
          if (charX + 50 > boss4Obj.x && charX < boss4Obj.x + BOSS_WIDTH)
            charX = boss4Obj.x - 50;
        }
      }

      bool reachedTarget = false;
      if (boss4Obj.pushDir == 1 && boss4Obj.x >= boss4Obj.pushTargetX)
        reachedTarget = true;
      if (boss4Obj.pushDir == -1 && boss4Obj.x <= boss4Obj.pushTargetX)
        reachedTarget = true;

      if (reachedTarget) {
        bool escaped = false;
        if (boss4Obj.pushDir == 1 && charX < boss4Obj.x)
          escaped = true;
        if (boss4Obj.pushDir == -1 && charX > boss4Obj.x)
          escaped = true;

        if (!escaped && !isInvincible) {
          lives--;
          if (lives <= 0) {
            lives = 0;
            gameState = GAME_OVER;
          } else {
            isInvincible = true;
            invincibilityTimer = 60;
          }
        }

        boss4Obj.pushSkillState = 5; // Transition to pushactive action
        boss4Obj.pushSkillTimer = 0;
        boss4Obj.pushFrameIndex = 0;
      }
    } else if (boss4Obj.pushSkillState == 5) {
      // State 5: Push Action
      boss4Obj.pushSkillTimer++;
      if (boss4Obj.pushSkillTimer % 4 == 0) {
        boss4Obj.pushFrameIndex++;
        if (boss4Obj.pushFrameIndex >= 8) {
          // Action complete
          boss4Obj.pushCount++;
          if (boss4Obj.pushCount >= boss4Obj.pushTargetCount) {
            boss4Obj.pushSkillState = 4; // Return
          } else {
            boss4Obj.pushSkillState = 1; // Repeat
            boss4Obj.pushSkillTimer = 0;
            boss4Obj.pushJumpCount = 0;
          }
          boss4Obj.pushSkillTimer = 0;
        }
      }
    } else if (boss4Obj.pushSkillState == 4) {
      // State 4: Return
      boss4Obj.pushSkillTimer++;
      float returnSpeed = 6.0f;
      if (boss4Obj.x < 750) {
        boss4Obj.x += returnSpeed;
        boss4Obj.facingLeft = false;
        if (boss4Obj.x > 750)
          boss4Obj.x = 750;
      } else if (boss4Obj.x > 750) {
        boss4Obj.x -= returnSpeed;
        boss4Obj.facingLeft = true;
        if (boss4Obj.x < 750)
          boss4Obj.x = 750;
      }

      if (boss4Obj.pushSkillTimer >= 5) {
        boss4Obj.pushSkillTimer = 0;
        boss4Obj.frameIndex = (boss4Obj.frameIndex + 1) % 8;
      }

      if (boss4Obj.x == 750) {
        boss4Obj.pushSkillState = 0;
        boss4Obj.pushCount = 0;
        boss4Obj.pushSkillTimer = 0;
        boss4Obj.facingLeft = true;
        boss4Obj.pushTargetCount = 2 + rand() % 2;
        boss4Obj.currentSkill = -1;
        boss4Obj.skillsCompleted++;
        boss4Obj.skillGapTimer = 60;
      }
    }
  }

  // --- SKILL 4: ENEMY ATTACK SKILL LOGIC ---
  if (level4Phase == 3 && boss4Obj.currentSkill == 4) {
    extern int charX, charY, lives, charWidth, charHeight, attackFrameIndex;
    extern bool isInvincible, isAttacking, npcSlashDone;
    extern int invincibilityTimer, charAttackDamagelessTimer;
    extern GameState gameState;
    extern bool isLeftArrowPressed;
    extern unsigned int specialKeyPressed[512];

    // --- SHARED ENEMY SPAWN/UPDATE LOGIC ---
    // Spawn starts immediately when the skill starts (States 1-4)
    if (boss4Obj.enemySkillState >= 1 && boss4Obj.enemySkillState <= 4) {
      boss4Obj.enemySpawnTimer++;
      if (boss4Obj.enemySpawnTimer > 20) {
        boss4Obj.enemySpawnTimer = 0;
        for (int i = 0; i < 5; i++) {
          if (!boss4Obj.bossEnemies[i].active) {
            boss4Obj.bossEnemies[i].active = true;
            boss4Obj.bossEnemies[i].x = 1050;
            boss4Obj.bossEnemies[i].y = 50;
            boss4Obj.bossEnemies[i].type = rand() % 2;
            boss4Obj.bossEnemies[i].frame = 0;
            boss4Obj.bossEnemies[i].frameTimer = 0;
            break;
          }
        }
      }

      // Bat Spawn
      boss4Obj.batSpawnTimer++;
      if (boss4Obj.batSpawnTimer > 40) {
        boss4Obj.batSpawnTimer = 0;
        for (int i = 0; i < 10; i++) {
          if (!boss4Obj.bats[i].active) {
            boss4Obj.bats[i].active = true;
            boss4Obj.bats[i].life = 2;
            boss4Obj.bats[i].hitFlashTimer = 0;
            boss4Obj.bats[i].frame = 0;
            boss4Obj.bats[i].frameTimer = 0;
            boss4Obj.bats[i].hitFlashTimer = 0;

            int side = rand() % 3; // 0: Left, 1: Right, 2: Top
            if (side == 0) {
              boss4Obj.bats[i].x = -100;
              boss4Obj.bats[i].y = (float)(rand() % 400 + 150);
            } else if (side == 1) {
              boss4Obj.bats[i].x = 1100;
              boss4Obj.bats[i].y = (float)(rand() % 400 + 150);
            } else {
              boss4Obj.bats[i].x = (float)(rand() % 1000);
              boss4Obj.bats[i].y = 700;
            }
            break;
          }
        }
      }
    }

    bool anyEnemyActive = false;
    // Update Spiders
    for (int i = 0; i < 5; i++) {
      if (boss4Obj.bossEnemies[i].active) {
        anyEnemyActive = true;
        boss4Obj.bossEnemies[i].x -= 7.0f;
        boss4Obj.bossEnemies[i].frameTimer++;
        if (boss4Obj.bossEnemies[i].frameTimer >= 5) {
          boss4Obj.bossEnemies[i].frameTimer = 0;
          boss4Obj.bossEnemies[i].frame =
              (boss4Obj.bossEnemies[i].frame + 1) % 4;
        }
        if (boss4Obj.bossEnemies[i].x < -150)
          boss4Obj.bossEnemies[i].active = false;

        if (boss4Obj.bossEnemies[i].active &&
            boss4Obj.bossEnemies[i].x < charX + 80 &&
            boss4Obj.bossEnemies[i].x + 100 > charX &&
            boss4Obj.bossEnemies[i].y < charY + 120 &&
            boss4Obj.bossEnemies[i].y + 40 > charY) {
          if (isAttacking)
            boss4Obj.bossEnemies[i].active = false;
          else if (!isInvincible) {
            lives--;
            if (lives <= 0) {
              lives = 0;
              gameState = GAME_OVER;
            } else {
              isInvincible = true;
              invincibilityTimer = 60;
            }
            boss4Obj.bossEnemies[i].active = false;
          }
        }
      }
    }

    // Update Bats
    for (int i = 0; i < 10; i++) {
      if (boss4Obj.bats[i].active) {
        anyEnemyActive = true;

        // Animation
        boss4Obj.bats[i].frameTimer++;
        if (boss4Obj.bats[i].frameTimer >= 4) {
          boss4Obj.bats[i].frameTimer = 0;
          boss4Obj.bats[i].frame = (boss4Obj.bats[i].frame + 1) % 6;
        }

        // Fly towards character (homing)
        float dx = (float)charX + 40.0f - boss4Obj.bats[i].x;
        float dy = (float)charY + 60.0f - boss4Obj.bats[i].y;
        float dist = (float)sqrt(dx * dx + dy * dy);
        float speed = 6.5f;
        if (dist > 1.0f) {
          boss4Obj.bats[i].velX = (dx / dist) * speed;
          boss4Obj.bats[i].velY = (dy / dist) * speed;
        }
        boss4Obj.bats[i].x += boss4Obj.bats[i].velX;
        boss4Obj.bats[i].y += boss4Obj.bats[i].velY;
        boss4Obj.bats[i].facingLeft = (boss4Obj.bats[i].velX < 0);

        // Bounds check
        if (boss4Obj.bats[i].x < -300 || boss4Obj.bats[i].x > 1300 ||
            boss4Obj.bats[i].y < -300 || boss4Obj.bats[i].y > 900) {
          boss4Obj.bats[i].active = false;
        }

        if (boss4Obj.bats[i].hitFlashTimer > 0)
          boss4Obj.bats[i].hitFlashTimer--;

        // Damage character
        if (boss4Obj.bats[i].active && boss4Obj.bats[i].x + 60 > charX &&
            boss4Obj.bats[i].x < charX + 80 && boss4Obj.bats[i].y + 60 > charY &&
            boss4Obj.bats[i].y < charY + 120) {
          if (!isInvincible && charAttackDamagelessTimer <= 0) {
            lives--;
            if (lives <= 0) {
              lives = 0;
              gameState = GAME_OVER;
            } else {
              isInvincible = true;
              invincibilityTimer = 60;
            }
            boss4Obj.bats[i].active = false;
          }
        }

        // Destroyed by player attack
        if (boss4Obj.bats[i].active && isAttacking && attackFrameIndex >= 1 &&
            attackFrameIndex <= 4 && !npcSlashDone) {
          float batCX = boss4Obj.bats[i].x;
          float batCY = boss4Obj.bats[i].y;
          bool facingBack =
              isLeftArrowPressed || (specialKeyPressed[GLUT_KEY_LEFT] != 0);
          float reachMin, reachMax;
          if (facingBack) {
            reachMin = (float)charX - 160;
            reachMax = (float)charX + 40;
          } else {
            reachMin = (float)charX + charWidth - 40;
            reachMax = (float)charX + charWidth + 160;
          }

          // Broadened hit vertical check (charY - 100 to charY + 250)
          if (batCX > reachMin && batCX < reachMax && batCY > charY - 100 &&
              batCY < charY + 250) {
            boss4Obj.bats[i].life--;
            boss4Obj.bats[i].hitFlashTimer = 15;
            if (boss4Obj.bats[i].life <= 0) {
              boss4Obj.bats[i].active = false;
            }
            npcSlashDone = true;
          }
        }
      }
    }

    // --- STATE LOGIC ---
    if (boss4Obj.enemySkillState == 1) {
      boss4Obj.enemySkillTimer++;
      if (boss4Obj.enemySkillTimer < 10)
        boss4Obj.y += 4.0f;
      else
        boss4Obj.y -= 4.0f;

      if (boss4Obj.enemySkillTimer >= 20) {
        boss4Obj.enemySkillState = 2; // high shake
        boss4Obj.enemySkillTimer = 0;
        boss4Obj.y = boss4Obj.enemyStartY;
        boss4Obj.shakeX = 0;
        boss4Obj.shakeY = 0;
      }
    } else if (boss4Obj.enemySkillState == 2) {
      boss4Obj.enemySkillTimer++;
      boss4Obj.shakeX = (float)(rand() % 40 - 20);
      boss4Obj.shakeY = (float)(rand() % 40 - 20);

      if (boss4Obj.enemySkillTimer > 30) {
        boss4Obj.enemySkillState = 3; // prep
        boss4Obj.enemySkillTimer = 0;
        boss4Obj.enemyFrameIndex = 0;
        boss4Obj.shakeX = 0;
        boss4Obj.shakeY = 0;
      }
    } else if (boss4Obj.enemySkillState == 3) {
      boss4Obj.enemySkillTimer++;
      if (boss4Obj.enemySkillTimer >= 5) {
        boss4Obj.enemySkillTimer = 0;
        boss4Obj.enemyFrameIndex++;
        if (boss4Obj.enemyFrameIndex >= 6) {
          boss4Obj.enemySkillState = 4; // Main Spawning State
          boss4Obj.enemySkillTimer = 0;
          boss4Obj.enemyFrameIndex = 0;
        }
      }
    } else if (boss4Obj.enemySkillState == 4) {
      boss4Obj.enemySkillTimer++;
      if (boss4Obj.enemySkillTimer % 5 == 0)
        boss4Obj.enemyFrameIndex = (boss4Obj.enemyFrameIndex + 1) % 6;
      if (boss4Obj.enemySkillTimer > 400) { // Slightly faster skill duration
        boss4Obj.enemySkillState = 5;       // Wait for enemies screen-clear
      }
    } else if (boss4Obj.enemySkillState == 5) {
      if (!anyEnemyActive) {
        boss4Obj.enemySkillState = 0;
        boss4Obj.enemySkillTimer = 0;
        boss4Obj.currentSkill = -1;
        boss4Obj.skillsCompleted++;
        boss4Obj.skillGapTimer = 60;
      }
    }
  }

  // --- SKILL 5: TOP ATTACK SKILL LOGIC ---
  if (level4Phase == 3 && boss4Obj.currentSkill == 5) {
    extern int charX, charY, lives;
    extern bool isInvincible;
    extern int invincibilityTimer;
    extern GameState gameState;

    // --- SHARED TOP SPAWN/UPDATE LOGIC ---
    // Spawn starts immediately when the skill starts (States 1-2)
    if (boss4Obj.topSkillState == 1 || boss4Obj.topSkillState == 2) {
      boss4Obj.topSpawnTimer++;
      if (boss4Obj.topSpawnTimer > 30) {
        boss4Obj.topSpawnTimer = 0;
        for (int i = 0; i < 10; i++) {
          if (!boss4Obj.topObstacles[i].active) {
            boss4Obj.topObstacles[i].active = true;
            boss4Obj.topObstacles[i].x = charX + 60.0f + (rand() % 400 - 200);
            boss4Obj.topObstacles[i].y = 650.0f;
            boss4Obj.topObstacles[i].speedY =
                (float)((rand() % 4) +
                        11); // Speed: 11-14 (Decreased from 14-19)
            boss4Obj.topObstacles[i].frame = 0;
            boss4Obj.topObstacles[i].frameTimer = 0;
            boss4Obj.topObstacles[i].type =
                rand() % 2; // 0=highobs1, 1=highobs2
            break;
          }
        }
      }
    }

    bool anyTopActive = false;
    for (int i = 0; i < 10; i++) {
      if (boss4Obj.topObstacles[i].active) {
        anyTopActive = true;
        boss4Obj.topObstacles[i].y -= boss4Obj.topObstacles[i].speedY;
        boss4Obj.topObstacles[i].frameTimer++;
        if (boss4Obj.topObstacles[i].frameTimer >= 2) {
          boss4Obj.topObstacles[i].frameTimer = 0;
          boss4Obj.topObstacles[i].frame =
              (boss4Obj.topObstacles[i].frame + 1) % 4;
        }
        if (boss4Obj.topObstacles[i].y < -100)
          boss4Obj.topObstacles[i].active = false;

        if (boss4Obj.topObstacles[i].active &&
            boss4Obj.topObstacles[i].x + 40 > charX &&
            boss4Obj.topObstacles[i].x < charX + 80 &&
            boss4Obj.topObstacles[i].y + 40 > charY &&
            boss4Obj.topObstacles[i].y < charY + 120) {
          if (!isInvincible && charAttackDamagelessTimer <= 0) {
            lives--;
            if (lives <= 0) {
              lives = 0;
              gameState = GAME_OVER;
            } else {
              isInvincible = true;
              invincibilityTimer = 60;
            }
            boss4Obj.topObstacles[i].active = false;
          }
        }
      }
    }

    // --- STATE LOGIC ---
    if (boss4Obj.topSkillState == 1) {
      boss4Obj.topSkillTimer++;
      if (boss4Obj.topSkillTimer >= 6) {
        boss4Obj.topSkillTimer = 0;
        boss4Obj.topFrameIndex++;
        if (boss4Obj.topFrameIndex >= 9) {
          boss4Obj.topSkillState = 2; // Thunder phase
          boss4Obj.topFrameIndex = 0;
          boss4Obj.topSkillTimer = 0;
        }
      }
    } else if (boss4Obj.topSkillState == 2) {
      boss4Obj.topSkillTimer++;
      if (boss4Obj.topSkillTimer % 5 == 0)
        boss4Obj.topFrameIndex = (boss4Obj.topFrameIndex + 1) % 4;

      if (boss4Obj.topSkillTimer > 400) { // Slightly faster skill duration
        if (!anyTopActive) {
          boss4Obj.topSkillState = 0;
          boss4Obj.topSkillTimer = 0;
          boss4Obj.currentSkill = -1;
          boss4Obj.skillsCompleted++;
          boss4Obj.skillGapTimer = 60;
        }
      }
    }
  }

  // --- SKILL 6: FIRE ATTACK SKILL LOGIC (NOW AT INDEX 6) ---
  if (level4Phase == 3 && boss4Obj.currentSkill == 6) {
    extern int charX, charY, lives;
    extern bool isInvincible;
    extern int invincibilityTimer;
    extern GameState gameState;

    if (boss4Obj.fireSkillState == 1) {
      boss4Obj.fireSkillTimer++;
      if (boss4Obj.fireSkillTimer >= 2) {
        boss4Obj.fireSkillTimer = 0;
        boss4Obj.fireFrameIndex++;
        if (boss4Obj.fireFrameIndex >= 10) {
          boss4Obj.fireSkillState = 2; // Launching
          boss4Obj.fbActive = true;
          boss4Obj.fbFrameIndex = 0;
          if (boss4Obj.facingLeft)
            boss4Obj.fbX = boss4Obj.x - 20;
          else
            boss4Obj.fbX = boss4Obj.x + BOSS_WIDTH;
          boss4Obj.fbY = boss4Obj.y + 100;

          float tX = (float)charX + 40, tY = (float)charY + 40;
          float dx = tX - boss4Obj.fbX, dy = tY - boss4Obj.fbY;
          float dist = (float)sqrt((double)(dx * dx + dy * dy));
          if (dist < 1.0f)
            dist = 1.0f;
          float speed = 12.0f; // Faster fireball travelling
          boss4Obj.fbVelX = (dx / dist) * speed;
          boss4Obj.fbVelY = (dy / dist) * speed;
          boss4Obj.fireSkillTimer = 0;
        }
      }
    } else if (boss4Obj.fireSkillState == 2) {
      if (boss4Obj.fbActive) {
        boss4Obj.fbX += boss4Obj.fbVelX;
        boss4Obj.fbY += boss4Obj.fbVelY;
        boss4Obj.fbFrameIndex = (boss4Obj.fbFrameIndex + 1) % 10;
        if (boss4Obj.fbX < -100 || boss4Obj.fbX > 1100 || boss4Obj.fbY < -100 ||
            boss4Obj.fbY > 700)
          boss4Obj.fbActive = false;
        if (boss4Obj.fbActive && boss4Obj.fbX + 40 > charX &&
            boss4Obj.fbX < charX + 80 && boss4Obj.fbY + 40 > charY &&
            boss4Obj.fbY < charY + 80) {
          if (!isInvincible && charAttackDamagelessTimer <= 0) {
            lives--;
            if (lives <= 0) {
              lives = 0;
              gameState = GAME_OVER;
            } else {
              isInvincible = true;
              invincibilityTimer = 60;
            }
            boss4Obj.charOnFire = true;
            boss4Obj.charFireTimer = 50;
          }
          boss4Obj.fbActive = false;
        }
      }
      if (!boss4Obj.fbActive) {
        boss4Obj.fireSkillState = 3;
        boss4Obj.fireSkillTimer = 0;
      }
    } else if (boss4Obj.fireSkillState == 3) {
      boss4Obj.fireSkillTimer++;
      if (boss4Obj.fireSkillTimer >= 40) { // Faster return to next fireball
        boss4Obj.fireAttackCount++;
        if (boss4Obj.fireAttackCount >= boss4Obj.fireAttackTarget) {
          boss4Obj.fireSkillState = 0;
          boss4Obj.currentSkill = -1;
          boss4Obj.skillsCompleted++;
          boss4Obj.skillGapTimer = 60;
        } else {
          boss4Obj.fireSkillState = 1;
          boss4Obj.fireFrameIndex = 0;
          if (charX < boss4Obj.x + BOSS_WIDTH / 2.0f)
            boss4Obj.facingLeft = true;
          else
            boss4Obj.facingLeft = false;
        }
        boss4Obj.fireSkillTimer = 0;
      }
    }
  }

  // --- SKILL 7: REST ATTACK SKILL LOGIC (NOW AT INDEX 7) ---
  if (level4Phase == 3 && boss4Obj.currentSkill == 7) {
    extern int charX, charY, charWidth, charHeight, lives, groundY;
    extern bool isInvincible, isAttacking, npcSlashDone, isFallingSequence;
    extern int invincibilityTimer, attackFrameIndex;
    extern GameState gameState;
    extern bool isLeftArrowPressed;
    extern unsigned int specialKeyPressed[512];

    if (boss4Obj.restSkillState == 1) {
      boss4Obj.restSkillTimer++;
      if (boss4Obj.restSkillTimer >= 5) {
        boss4Obj.restSkillTimer = 0;
        boss4Obj.restFrameIndex++;
        if (boss4Obj.restFrameIndex >= 4) {
          boss4Obj.restSkillState = 2;
          boss4Obj.restFrameIndex = 4;
          boss4Obj.restSkillTimer = 0;
          // Initialize rest NPC spawning
          boss4Obj.restNpcSpawnTimer = 0;
          boss4Obj.restBigNpcSpawned = false;
        }
      }
    } else if (boss4Obj.restSkillState == 2) {
      boss4Obj.restSkillTimer++;
      // Cycle breathing frames 4, 5, 6
      if (boss4Obj.restSkillTimer % 8 == 0) {
        boss4Obj.restFrameIndex++;
        if (boss4Obj.restFrameIndex >= 7)
          boss4Obj.restFrameIndex = 4;
      }

      // --- REST NPC SPAWNING ---
      // Spawn walking NPCs every ~150 ticks
      boss4Obj.restNpcSpawnTimer++;
      if (boss4Obj.restNpcSpawnTimer >= 150) {
        boss4Obj.restNpcSpawnTimer = 0;
        for (int i = 0; i < 3; i++) {
          if (!boss4Obj.restNpcs[i].active) {
            boss4Obj.restNpcs[i].active = true;
            boss4Obj.restNpcs[i].x = 1100.0f;
            boss4Obj.restNpcs[i].y = (float)groundY;
            boss4Obj.restNpcs[i].type = (rand() % 2) + 1; // 1: Melee, 2: Ranged
            boss4Obj.restNpcs[i].state = 0;
            boss4Obj.restNpcs[i].life = 2;
            boss4Obj.restNpcs[i].animFrame = 0;
            boss4Obj.restNpcs[i].animCounter = 0;
            boss4Obj.restNpcs[i].fireCooldown = 90 + (rand() % 90);
            boss4Obj.restNpcs[i].hasAttacked = false;
            boss4Obj.restNpcs[i].hitFlashTimer = 0;
            break;
          }
        }
      }

      // Spawn Big NPC after ~3 seconds (100 ticks)
      if (!boss4Obj.restBigNpcSpawned && boss4Obj.restSkillTimer >= 100) {
        boss4Obj.restBigNpcSpawned = true;
        boss4Obj.restBigNpc.active = true;
        boss4Obj.restBigNpc.x = 1100.0f;
        boss4Obj.restBigNpc.y = (float)groundY;
        boss4Obj.restBigNpc.life = 6;
        boss4Obj.restBigNpc.state = 0;
        boss4Obj.restBigNpc.animFrame = 0;
        boss4Obj.restBigNpc.animCounter = 0;
        boss4Obj.restBigNpc.attackTimer = 0;
        boss4Obj.restBigNpc.hasAttacked = false;
        boss4Obj.restBigNpc.hitFlashTimer = 0;
      }

      // --- UPDATE WALKING NPCs ---
      for (int i = 0; i < 3; i++) {
        if (!boss4Obj.restNpcs[i].active)
          continue;

        if (boss4Obj.restNpcs[i].hitFlashTimer > 0)
          boss4Obj.restNpcs[i].hitFlashTimer--;

        if (boss4Obj.restNpcs[i].stunTimer > 0)
          boss4Obj.restNpcs[i].stunTimer--;

        if (boss4Obj.restNpcs[i].state == 0) { // Walking
          boss4Obj.restNpcs[i].x -= 4.0f;
          boss4Obj.restNpcs[i].animCounter++;
          if (boss4Obj.restNpcs[i].animCounter >= 4) {
            boss4Obj.restNpcs[i].animCounter = 0;
            boss4Obj.restNpcs[i].animFrame =
                (boss4Obj.restNpcs[i].animFrame + 1) % 9;
          }

          if (boss4Obj.restNpcs[i].type == 1) { // Melee
            if (boss4Obj.restNpcs[i].x - charX < 150 &&
                boss4Obj.restNpcs[i].x > charX - 50) {
              boss4Obj.restNpcs[i].state = 1;
              boss4Obj.restNpcs[i].animFrame = 0;
              boss4Obj.restNpcs[i].attackTimer = 0;
              boss4Obj.restNpcs[i].hasAttacked = false;
            }
          } else { // Ranged
            boss4Obj.restNpcs[i].fireCooldown--;
            if (boss4Obj.restNpcs[i].fireCooldown <= 0 &&
                boss4Obj.restNpcs[i].stunTimer <= 0) {
              for (int j = 0; j < 3; j++) {
                if (!boss4Obj.restFires[j].active) {
                  boss4Obj.restFires[j].active = true;
                  boss4Obj.restFires[j].x = boss4Obj.restNpcs[i].x - 30;
                  boss4Obj.restFires[j].y = boss4Obj.restNpcs[i].y + 30;
                  boss4Obj.restFires[j].animFrame = 0;
                  boss4Obj.restFires[j].animCounter = 0;
                  break;
                }
              }
              boss4Obj.restNpcs[i].fireCooldown = 90 + (rand() % 90);
            }
          }
        } else if (boss4Obj.restNpcs[i].state == 1) { // Attacking (Melee)
          boss4Obj.restNpcs[i].attackTimer++;
          if (boss4Obj.restNpcs[i].attackTimer % 4 == 0) {
            boss4Obj.restNpcs[i].animFrame++;
          }
          if (!boss4Obj.restNpcs[i].hasAttacked &&
              boss4Obj.restNpcs[i].animFrame >= 5 &&
              boss4Obj.restNpcs[i].stunTimer <= 0) {
            if (charX + charWidth > boss4Obj.restNpcs[i].x - 30 &&
                charX < boss4Obj.restNpcs[i].x + 30 &&
                charY + charHeight > groundY && charY < groundY + 100) {
              if (!isInvincible && !isFallingSequence) {
                // 3 walking-NPC hits = 1 life lost
                walkNpcHitAccumulator++;
                isInvincible = true;
                invincibilityTimer = 30; // ~1s gap between counted hits
                if (walkNpcHitAccumulator >= 3) {
                  walkNpcHitAccumulator = 0;
                  lives--;
                  if (lives <= 0) {
                    lives = 0;
                    gameState = GAME_OVER;
                  } else {
                    invincibilityTimer = 60;
                  }
                }
              }
            }
            boss4Obj.restNpcs[i].hasAttacked = true;
          }
          if (boss4Obj.restNpcs[i].animFrame >= 9) {
            boss4Obj.restNpcs[i].state = 0;
            boss4Obj.restNpcs[i].animFrame = 0;
            boss4Obj.restNpcs[i].hasAttacked = false;
          }
        }

        if (boss4Obj.restNpcs[i].x < -200)
          boss4Obj.restNpcs[i].active = false;

        // Player attacking walking NPC
        if (isAttacking && attackFrameIndex == 3 && !npcSlashDone &&
            boss4Obj.restNpcs[i].active) {
          float npcCenterX = boss4Obj.restNpcs[i].x + 50.0f;
          bool facingBack =
              isLeftArrowPressed || (specialKeyPressed[GLUT_KEY_LEFT] != 0);
          float reachMin, reachMax;
          if (facingBack) {
            reachMin = (float)charX - 160;
            reachMax = (float)charX - 10;
          } else {
            reachMin = (float)(charX + charWidth + 10);
            reachMax = (float)(charX + charWidth + 160);
          }

          if (npcCenterX > reachMin && npcCenterX < reachMax &&
              charY < boss4Obj.restNpcs[i].y + 120 &&
              charY + charHeight > boss4Obj.restNpcs[i].y) {
            boss4Obj.restNpcs[i].life--;
            boss4Obj.restNpcs[i].stunTimer = 22; // 0.75-second stun
            npcSlashDone = true;
            if (boss4Obj.restNpcs[i].life <= 0) {
              boss4Obj.restNpcs[i].active = false;
            } else {
              boss4Obj.restNpcs[i].hitFlashTimer = 10;
            }
          }
        }

        // NPC contact damage to player - 3 hits = 1 life
        if (!isInvincible && !isFallingSequence &&
            boss4Obj.restNpcs[i].active &&
            boss4Obj.restNpcs[i].stunTimer <= 0) {
          if (charX + charWidth - 40 > boss4Obj.restNpcs[i].x + 20 &&
              charX + 40 < boss4Obj.restNpcs[i].x + 130 &&
              charY + charHeight > boss4Obj.restNpcs[i].y &&
              charY < boss4Obj.restNpcs[i].y + 120) {
            walkNpcHitAccumulator++;
            isInvincible = true;
            invincibilityTimer = 30; // ~1s gap between counted hits
            if (walkNpcHitAccumulator >= 3) {
              walkNpcHitAccumulator = 0;
              lives--;
              if (lives <= 0) {
                lives = 0;
                gameState = GAME_OVER;
              } else {
                invincibilityTimer = 60;
              }
            }
          }
        }
      }

      // --- UPDATE GREEN FIRE PROJECTILES ---
      for (int i = 0; i < 3; i++) {
        if (boss4Obj.restFires[i].active) {
          boss4Obj.restFires[i].x -= 10.0f;
          boss4Obj.restFires[i].animCounter++;
          if (boss4Obj.restFires[i].animCounter >= 3) {
            boss4Obj.restFires[i].animCounter = 0;
            boss4Obj.restFires[i].animFrame =
                (boss4Obj.restFires[i].animFrame + 1) % 9;
          }
          if (boss4Obj.restFires[i].x < -100)
            boss4Obj.restFires[i].active = false;

          // Fire collision with player — respects invincibility window
          if (boss4Obj.restFires[i].active && !isInvincible) {
            // Generous fire collision box for reliable hits
            if (charX + charWidth - 15 > boss4Obj.restFires[i].x &&
                charX + 15 < boss4Obj.restFires[i].x + 100 &&
                charY + charHeight > boss4Obj.restFires[i].y &&
                charY < boss4Obj.restFires[i].y + 100) {
              boss4Obj.restFires[i].active = false; // Vanish on impact
              lives--;
              if (lives <= 0) {
                lives = 0;
                gameState = GAME_OVER;
              } else {
                isInvincible = true;
                invincibilityTimer = 60;
              }
            }
          }
        }
      }

      // --- UPDATE BIG NPC ---
      if (boss4Obj.restBigNpc.active) {
        if (boss4Obj.restBigNpc.hitFlashTimer > 0)
          boss4Obj.restBigNpc.hitFlashTimer--;

        if (boss4Obj.restBigNpc.stunTimer > 0)
          boss4Obj.restBigNpc.stunTimer--;

        if (boss4Obj.restBigNpc.state == 0) { // Following player
          float speed = 3.0f;
          if (boss4Obj.restBigNpc.x > charX + 50)
            boss4Obj.restBigNpc.x -= speed;
          else if (boss4Obj.restBigNpc.x < charX - 50)
            boss4Obj.restBigNpc.x += speed;

          boss4Obj.restBigNpc.animCounter++;
          if (boss4Obj.restBigNpc.animCounter >= 5) {
            boss4Obj.restBigNpc.animCounter = 0;
            boss4Obj.restBigNpc.animFrame =
                (boss4Obj.restBigNpc.animFrame + 1) % 9;
          }

          // Switch to attack when close
          float dx = boss4Obj.restBigNpc.x - charX;
          if (dx < 0)
            dx = -dx;
          if (dx < 120) {
            boss4Obj.restBigNpc.state = 1;
            boss4Obj.restBigNpc.animFrame = 0;
            boss4Obj.restBigNpc.attackTimer = 0;
            boss4Obj.restBigNpc.hasAttacked = false;
          }
        } else if (boss4Obj.restBigNpc.state == 1) { // Attacking
          boss4Obj.restBigNpc.attackTimer++;
          if (boss4Obj.restBigNpc.attackTimer % 4 == 0) {
            boss4Obj.restBigNpc.animFrame++;
          }
          if (!boss4Obj.restBigNpc.hasAttacked &&
              boss4Obj.restBigNpc.animFrame >= 5 &&
              boss4Obj.restBigNpc.stunTimer <= 0) {
            // Deal damage - Adjusted hitbox for 115x115 size
            if (charX + charWidth > boss4Obj.restBigNpc.x - 30 &&
                charX < boss4Obj.restBigNpc.x + 130 &&
                charY + charHeight > boss4Obj.restBigNpc.y &&
                charY < boss4Obj.restBigNpc.y + 115) {
              if (!isInvincible && !isFallingSequence &&
                  charAttackDamagelessTimer <= 0) {
                lives--;
                if (lives <= 0) {
                  lives = 0;
                  gameState = GAME_OVER;
                } else {
                  isInvincible = true;
                  invincibilityTimer = 60;
                }
              }
            }
            boss4Obj.restBigNpc.hasAttacked = true;
          }
          if (boss4Obj.restBigNpc.animFrame >= 9) {
            boss4Obj.restBigNpc.state = 0;
            boss4Obj.restBigNpc.animFrame = 0;
            boss4Obj.restBigNpc.hasAttacked = false;
          }
        }

        // Player attacking Big NPC
        if (isAttacking && attackFrameIndex == 3 && !npcSlashDone) {
          float bigCenterX = boss4Obj.restBigNpc.x + 100.0f;
          bool facingBack =
              isLeftArrowPressed || (specialKeyPressed[GLUT_KEY_LEFT] != 0);
          float reachMin, reachMax;
          if (facingBack) {
            reachMin = (float)charX - 160;
            reachMax = (float)charX - 10;
          } else {
            reachMin = (float)(charX + charWidth + 10);
            reachMax = (float)(charX + charWidth + 160);
          }

          if (bigCenterX > reachMin && bigCenterX < reachMax &&
              charY < boss4Obj.restBigNpc.y + 115 &&
              charY + charHeight > boss4Obj.restBigNpc.y) {
            boss4Obj.restBigNpc.life--;
            boss4Obj.restBigNpc.stunTimer = 22; // 0.75-second stun
            npcSlashDone = true;
            if (boss4Obj.restBigNpc.life <= 0) {
              boss4Obj.restBigNpc.active = false;
            } else {
              boss4Obj.restBigNpc.hitFlashTimer = 10;
            }
          }
        }

        // Big NPC contact damage (Restored with stun guard)
        if (!isInvincible && !isFallingSequence && boss4Obj.restBigNpc.active &&
            boss4Obj.restBigNpc.stunTimer <= 0 &&
            charAttackDamagelessTimer <= 0) {
          if (charX + charWidth - 40 > boss4Obj.restBigNpc.x + 20 &&
              charX + 40 < boss4Obj.restBigNpc.x + 100 &&
              charY + charHeight > boss4Obj.restBigNpc.y &&
              charY < boss4Obj.restBigNpc.y + 115) {
            lives--;
            if (lives <= 0) {
              lives = 0;
              gameState = GAME_OVER;
            } else {
              isInvincible = true;
              invincibilityTimer = 60;
            }
          }
        }
        // Big NPC update loop ends
      }

      // 660 timer ticks = 20 seconds at 33fps
      if (boss4Obj.restSkillTimer > 660) {
        // Deactivate all rest NPCs
        for (int i = 0; i < 3; i++) {
          boss4Obj.restNpcs[i].active = false;
          boss4Obj.restFires[i].active = false;
        }
        boss4Obj.restBigNpc.active = false;
        boss4Obj.restBigNpcSpawned = false;
        walkNpcHitAccumulator = 0; // Reset hit counter on skill end

        boss4Obj.restSkillState = 0;
        boss4Obj.restSkillTimer = 0;
        boss4Obj.currentSkill = -1;
        boss4Obj.skillsCompleted++;
        boss4Obj.skillGapTimer = 40;
        boss4Obj.safeguardTimer = 0;
      }
    }
  }

  // --- SKILL 8: HITGROUND ATTACK SKILL LOGIC ---
  if (level4Phase == 3 && boss4Obj.currentSkill == 8) {
    extern int charX, charY, charWidth, charHeight, lives, groundY;
    extern bool isInvincible;
    extern int invincibilityTimer;
    extern GameState gameState;

    if (boss4Obj.hitgroundSkillState == 1) { // Shout (11 frames)
      extern float globalScreenShakeX, globalScreenShakeY;
      globalScreenShakeX = (float)(rand() % 20 - 10); // Earthquake feeling
      globalScreenShakeY = (float)(rand() % 20 - 10);

      boss4Obj.hitgroundSkillTimer++;
      if (boss4Obj.hitgroundSkillTimer >= 6) {
        boss4Obj.hitgroundSkillTimer = 0;
        boss4Obj.hitgroundFrameIndex++;
        if (boss4Obj.hitgroundFrameIndex >= 11) {
          boss4Obj.hitgroundSkillState = 2; // Hitground Start
          boss4Obj.hitgroundFrameIndex = 0;
          boss4Obj.hitgroundSkillTimer = 0;
          globalScreenShakeX = 0;
          globalScreenShakeY = 0;
        }
      }
    } else if (boss4Obj.hitgroundSkillState == 2) { // Hitground (1-7)
      boss4Obj.hitgroundSkillTimer++;
      if (boss4Obj.hitgroundSkillTimer >= 6) {
        boss4Obj.hitgroundSkillTimer = 0;
        boss4Obj.hitgroundFrameIndex++;
        if (boss4Obj.hitgroundFrameIndex >= 7) {
          boss4Obj.hitgroundSkillState = 3; // Hit loop (8-11)
          boss4Obj.hitgroundFrameIndex = 7; // indices 7,8,9,10
          boss4Obj.hitgroundLoopTimer = 0;
          boss4Obj.hitgroundSkillTimer = 0;
          boss4Obj.hitgroundHoleCount = 0;
          boss4Obj.hitgroundHoleTimer = 0;
          boss4Obj.hitgroundWaitTimer = 0;
          boss4Obj.birdDiveCooldown = 0;
          // Reset all birds
          for (int i = 0; i < 4; i++) {
            boss4Obj.holeBirds[i].active = false;
            boss4Obj.birdHoverTicks[i] = 150 + rand() % 60; // 5-7 sec
          }
        }
      }
    } else if (boss4Obj.hitgroundSkillState == 3) { // Hit Loop & Holes
      boss4Obj.hitgroundLoopTimer++;
      boss4Obj.hitgroundSkillTimer++;

      // Decrement global bird dive cooldown
      if (boss4Obj.birdDiveCooldown > 0)
        boss4Obj.birdDiveCooldown--;

      // Loop hitting images (indices 7 to 10)
      if (boss4Obj.hitgroundSkillTimer >= 5) {
        boss4Obj.hitgroundSkillTimer = 0;
        boss4Obj.hitgroundFrameIndex++;
        if (boss4Obj.hitgroundFrameIndex > 10)
          boss4Obj.hitgroundFrameIndex = 7;
      }

      // Hole Spawning logic: exactly 4 holes, one every ~165 ticks
      boss4Obj.hitgroundHoleTimer++;
      if (boss4Obj.hitgroundHoleTimer > 165 &&
          boss4Obj.hitgroundHoleCount < 4) {
        boss4Obj.hitgroundHoleTimer = 0;
        // Find inactive hole slot
        for (int i = 0; i < 4; i++) {
          if (!boss4Obj.bossHoles[i].active) {
            boss4Obj.bossHoles[i].active = true;
            boss4Obj.bossHoles[i].x = (float)(charX + (rand() % 400 - 200));
            boss4Obj.bossHoles[i].y = (float)groundY - 28;
            boss4Obj.bossHoles[i].state = 1; // Appearing
            boss4Obj.bossHoles[i].frame = 0;
            boss4Obj.bossHoles[i].frameTimer = 0;
            boss4Obj.bossHoles[i].stateTimer = 0;
            boss4Obj.bossHoles[i].birdSpawned = false;
            boss4Obj.hitgroundHoleCount++;
            break;
          }
        }
      }

      // ----------------------------------------------------------------
      // Update all holes
      // ----------------------------------------------------------------
      extern bool isAttacking;
      extern int attackFrameIndex;
      extern bool npcSlashDone;
      extern bool isLeftArrowPressed;
      extern unsigned int specialKeyPressed[512];

      for (int i = 0; i < 4; i++) {
        if (boss4Obj.bossHoles[i].active) {
          if (boss4Obj.bossHoles[i].state == 1) { // Appearing
            boss4Obj.bossHoles[i].frameTimer++;
            if (boss4Obj.bossHoles[i].frameTimer >= 6) { // Slower creation
              boss4Obj.bossHoles[i].frameTimer = 0;
              boss4Obj.bossHoles[i].frame++;
              if (boss4Obj.bossHoles[i].frame >= 9) {
                boss4Obj.bossHoles[i].frame = 9;
                boss4Obj.bossHoles[i].state = 2; // Stay – spawn bird now
                // Spawn bird from this hole if slot free
                if (!boss4Obj.bossHoles[i].birdSpawned) {
                  for (int b = 0; b < 4; b++) {
                    if (!boss4Obj.holeBirds[b].active) {
                      boss4Obj.holeBirds[b].active = true;
                      boss4Obj.holeBirds[b].type =
                          rand() % 2; // 0=artikuno, 1=volplex
                      // Start at hole center, tiny scale
                      float holeCX = boss4Obj.bossHoles[i].x + 90.0f;
                      boss4Obj.holeBirds[b].x = holeCX;
                      boss4Obj.holeBirds[b].y = boss4Obj.bossHoles[i].y + 35.0f;
                      boss4Obj.holeBirds[b].startX = holeCX;
                      boss4Obj.holeBirds[b].scale = 0.05f; // start very tiny
                      boss4Obj.holeBirds[b].state = 0;     // rising
                      boss4Obj.holeBirds[b].stateTimer = 0;
                      boss4Obj.holeBirds[b].frame = 0;
                      boss4Obj.holeBirds[b].frameTimer = 0;
                      boss4Obj.holeBirds[b].velX = 0;
                      boss4Obj.holeBirds[b].velY = 0;
                      boss4Obj.holeBirds[b].holeIndex = i;
                      boss4Obj.holeBirds[b].life = 2;
                      boss4Obj.holeBirds[b].hitFlashTimer = 0;
                      boss4Obj.bossHoles[i].birdSpawned = true;
                      break;
                    }
                  }
                }
              }
            }
          } else if (boss4Obj.bossHoles[i].state == 2) { // Staying
            boss4Obj.bossHoles[i].stateTimer++;
            if (boss4Obj.bossHoles[i].stateTimer >= 100) { // ~3 seconds stay
              boss4Obj.bossHoles[i].state = 3;             // Disappearing
            }

            // Collision check with hole when open
            if (charX + charWidth - 40 > boss4Obj.bossHoles[i].x &&
                charX + 40 < boss4Obj.bossHoles[i].x + 180 &&
                charY < boss4Obj.bossHoles[i].y + 70) {
              if (!isInvincible) {
                lives--;
                if (lives <= 0) {
                  lives = 0;
                  gameState = GAME_OVER;
                } else {
                  isInvincible = true;
                  invincibilityTimer = 60;
                }
              }
            }
          } else if (boss4Obj.bossHoles[i].state == 3) { // Disappearing
            boss4Obj.bossHoles[i].frameTimer++;
            if (boss4Obj.bossHoles[i].frameTimer >= 6) { // Slower vanishing
              boss4Obj.bossHoles[i].frameTimer = 0;
              boss4Obj.bossHoles[i].frame--;
              if (boss4Obj.bossHoles[i].frame < 0) {
                boss4Obj.bossHoles[i].active = false;
              }
            }
          }
        }
      }

// ----------------------------------------------------------------
// Update all birds
// ----------------------------------------------------------------
#define BIRD_W 100
#define BIRD_H 80
#define BIRD_CEIL_Y 470        // Y ceiling of screen
#define BIRD_PATROL_SPEED 5.0f // Fast patrol speed at ceiling
#define BIRD_DIVE_GAP_TICKS 60 // 2-second minimum gap between any two dives

      bool anyBirdAlive = false;
      for (int b = 0; b < 4; b++) {
        if (boss4Obj.holeBirds[b].active) {
          anyBirdAlive = true;
        }

        // Animate frames (6 frames cycling)
        boss4Obj.holeBirds[b].frameTimer++;
        if (boss4Obj.holeBirds[b].frameTimer >= 4) {
          boss4Obj.holeBirds[b].frameTimer = 0;
          boss4Obj.holeBirds[b].frame = (boss4Obj.holeBirds[b].frame + 1) % 6;
        }
        if (boss4Obj.holeBirds[b].hitFlashTimer > 0)
          boss4Obj.holeBirds[b].hitFlashTimer--;

        if (boss4Obj.holeBirds[b].state == 0) {
          // --- RISING: grow from tiny to full size ---
          boss4Obj.holeBirds[b].y += 4.0f;
          float holeY =
              boss4Obj.bossHoles[boss4Obj.holeBirds[b].holeIndex].active
                  ? boss4Obj.bossHoles[boss4Obj.holeBirds[b].holeIndex].y +
                        35.0f
                  : 55.0f;
          float totalRise = (float)BIRD_CEIL_Y - holeY;
          if (totalRise < 1.0f)
            totalRise = 1.0f;
          float risen = boss4Obj.holeBirds[b].y - holeY;
          if (risen < 0)
            risen = 0;
          boss4Obj.holeBirds[b].scale = 0.05f + 0.95f * (risen / totalRise);
          if (boss4Obj.holeBirds[b].scale > 1.0f)
            boss4Obj.holeBirds[b].scale = 1.0f;
          // Keep x fixed at hole center while rising
          boss4Obj.holeBirds[b].x = boss4Obj.holeBirds[b].startX;

          if (boss4Obj.holeBirds[b].y >= BIRD_CEIL_Y) {
            boss4Obj.holeBirds[b].y = (float)BIRD_CEIL_Y;
            boss4Obj.holeBirds[b].scale = 1.0f;
            boss4Obj.holeBirds[b].state = 1; // start patrolling
            boss4Obj.holeBirds[b].stateTimer = 0;
            // Assign random hover time 5-7 seconds (150-210 ticks)
            boss4Obj.birdHoverTicks[b] = 150 + rand() % 60;
            // Give initial patrol velocity so each bird moves differently
            boss4Obj.holeBirds[b].velX =
                (b % 2 == 0) ? BIRD_PATROL_SPEED : -BIRD_PATROL_SPEED;
            boss4Obj.holeBirds[b].velY =
                -(2.0f + (rand() % 3)); // Smooth vertical movement
          }

        } else if (boss4Obj.holeBirds[b].state == 1) {
          // --- PATROLLING at ceiling: fast movement, 100px gap from char ---
          // Smoothly patrols randomly across half screen up to ceiling
          boss4Obj.holeBirds[b].stateTimer++;

          // Move with patrol velocity
          boss4Obj.holeBirds[b].x += boss4Obj.holeBirds[b].velX;
          boss4Obj.holeBirds[b].y += boss4Obj.holeBirds[b].velY;

          // Bounce off screen edges horizontally
          if (boss4Obj.holeBirds[b].x < 20) {
            boss4Obj.holeBirds[b].x = 20;
            boss4Obj.holeBirds[b].velX = BIRD_PATROL_SPEED;
          }
          if (boss4Obj.holeBirds[b].x > 930) {
            boss4Obj.holeBirds[b].x = 930;
            boss4Obj.holeBirds[b].velX = -BIRD_PATROL_SPEED;
          }

          // Bounce smoothly on Y between half screen (300) and ceiling
          // (BIRD_CEIL_Y)
          if (boss4Obj.holeBirds[b].y < 300) {
            boss4Obj.holeBirds[b].y = 300;
            boss4Obj.holeBirds[b].velY = 2.0f + (rand() % 3);
          }
          if (boss4Obj.holeBirds[b].y > BIRD_CEIL_Y) {
            boss4Obj.holeBirds[b].y = (float)BIRD_CEIL_Y;
            boss4Obj.holeBirds[b].velY = -(2.0f + (rand() % 3));
          }

          // Maintain at least 100 px horizontal distance from character
          float dist = boss4Obj.holeBirds[b].x - (float)charX;
          if (dist >= 0 && dist < 100.0f) {
            boss4Obj.holeBirds[b].velX =
                BIRD_PATROL_SPEED * 0.4f; // move away slowly
          } else if (dist < 0 && dist > -100.0f) {
            boss4Obj.holeBirds[b].velX =
                -BIRD_PATROL_SPEED * 0.4f; // move away slowly
          }

          // Try to dive when hover time reached AND global cooldown is clear
          if (boss4Obj.holeBirds[b].stateTimer >= boss4Obj.birdHoverTicks[b] &&
              boss4Obj.birdDiveCooldown <= 0) {
            // Lock onto character middle and dive
            float dx = (float)charX + (float)charWidth / 2.0f -
                       boss4Obj.holeBirds[b].x;
            float dy = (float)charY + (float)charHeight / 2.0f -
                       boss4Obj.holeBirds[b].y;
            float mag = (float)sqrt((double)(dx * dx + dy * dy));
            if (mag < 1.0f)
              mag = 1.0f;
            float speed = 15.0f;
            boss4Obj.holeBirds[b].velX = (dx / mag) * speed;
            boss4Obj.holeBirds[b].velY = (dy / mag) * speed;
            boss4Obj.holeBirds[b].state = 2; // diving
            boss4Obj.birdDiveCooldown =
                BIRD_DIVE_GAP_TICKS; // enforce 2-sec global gap
          }

        } else if (boss4Obj.holeBirds[b].state == 2) {
          // --- DIVING TOWARD CHARACTER ---
          boss4Obj.holeBirds[b].x += boss4Obj.holeBirds[b].velX;
          boss4Obj.holeBirds[b].y += boss4Obj.holeBirds[b].velY;

          int bW = (int)(BIRD_W * boss4Obj.holeBirds[b].scale);
          int bH = (int)(BIRD_H * boss4Obj.holeBirds[b].scale);
          int bX = (int)boss4Obj.holeBirds[b].x - bW / 2;
          int bY = (int)boss4Obj.holeBirds[b].y - bH / 2;

          // Hit character
          if (bX + bW > charX + 20 && bX < charX + charWidth - 20 &&
              bY + bH > charY + 10 && bY < charY + charHeight - 10) {
            extern bool isAttacking;
            if (!isAttacking) {
              if (!isInvincible && charAttackDamagelessTimer <= 0) {
                lives--;
                if (lives <= 0) {
                  lives = 0;
                  gameState = GAME_OVER;
                } else {
                  isInvincible = true;
                  invincibilityTimer = 60;
                }
              }
            }
            // Swoop back up to ceiling instead of vanishing
            boss4Obj.holeBirds[b].state = 5;
          }

          // If bird flies below character or misses, swoop back up smoothly
          if (boss4Obj.holeBirds[b].y < (float)charY + 10.0f ||
              boss4Obj.holeBirds[b].y > 750 || boss4Obj.holeBirds[b].x < -100 ||
              boss4Obj.holeBirds[b].x > 1100) {
            // Start smooth swoop back to patrol
            boss4Obj.holeBirds[b].state = 5;
          }

        } else if (boss4Obj.holeBirds[b].state == 3) {
          // --- DYING: tumble-fall downward (drawn upside-down in render) ---
          boss4Obj.holeBirds[b].velY -= 0.3f; // slow motion gravity
          if (boss4Obj.holeBirds[b].velY < -4.0f)
            boss4Obj.holeBirds[b].velY = -4.0f; // terminal velocity
          boss4Obj.holeBirds[b].velX *= 0.96f;
          boss4Obj.holeBirds[b].x += boss4Obj.holeBirds[b].velX;
          boss4Obj.holeBirds[b].y += boss4Obj.holeBirds[b].velY;
          if (boss4Obj.holeBirds[b].y < -80) {
            boss4Obj.holeBirds[b].active = false; // gone
          }

        } else if (boss4Obj.holeBirds[b].state == 4) {
          // --- FLEEING: after hitting character, fly upward and OFF screen ---
          boss4Obj.holeBirds[b].x += boss4Obj.holeBirds[b].velX;
          boss4Obj.holeBirds[b].y += boss4Obj.holeBirds[b].velY;
          if (boss4Obj.holeBirds[b].y > 750) {
            boss4Obj.holeBirds[b].active = false;
          }
        } else if (boss4Obj.holeBirds[b].state == 5) {
          // --- SWOOPING BACK TO CEILING (Smooth Return) ---
          float dx = boss4Obj.holeBirds[b].startX - boss4Obj.holeBirds[b].x;
          float dy = (float)BIRD_CEIL_Y - boss4Obj.holeBirds[b].y;
          float mag = (float)sqrt((double)(dx * dx + dy * dy));
          if (mag < 1.0f)
            mag = 1.0f;
          float speed = 15.0f; // Smooth swoop speed
          boss4Obj.holeBirds[b].velX = (dx / mag) * speed;
          boss4Obj.holeBirds[b].velY = (dy / mag) * speed;

          boss4Obj.holeBirds[b].x += boss4Obj.holeBirds[b].velX;
          boss4Obj.holeBirds[b].y += boss4Obj.holeBirds[b].velY;

          if (boss4Obj.holeBirds[b].y >= BIRD_CEIL_Y - 15.0f) {
            boss4Obj.holeBirds[b].y = (float)BIRD_CEIL_Y;
            boss4Obj.holeBirds[b].state = 1; // back to patrol
            boss4Obj.holeBirds[b].stateTimer = 0;
            boss4Obj.birdHoverTicks[b] = 150 + rand() % 60;
            boss4Obj.holeBirds[b].velX =
                (rand() % 2 == 0) ? BIRD_PATROL_SPEED : -BIRD_PATROL_SPEED;
            boss4Obj.holeBirds[b].velY = -(2.0f + (rand() % 3));
          }
        }

        // ---- Check if player weapon kills the bird ----
        if (boss4Obj.holeBirds[b].active &&
            (boss4Obj.holeBirds[b].state == 0 ||
             boss4Obj.holeBirds[b].state == 1 ||
             boss4Obj.holeBirds[b].state == 2 ||
             boss4Obj.holeBirds[b].state == 5)) {
          if (isAttacking && attackFrameIndex == 3 && !npcSlashDone) {
            bool facingBack =
                isLeftArrowPressed || (specialKeyPressed[GLUT_KEY_LEFT] != 0);
            float reachMin, reachMax;
            if (facingBack) {
              reachMin = (float)charX - 200;
              reachMax = (float)charX + 80; // Significant overlap allowed
            } else {
              reachMin =
                  (float)charX + charWidth - 80; // Significant overlap allowed
              reachMax = (float)charX + charWidth + 200;
            }

            float birdCX = boss4Obj.holeBirds[b].x;
            float birdCY = boss4Obj.holeBirds[b].y;
            if (birdCX > reachMin && birdCX < reachMax &&
                birdCY > (float)charY - 120 &&
                birdCY < (float)(charY + charHeight) + 120) {
              boss4Obj.holeBirds[b].life--;
              npcSlashDone = true;
              if (boss4Obj.holeBirds[b].life <= 0) {
                // Bird hit by player – begins death fall
                boss4Obj.holeBirds[b].state = 3;
                boss4Obj.holeBirds[b].velX = (rand() % 10 - 4) * 1.2f;
                boss4Obj.holeBirds[b].velY =
                    4.0f; // smaller toss up for slow motion
              } else {
                boss4Obj.holeBirds[b].hitFlashTimer = 10;
              }
            }
          }
        }
      }

      bool anyHoleActive = false;
      for (int i = 0; i < 4; i++) {
        if (boss4Obj.bossHoles[i].active) {
          anyHoleActive = true;
          break;
        }
      }

      if (boss4Obj.hitgroundHoleCount >= 4 && !anyHoleActive) {
        // Boss 4 waits for all birds to be killed by the player
        if (!anyBirdAlive) {
          for (int i = 0; i < 4; i++) {
            boss4Obj.bossHoles[i].active = false;
            boss4Obj.holeBirds[i].active = false;
          }
          boss4Obj.hitgroundSkillState = 0;
          boss4Obj.currentSkill = -1;
          boss4Obj.skillsCompleted++;
          boss4Obj.skillGapTimer = 60;
          boss4Obj.safeguardTimer = 0;
        }
      }
    }
  }

  if (boss4Obj.charOnFire) {
    boss4Obj.charFireTimer--;
    if (boss4Obj.charFireTimer <= 0)
      boss4Obj.charOnFire = false;
  }

  // --- SMOOTH LARGE FLAME LOGIC ---
  if (boss4Obj.largeFlamePhase == 1) { // Growing
    boss4Obj.largeFlameScale += 0.05f;
    if (boss4Obj.largeFlameScale >= 1.0f) {
      boss4Obj.largeFlameScale = 1.0f;
      boss4Obj.largeFlamePhase = 2; // Stay Large
    }
  } else if (boss4Obj.largeFlamePhase == 3) { // Shrinking
    boss4Obj.largeFlameScale -= 0.05f;
    if (boss4Obj.largeFlameScale <= 0.0f) {
      boss4Obj.largeFlameScale = 0.0f;
      boss4Obj.largeFlamePhase = 0; // Back to Small
    }
  }
}

/**
 * Handles the high-level Level 4 logic, including transitions to the boss
 * fight.
 */
inline void updateLevel4Boss() {
  extern int currentLevel;
  extern GameState gameState;
  extern bool isGamePaused;
  extern int level4Phase;

  if (currentLevel != 4 || gameState != GAME || isGamePaused)
    return;

  if (level4Phase == 1) {
    // Background scroll handles shifting to phase 2
  } else if (level4Phase == 2) {
    // Boss walks in
    if (boss4Obj.x > 750) {
      boss4Obj.x -= 6;
      updateFinalBossLogic();
    } else {
      boss4Obj.x = 750;
      level4Phase = 3;
      boss4Obj.frameIndex = 0;
      boss4Obj.animCounter = 0;
    }
  } else if (level4Phase == 3 || level4Phase == 4) {
    updateFinalBossLogic();
  }
}

/**
 * Handles player attacking the boss.
 */
inline void handleBossHit() {
  extern int level4Phase;
  extern int charX, charY, charWidth, charHeight;
  extern bool isAttacking, npcSlashDone;
  extern int attackFrameIndex;

  if (!boss4Obj.active || level4Phase < 2 || boss4Obj.life <= 0)
    return;

  // Boss is invulnerable while vanishing (Skill 0 phases 1-3)
  if (boss4Obj.currentSkill == 0 &&
      (boss4Obj.skillState >= 1 && boss4Obj.skillState <= 3))
    return;

  // Boss is invulnerable while any bird from Skill 8 is alive (not
  // dying/fleeing) AND during the 12-second sat phase
  if (boss4Obj.currentSkill == 8) {
    if (boss4Obj.hitgroundWaitTimer > 0 && boss4Obj.hitgroundWaitTimer <= 396)
      return;

    for (int b = 0; b < 4; b++) {
      if (boss4Obj.holeBirds[b].active && boss4Obj.holeBirds[b].state != 3 &&
          boss4Obj.holeBirds[b].state != 4)
        return;
    }
  }

  // Boss is invulnerable during rest skill while any NPC or big NPC is alive
  if (boss4Obj.currentSkill == 7 && boss4Obj.restSkillState == 2) {
    if (boss4Obj.restBigNpc.active)
      return;
    for (int i = 0; i < 3; i++) {
      if (boss4Obj.restNpcs[i].active)
        return;
    }
  }

  if (isAttacking && attackFrameIndex == 3 && !npcSlashDone) {
    // Character's center
    float charCenterX = (float)charX + (float)charWidth / 2.0f;

    // Boss's horizontal middle point
    float bossMidX = boss4Obj.x + (float)BOSS_WIDTH / 2.0f;

    // Hit if character center is within 180px of boss middle point
    float dist = charCenterX - bossMidX;
    if (dist < 0)
      dist = -dist; // absolute distance

    if (dist <= 180.0f) {
      boss4Obj.life -= 5;
      spawnDamagePopup(5, bossMidX, boss4Obj.y + BOSS_HEIGHT / 2.0f);
      npcSlashDone = true;
      boss4Obj.isHit = true;
      boss4Obj.hitTimer = 15;

      if (boss4Obj.life <= 0) {
        boss4Obj.life = 0;
        level4Phase = 4; // Victory sequence
        boss4Obj.dieFrameIndex = 0;
        boss4Obj.dieAnimCounter = 0;
      }
    }
  }
}

// Custom wrapper for flipping based on state
inline void iDrawBossImage(int x, int y, int w, int h, int img) {
  if (boss4Obj.knockbackState == 1) {
    // Only override if the image being drawn isn't a flame segment
    bool isFlame = false;
    for (int i = 0; i < 11; i++) {
      if (img == bossFlameImgs[i]) {
        isFlame = true;
        break;
      }
    }
    if (!isFlame) {
      img = bossBackImgs[boss4Obj.knockbackFrameIndex];
    }
  }

  if (boss4Obj.facingLeft) {
    // Assuming images face LEFT by default
    iShowImage(x, y, w, h, img);
  } else {
    // Flip to face RIGHT
    iShowImageHFlip(x, y, w, h, img);
  }
}

/**
 * Renders the boss and the continuous flame effect at its base.
 * Flames are drawn AFTER the boss body so they always appear in FRONT.
 * Flame mid-point (fY + fH/2) is anchored within 5px of the entity's feet.
 */
inline void drawBoss() {
  if (!boss4Obj.active)
    return;
  extern int level4Phase;
  iSetColor(255, 255, 255); // Fix: Reset color to white so images don't inherit
                            // orange from life bar logic

  // ---------------------------------------------------------------------------
  // STEP 1: Draw the BOSS BODY first (flames will be drawn on top after)
  // ---------------------------------------------------------------------------
  if (boss4Obj.isHit && (boss4Obj.hitTimer / 5) % 2 == 0) {
    // Blink effect when hit
  } else {
    if (level4Phase == 4) {
      iShowImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH, BOSS_HEIGHT,
                 bossDieImgs[boss4Obj.dieFrameIndex]);
    } else if (level4Phase == 5) {
      // No boss rendering in victory
    } else if (level4Phase == 3 && boss4Obj.currentSkill != -1) {
      // SKILL 0: VANISH RENDERING
      if (boss4Obj.currentSkill == 0) {
        if (boss4Obj.skillState == 1 || boss4Obj.skillState == 3) {
          iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH,
                         BOSS_HEIGHT, bossVanishImgs[boss4Obj.skillFrameIndex]);
        } else if (boss4Obj.skillState == 4) {
          iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH,
                         BOSS_HEIGHT,
                         bossSmashImgs[boss4Obj.skillFrameIndex % 10]);
        } else if (boss4Obj.skillState == 5) {
          iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH,
                         BOSS_HEIGHT, bossWalkImgs[boss4Obj.frameIndex]);
        }
      }

      // SKILL 1: JUMP ATTACK RENDERING
      if (boss4Obj.currentSkill == 1) {
        if (boss4Obj.jumpSkillState == 1) {
          iDrawBossImage((int)(boss4Obj.x + boss4Obj.shakeX),
                         (int)(boss4Obj.y + boss4Obj.shakeY), BOSS_WIDTH,
                         BOSS_HEIGHT, bossStandImgs[boss4Obj.frameIndex]);
        } else if (boss4Obj.jumpSkillState == 2) {
          iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH,
                         BOSS_HEIGHT, bossJumpImgs[boss4Obj.jumpFrameIndex]);
        } else if (boss4Obj.jumpSkillState == 3) {
          iDrawBossImage((int)(boss4Obj.x + boss4Obj.shakeX),
                         (int)(boss4Obj.y + boss4Obj.shakeY), BOSS_WIDTH,
                         BOSS_HEIGHT, bossSmashImgs[boss4Obj.jumpLandingFrame]);
        } else if (boss4Obj.jumpSkillState == 4) {
          iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH,
                         BOSS_HEIGHT, bossWalkImgs[boss4Obj.frameIndex]);
        }
      }

      // SKILL 2: PULL ATTACK RENDERING
      if (boss4Obj.currentSkill == 2) {
        if (boss4Obj.pullSkillState == 1) {
          iDrawBossImage((int)(boss4Obj.x + boss4Obj.shakeX),
                         (int)(boss4Obj.y + boss4Obj.shakeY), BOSS_WIDTH,
                         BOSS_HEIGHT, bossStandImgs[boss4Obj.frameIndex]);
        } else if (boss4Obj.pullSkillState == 2) {
          iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH,
                         BOSS_HEIGHT, bossPullImgs[boss4Obj.pullFrameIndex]);
        } else if (boss4Obj.pullSkillState == 3) {
          iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH,
                         BOSS_HEIGHT,
                         bossPullDanceImgs[boss4Obj.pullDanceFrame]);
          iSetColor(200, 200, 255);
          for (int i = 0; i < 15; i++) {
            iFilledRectangle((int)boss4Obj.pullParticlesX[i],
                             (int)boss4Obj.pullParticlesY[i], 30, 2);
            iFilledCircle((int)boss4Obj.pullParticlesX[i] + 30,
                          (int)boss4Obj.pullParticlesY[i] + 1, 1);
          }
        }
      }

      // SKILL 3: PUSH ATTACK RENDERING
      if (boss4Obj.currentSkill == 3) {
        if (boss4Obj.pushSkillState == 1) {
          iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH,
                         BOSS_HEIGHT, bossStandImgs[boss4Obj.frameIndex]);
        } else if (boss4Obj.pushSkillState == 2 ||
                   boss4Obj.pushSkillState == 3) {
          iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH,
                         BOSS_HEIGHT, bossPushImgs[boss4Obj.pushFrameIndex]);
        } else if (boss4Obj.pushSkillState == 5) {
          int renderIdx = boss4Obj.pushFrameIndex;
          if (renderIdx > 7)
            renderIdx = 7;
          iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH,
                         BOSS_HEIGHT, bossPushActiveImgs[renderIdx]);
        } else if (boss4Obj.pushSkillState == 4) {
          iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH,
                         BOSS_HEIGHT, bossWalkImgs[boss4Obj.frameIndex]);
        }
      }

      // SKILL 4: ENEMY ATTACK RENDERING
      if (boss4Obj.currentSkill == 4) {
        if (boss4Obj.enemySkillState == 1) {
          iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH,
                         BOSS_HEIGHT, bossStandImgs[boss4Obj.frameIndex]);
        } else if (boss4Obj.enemySkillState == 2) {
          iDrawBossImage((int)(boss4Obj.x + boss4Obj.shakeX),
                         (int)(boss4Obj.y + boss4Obj.shakeY), BOSS_WIDTH,
                         BOSS_HEIGHT, bossStandImgs[boss4Obj.frameIndex]);
        } else if (boss4Obj.enemySkillState >= 3) {
          iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH,
                         BOSS_HEIGHT,
                         bossPreJumpImgs[boss4Obj.enemyFrameIndex]);
        }

        if (boss4Obj.enemySkillState >= 4) {
          for (int i = 0; i < 5; i++) {
            if (boss4Obj.bossEnemies[i].active) {
              int eImg = spiderImgs[boss4Obj.bossEnemies[i].frame];
              iShowImage((int)boss4Obj.bossEnemies[i].x,
                         (int)boss4Obj.bossEnemies[i].y, 100, 40, eImg);
            }
          }
          // Draw Bats
          for (int i = 0; i < 10; i++) {
            if (boss4Obj.bats[i].active) {
              if (boss4Obj.bats[i].hitFlashTimer > 0 &&
                  (boss4Obj.bats[i].hitFlashTimer / 3) % 2 == 1)
                continue;

              int bImg = batImgs[boss4Obj.bats[i].frame];
              if (boss4Obj.bats[i].facingLeft) {
                iShowImage((int)boss4Obj.bats[i].x, (int)boss4Obj.bats[i].y, 75,
                           75, bImg);
              } else {
                iShowImageHFlip((int)boss4Obj.bats[i].x, (int)boss4Obj.bats[i].y,
                                75, 75, bImg);
              }
            }
          }
        }
      }

      // SKILL 5: TOP ATTACK RENDERING
      if (boss4Obj.currentSkill == 5) {
        if (boss4Obj.topSkillState == 1) {
          iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH,
                         BOSS_HEIGHT, bossHighImgs[boss4Obj.topFrameIndex]);
        } else if (boss4Obj.topSkillState == 2) {
          iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH,
                         BOSS_HEIGHT, bossThunderImgs[boss4Obj.topFrameIndex]);
          for (int i = 0; i < 10; i++) {
            if (boss4Obj.topObstacles[i].active) {
              int *obsImgs = (boss4Obj.topObstacles[i].type == 0)
                                 ? highobs1Imgs
                                 : highobs2Imgs;
              iShowImage((int)boss4Obj.topObstacles[i].x,
                         (int)boss4Obj.topObstacles[i].y, 90, 90,
                         obsImgs[boss4Obj.topObstacles[i].frame]);
            }
          }
        }
      }

      // SKILL 6: FIRE ATTACK RENDERING (Swapped)
      if (boss4Obj.currentSkill == 6) {
        if (boss4Obj.fireSkillState == 1) {
          iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH,
                         BOSS_HEIGHT,
                         bossFireMotionImgs[boss4Obj.fireFrameIndex]);
        } else {
          iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH,
                         BOSS_HEIGHT, bossStandImgs[boss4Obj.frameIndex]);
        }
        if (boss4Obj.fbActive) {
          iShowImage((int)boss4Obj.fbX, (int)boss4Obj.fbY, 80, 80,
                     fireballImgs[boss4Obj.fbFrameIndex]);
        }
      }

      // SKILL 7: REST ATTACK RENDERING (Swapped)
      if (boss4Obj.currentSkill == 7) {
        iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH,
                       BOSS_HEIGHT, bossRestSkillImgs[boss4Obj.restFrameIndex]);

        // Draw Walking NPCs
        extern int npc1Walk[9], npc1Attack[9], npc2Walk[9], greenFireImgs[9];
        for (int i = 0; i < 3; i++) {
          if (boss4Obj.restNpcs[i].active) {
            if (boss4Obj.restNpcs[i].hitFlashTimer > 0 &&
                (boss4Obj.restNpcs[i].hitFlashTimer / 3) % 2 == 0) {
              // Blink on hit
            } else {
              int nImg;
              if (boss4Obj.restNpcs[i].type == 1) {
                nImg = (boss4Obj.restNpcs[i].state == 1)
                           ? npc1Attack[boss4Obj.restNpcs[i].animFrame % 9]
                           : npc1Walk[boss4Obj.restNpcs[i].animFrame % 9];
              } else {
                nImg = npc2Walk[boss4Obj.restNpcs[i].animFrame % 9];
              }
              int nw = 100, nh = 100;
              if (boss4Obj.restNpcs[i].type == 1 &&
                  boss4Obj.restNpcs[i].state == 0) {
                nw = 85;
                nh = 85;
              }
              iShowImage((int)boss4Obj.restNpcs[i].x,
                         (int)boss4Obj.restNpcs[i].y, nw, nh, nImg);
            }
          }
          // Draw green fire projectiles
          if (boss4Obj.restFires[i].active) {
            iShowImage((int)boss4Obj.restFires[i].x,
                       (int)boss4Obj.restFires[i].y, 100, 100,
                       greenFireImgs[boss4Obj.restFires[i].animFrame % 9]);
          }
        }

        // Draw Big NPC
        if (boss4Obj.restBigNpc.active) {
          if (boss4Obj.restBigNpc.hitFlashTimer > 0 &&
              (boss4Obj.restBigNpc.hitFlashTimer / 3) % 2 == 0) {
            // Blink on hit
          } else {
            int bImg;
            if (boss4Obj.restBigNpc.state == 1) {
              bImg = bigNpcAttackImgs[boss4Obj.restBigNpc.animFrame % 9];
            } else {
              bImg = bigNpcWalkImgs[boss4Obj.restBigNpc.animFrame % 9];
            }
            iShowImage((int)boss4Obj.restBigNpc.x, (int)boss4Obj.restBigNpc.y,
                       125, 125, bImg);
          }
        }
      }

      // SKILL 8: HITGROUND ATTACK RENDERING
      if (boss4Obj.currentSkill == 8) {
        if (boss4Obj.hitgroundSkillState == 1) {
          iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH,
                         BOSS_HEIGHT,
                         bossShoutImgs[boss4Obj.hitgroundFrameIndex]);
        } else if (boss4Obj.hitgroundSkillState >= 2) {
          iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH,
                         BOSS_HEIGHT,
                         bossHitgroundImgs[boss4Obj.hitgroundFrameIndex]);
        }

        // Draw holes
        for (int i = 0; i < 4; i++) {
          if (boss4Obj.bossHoles[i].active) {
            iShowImage((int)boss4Obj.bossHoles[i].x,
                       (int)boss4Obj.bossHoles[i].y, 180, 70,
                       bossHoleImgs[boss4Obj.bossHoles[i].frame]);
          }
        }

        // Draw birds (Volplex / Artikuno)
        for (int b = 0; b < 4; b++) {
          if (!boss4Obj.holeBirds[b].active)
            continue;

          if (boss4Obj.holeBirds[b].hitFlashTimer > 0 &&
              (boss4Obj.holeBirds[b].hitFlashTimer / 2) % 2 == 0) {
            continue; // Blink effect
          }

          int bW = (int)(100 * boss4Obj.holeBirds[b].scale);
          int bH = (int)(80 * boss4Obj.holeBirds[b].scale);
          int bX = (int)boss4Obj.holeBirds[b].x - bW / 2;
          int bY = (int)boss4Obj.holeBirds[b].y - bH / 2;

          int *birdFrames =
              (boss4Obj.holeBirds[b].type == 0) ? artikunoImgs : volplexImgs;
          int frameIdx = boss4Obj.holeBirds[b].frame;

          bool flipHorizontally = false;
          // flip logic based on movement direction (velX)
          if (boss4Obj.holeBirds[b].type == 0) {
            // Artikuno natively faces RIGHT (flip if moving left)
            if (boss4Obj.holeBirds[b].velX < -0.1f)
              flipHorizontally = true;
          } else {
            // Volplex natively faces LEFT (flip if moving right)
            if (boss4Obj.holeBirds[b].velX > 0.1f)
              flipHorizontally = true;
          }

          if (boss4Obj.holeBirds[b].state == 3) {
            // DYING: draw upside-down (vertical flip) to show death tumble
            if (flipHorizontally) {
              iShowImageHVFlip(bX, bY, bW, bH, birdFrames[frameIdx]);
            } else {
              iShowImageVFlip(bX, bY, bW, bH, birdFrames[frameIdx]);
            }

            // Draw particle effects specifically around dying birds
            if (boss4Obj.holeBirds[b].type == 0) {
              // Snowflakes (Artikuno)
              for (int p = 0; p < 12; p++) {
                int col = rand() % 4;
                if (col == 0)
                  iSetColor(255, 255, 255); // White
                else if (col == 1)
                  iSetColor(230, 240, 255); // Light Blue
                else if (col == 2)
                  iSetColor(200, 230, 255); // Cyany Blue
                else
                  iSetColor(255, 250, 250); // Snow
                int px = bX - 10 + (rand() % (bW + 20));
                int py = bY - 10 + (rand() % (bH + 20));

                double xs[4], ys[4];
                for (int k = 0; k < 4; k++) {
                  double angle =
                      1.5708 * k + ((rand() % 10) * 0.05); // random tilt
                  double r =
                      1.0 +
                      (rand() % 15) *
                          0.1; // tiny erratic radius (1.0 to 2.5) // size small
                  xs[k] = (double)px + r * cos(angle);
                  ys[k] = (double)py + r * sin(angle);
                }
                iFilledPolygon(xs, ys, 4); // topsy turvy shard!
              }
            } else {
              // Fire shards (Volplex)
              for (int p = 0; p < 12;
                   p++) { // synced to 12 exactly like Artikuno
                int col = rand() % 5;
                if (col == 0)
                  iSetColor(255, 69, 0); // Orange Red
                else if (col == 1)
                  iSetColor(255, 140, 0); // Dark Orange
                else if (col == 2)
                  iSetColor(255, 0, 0); // Red
                else if (col == 3)
                  iSetColor(255, 215, 0); // Gold Yellow
                else
                  iSetColor(255, 255, 200); // White core fire
                int px = bX - 10 + (rand() % (bW + 20));
                int py = bY - 10 + (rand() % (bH + 20));

                double xs[4], ys[4];
                for (int k = 0; k < 4; k++) {
                  double angle =
                      1.5708 * k + ((rand() % 10) * 0.05); // random tilt
                  double r =
                      1.0 +
                      (rand() % 15) *
                          0.1; // tiny erratic radius (1.0 to 2.5) // size small
                  xs[k] = (double)px + r * cos(angle);
                  ys[k] = (double)py + r * sin(angle);
                }
                iFilledPolygon(xs, ys, 4); // topsy turvy shard!
              }
            }
            iSetColor(255, 255, 255); // Reset color
          } else {
            if (flipHorizontally) {
              iShowImageHFlip(bX, bY, bW, bH, birdFrames[frameIdx]);
            } else {
              iShowImage(bX, bY, bW, bH, birdFrames[frameIdx]);
            }
          }
        }
      }
    } else {
      // Normal phase animations
      if (level4Phase == 2) {
        iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH,
                       BOSS_HEIGHT, bossWalkImgs[boss4Obj.frameIndex]);
      } else if (level4Phase == 3) {
        iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH,
                       BOSS_HEIGHT, bossStandImgs[boss4Obj.frameIndex % 6]);
      } else {
        iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH,
                       BOSS_HEIGHT, bossImgs[boss4Obj.frameIndex % 4]);
      }
    }
  }

  // ---------------------------------------------------------------------------
  // STEP 2: Draw BOSS BASE FLAME on top of the boss body (IN FRONT)
  // Rule: flame mid-point (fY + fH/2) == anchorY
  //   anchorY is ALWAYS boss4Obj.y - 3  (3 px below boss feet, at all times,
  //   including while the boss is airborne during a jump). This keeps the
  //   flame smoothly at foot level and never lets it rise into the belly.
  // Y offsets: regular base flame +3, large flame additional +10.
  // ---------------------------------------------------------------------------
  if (level4Phase < 4) {
    int fW = BOSS_FLAME_W;
    int fH = BOSS_FLAME_H;

    // Always anchor at boss feet - 3, regardless of jump state
    float anchorY = boss4Obj.y - 3.0f;

    // Base mid-point placement: fY + fH/2 = anchorY
    int fY = (int)(anchorY - fH / 2.0f);
    int fX = (int)boss4Obj.x + BOSS_FLAME_OFFSET_X;

    // Regular (base) flame: shift up by +3 on Y axis
    fY += 3;

    // Large flame: further reduced sizes (extraH: 40, extraW: 35, Xshift: 15)
    if (boss4Obj.largeFlameScale > 0.01f) {
      int extraH = (int)(40 * boss4Obj.largeFlameScale);
      int extraW = (int)(35 * boss4Obj.largeFlameScale);
      fH += extraH;
      fW += extraW;
      fX -= (int)(15 * boss4Obj.largeFlameScale);
      // Re-anchor after size change so midpoint remains at anchorY
      fY = (int)(anchorY - fH / 2.0f);
      // Large flame gets base +3 AND an extra +10 = +13 total Y shift
      fY += 13;
    }

    // Pre-jump anticipation boost (trimmed down: 25/15/8)
    if (boss4Obj.jumpSkillState == 1) {
      fH += 25;
      fW += 15;
      fX -= 8;
      fY = (int)(anchorY - fH / 2.0f) + 3; // re-anchor + base +3 shift
    }

    iSetColor(255, 255, 255);
    iDrawBossImage(fX, fY, fW, fH, bossFlameImgs[boss4Obj.flameFrameIndex]);
  }

  // ---------------------------------------------------------------------------
  // STEP 3: Character on-fire flame is now drawn by drawCharFireFlame()
  // which is called AFTER drawCharacter() in iMain.cpp so it renders
  // IN FRONT of the character sprite.
  // ---------------------------------------------------------------------------

  // 5. Boss Life HUD - LAYERED (Orange Bar BEHIND the Frame)
  if (level4Phase < 4) {
    int lifeFrameX = 720;
    int lifeFrameY = 516; // Raised by 16 units (total)
    int lifeFrameW = 250;
    int lifeFrameH = 80;

    float lifePercent = (float)boss4Obj.life / (float)boss4Obj.maxLife;
    if (lifePercent < 0)
      lifePercent = 0;

    // --- STEP 1: DRAW ORANGE LINE (BEHIND) ---
    iSetColor(255, 100, 0); // Deep Orange
    float totalWidth = 170.0f;
    float drawWidth = totalWidth * lifePercent;
    // Decrease from the left side: the left edge moves right (empty space
    // starts at left)
    float drawX = (float)lifeFrameX + 44.0f + (totalWidth - drawWidth);
    iFilledRectangle((double)drawX, (double)lifeFrameY + 37.0,
                     (double)drawWidth, 10.0);

    // --- STEP 2: DRAW FRAME (ON TOP) ---
    iShowImage(lifeFrameX, lifeFrameY, lifeFrameW, lifeFrameH,
               bossLifeFrameImg);
  }
}

/**
 * Draws the character on-fire flame effect.
 * MUST be called AFTER drawCharacter() so the flame renders IN FRONT of
 * the character sprite (painter's algorithm: later = on top).
 * Y is nudged +10 upward from the midpoint anchor so the flame visually
 * wraps around the character body rather than sitting at feet level.
 * Only active during the fireball attack (Skill 6) when charOnFire == true.
 */
inline void drawCharFireFlame() {
  if (!boss4Obj.charOnFire)
    return;
  extern int currentLevel;
  extern int level4Phase;
  if (currentLevel != 4)
    return;

  extern int charX, charY, charWidth, charHeight;
  int cfW = charWidth + 60;
  int cfH = charHeight + 40;
  int cfX = charX - 30;
  // Midpoint anchor at char feet, then shift up +10 as requested
  int cfY = (int)((float)charY - cfH / 2.0f) + 10;
  iSetColor(255, 255, 255);
  iShowImage(cfX, cfY, cfW, cfH, bossFlameImgs[boss4Obj.flameFrameIndex]);
}

#endif
