#ifndef BOSS_HEADER_H
#define BOSS_HEADER_H

#include "iGraphics.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "GameState.h"

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
SELECTANY int bossPullImgs[6];
SELECTANY int bossPullDanceImgs[4];
SELECTANY int bossPushImgs[8];
SELECTANY int bossHighImgs[9];
SELECTANY int bossThunderImgs[4];
SELECTANY int bossRestSkillImgs[7];
SELECTANY int bg41, bg42;
SELECTANY int bossLifeFrameImg;

SELECTANY int worm11, worm12, worm13, worm14;
SELECTANY int worm21, worm22, worm23, worm24;

struct BossEnemyObj {
    float x, y;
    bool active;
    int type; // 0 for worm1, 1 for worm2
    int frame;
    int frameTimer;
};

struct BossTopObstacle {
    float x, y;
    float speedY;
    bool active;
    int frame;
    int frameTimer;
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

    // Vanish Skill State
    int skillState; // 0: Idle, 1: Vanishing, 2: Hidden (Wait 2s), 3: Appearing (at char), 4: Attacking, 5: Gap (Before next)
    int skillTimer;
    int skillFrameIndex;
    int skillCount;
    float originalX, originalY;
    bool facingLeft;

    // Fire Attack Skill State
    int fireSkillState; // 0: Idle, 1: Fire Animation, 2: Fireball Traveling, 3: Gap
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
    int jumpSkillState; // 0: Idle, 1: Pre-Jump, 2: Jump/Motion (1.7s), 3: Landing Effect (attack11-16 + flame)
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
    int pullSkillState; // 0: Idle, 1: High Shake, 2: Initial Pull Prep, 3: Pulling Phase
    int pullSkillTimer;
    int pullFrameIndex;
    int pullDanceFrame;
    float pullParticlesX[15];
    float pullParticlesY[15];
    float pullParticlesSpd[15];
    int pullSkillCount;
    int pullTargetCount;

    // Push Attack Skill State
    int pushSkillState; // 0: Idle, 1: 3 small jumps, 2: Prep motion, 3: Push execution, 4: Return
    int pushSkillTimer;
    int pushFrameIndex;
    int pushJumpCount;
    int pushCount;
    int pushTargetCount;
    float pushStartY;
    float pushTargetX;
    int pushDir;

    // Enemy Attack Skill State
    int enemySkillState; // 0: Idle, 1: Small jump, 2: High shake, 3: Prep motion, 4: Spawn, 5: Return
    int enemySkillTimer;
    int enemyFrameIndex;
    int enemySpawnTimer;
    float enemyStartY;
    BossEnemyObj bossEnemies[5];

    // Top Attack Skill State
    int topSkillState; // 0: Idle, 1: High motion, 2: Thunder motion (15s)
    int topSkillTimer;
    int topFrameIndex;
    int topSpawnTimer;
    BossTopObstacle topObstacles[10];

    // Rest Attack Skill State
    int restSkillState; // 0: Idle, 1: First motion (rest11-rest14), 2: Second motion (rest15-rest17)
    int restSkillTimer;
    int restFrameIndex;

    // Skill Scheduler
    int currentSkill;       // -1: Choosing, 0-7: Active
    bool skillUsed[8];      // Track which skills have been used in current cycle
    int skillsCompleted;    // Count of skills finished in current cycle
    int skillGapTimer;      // Delay between skills
    int skillSequence[8];   // Shuffled array of indices 0-7
    int sequenceIndex;      // Current position in shuffled sequence
    int safeguardTimer;     // Timeout to prevent "stuck" skills

    // Smooth Flame Scaling
    float largeFlameScale;  // 0.0 to 1.0
    int largeFlamePhase;    // 0: Small, 1: Growing, 2: Large, 3: Shrinking
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
    if (bossAssetsLoaded) return;
    bossAssetsLoaded = true;
    
    char path[100];
    
    // Load Layered Level 4 Backgrounds
    bg41 = iLoadImage((char*)"Boss\\bg41.png");
    bg42 = iLoadImage((char*)"Boss\\bg42.png");
    
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

    for (int i = 0; i < 6; i++) {
        sprintf_s(path, sizeof(path), "Boss\\pull (%d).png", i + 1);
        bossPullImgs[i] = iLoadImage((char *)path);
    }

    for (int i = 0; i < 4; i++) {
        sprintf_s(path, sizeof(path), "Boss\\pulldance (%d).png", i + 1);
        bossPullDanceImgs[i] = iLoadImage((char *)path);
    }

