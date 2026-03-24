#pragma once

#include "iGraphics.h"
#include <stdlib.h>
#include <math.h>
#include "GameState.h"

#define CINDER_FLY_FRAMES 4
#define CINDER_ATTACK_FRAMES 4

// External variables from iMain.cpp
extern int charX;
extern int charY;
extern int charWidth;
extern int charHeight;
extern bool isAttacking;
extern int attackFrameIndex;
extern int lives;
extern bool isInvincible;
extern int invincibilityTimer;
extern GameState gameState;
extern bool isShiftPressed; // From iMain.cpp
extern bool npcSlashDone; // From iMain.cpp

int cinderFlyImgs[CINDER_FLY_FRAMES];
int cinderAttackImgs[CINDER_ATTACK_FRAMES];
int cinderBackFlyImgs[CINDER_FLY_FRAMES];
int cinderBackAttackImgs[CINDER_ATTACK_FRAMES];

struct CinderEnemy {
    float x, y;
    bool active;
    bool isVanishing;
    int state; // 0 = flying to char, 1 = attacking
    bool facingRight; 
    
    int animCounter;
    int flyFrameIndex;
    int attackFrameIndex;
    int blinkTimer;       
    int spawnTimer;
    int cooldownTimer;
    bool hasDamaged; // Tracks if damage applied during current attack
    int hitCount;
    bool isHitFlash;
    int hitFlashTimer;
};

CinderEnemy cinderEnemy;

void loadCinderAssets() {
    cinderFlyImgs[0] = iLoadImage("Cinder\\cinder1.png");
    cinderFlyImgs[1] = iLoadImage("Cinder\\cinder2.png");
    cinderFlyImgs[2] = iLoadImage("Cinder\\cinder3.png");
    cinderFlyImgs[3] = iLoadImage("Cinder\\cinder4.png");
    
    cinderAttackImgs[0] = iLoadImage("Cinder\\cinderattack1.png");
    cinderAttackImgs[1] = iLoadImage("Cinder\\cinderattack2.png");
    cinderAttackImgs[2] = iLoadImage("Cinder\\cinderattack3.png");
    cinderAttackImgs[3] = iLoadImage("Cinder\\cinderattack4.png");

    cinderBackFlyImgs[0] = iLoadImage("Cinder\\cinderback1.png");
    cinderBackFlyImgs[1] = iLoadImage("Cinder\\cinderback2.png");
    cinderBackFlyImgs[2] = iLoadImage("Cinder\\cinderback3.png");
    cinderBackFlyImgs[3] = iLoadImage("Cinder\\cinderback4.png");
    
    cinderBackAttackImgs[0] = iLoadImage("Cinder\\cinderbackattack1.png");
    cinderBackAttackImgs[1] = iLoadImage("Cinder\\cinderbackattack2.png");
    cinderBackAttackImgs[2] = iLoadImage("Cinder\\cinderbackattack3.png");
    cinderBackAttackImgs[3] = iLoadImage("Cinder\\cinderbackattack4.png");
}

void initCinder() {
    cinderEnemy.active = false;
    cinderEnemy.isVanishing = false;
    cinderEnemy.state = 0;
    cinderEnemy.flyFrameIndex = 0;
    cinderEnemy.attackFrameIndex = 0;
    cinderEnemy.animCounter = 0;
    cinderEnemy.blinkTimer = 0;
    cinderEnemy.spawnTimer = 60; // Spawn quickly
    cinderEnemy.cooldownTimer = 0;
    cinderEnemy.facingRight = false;
    cinderEnemy.hasDamaged = false;
    cinderEnemy.hitCount = 0;
    cinderEnemy.isHitFlash = false;
    cinderEnemy.hitFlashTimer = 0;
}

