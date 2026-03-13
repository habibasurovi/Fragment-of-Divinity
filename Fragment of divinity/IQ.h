#ifndef IQ_H
#define IQ_H

#include "iGraphics.h"
#include <stdlib.h>
#include <string.h>

extern int currentLevel;
// External Dialog variable
extern bool isWizardTalking;
extern bool isWizardMoving;
extern bool isCharacterEntering;
extern int wizardX;
extern int wizardY;
extern int charX;
extern int charY;
extern int charWidth;
extern int charHeight;     // 250
extern int dialogueBoxImg; // From cave.h (loaded there, extern-ed here)

// IQ State Variables
extern bool iqAnswered;
extern bool iqCorrect;

extern int miniGameTargetX;
extern int miniGameTargetY;
extern int miniGameTargetSpeedX;
extern int miniGameTargetSpeedY;
extern int miniGameClicks;
extern int shardImg;

struct IQButton {
  int x, y, w, h;
  char text[100];
};

struct Riddle {
  char line1[100];
  char line2[100];
  char line3[100];
  char answer[50];
};

extern Riddle riddlesLevel1[6];
extern Riddle riddlesLevel2[5];

// Reference to current active riddles
extern Riddle *currentRiddles;
extern int currentRiddlePoolSize;

// Pool of answers for options
extern const char *allAnswersL1[6];
extern const char *allAnswersL2[5];

extern int currentRiddleIndex;
extern int previousRiddleIndex;

// Updated Button Coordinates
// IQ Dialogue State Variables
extern int postAnswerTimer;
extern int postAnswerIndex; // 0: None, 1: First Line, 2: Second Line

// Updated Button Coordinates
extern IQButton iqOptions[3];

inline void shuffleOptions() {
  // 1. Get Correct Answer
  const char *correct = currentRiddles[currentRiddleIndex].answer;

  // 2. Pick 2 Unique Wrong Answers
  const char *wrong1;
  const char *wrong2;

  const char **answersPool = (currentLevel == 2) ? allAnswersL2 : allAnswersL1;

  do {
    int r = rand() % currentRiddlePoolSize;
    wrong1 = answersPool[r];
  } while (strcmp(wrong1, correct) == 0);

  do {
    int r = rand() % currentRiddlePoolSize;
    wrong2 = answersPool[r];
  } while (strcmp(wrong2, correct) == 0 || strcmp(wrong2, wrong1) == 0);

  // 3. Put into temporary array
  const char *currentOptions[3];
  currentOptions[0] = correct;
  currentOptions[1] = wrong1;
  currentOptions[2] = wrong2;

  // 4. Shuffle array
  for (int i = 0; i < 3; i++) {
    int r = rand() % 3;
    const char *temp = currentOptions[i];
    currentOptions[i] = currentOptions[r];
    currentOptions[r] = temp;
  }

  // 5. Assign to buttons (using strcpy_s for safety if available, or strcpy)
  strcpy_s(iqOptions[0].text, sizeof(iqOptions[0].text), currentOptions[0]);
  strcpy_s(iqOptions[1].text, sizeof(iqOptions[1].text), currentOptions[1]);
  strcpy_s(iqOptions[2].text, sizeof(iqOptions[2].text), currentOptions[2]);
}

inline void randomizeIQ() {
  if (currentLevel == 2) {
    currentRiddles = riddlesLevel2;
    currentRiddlePoolSize = 5;
  } else {
    currentRiddles = riddlesLevel1;
    currentRiddlePoolSize = 6;
  }

  int newIndex;
  // Select new riddle different from previous
  do {
    newIndex = rand() % currentRiddlePoolSize;
  } while (newIndex == previousRiddleIndex && previousRiddleIndex != -1);

  // Handle redundant fail-safe
  if (previousRiddleIndex != -1 && newIndex == previousRiddleIndex) {
    newIndex = (newIndex + 1) % currentRiddlePoolSize;
  }

  currentRiddleIndex = newIndex;
  previousRiddleIndex = currentRiddleIndex;

  shuffleOptions();
}

