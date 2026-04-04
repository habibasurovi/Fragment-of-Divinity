#ifndef CHARACTER_H
#define CHARACTER_H

#include "iGraphics.h"
#include "Boss.h"
#include <math.h>

// Character Global Variables
extern int charX; // Adjusted starting position
extern int charY;
extern int charWidth;
extern int charHeight;

extern int charFrameIndex;
extern bool isCharMoving;
extern int lives;
extern bool isInvincible;
extern int invincibilityTimer;
extern bool isCharFrozen;
extern bool isFallingSequence;
extern float fallY;
extern float fallVel;

// Animation Speed Control
extern int charAnimSpeed; // Adjust this to change speed (Higher = Slower)
extern int charAnimCounter;

// Jump & Bend Variables
extern bool isJumping;
extern bool isBending;
extern int jumpSpeed;        // Peak naturally at 180
extern int highJumpSpeed;    // Higher jump for spacebar
extern int gravity;          // Gravity effect
extern int verticalVelocity; // Current vertical speed
extern int groundY;          // Ground level
extern int originalHeight;
extern int bendHeight;     // Reduced height for bending
extern int jumpImageIndex; // 0: Start, 1: Mid, 2: Land

#include "CharacterCustomization.h"
#include "character 2.h"

// Helpers defined in CharacterCustomization.h are already included via
// CharacterCustomization.h

// Helpers and variables defined in character 2.h / iMain.cpp
extern int getCharacterRunImage(int frameIndex, bool isBack);
extern bool isRightArrowPressed;
extern bool isLeftArrowPressed;
extern int runFrameIndex;
extern int currentJumpDirection;
extern int jumpHorizontalSpeed;

extern bool isAttacking;
extern int attackFrameIndex;
extern int jumpPressTimer;
extern int bendPressTimer;
extern bool prevUpPressed;
extern bool prevDownPressed;
extern int doubleJumpSpeed;
extern int doubleJumpHorizontalSpeed;
extern int doubleBendHeight;
extern int currentLevel;
extern bool isShiftPressed;
extern bool isDoubleShiftTriggered;
extern unsigned int keyPressed[512];
extern unsigned int specialKeyPressed[512];

extern bool isGunAttacking;
extern int gunAttackFrameIndex;
extern bool isJumpAttacking;
extern int jumpAttackFrameIndex;
extern int jumpAttackAnimCounter;
int getSelectedCharacterGunImage(int frame, bool isBack);
int getSelectedCharacterJumpAttackImage(int index);
int getSelectedCharacterStaticImage(int frameIndex, bool isBack);

inline bool isSpecialKeyPressed(int key) {
  if (key < 0 || key >= 512)
    return false;
  return specialKeyPressed[key] != 0;
}

inline bool isKeyPressed(int key) {
  if (key < 0 || key >= 512)
    return false;
  return keyPressed[key] != 0;
}

// Movement helpers
inline void moveCharRight() {
  if (charX + charWidth < 1000) { // Screen Width Boundary (1000)
    if (currentLevel == 4) charX += 8; // Slower walking speed for Level 4
    else charX += 20;                  // Increased Speed
  }
}

inline void moveCharLeft() {
  if (charX > 0) { // Left Boundary
    if (currentLevel == 4) charX -= 8; // Slower walking speed for Level 4
    else charX -= 20;   // Increased Speed
  }
}

inline void stopCharMovement() {
  charFrameIndex = 0; // Reset frame to stand/first image
}

// Draw character
extern bool isHaloActive;
// Helper to get frame info for drawing character or clones
inline void getCharacterFrameInfo(int customX, int customY, int &rX, int &rY, int &rW, int &rH, int &frame) {
  bool isBack = isLeftArrowPressed || isSpecialKeyPressed(GLUT_KEY_LEFT);
  rX = customX; rY = customY; rW = charWidth; rH = charHeight;
  frame = -1;

  if (isFallingSequence) {
    frame = getSelectedCharacterFallImage();
    rY = (int)fallY - (charY - customY); // Adjust relative to current charY
    rW = 125;
    rH = 125;
  } else if (isGunAttacking) {
    frame = getSelectedCharacterGunImage(gunAttackFrameIndex, isBack);
  } else if (isAttacking) {
    frame = getSelectedCharacterAttackImage(attackFrameIndex, isBack);
  } else if (isJumping) {
    if (isJumpAttacking && jumpAttackFrameIndex < 2) {
      frame = getSelectedCharacterJumpAttackImage(jumpAttackFrameIndex);
    } else {
      int currentJumpImg = 0;
      if (verticalVelocity > 0)
        currentJumpImg = 0; // jump1
      else if (verticalVelocity > -10)
        currentJumpImg = 1; // jump2
      else
        currentJumpImg = 2; // jump3

      frame = getSelectedCharacterJumpImage(currentJumpImg, isBack);
    }
  } else if (isRightArrowPressed || isLeftArrowPressed) {
    if (currentLevel == 4 && level4Phase == 1) {
      frame = getSelectedCharacterImage(charFrameIndex, isBack);
    } else {
      frame = getCharacterRunImage(runFrameIndex, isBack);
      if (selectedCharacter == 1) { // Kaero specifically needs offsets for run images
        rX = customX - 25;
        rY = customY - 20;
        rW = charWidth + 50;
        rH = charHeight + 50;
      }
    }
  } else {
    // Idle
    if (currentLevel == 4) {
      frame = getSelectedCharacterStaticImage(charFrameIndex, isBack);
    } else {
      frame = getSelectedCharacterImage(charFrameIndex, isBack);
    }
  }
}