void updateCinderPhysics() {
    if (!cinderEnemy.active && !cinderEnemy.isVanishing) {
        cinderEnemy.spawnTimer--;
        if (cinderEnemy.spawnTimer <= 0) {
            cinderEnemy.active = true;
            cinderEnemy.x = 1050; // Coming from right out of screen
            cinderEnemy.y = 150 + rand() % 250; 
            cinderEnemy.state = 0;
            cinderEnemy.blinkTimer = 0;
            cinderEnemy.facingRight = false;
            cinderEnemy.cooldownTimer = 0;
        }
        return;
    }
    
    if (cinderEnemy.isVanishing) {
        cinderEnemy.blinkTimer--;
        if (cinderEnemy.blinkTimer <= 0) {
            cinderEnemy.active = false;
            cinderEnemy.isVanishing = false;
            cinderEnemy.spawnTimer = 60; // quick respawn when hit
        }
        return;
    }
    
    if (cinderEnemy.isHitFlash) {
        cinderEnemy.hitFlashTimer--;
        if (cinderEnemy.hitFlashTimer <= 0) cinderEnemy.isHitFlash = false;
    }
    
    float targetX = charX + charWidth / 2.0f;
    float targetY = charY + charHeight / 2.0f;
    float dx = targetX - (cinderEnemy.x + 100.0f); // Half of 200x200
    float dy = targetY - (cinderEnemy.y + 100.0f);
    float dist = sqrt(dx * dx + dy * dy);
    
    // Determine facing based on character X (if dx > 0, cinder needs to look right)
    cinderEnemy.facingRight = (dx > 0);
    
    if (cinderEnemy.cooldownTimer > 0) cinderEnemy.cooldownTimer--;

    if (cinderEnemy.state == 0) {
        float speed = 6.0f;
        if (cinderEnemy.cooldownTimer > 0) {
            // Flies normally up high
            float targetAltitude = charY + 300.0f;
            float dyUp = targetAltitude - (cinderEnemy.y + 100.0f); // Use half of 200 size
            
            // Still follows character X but targets a high Y coordinate
            if (dist > 0) {
                cinderEnemy.x += (dx / dist) * speed; 
                float moveY = speed * 0.8f;
                if (fabs(dyUp) <= moveY) cinderEnemy.y += dyUp;
                else cinderEnemy.y += (dyUp > 0 ? 1 : -1) * moveY;
            }
        } else {
            // Normal chase
            if (dist > 0) {
                cinderEnemy.x += (dx / dist) * speed;
                cinderEnemy.y += (dy / dist) * speed;
            }
            
            if (dist < 150.0f) { // Comes much closer before attacking
                cinderEnemy.state = 1; // Transition to attack
                cinderEnemy.attackFrameIndex = 0;
                cinderEnemy.animCounter = 0;
                cinderEnemy.hasDamaged = false;
            }
        }
    } else if (cinderEnemy.state == 1) {
        // Track slowly while attacking
        float speed = 3.0f; // Track slightly faster during attack
        if (dist > 0) {
            float actualSpeed = speed;
            if (dist < speed) actualSpeed = dist; // Prevent shaking
            cinderEnemy.x += (dx / dist) * actualSpeed;
            cinderEnemy.y += (dy / dist) * actualSpeed;
        }

        // Deal damage specifically at the very last attack frame (frame 3)
        if (cinderEnemy.attackFrameIndex == 3 && !cinderEnemy.hasDamaged) {
            // Require it to be virtually touching the player
            if (!isInvincible && dist < 120.0f) { 
                lives--;
                if (lives <= 0) {
                    gameState = GAME_OVER;
                } else {
                    isInvincible = true;
                    invincibilityTimer = 60;
                }
            }
            cinderEnemy.hasDamaged = true;
        }
        
        // If character tries to run far away from attack range, immediately break attack
        if (dist > 250.0f) { // Shrink distance threshold to break off attack
            cinderEnemy.state = 0;
            cinderEnemy.cooldownTimer = 120; // Stays up normally for a while
        }
    }
    
    // Animation updates
    cinderEnemy.animCounter++;
    if (cinderEnemy.animCounter >= 5) {
        cinderEnemy.animCounter = 0;
        if (cinderEnemy.state == 1) {
            cinderEnemy.attackFrameIndex++;
            if (cinderEnemy.attackFrameIndex >= CINDER_ATTACK_FRAMES) {
                // Done full attack sequence, return to up/flying mode with a cooldown
                cinderEnemy.state = 0;
                cinderEnemy.cooldownTimer = 90; // Wait before bombing down again
                cinderEnemy.attackFrameIndex = 0;
            }
        } else {
            cinderEnemy.flyFrameIndex = (cinderEnemy.flyFrameIndex + 1) % CINDER_FLY_FRAMES;
        }
    }
}

void checkCinderHit() {
    if (!cinderEnemy.active || cinderEnemy.isVanishing) return;
    
    // When character hits cinder using the attack slash
    if (isAttacking && attackFrameIndex == 3 && !npcSlashDone) { 
        float playerReachRight = charX + charWidth + 100;
        float playerReachLeft = charX - 20;
        float targetX = cinderEnemy.x + 100.0f; // Half of 200x200 
        float targetY = cinderEnemy.y + 100.0f;
        
        // Exact bounding box to easily hit Cinder
        if (targetX > playerReachLeft && targetX < playerReachRight && 
            targetY > charY - 50 && targetY < charY + charHeight + 150) {
            
            cinderEnemy.hitCount++;
            if (cinderEnemy.hitCount >= 2) {
                // Disappears immediately
                cinderEnemy.isVanishing = true;
                cinderEnemy.blinkTimer = 30; // Quick blink off screen
                cinderEnemy.hitCount = 0;
            } else {
                // Flash briefly on first hit
                cinderEnemy.isHitFlash = true;
                cinderEnemy.hitFlashTimer = 15;
            }
            
            npcSlashDone = true; // Mark slash as used so we don't double-hit
        }
    }
}

void drawCinder() {
    if (!cinderEnemy.active) return;
    
    // Blinking effect when dying
    if (cinderEnemy.isVanishing) {
        if ((cinderEnemy.blinkTimer / 3) % 2 == 0) return; // Flash quickly
    }
    if (cinderEnemy.isHitFlash) {
        if ((cinderEnemy.hitFlashTimer / 2) % 2 == 0) return;
    }
    
    // Draw logic
    if (cinderEnemy.state == 1) {
        if (cinderEnemy.facingRight) {
            iShowImage(cinderEnemy.x, cinderEnemy.y, 200, 200, cinderAttackImgs[cinderEnemy.attackFrameIndex]);
        } else {
            iShowImage(cinderEnemy.x, cinderEnemy.y, 200, 200, cinderBackAttackImgs[cinderEnemy.attackFrameIndex]);
        }
    } else {
        if (cinderEnemy.facingRight) {
            iShowImage(cinderEnemy.x, cinderEnemy.y, 200, 200, cinderFlyImgs[cinderEnemy.flyFrameIndex]);
        } else {
            iShowImage(cinderEnemy.x, cinderEnemy.y, 200, 200, cinderBackFlyImgs[cinderEnemy.flyFrameIndex]);
        }
    }
}
