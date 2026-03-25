#ifndef GUNATTACK_H
#define GUNATTACK_H

#include "iGraphics.h"
#include <stdio.h>
#include <math.h>

// Globals from elsewhere to interact with enemies
#include "Character.h"
#include "ObstacleHandler.h"
#include "dragon.h"

// Define max values for enemies if not exposed


extern WalkingNPC npcList[MAX_NPCS];
extern Skull skulls[3];
extern int currentLevel;

// Fireballs
#define MAX_FIREBALLS 40
struct Fireball {
    float x;
    float y;
    bool active;
    int dir; // 1 = right, -1 = left
    int frameIndex;
    int animCounter;
    int type; // 0 = blue (for Kaero/Character), 1 = red (for Aryn/Leora)
};

extern Fireball fireballs[MAX_FIREBALLS];
extern bool isGunAttacking;
extern int gunAttackFrameIndex;
extern int gunAttackAnimCounter;

extern int arynGun[3];
extern int kaeroGun[3];
extern int leoraGun[3];

extern int blueFireball[3];
extern int blueFireballBack[3];
extern int redFireball[3];
extern int redFireballBack[3];

inline void loadGunAttackAssets() {
    static bool loaded = false;
    if (loaded) return;
    loaded = true;

    char filename[100];
    
    // Load Aryn Gun (3 frames)
    for (int i = 0; i < 3; i++) {
        sprintf_s(filename, sizeof(filename), "aryn\\aryngun%d.png", i + 1);
        arynGun[i] = iLoadImage((char*)filename);
    }
    
    // Load Kaero Gun (3 frames)
    for (int i = 0; i < 3; i++) {
        sprintf_s(filename, sizeof(filename), "Character\\kaerogun%d.png", i + 1);
        kaeroGun[i] = iLoadImage((char*)filename);
    }
    
    // Load Leora Gun (3 frames)
    for (int i = 0; i < 3; i++) {
        sprintf_s(filename, sizeof(filename), "Leora\\leoragun%d(1).png", i + 1);
        leoraGun[i] = iLoadImage((char*)filename);
    }

    // Load Fireballs (3 frames each)
    for (int i = 0; i < 3; i++) {
        sprintf_s(filename, sizeof(filename), "Character\\blue%d.png", i + 1);
        blueFireball[i] = iLoadImage((char*)filename);
        sprintf_s(filename, sizeof(filename), "Character\\blueback%d.png", i + 1);
        blueFireballBack[i] = iLoadImage((char*)filename);
        
        sprintf_s(filename, sizeof(filename), "aryn\\redfireball%d.png", i + 1);
        redFireball[i] = iLoadImage((char*)filename);
        sprintf_s(filename, sizeof(filename), "aryn\\redfireballback%d.png", i + 1);
        redFireballBack[i] = iLoadImage((char*)filename);
    }
}

inline int getSelectedCharacterGunImage(int frame, bool isBack) {
    // 0: Aryn, 1: Kaero, 2: Leora
    if (selectedCharacter == 0) {
        if (frame < 0 || frame >= 3) return -1;
        return arynGun[frame]; // Only standard gun images exist, no "back"
    } else if (selectedCharacter == 1) {
        if (frame < 0 || frame >= 3) return -1;
        return kaeroGun[frame];
    } else if (selectedCharacter == 2) {
        if (frame < 0 || frame >= 3) return -1;
        return leoraGun[frame];
    }
    return -1;
}

inline void spawnFireball() {
    bool leftKey = isLeftArrowPressed || isSpecialKeyPressed(GLUT_KEY_LEFT);
    int dir = leftKey ? -1 : 1;
    int type = (selectedCharacter == 1) ? 0 : 1; 
    extern bool isCloneMirroringActive();

    // Spawn for character
    for (int i = 0; i < MAX_FIREBALLS; i++) {
        if (!fireballs[i].active) {
            fireballs[i].active = true;
            fireballs[i].dir = dir;
            int spawnOffsetX = leftKey ? -20 : charWidth;
            fireballs[i].x = (float)charX + spawnOffsetX;
            fireballs[i].y = (float)charY + (float)charHeight * 0.5f;
            fireballs[i].frameIndex = 0;
            fireballs[i].animCounter = 0;
            fireballs[i].type = type;
            break;
        }
    }

    // Spawn for clones if active
    if (isCloneMirroringActive()) {
        for (int c = 0; c < 3; c++) {
            int offset = (c + 1) * 80;
            int cx = leftKey ? charX + offset : charX - offset;
            for (int i = 0; i < MAX_FIREBALLS; i++) {
                if (!fireballs[i].active) {
                    fireballs[i].active = true;
                    fireballs[i].dir = dir;
                    int spawnOffsetX = leftKey ? -20 : charWidth;
                    fireballs[i].x = (float)cx + spawnOffsetX;
                    fireballs[i].y = (float)charY + (float)charHeight * 0.5f;
                    fireballs[i].frameIndex = 0;
                    fireballs[i].animCounter = 0;
                    fireballs[i].type = type;
                    break;
                }
            }
        }
    }
}

