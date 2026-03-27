#ifndef CAVE_H
#define CAVE_H

#include "Character.h"

// Externs (defined in iMain.cpp)
extern int caveSceneBg;
extern int wizardImgs[10];
extern int wizardX;
extern int wizardY;
extern int wizardIndex;
extern bool isWizardMoving;
extern bool isGamePaused;
extern int wizardTimer;
extern bool isCharacterEntering;
extern int levelChange1;
extern int levelChange2;
extern bool isLevelTransitioning;
extern int transitionTimer;
extern int transitionPhase;

// Dialogue System
extern bool isWizardTalking;
extern int wizardDialogueIndex;
// 0: None, 1: "Speak the truth...", 2: "Only wisdom...", 3: "Answer or perish"
extern int wizardDialogueTimer;

extern int imgNextLevel;    // From iMain.cpp
extern int dialogueBoxImg;  // New dialogue box image
extern bool iqCorrect;      // Defined in IQ.h
extern int postAnswerIndex; // Defined in IQ.h
extern int caveImgL2, caveSceneBgL2, wizardL2Img;
extern int caveImgL3, wizardL3Img;
extern int currentLevel;
extern int shardImg;
extern int gunImg, gunExplainImg;
extern int l3CompanionImg;

// Forward Declaration
void updateIQLogic();

// Function Implementations
inline void loadCaveAssets() {
  caveSceneBg = iLoadImage((char *)"Wizard\\cave.bmp");
  char filename[100];
  for (int i = 0; i < 10; i++) {
    sprintf_s(filename, sizeof(filename), "Wizard\\wizard%d.png", i + 1);
    wizardImgs[i] = iLoadImage((char *)filename);
  }
  levelChange1 = iLoadImage((char *)"Wizard\\entryCave.png");
  levelChange2 = iLoadImage((char *)"Wizard\\shardexplain.png");
  dialogueBoxImg = iLoadImage((char *)"Wizard\\textbox.png");
  shardImg = iLoadImage((char *)"Wizard\\shard.png");

  gunImg = iLoadImage((char *)"Wizard\\gun.png");
  gunExplainImg = iLoadImage((char *)"Wizard\\gunexplain.png");
}

inline void initCaveState() {
  // Character setup
  charX = -200; // Start off-screen left
  charY = (currentLevel == 3)
              ? 80
              : 30; // Decreased by 70 as requested (80 from 150, 30 from 100)
  charHeight = 250; // Ensure normal size
  charWidth = 200;  // Ensure normal size
  charFrameIndex = 0;

  // Reset character states to ensure walking motion
  isJumping = false;
  isBending = false;
  isAttacking = false;
  isFallingSequence = false;
  verticalVelocity = 0;
  isRightArrowPressed = false;
  isLeftArrowPressed = false;

  wizardX = 800;   // Start partially on-screen so they both appear
  wizardY = charY; // Same Y axis for both character and wizard

  wizardIndex = 0;
  isWizardMoving = false;     // Wizard waits for character
  isCharacterEntering = true; // Character starts moving
  wizardTimer = 0;

  // Transition Logic - Start with entry splash screen (Phase 1)
  // Remove level 1 pages for level 2
  if (currentLevel == 2) {
    isLevelTransitioning = false;
    transitionPhase = 0;
  } else {
    isLevelTransitioning = true;
    transitionPhase = 1;
  }
  transitionTimer = 0;

  // Dialogue Init
  isWizardTalking = false;
  wizardDialogueIndex = 0;
  wizardDialogueTimer = 0;
}

inline void updateWizardLogic() {
  if (isGamePaused)
    return;

  if (isCharacterEntering) {
    wizardTimer++;
    // Character Animation
    if (wizardTimer % 5 == 0) {
      charFrameIndex++;
      if (charFrameIndex >= 9) // Loop 1-8
        charFrameIndex = 1;
    }

    charX += 5;         // Average speed
    if (charX >= 150) { // Stop position
      charX = 150;
      isCharacterEntering = false;
      charFrameIndex = 0;    // Frame 1 (Index 0) - leora1.png
      isWizardMoving = true; // Start wizard
      wizardTimer = 0;
    }
  } else if (isWizardMoving) {
    wizardTimer++;

    // Animation logic
    if (wizardTimer % 5 == 0) {
      wizardIndex++;
      if (wizardIndex >= 10)
        wizardIndex = 0;
    }

    // Movement logic
    wizardX -= 5;         // Move left
    if (wizardX <= 650) { // Specific position to create hole/gap
      wizardX = 650;
      isWizardMoving = false;
      wizardIndex = 0; // Set to first frame (standing)

      // Start Dialogue Sequence
      isWizardTalking = true;
      wizardDialogueIndex = 1;
      wizardDialogueTimer = 0;
    }
  } else if (isWizardTalking) {
    wizardDialogueTimer++;
    // Display each message for 120 frames (approx 4 seconds)
    if (wizardDialogueTimer >= 120) {
      wizardDialogueTimer = 0;
      wizardDialogueIndex++;
      if (wizardDialogueIndex > 3) {
        isWizardTalking = false;
        // IQ question will appear now (checked in IQ.h)
      }
    }
  }

  // IQ Logic Update
  if (!isLevelTransitioning && !isCharacterEntering && !isWizardMoving &&
      !isWizardTalking) {
    // We need to validly call updateIQLogic() here.
    // Assuming IQ.h is included before cave.h in iMain.cpp and updateIQLogic is
    // available.
    updateIQLogic();
  }
}

