#ifndef CHARACTER_H
#define CHARACTER_H

#include "iGraphics.h"

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
extern void startScreenShake(int duration, int magnitude);

// Animation Speed Control
extern int charAnimSpeed; // Adjust this to change speed (Higher = Slower)
extern int charAnimCounter;

// Jump & Bend Variables
extern bool isJumping;
extern bool isBending;
extern int shakeOffsetX;
extern int shakeOffsetY;
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

// Movement helpers
inline void moveCharRight() {
  if (charX + charWidth < 1000) { // Screen Width Boundary (1000)
    charX += 20;                  // Increased Speed
  }
}

inline void moveCharLeft() {
  if (charX > 0) { // Left Boundary
    charX -= 20;   // Increased Speed
  }
}

inline void stopCharMovement() {
  charFrameIndex = 0; // Reset frame to stand/first image
}

// Draw character
extern bool isHaloActive;
inline void drawCharacter() {
  if (isInvincible && !isHaloActive && (invincibilityTimer / 5) % 2 == 0) {
    // Skip drawing to create blink effect
    return;
  }

  bool isBack = isLeftArrowPressed || isSpecialKeyPressed(GLUT_KEY_LEFT);
  int frame = -1;
  int rX = charX, rY = charY, rW = charWidth, rH = charHeight;

  if (isFallingSequence) {
    frame = getSelectedCharacterFallImage();
    rY = (int)fallY;
    // Potentially adjust width/height for fall image if needed
    rW = 125;
    rH = 125;
  } else if (isAttacking) {
    frame = getSelectedCharacterAttackImage(attackFrameIndex, isBack);
    // Standardizing attack size boost for all to prevent "getting small"
    rX = charX - 30;
    rW = charWidth + 60;
    rH = charHeight + 60;
    // Grounding: Apply stronger Y-offsets to ensure feet stay on the ground
    if (selectedCharacter == 1) {
      rY = charY - 50; // Kael/Kaero offset
    } else {
      rY = charY - 40; // Aryn and Leora offset to match walking feet level
    }
  } else if (isJumping) {
    int currentJumpImg = 0;
    if (verticalVelocity > 0)
      currentJumpImg = 0; // jump1
    else if (verticalVelocity > -10)
      currentJumpImg = 1; // jump2
    else
      currentJumpImg = 2; // jump3

    frame = getSelectedCharacterJumpImage(currentJumpImg, isBack);
    if (selectedCharacter == 1) {
      rX = charX - 25;
      rY = charY - 20;
      rW = charWidth + 50;
      rH = charHeight + 50;
    } else {
      rX = charX;
      rY = charY;
      rW = charWidth;
      rH = charHeight;
    }
  } else if (isSpecialKeyPressed(GLUT_KEY_RIGHT) ||
             isSpecialKeyPressed(GLUT_KEY_LEFT)) {
    // Show run images
    frame = getCharacterRunImage(runFrameIndex, isBack);
    if (selectedCharacter == 1) {
      rX = charX - 25;
      rY = charY - 20;
      rW = charWidth + 50;
      rH = charHeight + 50;
    } else {
      rX = charX;
      rY = charY;
      rW = charWidth;
      rH = charHeight;
    }
  } else {
    frame = getSelectedCharacterImage(charFrameIndex, isBack);
    // Standard walking size uses default charX, charY, charWidth, charHeight
  }

  if (frame != -1) {
    iShowImage(rX + shakeOffsetX, rY + shakeOffsetY, rW, rH, frame);
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
  bool isMoving = false;
  if (isSpecialKeyPressed(GLUT_KEY_RIGHT) ||
      isSpecialKeyPressed(GLUT_KEY_LEFT)) {
    isMoving = true;
  }

  int currentAnimSpeed = isMoving ? 2 : 6;

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
  if (currentLevel == 2) {
    // --- INSTANT ACTION LOGIC (LEVEL 2) ---
    bool upNow = isSpecialKeyPressed(GLUT_KEY_UP) != 0;
    bool downNow = isSpecialKeyPressed(GLUT_KEY_DOWN) != 0;

    // Explicitly ignore Shift for any movement initiation
    if (isShiftPressed) {
      upNow = false;
      downNow = false;
    }

    // Bending Logic (Instant)
    if (!isJumping && !isSpecialKeyPressed(GLUT_KEY_LEFT)) {
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
    if (upNow && !prevUpPressed && !isSpecialKeyPressed(GLUT_KEY_LEFT)) {
      if (!isJumping) {
        isJumping = true;
        verticalVelocity = 26; // Clears shark2 peak safely
        jumpHorizontalSpeed = 10;
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
        verticalVelocity = 0;
        currentJumpDirection = 0;
        jumpHorizontalSpeed = 10;
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
        jumpHorizontalSpeed = 20; // Original
      }
    } else {
      if (isSpecialKeyPressed(GLUT_KEY_UP) && !isSpecialKeyPressed(GLUT_KEY_LEFT)) {
        isJumping = true;
        verticalVelocity = jumpSpeed;
        jumpHorizontalSpeed = 20; // Original
        currentJumpDirection =
            (isLeftArrowPressed) ? -1 : 1;
        if (isBending) {
          isBending = false;
          charHeight = originalHeight;
        }
      }

      if (isSpecialKeyPressed(GLUT_KEY_DOWN) && !isSpecialKeyPressed(GLUT_KEY_LEFT)) {
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
    isLeftArrowPressed = false;
  }
}
inline void updateCharacterAnimation() {}

#endif