inline void initIQ() {
  iqAnswered = false;
  iqCorrect = false;
  postAnswerIndex = 0;
  postAnswerTimer = 0;
  randomizeIQ();
  if (currentLevel == 3) {
    miniGameTargetX = 500;
    miniGameTargetY = 300;
    miniGameTargetSpeedX = 6;
    miniGameTargetSpeedY = 6;
    miniGameClicks = 0;
  }
}

inline void updateIQLogic() {
  if (currentLevel == 3 && !iqAnswered) {
    miniGameTargetX += miniGameTargetSpeedX;
    miniGameTargetY += miniGameTargetSpeedY;
    if (miniGameTargetX <= 0 || miniGameTargetX >= 900) {
      miniGameTargetSpeedX *= -1;
    }
    if (miniGameTargetY <= 150 || miniGameTargetY >= 500) {
      miniGameTargetSpeedY *= -1;
    }
  }

  if (iqAnswered && postAnswerIndex > 0 && postAnswerIndex < 3) {
    postAnswerTimer++;
    // Change dialogue every 90 frames (approx 3 seconds)
    if (postAnswerTimer >= 90) {
      postAnswerTimer = 0;
      postAnswerIndex++;
    }
  }
}

inline void drawIQ() {
  if (!isWizardMoving && !isCharacterEntering && !isWizardTalking) {

    // --- 1. Wizard Question Box ---
    int wizBoxX = 575;
    int wizBoxY = 350; // Higher
    int wizBoxW = 400; // Increased width slightly for text
    int wizBoxH = 200; // Taller

    // Use Image instead of Rectangle
    iSetColor(255, 255, 255); // Reset color to white (no tint)
    iShowImage(wizBoxX, wizBoxY, wizBoxW, wizBoxH, dialogueBoxImg);

    // Text inside Wizard Box
    iSetColor(0, 0, 0); // Black Text

    if (currentLevel == 3) {
      if (!iqAnswered) {
        iText(wizBoxX + 60, wizBoxY + 115, (char *)"Catch the Shard!", GLUT_BITMAP_TIMES_ROMAN_24);
        iText(wizBoxX + 60, wizBoxY + 85, (char *)"Click it 3 times.", GLUT_BITMAP_TIMES_ROMAN_24);
        
        iSetColor(255, 255, 255);
        iShowImage(miniGameTargetX, miniGameTargetY, 80, 80, shardImg);
      } else {
        iSetColor(0, 0, 0);
        if (postAnswerIndex == 1) {
          iText(wizBoxX + 60, wizBoxY + 110, (char *)"You are swift.", GLUT_BITMAP_TIMES_ROMAN_24);
          iText(wizBoxX + 60, wizBoxY + 80, (char *)"Take the Shard.", GLUT_BITMAP_TIMES_ROMAN_24);
        } else {
          iText(wizBoxX + 60, wizBoxY + 110, (char *)"Press Next to proceed.", GLUT_BITMAP_TIMES_ROMAN_24);
        }
      }
      return;
    }

    if (!iqAnswered) {
      // Show Question
      if (currentRiddleIndex != -1) {
        iText(wizBoxX + 60, wizBoxY + 115,
              currentRiddles[currentRiddleIndex].line1,
              GLUT_BITMAP_TIMES_ROMAN_24);
        iText(wizBoxX + 60, wizBoxY + 85,
              currentRiddles[currentRiddleIndex].line2,
              GLUT_BITMAP_TIMES_ROMAN_24);
        iText(wizBoxX + 130, wizBoxY + 55,
              currentRiddles[currentRiddleIndex].line3,
              GLUT_BITMAP_TIMES_ROMAN_24);
      }
    } else {
      // Show Result Dialogue
      if (iqCorrect) {
        if (postAnswerIndex == 1) {
          iText(wizBoxX + 60, wizBoxY + 110, (char *)"Wise indeed.",
                GLUT_BITMAP_TIMES_ROMAN_24);
          iText(wizBoxX + 60, wizBoxY + 80, (char *)"Time bends to those who",
                GLUT_BITMAP_TIMES_ROMAN_24);
          iText(wizBoxX + 60, wizBoxY + 50, (char *)"understand it.",
                GLUT_BITMAP_TIMES_ROMAN_24);
        } else { // Index 2 or higher
          iText(wizBoxX + 60, wizBoxY + 110,
                (char *)"Take this, the OBSIDIAN AEGIS.",
                GLUT_BITMAP_TIMES_ROMAN_24);
          iText(wizBoxX + 60, wizBoxY + 80,
                (char *)"Let moments obey your will.",
                GLUT_BITMAP_TIMES_ROMAN_24);
        }
      } else {
        if (postAnswerIndex == 1) {
          iText(wizBoxX + 60, wizBoxY + 100, (char *)"Foolish answer.",
                GLUT_BITMAP_TIMES_ROMAN_24);
          iText(wizBoxX + 60, wizBoxY + 70, (char *)"Your mind is not ready.",
                GLUT_BITMAP_TIMES_ROMAN_24);
        } else { // Index 2 or higher
          iText(wizBoxX + 60, wizBoxY + 100, (char *)"Go... and pray time",
                GLUT_BITMAP_TIMES_ROMAN_24);
          iText(wizBoxX + 60, wizBoxY + 70, (char *)"shows you mercy.",
                GLUT_BITMAP_TIMES_ROMAN_24);
        }
      }
    }

    // --- 2. Character Options (Buttons) ---
    // Only show options if not answered
    if (!iqAnswered) {
      for (int i = 0; i < 3; i++) {
        // Draw Image for each button
        iSetColor(255, 255, 255);
        iShowImage(iqOptions[i].x, iqOptions[i].y, iqOptions[i].w,
                   iqOptions[i].h, dialogueBoxImg);

        // Centered Button Text
        int len = (int)strlen(iqOptions[i].text);
        // Rough centering: 250 px width. ~10-11px per char for Times Roman 24.
        int textOffset = (250 - (len * 11)) / 2;
        if (textOffset < 10)
          textOffset = 10; // Padding safety

        iSetColor(0, 0, 0);
        iText(iqOptions[i].x + textOffset, iqOptions[i].y + 20,
              iqOptions[i].text, GLUT_BITMAP_TIMES_ROMAN_24);
      }
    }
  }
}

