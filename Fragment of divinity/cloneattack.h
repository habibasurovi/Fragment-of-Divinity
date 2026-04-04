// cloneattack.h
// Clone Attack system for Level 4.
//
// FLOW:
//  1. Press 'C'  (during level4Phase == 3)
//        -> CLONE_SUMMONING: plays arynclone / kaeroclone / leoraclone frames
//  2. Last summon frame reached
//        -> CLONE_READY: holds last frame, waiting for arrow key
//  3. Arrow key pressed
//        -> CLONE_RUNNING: 3 clones appear around the character.
//           Clones are rendered at fixed offsets from charX so they move
//           naturally as the player presses arrow keys (no separate AI).
//           Clone 0: charX          (front)
//           Clone 1: charX - 75     (middle)
//           Clone 2: charX - 150    (back)
//  4. SHIFT pressed (while in CLONE_RUNNING)
//        -> CLONE_ATTACKING: per-clone hit-check vs boss.
//           Each clone that is within attack reach of the boss deals 5 HP.
//           All 3 play the attack animation simultaneously.
//  5. Attack animation ends
//        -> CLONE_DONE -> CLONE_IDLE (clones vanish, normal play resumes)
//
// Size rule: every clone frame is drawn at the SAME pixel size as the
//            character's idle / walk frame for that character
//            (charX, charY, charWidth, charHeight for Aryn/Leora;
//             charX-25, charY-20, charWidth+50, charHeight+50 for Kaero).

#ifndef CLONEATTACK_H
#define CLONEATTACK_H

#include "iGraphics.h"
#include "GameState.h"
#include "Boss.h"                    // BossLevel4, boss4Obj, level4Phase
#include "CharacterCustomization.h"  // getSelectedCharacterAttackImage
#include "character 2.h"             // getCharacterRunImage
#include <stdio.h>
#include <windows.h>                 // GetAsyncKeyState / VK_SHIFT

// ---- externals ----
extern int  charX, charY, charWidth, charHeight;
extern int  selectedCharacter;    // 0=Aryn, 1=Kaero, 2=Leora
extern bool isLeftArrowPressed;
extern unsigned int specialKeyPressed[512];
extern int  currentLevel;
extern GameState gameState;
extern bool isAttacking;
extern int  attackFrameIndex;
extern void getCharacterFrameInfo(int customX, int customY, int &rX, int &rY, int &rW, int &rH, int &frame);

// ============================================================
//  Summon-animation image storage
// ============================================================
static int arynCloneImgs[4];
static int arynCloneBackImgs[4];
static int kaeroCloneImgs[4];
static int kaeroCloneBackImgs[4];
static int leoraCloneImgs[3];
static int leoraCloneBackImgs[3];

// ============================================================
//  State machine
// ============================================================
enum ClonePhase {
    CLONE_IDLE      = 0,
    CLONE_SUMMONING = 1,   // playing the clone summon images
    CLONE_READY     = 2,   // holding last summon frame; waiting for arrow key
    CLONE_RUNNING   = 3,   // 3 clones visible; follow character's movement
    CLONE_ATTACKING = 4,   // all 3 playing attack animation simultaneously
    CLONE_DONE      = 5    // brief pause then back to IDLE
};

extern ClonePhase clonePhase;

// ----- Summon animation -----
extern int cloneSummonFrame;
extern int cloneSummonCounter;
static const int CLONE_SUMMON_SPEED = 6; // ticks per frame

// ----- Shared run / attack animation (all 3 stay in sync) -----
extern int cloneRunFrame;
extern int cloneRunCounter;
static const int CLONE_RUN_SPEED = 3;

extern int cloneAtkFrame;
extern int cloneAtkCounter;
static const int CLONE_ATK_SPEED = 4;

extern int cloneDoneTimer;

// Attack reach: how far left of the boss front-edge a clone can be and still hit
static const float CLONE_ATTACK_REACH = 200.0f;

// ============================================================
//  Helpers — per-character frame counts & sizes
// ============================================================
inline int cloneSummonMaxFrames() {
    return (selectedCharacter == 2) ? 3 : 4;
}
inline int cloneRunMaxFrames() {
    if (selectedCharacter == 0) return 9;
    if (selectedCharacter == 1) return 8;
    return 7;
}
inline int cloneAtkMaxFrames() {
    if (selectedCharacter == 0) return 4;
    if (selectedCharacter == 1) return 6;
    return 3;
}