    for (int i = 0; i < 8; i++) {
        sprintf_s(path, sizeof(path), "Boss\\push%d.png", i + 11);
        bossPushImgs[i] = iLoadImage((char *)path);
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
    
    bossLifeFrameImg = iLoadImage((char*)"Boss\\showbosslife.png");
}

/**
 * Shuffles the boss's skill sequence to ensure random but unique cycles.
 */
inline void shuffleBossSkills() {
    for (int i = 0; i < 8; i++) boss4Obj.skillSequence[i] = i;
    for (int i = 7; i > 0; i--) {
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
    boss4Obj.y = 83.0f;  // Anchored to Level 4 ground
    boss4Obj.life = 100;
    boss4Obj.maxLife = 100;
    boss4Obj.facingLeft = true; 
    boss4Obj.largeFlameScale = 0.0f;
    boss4Obj.largeFlamePhase = 0;
    // Pull Attack Initialized to avoid 0 speed junk
    for (int i = 0; i < 15; i++) {
        boss4Obj.pullParticlesX[i] = (float)(rand() % 1000);
        boss4Obj.pullParticlesY[i] = (float)(rand() % 500 + 50);
        boss4Obj.pullParticlesSpd[i] = (float)(rand() % 10 + 10);
    }
    boss4Obj.pullTargetCount = 3 + rand() % 3; 

    // Push Attack target count
    boss4Obj.pushTargetCount = 2 + rand() % 2; 

    // Fire Attack target count
    boss4Obj.fireAttackTarget = 2 + rand() % 4;

    // Scheduler Init
    boss4Obj.currentSkill = -1;
    boss4Obj.skillGapTimer = 10;
    boss4Obj.safeguardTimer = 0;
    shuffleBossSkills();
}

/**
 * Updates boss and flame animations.
 * Renamed to avoid collisions with other headers.
 */
inline void updateFinalBossLogic() {
    if (!boss4Obj.active) return;
    extern int level4Phase;
    
    // Boss breathing/resting animation speed
    boss4Obj.animCounter++;
    
    int animLimit = 10;
    if (level4Phase == 2) animLimit = 5; // Faster walk anim

    if (boss4Obj.animCounter >= animLimit) {
        boss4Obj.animCounter = 0;
        if (level4Phase == 2) {
            boss4Obj.frameIndex = (boss4Obj.frameIndex + 1) % 8;
            boss4Obj.facingLeft = true; 
        } else if (level4Phase == 3) {
            boss4Obj.frameIndex = (boss4Obj.frameIndex + 1) % 6; // bossStandImgs has 6
        } else {
            boss4Obj.frameIndex = (boss4Obj.frameIndex + 1) % 4; // bossImgs has 4
        }
    }
    
    // Flame flicker animation speed (using all 11 frames)
    boss4Obj.flameAnimCounter++;
    if (boss4Obj.flameAnimCounter >= 5) { // Adjust speed as needed
        boss4Obj.flameAnimCounter = 0;
        boss4Obj.flameFrameIndex = (boss4Obj.flameFrameIndex + 1) % 11; // Loop all 11 frames
    }
    
    if (boss4Obj.isHit) {
        boss4Obj.hitTimer--;
        if (boss4Obj.hitTimer <= 0) boss4Obj.isHit = false;
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
            if (boss4Obj.safeguardTimer > 1150) { 
                boss4Obj.currentSkill = -1;
                boss4Obj.skillGapTimer = 30;
                boss4Obj.safeguardTimer = 0;
            }
        }

        // Only allow picking a new skill if level4Phase == 3 (true fight) AND the current one is truly finished (-1)
        if (level4Phase == 3 && boss4Obj.currentSkill == -1) {
            boss4Obj.skillGapTimer--;
            if (boss4Obj.skillGapTimer <= 0) {
                // Reset safeguard for the new skill
                boss4Obj.safeguardTimer = 0;

                // If we finished the current shuffled sequence, shuffle again
                if (boss4Obj.sequenceIndex >= 8) {
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
                if (pick == 0) { boss4Obj.skillState = 1; boss4Obj.skillCount = 0; boss4Obj.skillFrameIndex = 0; }
                else if (pick == 1) { boss4Obj.jumpSkillState = 1; boss4Obj.jumpSkillTimer = 0; boss4Obj.jumpSkillCount = 0; }
                else if (pick == 2) { boss4Obj.pullSkillState = 1; boss4Obj.pullSkillTimer = 0; boss4Obj.pullSkillCount = 0; }
                else if (pick == 3) { boss4Obj.pushSkillState = 1; boss4Obj.pushSkillTimer = 0; boss4Obj.pushJumpCount = 0; boss4Obj.pushCount = 0; boss4Obj.pushStartY = boss4Obj.y; }
                else if (pick == 4) { boss4Obj.enemySkillState = 1; boss4Obj.enemySkillTimer = 0; boss4Obj.enemyStartY = boss4Obj.y; }
                else if (pick == 5) { boss4Obj.topSkillState = 1; boss4Obj.topSkillTimer = 0; }
                else if (pick == 6) { boss4Obj.fireSkillState = 1; boss4Obj.fireAttackCount = 0; boss4Obj.fireSkillTimer = 0; boss4Obj.fireAttackTarget = 2 + rand() % 4; }
                else if (pick == 7) { boss4Obj.restSkillState = 1; boss4Obj.restSkillTimer = 0; boss4Obj.restFrameIndex = 0; }
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
        }
        else if (boss4Obj.skillState == 2) { // Hidden
            boss4Obj.skillTimer++;
            if (boss4Obj.skillTimer > 30) { // Slightly faster hidden phase (was 50)
                boss4Obj.skillState = 3; // Appearing
                boss4Obj.skillTimer = 0;
                boss4Obj.skillFrameIndex = 7; // Start from last frame backwards
                boss4Obj.x = (float)charX - 50;
                boss4Obj.y = 83.0f;
                boss4Obj.largeFlamePhase = 1; // Start GROWING flame when appearing
                // Face the character when reappearing
                boss4Obj.facingLeft = (charX <= (int)boss4Obj.x);
            }
        }
        else if (boss4Obj.skillState == 3) { // Appearing
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
        }
        else if (boss4Obj.skillState == 4) { // Smash Attack at character site
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
            if (charX + charWidth - 40 > (smashCenterX - totalWidth/2.0f) && charX + 40 < (smashCenterX + totalWidth/2.0f) &&
                charY + charHeight > boss4Obj.y && charY < boss4Obj.y + BOSS_HEIGHT) {
                if (!isInvincible) {
                    lives--;
                    if (lives <= 0) { lives = 0; gameState = GAME_OVER; }
                    else { isInvincible = true; invincibilityTimer = 60; }
                }
            }

            if (boss4Obj.skillFrameIndex >= 10) {
                boss4Obj.skillCount++;
                if (boss4Obj.skillCount < 3) {
                    boss4Obj.skillState = 1; // Repeat Vanish (Vanishes from current spot)
                    boss4Obj.skillFrameIndex = 0;
                    boss4Obj.skillTimer = 0;
                    boss4Obj.largeFlameScale = 0.0f; // Reset scale for next appearance smooth growth
                    boss4Obj.largeFlamePhase = 0;    // Reset phase for next appearance growth transition
                } else {
                    boss4Obj.skillState = 5; // Final Return to base
                    boss4Obj.skillTimer = 0;
                    boss4Obj.skillFrameIndex = 0;
                    boss4Obj.largeFlamePhase = 3; // SHRINK flame when finishing attack
                }
            }
        }
        else if (boss4Obj.skillState == 5) { // Return
            boss4Obj.skillTimer++;
            float returnSpeed = 12.0f;
            if (boss4Obj.x < 750) {
                boss4Obj.x += returnSpeed;
                boss4Obj.facingLeft = false;
                if (boss4Obj.x > 750) boss4Obj.x = 750;
            } else if (boss4Obj.x > 750) {
                boss4Obj.x -= returnSpeed;
                boss4Obj.facingLeft = true;
                if (boss4Obj.x < 750) boss4Obj.x = 750;
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

            if (boss4Obj.jumpSkillTimer >= 5) { 
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
        }
        else if (boss4Obj.jumpSkillState == 2) {
            // State 2: Parabolic Jump
            boss4Obj.jumpSkillTimer++;
            float dur = 57.0f; 
            float t = boss4Obj.jumpSkillTimer / dur;
            
            // Face the character at all times while airborne
            boss4Obj.facingLeft = (charX <= (int)boss4Obj.x);

            if (t <= 1.0f) {
                boss4Obj.x = boss4Obj.jumpStartX + (boss4Obj.jumpTargetX - boss4Obj.jumpStartX) * t;
                boss4Obj.y = boss4Obj.jumpStartY + (float)sin(3.14159 * t) * 250.0f;
                if (boss4Obj.y < boss4Obj.jumpTargetY && t > 0.5f) boss4Obj.y = boss4Obj.jumpTargetY;

                boss4Obj.jumpFrameIndex = (int)(t * 8);
                if (boss4Obj.jumpFrameIndex > 7) boss4Obj.jumpFrameIndex = 7;
            } else {
                boss4Obj.y = boss4Obj.jumpTargetY;
                boss4Obj.jumpSkillState = 3; // Smash motion
                boss4Obj.jumpSkillTimer = 0;
                boss4Obj.jumpLandingFrame = 0;
                boss4Obj.landingFlameTimer = 60; // Slightly reduced duration since aura is gone
                boss4Obj.shakeX = 15;
                boss4Obj.shakeY = 15;
                boss4Obj.largeFlamePhase = 1; // GROW flame on landing
            }
        }
        else if (boss4Obj.jumpSkillState == 3) {
            // State 3: SMASH Motion
            boss4Obj.jumpSkillTimer++;
            
            if (boss4Obj.jumpSkillTimer >= 5) { 
                boss4Obj.jumpSkillTimer = 0;
                boss4Obj.jumpLandingFrame++;
                if (boss4Obj.jumpLandingFrame >= 10) boss4Obj.jumpLandingFrame = 9; 
            }

            boss4Obj.shakeX *= 0.85f;
            boss4Obj.shakeY *= 0.85f;

            boss4Obj.landingFlameTimer--;
            
            float smashCenterX = boss4Obj.x + BOSS_WIDTH / 2.0f;
            float totalWidth = BOSS_WIDTH + 40.0f; // Matches boss body more closely

            if (charX + 60 > (smashCenterX - totalWidth / 2.0f) && charX < (smashCenterX + totalWidth / 2.0f)) {
                if (!isInvincible) {
                    lives--;
                    if (lives <= 0) { lives = 0; gameState = GAME_OVER; }
                    else { isInvincible = true; invincibilityTimer = 60; }
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
        }
        else if (boss4Obj.jumpSkillState == 4) {
            // State 4: Return
            boss4Obj.jumpSkillTimer++;
            float returnSpeed = 10.0f;
            if (boss4Obj.x < 750) {
                boss4Obj.x += returnSpeed;
                boss4Obj.facingLeft = false;
                if (boss4Obj.x > 750) boss4Obj.x = 750;
            } else if (boss4Obj.x > 750) {
                boss4Obj.x -= returnSpeed;
                boss4Obj.facingLeft = true;
                if (boss4Obj.x < 750) boss4Obj.x = 750;
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
        }
        else if (boss4Obj.pullSkillState == 2) {
            // State 2: Pull Prep
            boss4Obj.pullSkillTimer++;
            if (boss4Obj.pullSkillTimer >= 6) {
                boss4Obj.pullSkillTimer = 0;
                boss4Obj.pullFrameIndex++;
                if (boss4Obj.pullFrameIndex >= 6) {
                    boss4Obj.pullSkillState = 3; // Pulling
                    boss4Obj.pullSkillTimer = 0;
                    boss4Obj.pullDanceFrame = 0;
                }
            }
        }
        else if (boss4Obj.pullSkillState == 3) {
            // State 3: Pulling Phase
            boss4Obj.pullSkillTimer++;
            if (boss4Obj.pullSkillTimer % 5 == 0) {
                boss4Obj.pullDanceFrame = (boss4Obj.pullDanceFrame + 1) % 4;
            }

            if (charX < boss4Obj.x + 50) {
                charX += 6; 
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
                if (!isInvincible) {
                    lives--;
                    if (lives <= 0) { lives = 0; gameState = GAME_OVER; }
                    else { isInvincible = true; invincibilityTimer = 60; }
                }
            }

            if (boss4Obj.pullSkillTimer > 200) {
                boss4Obj.pullSkillState = 0; 
                boss4Obj.pullSkillTimer = 0;
                boss4Obj.pullSkillCount++;
                
                if (boss4Obj.pullSkillCount >= boss4Obj.pullTargetCount) {
                    boss4Obj.pullSkillCount = 0;
                    boss4Obj.pullTargetCount = 3 + rand() % 3;
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
            if (cycle < 5) boss4Obj.y += 4.0f;
            else boss4Obj.y -= 4.0f;
            
            if (boss4Obj.pushSkillTimer > 0 && boss4Obj.pushSkillTimer % 10 == 0) {
                boss4Obj.pushJumpCount++;
            }
            
            if (boss4Obj.pushJumpCount >= 3) {
                boss4Obj.pushSkillState = 2; // Prep
                boss4Obj.pushSkillTimer = 0;
                boss4Obj.pushFrameIndex = 0;
                boss4Obj.y = boss4Obj.pushStartY;
            }
        }
        else if (boss4Obj.pushSkillState == 2) {
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
                        boss4Obj.pushTargetX = charX + 150.0f;
                        boss4Obj.facingLeft = false;
                    } else {
                        boss4Obj.pushDir = -1; 
                        boss4Obj.pushTargetX = charX - 150.0f;
                        boss4Obj.facingLeft = true;
                    }
                }
            }
        }
        else if (boss4Obj.pushSkillState == 3) {
            // State 3: Pushing
            boss4Obj.pushSkillTimer++;
            if (boss4Obj.pushSkillTimer % 4 == 0) {
                boss4Obj.pushFrameIndex = (boss4Obj.pushFrameIndex == 6) ? 7 : 6;
            }
            boss4Obj.x += boss4Obj.pushDir * 12.0f;
            
            if (charY < boss4Obj.pushStartY + BOSS_HEIGHT - 30) {
                if (boss4Obj.pushDir == 1) { 
                    if (charX < boss4Obj.x + BOSS_WIDTH && charX + 50 > boss4Obj.x) charX = boss4Obj.x + BOSS_WIDTH;
                } else { 
                    if (charX + 50 > boss4Obj.x && charX < boss4Obj.x + BOSS_WIDTH) charX = boss4Obj.x - 50;
                }
            }
            
            bool reachedTarget = false;
            if (boss4Obj.pushDir == 1 && boss4Obj.x >= boss4Obj.pushTargetX) reachedTarget = true;
            if (boss4Obj.pushDir == -1 && boss4Obj.x <= boss4Obj.pushTargetX) reachedTarget = true;
            
            if (reachedTarget) {
                bool escaped = false;
                if (boss4Obj.pushDir == 1 && charX < boss4Obj.x) escaped = true;
                if (boss4Obj.pushDir == -1 && charX > boss4Obj.x) escaped = true;
                
                if (!escaped && !isInvincible) {
                    lives--;
                    if (lives <= 0) { lives = 0; gameState = GAME_OVER; }
                    else { isInvincible = true; invincibilityTimer = 60; }
                }
                
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
        else if (boss4Obj.pushSkillState == 4) {
            // State 4: Return
            boss4Obj.pushSkillTimer++;
            float returnSpeed = 10.0f;
            if (boss4Obj.x < 750) {
                boss4Obj.x += returnSpeed;
                boss4Obj.facingLeft = false;
                if (boss4Obj.x > 750) boss4Obj.x = 750;
            } else if (boss4Obj.x > 750) {
                boss4Obj.x -= returnSpeed;
                boss4Obj.facingLeft = true;
                if (boss4Obj.x < 750) boss4Obj.x = 750;
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
        extern int charX, charY, lives;
        extern bool isInvincible;
        extern int invincibilityTimer;
        extern GameState gameState;
        extern bool isAttacking;

        if (boss4Obj.enemySkillState == 1) {
            boss4Obj.enemySkillTimer++;
            if (boss4Obj.enemySkillTimer < 5) boss4Obj.y += 8.0f;
            else boss4Obj.y -= 8.0f;

            if (boss4Obj.enemySkillTimer >= 10) {
                boss4Obj.enemySkillState = 2; // high shake
                boss4Obj.enemySkillTimer = 0;
                boss4Obj.y = boss4Obj.enemyStartY;
                boss4Obj.shakeX = 0;
                boss4Obj.shakeY = 0;
            }
        }
        else if (boss4Obj.enemySkillState == 2) {
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
        }
        else if (boss4Obj.enemySkillState == 3) {
            boss4Obj.enemySkillTimer++;
            if (boss4Obj.enemySkillTimer >= 5) {
                boss4Obj.enemySkillTimer = 0;
                boss4Obj.enemyFrameIndex++;
                if (boss4Obj.enemyFrameIndex >= 6) {
                    boss4Obj.enemySkillState = 4; // Spawning
                    boss4Obj.enemySkillTimer = 0;
                    boss4Obj.enemySpawnTimer = 0;
                    boss4Obj.enemyFrameIndex = 0;
                }
            }
        }
        else if (boss4Obj.enemySkillState == 4 || boss4Obj.enemySkillState == 5) {
            boss4Obj.enemySkillTimer++;
            if (boss4Obj.enemySkillState == 4) {
                boss4Obj.enemySpawnTimer++;
                if (boss4Obj.enemySpawnTimer > 40) {
                    boss4Obj.enemySpawnTimer = 0;
                    for (int i=0; i<5; i++) {
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
                if (boss4Obj.enemySkillTimer % 5 == 0) boss4Obj.enemyFrameIndex = (boss4Obj.enemyFrameIndex + 1) % 6;
                if (boss4Obj.enemySkillTimer > 495) boss4Obj.enemySkillState = 5;
            }
            
            bool anyActive = false;
            for (int i=0; i<5; i++) {
                if (boss4Obj.bossEnemies[i].active) {
                    anyActive = true;
                    boss4Obj.bossEnemies[i].x -= 5.0f;
                    boss4Obj.bossEnemies[i].frameTimer++;
                    if (boss4Obj.bossEnemies[i].frameTimer >= 5) {
                        boss4Obj.bossEnemies[i].frameTimer = 0;
                        boss4Obj.bossEnemies[i].frame = (boss4Obj.bossEnemies[i].frame + 1) % 4;
                    }
                    if (boss4Obj.bossEnemies[i].x < -150) boss4Obj.bossEnemies[i].active = false;

                    if (boss4Obj.bossEnemies[i].active && boss4Obj.bossEnemies[i].x < charX + 80 && boss4Obj.bossEnemies[i].x + 100 > charX && 
                        boss4Obj.bossEnemies[i].y < charY + 120 && boss4Obj.bossEnemies[i].y + 40 > charY) {
                        if (isAttacking) boss4Obj.bossEnemies[i].active = false;
                        else if (!isInvincible) {
                            lives--;
                            if (lives <= 0) { lives = 0; gameState = GAME_OVER; }
                            else { isInvincible = true; invincibilityTimer = 60; }
                            boss4Obj.bossEnemies[i].active = false;
                        }
                    }
                }
            }
            if (boss4Obj.enemySkillState == 5 && !anyActive) {
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
        
        if (boss4Obj.topSkillState == 1) {
            boss4Obj.topSkillTimer++;
            if (boss4Obj.topSkillTimer >= 6) { 
                boss4Obj.topSkillTimer = 0;
                boss4Obj.topFrameIndex++;
                if (boss4Obj.topFrameIndex >= 9) {
                    boss4Obj.topSkillState = 2; // Thunder
                    boss4Obj.topFrameIndex = 0;
                    boss4Obj.topSkillTimer = 0;
                }
            }
        }
        else if (boss4Obj.topSkillState == 2) {
            boss4Obj.topSkillTimer++;
            if (boss4Obj.topSkillTimer % 5 == 0) boss4Obj.topFrameIndex = (boss4Obj.topFrameIndex + 1) % 4;
            
            boss4Obj.topSpawnTimer++;
            if (boss4Obj.topSpawnTimer > 30) {
                boss4Obj.topSpawnTimer = 0;
                for (int i = 0; i < 10; i++) {
                    if (!boss4Obj.topObstacles[i].active) {
                        boss4Obj.topObstacles[i].active = true;
                        boss4Obj.topObstacles[i].x = charX + 60.0f + (rand() % 400 - 200);
                        boss4Obj.topObstacles[i].y = 650.0f;
                        boss4Obj.topObstacles[i].speedY = (float)((rand() % 5) + 12);
                        boss4Obj.topObstacles[i].frame = rand() % 10;
                        boss4Obj.topObstacles[i].frameTimer = 0;
                        break;
                    }
                }
            }
            
            bool anyActive = false;
            for (int i = 0; i < 10; i++) {
                if (boss4Obj.topObstacles[i].active) {
                    boss4Obj.topObstacles[i].y -= boss4Obj.topObstacles[i].speedY;
                    boss4Obj.topObstacles[i].frameTimer++;
                    if (boss4Obj.topObstacles[i].frameTimer >= 4) {
                        boss4Obj.topObstacles[i].frameTimer = 0;
                        boss4Obj.topObstacles[i].frame = (boss4Obj.topObstacles[i].frame + 1) % 10;
                    }
                    if (boss4Obj.topObstacles[i].y < -100) boss4Obj.topObstacles[i].active = false;
                    
                    if (boss4Obj.topObstacles[i].active && boss4Obj.topObstacles[i].x + 40 > charX && boss4Obj.topObstacles[i].x < charX + 80 &&
                        boss4Obj.topObstacles[i].y + 40 > charY && boss4Obj.topObstacles[i].y < charY + 120) {
                        if (!isInvincible) {
                            lives--;
                            if (lives <= 0) { lives = 0; gameState = GAME_OVER; }
                            else { isInvincible = true; invincibilityTimer = 60; }
                            boss4Obj.topObstacles[i].active = false;
                        }
                    }
                    if (boss4Obj.topObstacles[i].active) anyActive = true;
                }
            }
            
            if (boss4Obj.topSkillTimer > 495) {
                if (!anyActive) {
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
            if (boss4Obj.fireSkillTimer >= 4) {
                boss4Obj.fireSkillTimer = 0;
                boss4Obj.fireFrameIndex++;
                if (boss4Obj.fireFrameIndex >= 10) {
                    boss4Obj.fireSkillState = 2; // Launching
                    boss4Obj.fbActive = true;
                    boss4Obj.fbFrameIndex = 0;
                    if (boss4Obj.facingLeft) boss4Obj.fbX = boss4Obj.x - 20;
                    else boss4Obj.fbX = boss4Obj.x + BOSS_WIDTH;
                    boss4Obj.fbY = boss4Obj.y + 100;

                    float tX = (float)charX + 40, tY = (float)charY + 40;
                    float dx = tX - boss4Obj.fbX, dy = tY - boss4Obj.fbY;
                    float dist = (float)sqrt((double)(dx*dx + dy*dy));
                    if (dist < 1.0f) dist = 1.0f;
                    float speed = 12.0f; // Faster fireball travelling
                    boss4Obj.fbVelX = (dx/dist) * speed;
                    boss4Obj.fbVelY = (dy/dist) * speed;
                    boss4Obj.fireSkillTimer = 0;
                }
            }
        }
        else if (boss4Obj.fireSkillState == 2) {
            if (boss4Obj.fbActive) {
                boss4Obj.fbX += boss4Obj.fbVelX;
                boss4Obj.fbY += boss4Obj.fbVelY;
                boss4Obj.fbFrameIndex = (boss4Obj.fbFrameIndex + 1) % 10;
                if (boss4Obj.fbX < -100 || boss4Obj.fbX > 1100 || boss4Obj.fbY < -100 || boss4Obj.fbY > 700) boss4Obj.fbActive = false;
                if (boss4Obj.fbActive && boss4Obj.fbX + 40 > charX && boss4Obj.fbX < charX + 80 && boss4Obj.fbY + 40 > charY && boss4Obj.fbY < charY + 80) {
                    if (!isInvincible) {
                        lives--;
                        if (lives <= 0) { lives = 0; gameState = GAME_OVER; }
                        else { isInvincible = true; invincibilityTimer = 60; }
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
        }
        else if (boss4Obj.fireSkillState == 3) {
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
                    if (charX < boss4Obj.x + BOSS_WIDTH / 2.0f) boss4Obj.facingLeft = true;
                    else boss4Obj.facingLeft = false;
                }
                boss4Obj.fireSkillTimer = 0;
            }
        }
    }

    // --- SKILL 7: REST ATTACK SKILL LOGIC (NOW AT INDEX 7) ---
    if (level4Phase == 3 && boss4Obj.currentSkill == 7) {
        if (boss4Obj.restSkillState == 1) {
            boss4Obj.restSkillTimer++;
            if (boss4Obj.restSkillTimer >= 5) {
                boss4Obj.restSkillTimer = 0;
                boss4Obj.restFrameIndex++;
                if (boss4Obj.restFrameIndex >= 4) {
                    boss4Obj.restSkillState = 2;
                    boss4Obj.restFrameIndex = 4;
                    boss4Obj.restSkillTimer = 0;
                }
            }
        }
        else if (boss4Obj.restSkillState == 2) {
            boss4Obj.restSkillTimer++;
            // Cycle breathing frames 4, 5, 6
            if (boss4Obj.restSkillTimer % 8 == 0) {
                boss4Obj.restFrameIndex++;
                if (boss4Obj.restFrameIndex >= 7) boss4Obj.restFrameIndex = 4;
            }
            // 660 timer ticks = 20 seconds at 33fps
            if (boss4Obj.restSkillTimer > 660) { 
                boss4Obj.restSkillState = 0;
                boss4Obj.restSkillTimer = 0;
                boss4Obj.currentSkill = -1;
                boss4Obj.skillsCompleted++;
                boss4Obj.skillGapTimer = 40; 
                boss4Obj.safeguardTimer = 0;
            }
        }
    }
    
    if (boss4Obj.charOnFire) {
        boss4Obj.charFireTimer--;
        if (boss4Obj.charFireTimer <= 0) boss4Obj.charOnFire = false;
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
 * Handles the high-level Level 4 logic, including transitions to the boss fight.
 */
inline void updateLevel4Boss() {
    extern int currentLevel;
    extern GameState gameState;
    extern bool isGamePaused;
    extern int level4Phase;

    if (currentLevel != 4 || gameState != GAME || isGamePaused) return;

    if (level4Phase == 1) {
        // Background scroll handles shifting to phase 2
    } else if (level4Phase == 2) {
        // Boss walks in
        if (boss4Obj.x > 750) {
            boss4Obj.x -= 4;
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
    extern unsigned int specialKeyPressed[512];
    extern bool isLeftArrowPressed;

    if (!boss4Obj.active || level4Phase < 2 || boss4Obj.life <= 0) return;
    
    // Check if boss is currently in an invulnerable state (e.g. Skill 0 Vanish hidden)
    if (boss4Obj.currentSkill == 0 && (boss4Obj.skillState >= 1 && boss4Obj.skillState <= 3)) return;

    if (isAttacking && attackFrameIndex == 3 && !npcSlashDone) {
        bool facingBack = isLeftArrowPressed || (specialKeyPressed[GLUT_KEY_LEFT] != 0);
        float weaponZoneMin, weaponZoneMax;
        
        if (facingBack) {
            weaponZoneMin = (float)charX - 160;
            weaponZoneMax = (float)charX - 10;
        } else {
            weaponZoneMin = (float)charX + charWidth + 10;
            weaponZoneMax = (float)charX + charWidth + 160;
        }

        float bossL = boss4Obj.x;
        float bossR = boss4Obj.x + (float)BOSS_WIDTH;
        float bossBottom = boss4Obj.y;
        float bossTop = boss4Obj.y + (float)BOSS_HEIGHT;

        bool weaponHit = false;
        if (((bossL >= weaponZoneMin && bossL <= weaponZoneMax) || 
             (bossR >= weaponZoneMin && bossR <= weaponZoneMax) ||
             (weaponZoneMin >= bossL && weaponZoneMin <= bossR))) {
           weaponHit = true;
        }

        if (weaponHit && (float)charY < bossTop && (float)charY + (float)charHeight > bossBottom) {
            boss4Obj.life -= 5;
            npcSlashDone = true;
            boss4Obj.isHit = true;
            boss4Obj.hitTimer = 15;
            
            if (boss4Obj.life <= 0) {
                level4Phase = 4; // Victory sequence
                boss4Obj.dieFrameIndex = 0;
                boss4Obj.dieAnimCounter = 0;
            }
        }
    }
}

// Custom wrapper for flipping based on state
inline void iDrawBossImage(int x, int y, int w, int h, int img) {
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
    if (!boss4Obj.active) return;
    extern int level4Phase;
    iSetColor(255, 255, 255); // Fix: Reset color to white so images don't inherit orange from life bar logic

    // ---------------------------------------------------------------------------
    // STEP 1: Draw the BOSS BODY first (flames will be drawn on top after)
    // ---------------------------------------------------------------------------
    if (boss4Obj.isHit && (boss4Obj.hitTimer / 5) % 2 == 0) {
        // Blink effect when hit
    } else {
        if (level4Phase == 4) {
            iShowImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH, BOSS_HEIGHT, bossDieImgs[boss4Obj.dieFrameIndex]);
        } else if (level4Phase == 5) {
            // No boss rendering in victory
        } else if (level4Phase == 3 && boss4Obj.currentSkill != -1) {
            // SKILL 0: VANISH RENDERING
            if (boss4Obj.currentSkill == 0) {
                if (boss4Obj.skillState == 1 || boss4Obj.skillState == 3) {
                    iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH, BOSS_HEIGHT, bossVanishImgs[boss4Obj.skillFrameIndex]);
                } else if (boss4Obj.skillState == 4) {
                    iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH, BOSS_HEIGHT, bossSmashImgs[boss4Obj.skillFrameIndex % 10]);
                } else if (boss4Obj.skillState == 5) {
                    iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH, BOSS_HEIGHT, bossWalkImgs[boss4Obj.frameIndex]);
                }
            }
            
            // SKILL 1: JUMP ATTACK RENDERING
            if (boss4Obj.currentSkill == 1) {
                if (boss4Obj.jumpSkillState == 1) {
                     iDrawBossImage((int)(boss4Obj.x + boss4Obj.shakeX), (int)(boss4Obj.y + boss4Obj.shakeY), BOSS_WIDTH, BOSS_HEIGHT, bossStandImgs[boss4Obj.frameIndex]);
                } else if (boss4Obj.jumpSkillState == 2) {
                     iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH, BOSS_HEIGHT, bossJumpImgs[boss4Obj.jumpFrameIndex]);
                } else if (boss4Obj.jumpSkillState == 3) {
                     iDrawBossImage((int)(boss4Obj.x + boss4Obj.shakeX), (int)(boss4Obj.y + boss4Obj.shakeY), BOSS_WIDTH, BOSS_HEIGHT, bossSmashImgs[boss4Obj.jumpLandingFrame]);
                } else if (boss4Obj.jumpSkillState == 4) {
                     iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH, BOSS_HEIGHT, bossWalkImgs[boss4Obj.frameIndex]);
                }
            }

             // SKILL 2: PULL ATTACK RENDERING
            if (boss4Obj.currentSkill == 2) {
                if (boss4Obj.pullSkillState == 1) {
                    iDrawBossImage((int)(boss4Obj.x + boss4Obj.shakeX), (int)(boss4Obj.y + boss4Obj.shakeY), BOSS_WIDTH, BOSS_HEIGHT, bossStandImgs[boss4Obj.frameIndex]);
                } else if (boss4Obj.pullSkillState == 2) {
                    iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH, BOSS_HEIGHT, bossPullImgs[boss4Obj.pullFrameIndex]);
                } else if (boss4Obj.pullSkillState == 3) {
                    iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH, BOSS_HEIGHT, bossPullDanceImgs[boss4Obj.pullDanceFrame]);
                    iSetColor(200, 200, 255); 
                    for (int i = 0; i < 15; i++) {
                        iFilledRectangle((int)boss4Obj.pullParticlesX[i], (int)boss4Obj.pullParticlesY[i], 30, 2);
                        iFilledCircle((int)boss4Obj.pullParticlesX[i] + 30, (int)boss4Obj.pullParticlesY[i] + 1, 1);
                    }
                }
            }

             // SKILL 3: PUSH ATTACK RENDERING
            if (boss4Obj.currentSkill == 3) {
                if (boss4Obj.pushSkillState == 1) {
                    iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH, BOSS_HEIGHT, bossStandImgs[boss4Obj.frameIndex]);
                } else if (boss4Obj.pushSkillState == 2 || boss4Obj.pushSkillState == 3) {
                    iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH, BOSS_HEIGHT, bossPushImgs[boss4Obj.pushFrameIndex]);
                } else if (boss4Obj.pushSkillState == 4) {
                    iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH, BOSS_HEIGHT, bossWalkImgs[boss4Obj.frameIndex]);
                }
            }

             // SKILL 4: ENEMY ATTACK RENDERING
            if (boss4Obj.currentSkill == 4) {
                if (boss4Obj.enemySkillState == 1) {
                    iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH, BOSS_HEIGHT, bossStandImgs[boss4Obj.frameIndex]);
                } else if (boss4Obj.enemySkillState == 2) {
                    iDrawBossImage((int)(boss4Obj.x + boss4Obj.shakeX), (int)(boss4Obj.y + boss4Obj.shakeY), BOSS_WIDTH, BOSS_HEIGHT, bossStandImgs[boss4Obj.frameIndex]);
                } else if (boss4Obj.enemySkillState >= 3) {
                    iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH, BOSS_HEIGHT, bossPreJumpImgs[boss4Obj.enemyFrameIndex]);
                }
                
                if (boss4Obj.enemySkillState >= 4) {
                    for(int i=0; i<5; i++) {
                        if (boss4Obj.bossEnemies[i].active) {
                            int eImg = worm11;
                            if (boss4Obj.bossEnemies[i].type == 0) {
                                if (boss4Obj.bossEnemies[i].frame == 0) eImg = worm11;
                                else if (boss4Obj.bossEnemies[i].frame == 1) eImg = worm12;
                                else if (boss4Obj.bossEnemies[i].frame == 2) eImg = worm13;
                                else if (boss4Obj.bossEnemies[i].frame == 3) eImg = worm14;
                            } else {
                                if (boss4Obj.bossEnemies[i].frame == 0) eImg = worm21;
                                else if (boss4Obj.bossEnemies[i].frame == 1) eImg = worm22;
                                else if (boss4Obj.bossEnemies[i].frame == 2) eImg = worm23;
                                else if (boss4Obj.bossEnemies[i].frame == 3) eImg = worm24;
                            }
                            iShowImage((int)boss4Obj.bossEnemies[i].x, (int)boss4Obj.bossEnemies[i].y, 100, 40, eImg);
                        }
                    }
                }
            }

             // SKILL 5: TOP ATTACK RENDERING
             if (boss4Obj.currentSkill == 5) {
                 if (boss4Obj.topSkillState == 1) {
                     iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH, BOSS_HEIGHT, bossHighImgs[boss4Obj.topFrameIndex]);
                 } else if (boss4Obj.topSkillState == 2) {
                     iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH, BOSS_HEIGHT, bossThunderImgs[boss4Obj.topFrameIndex]);
                     for(int i = 0; i < 10; i++) {
                         if (boss4Obj.topObstacles[i].active) {
                             iShowImage((int)boss4Obj.topObstacles[i].x, (int)boss4Obj.topObstacles[i].y, 50, 50, fireballImgs[boss4Obj.topObstacles[i].frame]);
                         }
                     }
                 }
             }

             // SKILL 6: FIRE ATTACK RENDERING (Swapped)
             if (boss4Obj.currentSkill == 6) {
                 if (boss4Obj.fireSkillState == 1) {
                     iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH, BOSS_HEIGHT, bossFireMotionImgs[boss4Obj.fireFrameIndex]);
                 } else {
                     iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH, BOSS_HEIGHT, bossStandImgs[boss4Obj.frameIndex]);
                 }
                 if (boss4Obj.fbActive) {
                     iShowImage((int)boss4Obj.fbX, (int)boss4Obj.fbY, 80, 80, fireballImgs[boss4Obj.fbFrameIndex]);
                 }
             }

             // SKILL 7: REST ATTACK RENDERING (Swapped)
             if (boss4Obj.currentSkill == 7) {
                 iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH, BOSS_HEIGHT, bossRestSkillImgs[boss4Obj.restFrameIndex]);
             }
        } else {
            // Normal phase animations
            if (level4Phase == 2) {
                iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH, BOSS_HEIGHT, bossWalkImgs[boss4Obj.frameIndex]);
            } else if (level4Phase == 3) {
                iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH, BOSS_HEIGHT, bossStandImgs[boss4Obj.frameIndex % 6]);
            } else {
                iDrawBossImage((int)boss4Obj.x, (int)boss4Obj.y, BOSS_WIDTH, BOSS_HEIGHT, bossImgs[boss4Obj.frameIndex % 4]);
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
        if (lifePercent < 0) lifePercent = 0;
        
        // --- STEP 1: DRAW ORANGE LINE (BEHIND) ---
        iSetColor(255, 100, 0); // Deep Orange
        float totalWidth = 170.0f;
        float drawWidth = totalWidth * lifePercent;
        // Decrease from the left side: the left edge moves right (empty space starts at left)
        float drawX = (float)lifeFrameX + 44.0f + (totalWidth - drawWidth);
        iFilledRectangle((double)drawX, (double)lifeFrameY + 37.0, (double)drawWidth, 10.0);

        // --- STEP 2: DRAW FRAME (ON TOP) ---
        iShowImage(lifeFrameX, lifeFrameY, lifeFrameW, lifeFrameH, bossLifeFrameImg);
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
    if (!boss4Obj.charOnFire) return;
    extern int currentLevel;
    extern int level4Phase;
    if (currentLevel != 4) return;

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