inline void drawCave() {
  if (isLevelTransitioning) {
    // Render scene background, character, and wizard beneath the splash screen
    if (currentLevel == 3) {
      iShowImage(0, 0, 1000, 600, caveImgL3);
    } else if (currentLevel == 2) {
      iShowImage(0, 0, 1000, 600, caveSceneBgL2);
    } else {
      iShowImage(0, 0, 1000, 600, caveSceneBg);
    }
    drawCharacter();
    if (currentLevel == 3) {
      iShowImage(wizardX, wizardY, 200, 200, wizardL3Img);
    } else if (currentLevel == 2) {
      iShowImage(wizardX, wizardY, 200, 200, wizardL2Img);
    } else {
      iShowImage(wizardX, wizardY, 200, 200, wizardImgs[wizardIndex]);
    }

    if (transitionPhase == 1) {
      iShowImage(0, 0, 1000, 600, levelChange1);
    } else if (transitionPhase == 2) {
      // Explain image centered in the middle of the screen
      int imgW = 700;
      int imgH = 350;
      int imgX = (1000 - imgW) / 2; // 250
      int imgY = (600 - imgH) / 2;  // 125
      iSetColor(255, 255, 255);
      if (currentLevel == 2) {
        iShowImage(imgX, imgY, imgW, imgH, gunExplainImg);
      } else if (currentLevel == 3) {
        iShowImage(imgX, imgY, imgW, imgH, l3CompanionImg);
      } else {
        iShowImage(imgX, imgY, imgW, imgH, levelChange2);
      }
    }

    // Draw Next Button at bottom right (Bigger)
    iShowImage(750, 50, 220, 70, imgNextLevel);
    return;
  }
  if (currentLevel == 3) {
    iShowImage(0, 0, 1000, 600, caveImgL3);
  } else if (currentLevel == 2) {
    iShowImage(0, 0, 1000, 600, caveSceneBgL2);
  } else {
    iShowImage(0, 0, 1000, 600, caveSceneBg);
  }

  // Draw Character
  // We can use the existing drawCharacter() function from Character.h
  // However, drawCharacter() handles jumping/bending logic which might be
  // interfering. But since we are just moving x/y, it should be fine as long as
  // isJumping/isBending are false. Since we set charY = 100, we need to make
  // sure drawCharacter uses that charY. drawCharacter() uses global charY.
  drawCharacter();

  // Draw Shard and Info if correct and Wizard finished sequence
  if (iqCorrect && postAnswerIndex >= 3) {
    // Adjustable Parameters - Revised Layout
    int shardW = 150;
    int shardH = 150;
    int shardX = charX - 100; // Keep position
    // 10 pixels up from character's head
    int shardY = charY + charHeight + 10;

    int boxW = 350;
    int boxH = 200; // Match Wizard's height (200)
    // Position textbox closer to shard (Smaller gap)
    int boxX = shardX + 120;
    // Align Y with Shard (Same level)
    int boxY = shardY;

    // Draw Info Box (Background)
    iSetColor(255, 255, 255);
    iShowImage(boxX, boxY, boxW, boxH, dialogueBoxImg);

    // Draw Shard / Gun (not for level 3 - companion reward, no item picture)
    if (currentLevel == 2) {
      iShowImage(shardX, shardY, shardW, shardH, gunImg);
    } else if (currentLevel != 3) {
      iShowImage(shardX, shardY, shardW, shardH, shardImg);
    }

    // Draw Info Text (Centered Title)
    iSetColor(0, 0, 0);

    // Title text
    if (currentLevel == 3) {
      iText(boxX + 85, boxY + 95, (char *)"Companion Gained!",
            (void *)GLUT_BITMAP_TIMES_ROMAN_24);
    } else {
      iText(boxX + 110, boxY + 95, (char *)"Item Claimed",
            (void *)GLUT_BITMAP_TIMES_ROMAN_24);
    }

    // Draw Next Button at bottom right (Bigger)
    iShowImage(750, 50, 220, 70, imgNextLevel);
  }

  // Draw Wizard
  if (currentLevel == 3) {
    iShowImage(wizardX, wizardY, 200, 200, wizardL3Img);
  } else if (currentLevel == 2) {
    iShowImage(wizardX, wizardY, 200, 200, wizardL2Img);
  } else {
    iShowImage(wizardX, wizardY, 200, 200, wizardImgs[wizardIndex]);
  }

  // Draw Dialogue Bubble
  if (isWizardTalking) {
    // Wizard Center ~750 (wizardX + 100).
    // Box Width 350. Start X = 750 - 175 = 575 (wizardX - 75).
    // Box Height 150 (User requested image replacement).
    // Box Height 200 (Increased to fit).
    int boxX = wizardX - 100;
    int boxY = wizardY + 220; // Raised higher (above head)
    int boxW = 400;
    int boxH = 200;

    iSetColor(255, 255,
              255); // Reset color to white so image isn't tinted black
    iShowImage(boxX, boxY, boxW, boxH, dialogueBoxImg);

    iSetColor(0, 0, 0); // Text Color
    // Center text vertically
    int textY = boxY + 90;
    int textX = boxX + 70; // Increased padding for right shift

    if (currentLevel == 3) {
      if (wizardDialogueIndex == 1) {
        iText(textX + 10, textY, (char *)"You have passed every trial.",
              (void *)GLUT_BITMAP_TIMES_ROMAN_24);
      } else if (wizardDialogueIndex == 2) {
        iText(textX, textY, (char *)"A companion shall join you now.",
              (void *)GLUT_BITMAP_TIMES_ROMAN_24);
      } else if (wizardDialogueIndex == 3) {
        iText(textX + 10, textY, (char *)"Aizen will fight by your side!",
              (void *)GLUT_BITMAP_TIMES_ROMAN_24);
      }
    } else if (currentLevel == 2) {
      if (wizardDialogueIndex == 1) {
        iText(textX + 20, textY, (char *)"You have proven your worth.",
              (void *)GLUT_BITMAP_TIMES_ROMAN_24);
      } else if (wizardDialogueIndex == 2) {
        iText(textX + 20, textY, (char *)"Time to wield a weapon of power.",
              (void *)GLUT_BITMAP_TIMES_ROMAN_24);
      } else if (wizardDialogueIndex == 3) {
        iText(textX + 20, textY, (char *)"But first, prove your mind!",
              (void *)GLUT_BITMAP_TIMES_ROMAN_24);
      }
    } else {
      if (wizardDialogueIndex == 1) {
        iText(textX + 20, textY, (char *)"Speak the truth, traveler",
              (void *)GLUT_BITMAP_TIMES_ROMAN_24);
      } else if (wizardDialogueIndex == 2) {
        iText(textX + 10, textY, (char *)"Only wisdom shall unlock it",
              (void *)GLUT_BITMAP_TIMES_ROMAN_24);
      } else if (wizardDialogueIndex == 3) {
        iText(textX + 60, textY, (char *)"Answer or perish!",
              (void *)GLUT_BITMAP_TIMES_ROMAN_24);
      }
    }
  }
}

inline int handleCaveTransitionClick(int mx, int my) {
  // Button Rect: 750, 50, 220, 70
  if (mx >= 750 && mx <= 970 && my >= 50 && my <= 120) {
    if (isLevelTransitioning) {
      if (transitionPhase == 1) {
        // Exit entry splash screen into cave scene
        isLevelTransitioning = false;
        transitionPhase = 0;
        return 2; // Stay in NEXT_LEVEL_IQ state, but show cave
      } else if (transitionPhase == 2) {
        // Finalize transition directly for all levels from Phase 2 (Explain/Reward)
        isLevelTransitioning = false;
        transitionPhase = 0;
        return 1; // Transitions to next level intro in iMain.cpp
      }
    } else {
      // In cave scene, check if can start exit transition
      if (iqCorrect && postAnswerIndex >= 3) {
        isLevelTransitioning = true;
        transitionPhase = 2; // Show companion text / shard explain
        return 0;
      }
    }
  }
  return 0;
}

#endif