inline void handleIQClick(int mx, int my) {
  if (currentLevel == 3) {
    if (!isWizardMoving && !isCharacterEntering && !iqAnswered && !isWizardTalking) {
      if (mx >= miniGameTargetX && mx <= miniGameTargetX + 80 && my >= miniGameTargetY && my <= miniGameTargetY + 80) {
        miniGameClicks++;
        if (miniGameTargetSpeedX > 0) miniGameTargetSpeedX += 3;
        else miniGameTargetSpeedX -= 3;
        if (miniGameTargetSpeedY > 0) miniGameTargetSpeedY += 3;
        else miniGameTargetSpeedY -= 3;
        
        if (miniGameClicks >= 3) {
          iqAnswered = true;
          iqCorrect = true;
          postAnswerIndex = 1;
          postAnswerTimer = 0;
        }
      }
    }
    return;
  }

  if (!isWizardMoving && !isCharacterEntering && !iqAnswered &&
      !isWizardTalking) {
    for (int i = 0; i < 3; i++) {
      if (mx >= iqOptions[i].x && mx <= iqOptions[i].x + iqOptions[i].w &&
          my >= iqOptions[i].y && my <= iqOptions[i].y + iqOptions[i].h) {

        iqAnswered = true;

        // Start Dialogue Sequence
        postAnswerIndex = 1;
        postAnswerTimer = 0;

        // Check Validity
        if (strcmp(iqOptions[i].text,
                   currentRiddles[currentRiddleIndex].answer) == 0) {
          iqCorrect = true;
        } else {
          iqCorrect = false;
        }
      }
    }
  }
}

#endif
