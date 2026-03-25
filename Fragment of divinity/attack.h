// attack.h
// Teleport Attack system for Level 4.
//
// FLOW:
//  1. Press 'B' (during level4Phase == 3)
//     -> Starts teleport attack animation.
//  2. Animation plays frames 1 to 3.
//  3. If frame 3 is reached AND boss is close to character, drain boss power.

#ifndef ATTACK_H
#define ATTACK_H

#include "iGraphics.h"
#include "GameState.h"
#include "Boss.h"
#include "CharacterCustomization.h"
#include <stdio.h>

// ---- externals ----
extern int charX, charY, charWidth, charHeight;
extern int selectedCharacter; // 0=Aryn, 1=Kaero, 2=Leora
extern int currentLevel;
extern GameState gameState;

// ============================================================
//  Teleport Attack image storage
// ============================================================
static int arynTeleportImgs[3];
static int kaeroTeleportImgs[3];
static int leoraTeleportImgs[3];

static int arynMeteroidImgs[3];
static int kaeroMeteroidImgs[3];
static int leoraMeteroidImgs[3];

// ============================================================
//  State variables
// ============================================================
extern bool isTeleportAttacking;
extern int teleportFrameIndex;
extern int teleportAnimCounter;
static const int TELEPORT_ANIM_SPEED = 5; // ticks per frame

extern bool isMeteroidAttacking;
extern int meteroidFrameIndex;
extern int meteroidAnimCounter;
static const int METEROID_ANIM_SPEED = 5;

// ============================================================
//  Load Assets
// ============================================================
inline void loadTeleportAttackAssets() {
    static bool loaded = false;
    if (loaded) return;
    loaded = true;
    char p[120];

    // 1. Load Teleport Attack Assets
    for (int i = 0; i < 3; i++) {
        sprintf_s(p, sizeof(p), "aryn\\arynteleportattack%d.png", i + 1);
        arynTeleportImgs[i] = iLoadImage((char*)p);
        
        sprintf_s(p, sizeof(p), "Character\\kaeroteleportattack%d.png", i + 1);
        kaeroTeleportImgs[i] = iLoadImage((char*)p);
        
        sprintf_s(p, sizeof(p), "Leora\\leorateleportattack%d.png", i + 1);
        leoraTeleportImgs[i] = iLoadImage((char*)p);
    }

    // 2. Load Meteoroid Attack Assets
    for (int i = 0; i < 3; i++) {
        FILE *f = NULL;
        // --- ARYN ---
        bool arynLoaded = false;
        const char* names[] = { "aryn\\arynmeteroidattack%d.png", "aryn/arynmeteroidattack%d.png", "aryn\\arynmeteorattack%d.png", "aryn/arynmeteorattack%d.png" };
        
        for (int k = 0; k < 4; k++) {
            sprintf_s(p, sizeof(p), names[k], i + 1);
            fopen_s(&f, p, "rb");
            if (f) {
                fclose(f);
                arynMeteroidImgs[i] = iLoadImage((char*)p);
                if (arynMeteroidImgs[i] > 0) {

                    arynLoaded = true;
                    break;
                }
            }
        }
        if (!arynLoaded) {
            arynMeteroidImgs[i] = 0;

        }

        // --- KAERO ---
        sprintf_s(p, sizeof(p), "Character\\kaerometeroidattack%d.png", i + 1);
        fopen_s(&f, p, "rb");
        if (f) {
            fclose(f);
            kaeroMeteroidImgs[i] = iLoadImage((char*)p);
        } else {
            kaeroMeteroidImgs[i] = 0;
        }

        // --- LEORA ---
        sprintf_s(p, sizeof(p), "Leora\\leorameteroidattack%d.png", i + 1);
        fopen_s(&f, p, "rb");
        if (f) {
            fclose(f);
            leoraMeteroidImgs[i] = iLoadImage((char*)p);
        } else {
            leoraMeteroidImgs[i] = 0;
        }
    }
}

// ============================================================
//  Trigger
// ============================================================
inline void triggerTeleportAttack() {
    if (currentLevel != 4 || gameState != GAME || level4Phase != 3) return;
    if (isTeleportAttacking) return;

    isTeleportAttacking = true;
    teleportFrameIndex = 0;
    teleportAnimCounter = 0;
}

inline void triggerMeteroidAttack() {
    if (currentLevel != 4 || gameState != GAME || level4Phase != 3) return;
    if (isMeteroidAttacking) return;

    isMeteroidAttacking = true;
    meteroidFrameIndex = 0;
    meteroidAnimCounter = 0;
}

// ============================================================
//  Collision/Hit Detection
// ============================================================
extern bool isCloneMirroringActive();
extern bool isLeftArrowPressed;

inline void applyTeleportDamage() {
    float bossX = boss4Obj.x;
    float bossW = (float)BOSS_WIDTH;
    
    // Check for character
    if (fabsf((float)charX - bossX) < 200.0f) {
        boss4Obj.life -= 10;
        boss4Obj.isHit = true;
        boss4Obj.hitTimer = 15;
    }

    // Check for clones if active
    if (isCloneMirroringActive()) {
        bool isBack = isLeftArrowPressed || isSpecialKeyPressed(GLUT_KEY_LEFT);
        for (int i = 0; i < 3; i++) {
            int offset = (i + 1) * 80;
            int cx = isBack ? charX + offset : charX - offset;
            if (fabsf((float)cx - bossX) < 200.0f) {
                boss4Obj.life -= 10;
                boss4Obj.isHit = true;
                boss4Obj.hitTimer = 15;
            }
        }
    }

    if (boss4Obj.life <= 0) {
        boss4Obj.life = 0;
        level4Phase = 4;
        boss4Obj.dieFrameIndex = 0;
        boss4Obj.dieAnimCounter = 0;
    }
}