// Returns the pixel rect for one clone entity at position (cx, cy).
// Mirrors the sizing logic in drawCharacter() so clones look identical
// to the main character.
struct CloneRect { int x, y, w, h; };
inline CloneRect cloneRect(int cx, int cy) {
    if (selectedCharacter == 1) {
        // Kaero run/idle is drawn larger
        return { cx - 25, cy - 20, charWidth + 50, charHeight + 50 };
    }
    return { cx, cy, charWidth, charHeight };
}
// For summon / attack we always use the plain idle size
inline CloneRect cloneRectIdle(int cx, int cy) {
    return { cx, cy, charWidth, charHeight };
}

// ============================================================
//  Summon image getter
// ============================================================
inline int getCloneSummonImage(int frame, bool isBack) {
    if (selectedCharacter == 0) {
        if (frame < 0 || frame >= 4) return -1;
        return isBack ? arynCloneBackImgs[frame] : arynCloneImgs[frame];
    } else if (selectedCharacter == 1) {
        if (frame < 0 || frame >= 4) return -1;
        return isBack ? kaeroCloneBackImgs[frame] : kaeroCloneImgs[frame];
    } else {
        if (frame < 0 || frame >= 3) return -1;
        return isBack ? leoraCloneBackImgs[frame] : leoraCloneImgs[frame];
    }
}

// ============================================================
//  Load
// ============================================================
inline void loadCloneAttackAssets() {
    static bool loaded = false;
    if (loaded) return;
    loaded = true;
    char p[120];

    for (int i = 0; i < 4; i++) {
        sprintf_s(p, sizeof(p), "aryn\\arynclone%d.png", i + 1);
        arynCloneImgs[i] = iLoadImage((char*)p);
        sprintf_s(p, sizeof(p), "aryn\\aryncloneback%d.png", i + 1);
        arynCloneBackImgs[i] = iLoadImage((char*)p);
    }
    for (int i = 0; i < 4; i++) {
        sprintf_s(p, sizeof(p), "Character\\kaeroclone%d.png", i + 1);
        kaeroCloneImgs[i] = iLoadImage((char*)p);
        sprintf_s(p, sizeof(p), "Character\\kaerocloneback%d.png", i + 1);
        kaeroCloneBackImgs[i] = iLoadImage((char*)p);
    }
    for (int i = 0; i < 3; i++) {
        sprintf_s(p, sizeof(p), "Leora\\leoraclone%d.png", i + 1);
        leoraCloneImgs[i] = iLoadImage((char*)p);
        sprintf_s(p, sizeof(p), "Leora\\leoracloneback%d.png", i + 1);
        leoraCloneBackImgs[i] = iLoadImage((char*)p);
    }
}

// ============================================================
//  Trigger  (called from iKeyboard on 'C' in Level 4)
// ============================================================
inline void triggerCloneAttack() {
    if (currentLevel != 4)          return;
    if (level4Phase != 3)           return;
    if (clonePhase != CLONE_IDLE)   return;

    clonePhase        = CLONE_SUMMONING;
    cloneSummonFrame  = 0;
    cloneSummonCounter= 0;
}

// ============================================================
//  Per-clone damage check
//  Each clone is at  charX - i*75.  If that position is within
//  CLONE_ATTACK_REACH of the boss left edge, it lands a hit.
// ============================================================
inline void applyCloneDamage() {
    float bossLeft  = boss4Obj.x;
    float bossRight = boss4Obj.x + (float)BOSS_WIDTH;

    for (int i = 0; i < 3; i++) {
        float cloneRight = (float)(charX - i * 75 + charWidth) + CLONE_ATTACK_REACH;
        float cloneLeft  = (float)(charX - i * 75) - 20.0f;

        // Clone hits if its extended reach overlaps with the boss body
        if (cloneRight > bossLeft && cloneLeft < bossRight) {
            boss4Obj.life -= 5;
            spawnDamagePopup(5, bossLeft + BOSS_WIDTH / 2.0f, boss4Obj.y + BOSS_HEIGHT / 2.0f);
            boss4Obj.isHit   = true;
            boss4Obj.hitTimer = 15;
            if (boss4Obj.life <= 0) {
                boss4Obj.life = 0;
                level4Phase   = 4;
                boss4Obj.dieFrameIndex  = 0;
                boss4Obj.dieAnimCounter = 0;
            }
        }
    }
}