// Draw character
extern bool isHaloActive;
inline void drawCharacter() {
  iSetColor(255, 255, 255); // Reset color to prevent orange ghosts
  if (isInvincible && !isHaloActive && (invincibilityTimer / 5) % 2 == 0) {
    // Skip drawing to create blink effect
    return;
  }

  int rX, rY, rW, rH, frame;
  getCharacterFrameInfo(charX, charY, rX, rY, rW, rH, frame);

  if (frame != -1) {
    iShowImage(rX, rY, rW, rH, frame);
  }
}

extern int jumpPressTimer;
extern int bendPressTimer;
extern bool prevUpPressed;
extern bool prevDownPressed;
extern int doubleJumpSpeed;
extern int doubleJumpHorizontalSpeed;
extern int doubleBendHeight;

extern int currentLevel;

// Update character movement based on key states (called by timer)
inline void updateCharacterMovement() {
  if (isCharFrozen)
    return;
    
  extern int level4Phase;
  if (currentLevel == 4 && level4Phase == 1) {
    isRightArrowPressed = true;
    isLeftArrowPressed = false;
    // Walk Animation for Level 4 Phase 1 Intro
    charAnimCounter++;
    if (charAnimCounter >= charAnimSpeed) {
      charAnimCounter = 0;
      charFrameIndex++;
      if (charFrameIndex >= 10) charFrameIndex = 1; // Images 2 to 10 (indices 1 to 9)
    }
    return; // block other inputs during auto-scroll
  }
  // FIX: Once phase 1 ends, clear the forced right-arrow state
  if (currentLevel == 4 && level4Phase > 1) {
    // Only clear if no real key is pressed (prevents overriding actual input)
    if (!isSpecialKeyPressed(GLUT_KEY_RIGHT)) isRightArrowPressed = false;
    if (!isSpecialKeyPressed(GLUT_KEY_LEFT))  isLeftArrowPressed  = false;
  }

  bool isMoving = false;
  if (isSpecialKeyPressed(GLUT_KEY_RIGHT) ||
      isSpecialKeyPressed(GLUT_KEY_LEFT)) {
    isMoving = true;
  }

  int currentAnimSpeed;
  if (currentLevel == 4) {
    // Idle: slow cycle (14 ticks) so 3-frame static pose looks natural
    // Moving: keep the same brisk walking pace (4 ticks)
    currentAnimSpeed = isMoving ? 4 : 14;
  } else {
    currentAnimSpeed = isMoving ? 2 : 6;
  }

  if (!isJumping) {
    charAnimCounter++;
    if (charAnimCounter >= currentAnimSpeed) {
      charAnimCounter = 0;
      charFrameIndex++;
      if (charFrameIndex >= 9) {
        charFrameIndex = 1;
      }
    }
  }

  // --- CONTROL LOGIC ---
  if (currentLevel == 2 || currentLevel == 3 || currentLevel == 4) {
    // --- INSTANT ACTION LOGIC (LEVEL 2/3/4) ---
    bool upNow = (isSpecialKeyPressed(GLUT_KEY_UP) != 0);
    bool downNow = (isSpecialKeyPressed(GLUT_KEY_DOWN) != 0);

    // Explicitly ignore Shift for any movement initiation
    if (isShiftPressed) {
      upNow = false;
      downNow = false;
    }

    // Bending Logic (Instant)
    if (!isJumping) {
      if (downNow) {
        if (!prevDownPressed) {
          if (bendPressTimer > 0) {
            charHeight = doubleBendHeight;
          } else {
            charHeight = bendHeight;
            bendPressTimer = 15;
          }
          isBending = true;
        }
      } else {
        if (isBending) {
          isBending = false;
          charHeight = originalHeight;
        }
      }
    }
    prevDownPressed = downNow;
    if (bendPressTimer > 0)
      bendPressTimer--;

    // Jumping Logic (Instant + Upgrade)
    if (upNow && !prevUpPressed) {
      if (!isJumping) {
        isJumping = true;
        verticalVelocity = 26; // Clears shark2 peak safely
        jumpHorizontalSpeed = (currentLevel == 1) ? 12 : 7;
        currentJumpDirection =
            (isSpecialKeyPressed(GLUT_KEY_LEFT) || isLeftArrowPressed) ? -1 : 1;
        jumpPressTimer = 18; // Slightly longer window for upgrade (~500ms)
        if (isBending) {
          isBending = false;
          charHeight = originalHeight;
        }
      } else if (jumpPressTimer > 0) {
        // Upgrade to Double Jump
        verticalVelocity = 28;    // Reaches Red Light
        jumpHorizontalSpeed = 12; // Matches broken bridge width
        jumpPressTimer = 0;
      }
    }
    prevUpPressed = upNow;
    if (jumpPressTimer > 0)
      jumpPressTimer--;

    // --- JUMP PHYSICS (LEVEL 2) ---
    if (isJumping) {
      charY += verticalVelocity;
      if (charY > 520) {
        charY = 520;
        if (verticalVelocity > 0)
          verticalVelocity = 0;
      }
      verticalVelocity -= gravity;

      if (currentJumpDirection == 1) {
        charX += jumpHorizontalSpeed;
        if (charX > 855)
          charX = 855;
      } else if (currentJumpDirection == -1) {
        charX -= jumpHorizontalSpeed;
        if (charX < 20)
          charX = 20;
      }

      if (charY <= groundY) {
        charY = groundY;
        isJumping = false;
        isJumpAttacking = false;
        verticalVelocity = 0;
        currentJumpDirection = 0;
        jumpHorizontalSpeed = (currentLevel == 1) ? 12 : 7;
      }

      // Jump Attack Animation
      if (isJumpAttacking) {
        jumpAttackAnimCounter++;
        if (jumpAttackAnimCounter >= 8) {
          jumpAttackAnimCounter = 0;
          jumpAttackFrameIndex++;
          // Turn off attack after frame 2 (index 1 is 2nd frame, so index 2 is finished)
          if (jumpAttackFrameIndex >= 2) {
             isJumpAttacking = false;
          }
        }

        // Logic hit check: if boss also jumps and last jumpattack frame isactive
        if (jumpAttackFrameIndex == 1 && boss4Obj.active && boss4Obj.currentSkill == 1 && boss4Obj.jumpSkillState == 2) {
          // Check collision
          float charCenterX = charX + charWidth / 2.0f;
          float bossCenterX = boss4Obj.x + 75.0f; // BOSS_WIDTH/2
          float dist = (float)fabs(charCenterX - bossCenterX);
          // Collision distance (approximate body width overlap)
          if (dist < 100 && (float)fabs(charY - boss4Obj.y) < 150) {
             if (!boss4Obj.isHit) {
                boss4Obj.life -= 5;
                spawnDamagePopup(5, bossCenterX, boss4Obj.y + 150.0f);
                boss4Obj.isHit = true;
                boss4Obj.hitTimer = 20;
                // Transition to normal jump after hit
                isJumpAttacking = false;
             }
          }
        }
      }
    }
  } else {
    // --- ORIGINAL LOGIC (LEVEL 1 & OTHERS) ---
    if (isJumping) {
      charY += verticalVelocity;
      if (charY > 450) {
        charY = 450;
        if (verticalVelocity > 0)
          verticalVelocity = 0;
      }
      verticalVelocity -= gravity;

      if (currentJumpDirection == 1) {
        charX += jumpHorizontalSpeed;
        if (charX > 855)
          charX = 855;
      } else if (currentJumpDirection == -1) {
        charX -= jumpHorizontalSpeed;
        if (charX < 20)
          charX = 20;
      }
      if (charY <= groundY) {
        charY = groundY;
        isJumping = false;
        verticalVelocity = 0;
        currentJumpDirection = 0;
        jumpHorizontalSpeed = (currentLevel == 1) ? 12 : 7;
      }
    } else {
      if (isSpecialKeyPressed(GLUT_KEY_UP)) {
        isJumping = true;
        verticalVelocity = jumpSpeed;
        jumpHorizontalSpeed = (currentLevel == 1) ? 12 : 7;
        currentJumpDirection =
            (isSpecialKeyPressed(GLUT_KEY_LEFT) || isLeftArrowPressed) ? -1 : 1;
        if (isBending) {
          isBending = false;
          charHeight = originalHeight;
        }
      }

      if (isSpecialKeyPressed(GLUT_KEY_DOWN)) {
        if (!isBending) {
          isBending = true;
          charHeight = bendHeight;
          charY = groundY;
        }
      } else {
        if (isBending) {
          isBending = false;
          charHeight = originalHeight;
        }
      }
    }
  }
  // Movement while grounded or jumping
  if (isSpecialKeyPressed(GLUT_KEY_RIGHT)) {
    isRightArrowPressed = true;
    isLeftArrowPressed = false;
    moveCharRight();
  } else if (isSpecialKeyPressed(GLUT_KEY_LEFT)) {
    isRightArrowPressed = false;
    isLeftArrowPressed = true;
    moveCharLeft();
  } else {
    isRightArrowPressed = false;
    isLeftArrowPressed = false; // FIX: Reset left flag so character stops going left
  }
}
inline void updateCharacterAnimation() {}

#endif