inline void updateGunAttackAnimation() {
    if (!isGunAttacking) return;
    
    int maxFrames = 3;
    
    // Animate the gun sequence
    gunAttackAnimCounter++;
    if (gunAttackAnimCounter >= 4) { // speed of animation
        gunAttackAnimCounter = 0;
        gunAttackFrameIndex++;
        
        // When reaching the last frame, spawn fireball and end attack
        if (gunAttackFrameIndex >= maxFrames) {
            spawnFireball();
            isGunAttacking = false;
            gunAttackFrameIndex = 0;
        }
    }
}

inline void updateFireballs() {
    float speed = 25.0f;
    for (int i = 0; i < MAX_FIREBALLS; i++) {
        if (fireballs[i].active) {
            fireballs[i].x += fireballs[i].dir * speed;
            
            fireballs[i].animCounter++;
            if (fireballs[i].animCounter >= 3) {
                fireballs[i].animCounter = 0;
                fireballs[i].frameIndex = (fireballs[i].frameIndex + 1) % 3;
            }
            
            if (fireballs[i].x < -100 || fireballs[i].x > 1100) {
                fireballs[i].active = false;
                continue;
            }
            
            float fX = fireballs[i].x;
            float fY = fireballs[i].y;
            
            for (int k = 0; k < 2; k++) {
                if (sharks[k].active && !sharks[k].isDying) {
                    if (fX > sharks[k].x && fX < sharks[k].x + 70 &&
                        fY > sharks[k].y && fY < sharks[k].y + 70) {
                        sharks[k].isDying = true;
                        sharks[k].dyingVel = 5.0f;
                        fireballs[i].active = false;
                    }
                }
            }
            
            for (int k = 0; k < 5; k++) {
                if (dragons[k].active && !dragons[k].isVanishing) {
                    float targetX = dragons[k].x + 150.0f;
                    float targetY = dragons[k].y + 150.0f;
                    float dx = fX - targetX;
                    float dy = fY - targetY;
                    float dist = sqrt(dx * dx + dy * dy);
                    if (dist < 100.0f) {
                        dragons[k].isVanishing = true;
                        dragons[k].vanishingTimer = 30;
                        fireballs[i].active = false;
                    }
                }
            }
            
            for (int k = 0; k < 3; k++) {
                if (skulls[k].active && !skulls[k].isVanishing) {
                    float targetX = skulls[k].x + 30.0f;
                    float targetY = skulls[k].y + 30.0f;
                    float dx = fX - targetX;
                    float dy = fY - targetY;
                    float dist = sqrt(dx * dx + dy * dy);
                    if (dist < 40.0f) {
                        skulls[k].isVanishing = true;
                        skulls[k].vanishingTimer = 10;
                        fireballs[i].active = false;
                    }
                }
            }
            
            for (int k = 0; k < MAX_NPCS; k++) {
                if (npcList[k].active) {
                    if (fX > npcList[k].x + 20 && fX < npcList[k].x + 130 &&
                        fY > npcList[k].y && fY < npcList[k].y + 150) {
                        npcList[k].life = 0;
                        npcList[k].active = false;
                        fireballs[i].active = false;
                    }
                }
            }
        }
    }
}

inline void drawFireballs() {
    for (int i = 0; i < MAX_FIREBALLS; i++) {
        if (fireballs[i].active) {
            int img = -1;
            if (fireballs[i].type == 0) {
                if (fireballs[i].dir == 1) img = blueFireball[fireballs[i].frameIndex];
                else img = blueFireballBack[fireballs[i].frameIndex];
            } else {
                if (fireballs[i].dir == 1) img = redFireball[fireballs[i].frameIndex];
                else img = redFireballBack[fireballs[i].frameIndex];
            }
            
            if (img != -1) {
                iShowImage((int)fireballs[i].x - 20, (int)fireballs[i].y - 20, 60, 60, img);
            }
        }
    }
}

#endif // GUNATTACK_H