// ============================================================
//  Update  (called every 30 ms from masterGameLoop)
// ============================================================
inline void updateCloneAttackAnimation() {
    if (currentLevel != 4)        return;
    if (clonePhase == CLONE_IDLE) return;

    // ---- CLONE_SUMMONING ----
    if (clonePhase == CLONE_SUMMONING) {
        cloneSummonCounter++;
        if (cloneSummonCounter >= CLONE_SUMMON_SPEED) {
            cloneSummonCounter = 0;
            cloneSummonFrame++;
            if (cloneSummonFrame >= cloneSummonMaxFrames()) {
                cloneSummonFrame = cloneSummonMaxFrames() - 1; // hold last frame
                clonePhase = CLONE_READY;
            }
        }
        return;
    }

    // ---- CLONE_READY ----
    if (clonePhase == CLONE_READY) {
        // Activate clones when mover keys are pressed
        if (isLeftArrowPressed || isRightArrowPressed || 
            isSpecialKeyPressed(GLUT_KEY_LEFT) || isSpecialKeyPressed(GLUT_KEY_RIGHT)) {
            clonePhase = CLONE_RUNNING;
        }
        return;
    }

    // ---- CLONE_RUNNING ----
    if (clonePhase == CLONE_RUNNING) {
        // Clones now persistent during the boss fight phase.
        // They stay active as long as the boss is alive (level4Phase 3).
        if (level4Phase > 3) {
            clonePhase = CLONE_DONE;
            cloneDoneTimer = 20;
        }
        
        // Damage logic: if character is in middle of attack frame, apply damage for clones
        extern bool isTeleportAttacking, isMeteroidAttacking;
        extern int teleportFrameIndex, meteroidFrameIndex;

        if (isAttacking && attackFrameIndex == 2) {
             applyCloneDamage();
             clonePhase = CLONE_DONE;
             cloneDoneTimer = 10; 
        } else if (isTeleportAttacking && teleportFrameIndex == 2) {
             // applyTeleportDamage already handles clone contribution internally
             clonePhase = CLONE_DONE;
             cloneDoneTimer = 10;
        } else if (isMeteroidAttacking) {
            // Check for last frame of meteoroid attack (differs per character)
            int maxF = (selectedCharacter == 0) ? 2 : 3;
            if (meteroidFrameIndex == maxF - 1) {
                clonePhase = CLONE_DONE;
                cloneDoneTimer = 10;
            }
        }
        return;
    }

    // ---- CLONE_DONE ----
    if (clonePhase == CLONE_DONE) {
        cloneDoneTimer--;
        if (cloneDoneTimer <= 0) {
            clonePhase = CLONE_IDLE;
        }
        return;
    }
}

// ============================================================
//  Draw  (called from iDraw after drawCharacter)
// ============================================================
inline void drawCloneAttack() {
    if (clonePhase == CLONE_IDLE) return;

    iSetColor(255, 255, 255);

    // ---- Summon animation ----
    if (clonePhase == CLONE_SUMMONING || clonePhase == CLONE_READY) {
        bool isBack = isLeftArrowPressed || isSpecialKeyPressed(GLUT_KEY_LEFT);
        int frame = getCloneSummonImage(cloneSummonFrame, isBack);
        if (frame != -1) {
            CloneRect r = cloneRectIdle(charX, charY);
            iShowImage(r.x, r.y, r.w, r.h, frame);
        }
        return;
    }

    // ---- Mirror character state for 3 clones ----
    if (clonePhase == CLONE_RUNNING) {
        for (int i = 0; i < 3; i++) {
            // Clones staggered behind the character based on direction
            bool isBack = isLeftArrowPressed || isSpecialKeyPressed(GLUT_KEY_LEFT);
            int offset = (i + 1) * 80;
            int cx = isBack ? charX + offset : charX - offset;
            int cy = charY;

            // Clones mirror the exact same state/frame as character
            int rX, rY, rW, rH, frame;
            getCharacterFrameInfo(cx, cy, rX, rY, rW, rH, frame);

            if (frame != -1) {
                iShowImage(rX, rY, rW, rH, frame);
            }
        }
    }
}

// ============================================================
//  Public query helper
// ============================================================
inline bool isCloneSystemActive() {
    return (clonePhase != CLONE_IDLE);
}

inline bool isCloneMirroringActive() {
    return (clonePhase == CLONE_RUNNING);
}

#endif // CLONEATTACK_H