inline void applyMeteroidVanish() {
    bool isBack = isLeftArrowPressed || isSpecialKeyPressed(GLUT_KEY_LEFT);
    int numEntities = isCloneMirroringActive() ? 4 : 1;

    for (int e = 0; e < numEntities; e++) {
        int cx = charX;
        if (e > 0) {
            int offset = e * 80;
            cx = isBack ? charX + offset : charX - offset;
        }

        float charBoxX1 = (float)cx - 50.0f;
        float charBoxX2 = (float)cx + (float)charWidth + 50.0f;
        float charBoxY1 = (float)charY - 50.0f;
        float charBoxY2 = (float)charY + (float)charHeight + 200.0f;

        // Vanish falling fireballs from Skill 5 (topObstacles)
        for (int i = 0; i < 10; i++) {
            if (boss4Obj.topObstacles[i].active) {
                float ox = boss4Obj.topObstacles[i].x;
                float oy = boss4Obj.topObstacles[i].y;
                if (ox + 50.0f > charBoxX1 && ox < charBoxX2 &&
                    oy + 50.0f > charBoxY1 && oy < charBoxY2) {
                    boss4Obj.topObstacles[i].active = false;
                }
            }
        }

        // Vanish standard fireballs (Skill 6)
        if (boss4Obj.fbActive) {
            float fbx = boss4Obj.fbX;
            float fby = boss4Obj.fbY;
            if (fbx + 80.0f > charBoxX1 && fbx < charBoxX2 &&
                fby + 80.0f > charBoxY1 && fby < charBoxY2) {
                boss4Obj.fbActive = false;
            }
        }
    }
}

// ============================================================
//  Update
// ============================================================
inline void updateTeleportAttackAnimation() {
    if (!isTeleportAttacking) return;

    teleportAnimCounter++;
    if (teleportAnimCounter >= TELEPORT_ANIM_SPEED) {
        teleportAnimCounter = 0;
        teleportFrameIndex++;
        
        // Special logic: If last image (index 2) is reached, check for hit
        if (teleportFrameIndex == 2) {
            applyTeleportDamage();
        }
        
        if (teleportFrameIndex >= 3) {
            isTeleportAttacking = false;
            teleportFrameIndex = 0;
        }
    }
}

inline void updateMeteroidAttackAnimation() {
    if (!isMeteroidAttacking) return;

    int maxFrames = 3;
    if (selectedCharacter == 0) maxFrames = 2; // Aryn has 2 meteor images

    meteroidAnimCounter++;
    if (meteroidAnimCounter >= METEROID_ANIM_SPEED) {
        meteroidAnimCounter = 0;
        meteroidFrameIndex++;

        // Last frame reached: vanish fireballs
        if (meteroidFrameIndex == maxFrames - 1) {
            applyMeteroidVanish();
        }

        if (meteroidFrameIndex >= maxFrames) {
            isMeteroidAttacking = false;
            meteroidFrameIndex = 0;
        }
    }
}

// ============================================================
//  Draw
// ============================================================
inline void drawTeleportAttack() {
    if (!isTeleportAttacking) return;

    int img = -1;
    if (selectedCharacter == 0) img = arynTeleportImgs[teleportFrameIndex];
    else if (selectedCharacter == 1) img = kaeroTeleportImgs[teleportFrameIndex];
    else if (selectedCharacter == 2) img = leoraTeleportImgs[teleportFrameIndex];

    if (img > 0) {
        iSetColor(255, 255, 255);
        bool isBack = isLeftArrowPressed || isSpecialKeyPressed(GLUT_KEY_LEFT);
        
        // Draw for character
        iShowImage(charX, charY, charWidth, charHeight, img);

        // Draw for clones if mirroring
        if (isCloneMirroringActive()) {
            for (int i = 0; i < 3; i++) {
                int offset = (i + 1) * 80;
                int cx = isBack ? charX + offset : charX - offset;
                iShowImage(cx, charY, charWidth, charHeight, img);
            }
        }
    }
}

inline void drawMeteroidAttack() {
    if (!isMeteroidAttacking) return;

    int img = -1;
    if (selectedCharacter == 0) img = arynMeteroidImgs[meteroidFrameIndex];
    else if (selectedCharacter == 1) img = kaeroMeteroidImgs[meteroidFrameIndex];
    else if (selectedCharacter == 2) img = leoraMeteroidImgs[meteroidFrameIndex];

    if (img > 0) {
        iSetColor(255, 255, 255);
        bool isBack = isLeftArrowPressed || isSpecialKeyPressed(GLUT_KEY_LEFT);

        // Draw for character
        iShowImage(charX, charY, charWidth, charHeight, img);

        // Draw for clones if mirroring
        if (isCloneMirroringActive()) {
            for (int i = 0; i < 3; i++) {
                int offset = (i + 1) * 80;
                int cx = isBack ? charX + offset : charX - offset;
                iShowImage(cx, charY, charWidth, charHeight, img);
            }
        }
    }
}

#endif // ATTACK_H
