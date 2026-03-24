#ifndef BOSSHANDLER_H
#define BOSSHANDLER_H

#include "iGraphics.h"
#include <stdio.h>

// Boss Images
extern int bossImgs[4];
extern int bossFlameImgs[9];
extern int bg41, bg42, bg43;

// Boss State
struct Boss {
    float x, y;
    int frameIndex;
    int flameFrameIndex;
    bool active;
    int animCounter;
    int flameAnimCounter;
};

extern Boss level4Boss;

// Parameters
#define BOSS_W 400
#define BOSS_H 400
#define BOSS_FLAME_W 100
#define BOSS_FLAME_H 80
#define BOSS_FLAME_OFFSET_Y 10

inline void loadBossAssets() {
    char path[100];
    
    // Load backgrounds
    bg41 = iLoadImage((char*)"Boss\\bg41.png");
    bg42 = iLoadImage((char*)"Boss\\bg42.png");
    bg43 = iLoadImage((char*)"Boss\\bg43.png");
    
    // Load boss images (rest11 to rest14)
    for (int i = 0; i < 4; i++) {
        sprintf_s(path, sizeof(path), "Boss\\rest1%d.png", i + 1);
        bossImgs[i] = iLoadImage(path);
    }
    
    // Load flame images (flame (1) to flame (9))
    for (int i = 0; i < 9; i++) {
        sprintf_s(path, sizeof(path), "Boss\\flame (%d).png", i + 1);
        bossFlameImgs[i] = iLoadImage(path);
    }
}

inline void initBoss() {
    level4Boss.active = false;
    level4Boss.x = 600; // Right side
    level4Boss.y = 80;  // Match ground level roughly
    level4Boss.frameIndex = 0;
    level4Boss.flameFrameIndex = 0;
    level4Boss.animCounter = 0;
    level4Boss.flameAnimCounter = 0;
}

inline void updateBossAnimation() {
    if (!level4Boss.active) return;
    
    // Boss rest animation
    level4Boss.animCounter++;
    if (level4Boss.animCounter >= 10) {
        level4Boss.animCounter = 0;
        level4Boss.frameIndex = (level4Boss.frameIndex + 1) % 4;
    }
    
    // Flame animation
    level4Boss.flameAnimCounter++;
    if (level4Boss.flameAnimCounter >= 4) {
        level4Boss.flameAnimCounter = 0;
        level4Boss.flameFrameIndex = (level4Boss.flameFrameIndex + 1) % 9;
    }
}

inline void drawBoss() {
    if (!level4Boss.active) return;
    
    // Draw Boss
    iShowImage((int)level4Boss.x, (int)level4Boss.y, BOSS_W, BOSS_H, bossImgs[level4Boss.frameIndex]);
    
    // Draw Flames at the bottom of the boss
    // Anchor flames to the bottom edge of the boss with a small 10px overlap/gap for "continuous effect"
    int flameY = (int)level4Boss.y - 10; 
    
    // Distribute 4 small flames across the boss base (~400px width)
    iShowImage((int)level4Boss.x + 20, flameY, BOSS_FLAME_W, BOSS_FLAME_H, bossFlameImgs[level4Boss.flameFrameIndex]);
    iShowImage((int)level4Boss.x + 120, flameY, BOSS_FLAME_W, BOSS_FLAME_H, bossFlameImgs[level4Boss.flameFrameIndex]);
    iShowImage((int)level4Boss.x + 220, flameY, BOSS_FLAME_W, BOSS_FLAME_H, bossFlameImgs[level4Boss.flameFrameIndex]);
    iShowImage((int)level4Boss.x + 320, flameY, BOSS_FLAME_W, BOSS_FLAME_H, bossFlameImgs[level4Boss.flameFrameIndex]);
}

#endif
