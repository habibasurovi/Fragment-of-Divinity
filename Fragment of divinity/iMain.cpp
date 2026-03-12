#include "AppleHandler.h"
#include "BackgroundHandler.h"
#include "Character.h"
#include "CharacterCustomization.h"
#include "CustomizationMenu.h"
#include "GameState.h"
#include "HighScoreHandler.h"
#include "IQ.h"
#include "LevelCompleteHandler.h"
#include "LifeHandler.h"
#include "MainMenu.h"
#include "ObstacleHandler.h"
#include "PauseMenu.h"
#include "SettingsMenu.h"
#include "ShardHandler.h"
#include "StoryHandler.h"
#include "WeatherHandler.h"
#include "cave.h"
#include "character 2.h"
#include "iGraphics.h"

#include <time.h>

// Forward declarations for functions defined in headers to assist
// IntelliSense/Linking
void checkMagnetCollision();
void drawMagnet();

#pragma comment(lib, "winmm.lib")

int highScores[4] = {0, 0, 0, 0};

/* -------------------- CONSTANTS -------------------- */
#define screenWidth 1000
#define screenHeight 600

// Forward Declarations
void drawLevelTwoStory();

/* -------------------- GLOBAL VARIABLES -------------------- */
// Game States (Moved to GameState.h)
GameState gameState = INTRO; // Reset to INTRO for production
#include "dragon.h"

// iGraphics.h variables
int iScreenHeight, iScreenWidth;
int iMouseX, iMouseY;
int ifft = 0;
void (*iAnimFunction[10])(void) = {0};
int iAnimCount = 0;
int iAnimDelays[10];
int iAnimPause[10];
unsigned int keyPressed[512];
unsigned int specialKeyPressed[512];

// Intro Variables
int introImage;
int introTimer = 0;

// Story Variables
int initialStoryImages[12];
int level1IntroImage;
int level2IntroImage, level3IntroImage, level4IntroImage;
int level1StoryImages[3];
int levelSelectionBG;
int level1BtnImg, level2BtnImg, level3BtnImg, level4BtnImg;
int nextBtnImg, skipBtnImg, homeBtnImg, prevBtnImg, btnLvlCustomImg;
LevelButton lvl1Btn, lvl2Btn, lvl3Btn, lvl4Btn, lvlCustomBtn;

int initialStoryIndex = 0;
int level1StoryIndex = 0;
int level2StoryImages[3];
int level2CaveStoryImages[3];
int level2StoryIndex = 0;
int level2CaveStoryIndex = 0;
int storyTimerCount = 0;

// Level 1 Riddles (6 Questions - Defined here to avoid ODR redefinition)
Riddle riddlesLevel1[6] = {
    {"I drink but never thirst,", "I eat but never hunger.", "What am I?",
     "Fire"},
    {"The more you take,", "the more you leave behind.", "What are they?",
     "Footsteps"},
    {"I can build cities and", "destroy mountains, yet I am invisible.",
     "What am I?", "Wind"},
    {"I exist only when there is light,", "yet I die when the light shines.",
     "What am I?", "Shadow"},
    {"I devour all things:", "birds, beasts, trees, and steel.", "What am I?",
     "Time"},
    {"I have no wings, yet I fly.", "I have no eyes, yet I cry.", "What am I?",
     "Cloud"}};

// Level 2 Riddles (5 Questions)
Riddle riddlesLevel2[5] = {
    {"I have keys but no locks,", "I have space but no room.", "What am I?",
     "Keyboard"},
    {"The more of me there is,", "the less you see.", "What am I?", "Darkness"},
    {"I am tall when young,", "and short when I am old.", "What am I?",
     "Candle"},
    {"I have hands but cannot clap,", "a face but cannot speak.", "What am I?",
     "Clock"},
    {"I can be cracked, made, told,", "and played.", "What am I?", "Joke"}};

// Pool of answers for options
const char *allAnswersL1[6] = {"Fire",   "Footsteps", "Wind",
                               "Shadow", "Time",      "Cloud"};
const char *allAnswersL2[5] = {"Keyboard", "Darkness", "Candle", "Clock",
                               "Joke"};

// Life Image
int heartImg;
int heartItemImg;
HeartItem heartItems[2];
int heartsSpawnedCount = 0;
int heart1SpawnTime = 0;
int heart2SpawnTime = 0;
bool heart1Queued = false;
bool heart2Queued = false;

// Weather Variables (Task 2)
int imgIce, imgWater;
WeatherParticle weatherParticles[MAX_PARTICLES];

// Game Timer (Task 1 & 3)
int gameRunTimeSeconds = 0;

// Cave (Task 3)
int caveImg;
int caveX = -1000;
bool isCaveActive = false;
bool levelOneComplete = false;

// Level Complete & IQ Screen (Task Updated)
int levelCompleteBG;                             // lcf.bmp
int caveIQBG;                                    // cavebg.bmp
int imgNextLevel, imgRestartLevel, imgHomeLevel; // New button images

ButtonRect btnNextLevel, btnRestartLevel, btnHomeLevel;
ButtonRect btnNextIQ;        // For the button on the IQ screen
ButtonRect btnNextIQ_Level2; // Next level button for IQ screen

// Cave Logic Variables
int caveSceneBg;
int shardImg; // Added for Task 5
int wizardImgs[10];
int wizardX;
int wizardY;
int wizardIndex = 0;
// Wizard Dialogue Variables
bool isWizardTalking = false;
int wizardDialogueIndex = 0;
int wizardDialogueTimer = 0;
bool isWizardMoving = false;
int wizardTimer = 0;
bool isCharacterEntering = false;

// Cave Transition Variables
int levelChange1, levelChange2;
int dialogueBoxImg; // Added for Task 5
bool isLevelTransitioning = false;
int transitionTimer = 0;
int transitionPhase = 0;

// Level 2 Cave & Wizard Assets
int caveImgL2, caveSceneBgL2, wizardL2Img;

// BackgroundHandler.h variables
int bG1, bG2, bG3, bG4;
int sbG1, sbG2, sbG3, sbG4;
int wbG1, wbG2, wbG3, wbG4;

// Level 2 background assets
int s2bG1, s2bG2, s2bG3, s2bG4;
int w2bG1, w2bG2, w2bG3, w2bG4;
int r2bG1, r2bG2, r2bG3, r2bG4;
int currentLevel = 1; // Start with level 1
int bGX = 0;

// IQ State Variables (Declared extern, defined in iMain.cpp)
bool iqAnswered = false;
bool iqCorrect = false;
Riddle *currentRiddles = NULL;
int currentRiddlePoolSize = 6;
int currentRiddleIndex = -1;
int previousRiddleIndex = -1;
int postAnswerTimer = 0;
int postAnswerIndex = 0; // 0: None, 1: First Line, 2: Second Line
IQButton iqOptions[3] = {
    {150, 480, 250, 60, ""}, {150, 410, 250, 60, ""}, {150, 340, 250, 60, ""}};

// Character.h variables
int charX = 100;
int charY = 30;
int charWidth = 125;
int charHeight = 125;
int charFrameIndex = 1; // Start at frame 2 (Index 1)
bool isCharMoving = false;
int lives = 5;
bool isInvincible = false;
int invincibilityTimer = 0;
int charAnimSpeed = 4;
int charAnimCounter = 0;
bool isJumping = false;
bool isBending = false;
int jumpSpeed = 32;
int doubleJumpSpeed = 55;
int highJumpSpeed = 60;
int gravity = 3;
int verticalVelocity = 0;
int groundY = 30;
int originalHeight = 125;
int bendHeight = 70;
int doubleBendHeight = 40;
int jumpImageIndex = 0;
int currentJumpDirection = 0;
int jumpHorizontalSpeed = 10;
int doubleJumpHorizontalSpeed = 14;
bool isShiftPressed = false;
bool isDoubleShiftTriggered = false;

// AppleHandler.h variables
Apple apples[20];
int applesCollected = 0;
int appleImg;
int appleSpawnTimer = 0;

// ObstacleHandler.h variables
int obs1, obs2, obs3;
int shark11, shark12, shark13;
int shark21, shark22, shark23;
int obsBrokenBridge;
int obs221, obs222, obs223;
int imgBroken;
int imgRLL, imgGLL, imgRBL, imgGBL;
int obstacleSpawnTimer = 0;

Obstacle obstacles[3];
Shark sharks[2];
BrokenBridge bridges[2];
int sharksSpawnedInLevel = 0;

// Bomb Mechanic Globals
int bombImg;
int explosionImgs[7];
Bomb bombs[3];
Explosion explosions[3];
bool isCharFrozen = false;
int freezeTimer = 0;
bool isFallingSequence = false;
float fallY = 0;
float fallVel = 0;
int hitBridgeIndex = -1;
int obsGapMin = 400;
int obsGapMax = 700;
int obsHighY = 100;
int obsLowY = 30;

// Game Juice Globals (Screen Shake & Score Pop-ups)
int screenShakeTimer = 0;
int screenShakeMagnitude = 0;
int shakeOffsetX = 0;
int shakeOffsetY = 0;

struct ScorePopup {
  float x, y;
  int timer;
  bool active;
};
ScorePopup popups[10];

// Shard & Halo Globals
float fallingShardX = 0, fallingShardY = 0;
bool isFallingShardActive = false;
int shardSpawnTimer = 0;
bool isHaloActive = false;
int haloTimer = 0;
int haloImg;
bool hasClaimedShard = false;

// Magnet Power-up
int magnetImg;
float magnetX = 0, magnetY = 0;
bool isMagnetVisible = false;
bool isMagnetBuffActive = false;
int magnetBuffTimer = 0;
int magnetSpawnTimer = 0;
int magnetNextSpawnThreshold =
    233 +
    rand() %
        201; // 7-13 seconds at 30fps (30ms * 233 = 6.99s, 30ms * 433 = 12.99s)

// MainMenu.h variables
int mainMenuBG;
int btnStart, btnSettings, btnCustomization, btnExit, btnHighscore;
Button bStart, bSettings, bCustomization, bExit, bHighscore;

// Button Click Animation globals
bool btnAnimActive = false;
int btnAnimTimer = 0;          // counts down in seconds (1-second delay)
int btnAnimCode = -1;          // which button is animating
int btnAnimContext = -1;       // 0=MainMenu, 1=Custom, 2=Story
int pendingAction = -1;        // action to execute when timer fires
GameState pendingState = MENU; // state to switch to on action 0 (direct)

// PauseMenu.h variables
bool isGamePaused = false;
int pauseBtnImage;
int pauseFrameImage;
int btnContinue, btnRestart, btnSettingsPause, btnExitPause;
PauseButton bPause, bContinue, bRestart, bSettingsP, bExitP;
int frameX, frameY, frameW, frameH;

// SettingsMenu.h variables
bool isMusicOn = true;
bool isSoundOn = true;
int previousState = -1;
int settingsBG, settingsFrame, crossImage;
int btnMusic, btnSound, btnExitSettings;
int sFrameW = 600;
int sFrameH = 600;
int sFrameX = (1000 - 600) / 2;
int sFrameY = (600 - 600) / 2;
SettingsButton bMusic, bSound, bBackSettings;

// CustomizationMenu.h variables
int customMenuBG;
int btnWeather, btnCharCustom;
int selectedWeather = 0;
int selectedCharacter = 1;
int customSubState = 0;
int imgCharSubBG, imgWeatherSubBG;
int btnCharCstm, btnWeatherCstm;
CustomButton bCharCstm, bWeatherCstm, bBack;
SelectButton selChar[3], selWeather[3];

// CharacterCustomization.h variables
int arynWalk[10];
int kaelWalk[10];
int leoraWalk[10];
int arynJump[3];
int kaelJump[3];
int leoraJump[3];

int arynWalkBack[10];
int kaelWalkBack[10];
int leoraWalkBack[10];
int arynJumpBack[3];
int kaelJumpBack[3];
int leoraJumpBack[3];

int arynAttack[7];
int kaeroAttack[6];
int leoraAttack[6];
int arynAttackBack[7];
int kaeroAttackBack[6];
int leoraAttackBack[6];

int arynFall;
int kaeroFall;
int leoraFall;

bool isAttacking = false;
int attackFrameIndex = 0;
int attackAnimCounter = 0;
int attackAnimSpeed = 3;

// Running Animation Variables
int arynRun[9];
int kaeroRun[8];
int leoraRun[7];
int arynRunBack[9];
int kaeroRunBack[8];
int leoraRunBack[7];
bool isRightArrowPressed = false;
bool isLeftArrowPressed = false;
int lastShiftPressTime = 0;
int runFrameIndex = 0;
int runAnimCounter = 0;
int runAnimSpeed = 3; // Adjust for speed

// Double Tap Tracking (Shared with Logic)
int jumpPressTimer = 0;
int bendPressTimer = 0;
bool prevUpPressed = false;
bool prevDownPressed = false;

// dragon.h variables
int dragon1Imgs[9];
int dragon2Imgs[10];
int dragon3Imgs[6];
Dragon dragons[5];
int dragonSpawnTimeCounter = 0;

// ---- GAME JUICE HELPERS ----
void startScreenShake(int duration, int magnitude) {
  screenShakeTimer = duration;
  screenShakeMagnitude = magnitude;
}

void updateScreenShake() {
  if (screenShakeTimer > 0) {
    shakeOffsetX = (rand() % (screenShakeMagnitude * 2)) - screenShakeMagnitude;
    shakeOffsetY = (rand() % (screenShakeMagnitude * 2)) - screenShakeMagnitude;
    screenShakeTimer--;
  } else {
    shakeOffsetX = 0;
    shakeOffsetY = 0;
  }
}

void addScorePopup(float x, float y) {
  for (int i = 0; i < 10; i++) {
    if (!popups[i].active) {
      popups[i].x = x;
      popups[i].y = y;
      popups[i].timer = 45; // 1.5 seconds at 30fps
      popups[i].active = true;
      break;
    }
  }
}

void updateScorePopups() {
  for (int i = 0; i < 10; i++) {
    if (popups[i].active) {
      popups[i].timer--;
      popups[i].y += 2; // Float upwards
      if (popups[i].timer <= 0) {
        popups[i].active = false;
      }
    }
  }
}

void drawTimerUI() {
  int uiX = screenWidth / 2 - 50;
  int uiY = screenHeight - 60;

  iSetColor(255, 255, 255);
  char timerText[32];
  int minutes = gameRunTimeSeconds / 60;
  int seconds = gameRunTimeSeconds % 60;
  sprintf_s(timerText, "Time: %02d:%02d", minutes, seconds);
  iText(uiX, uiY + 15, timerText, GLUT_BITMAP_TIMES_ROMAN_24);
}

/* -------------------- LOGIC FUNCTIONS -------------------- */
enum MusicTrack {
  TRACK_NONE,
  TRACK_INTRO,
  TRACK_STORY,
  TRACK_LEVEL1,
  TRACK_LEVEL2,
  TRACK_CAVE
};

MusicTrack currentTrack = TRACK_NONE;

void playMusicTrack(MusicTrack track) {
  if (!isMusicOn) {
    PlaySound(NULL, 0, 0);
    currentTrack = TRACK_NONE;
    return;
  }

  if (currentTrack == track)
    return; // Already playing

  currentTrack = track;
  switch (track) {
  case TRACK_NONE:
    PlaySound(NULL, 0, 0);
    break;
  case TRACK_INTRO:
    PlaySound((char *)"sound\\intro.wav", NULL,
              SND_FILENAME | SND_ASYNC | SND_LOOP);
    break;
  case TRACK_STORY:
    PlaySound((char *)"sound\\story.wav", NULL,
              SND_FILENAME | SND_ASYNC | SND_LOOP);
    break;
  case TRACK_LEVEL1:
    PlaySound((char *)"sound\\level1.wav", NULL,
              SND_FILENAME | SND_ASYNC | SND_LOOP);
    break;
  case TRACK_LEVEL2:
    PlaySound((char *)"sound\\level2.wav", NULL,
              SND_FILENAME | SND_ASYNC | SND_LOOP);
    break;
  case TRACK_CAVE:
    PlaySound((char *)"sound\\cave.wav", NULL,
              SND_FILENAME | SND_ASYNC | SND_LOOP);
    break;
  }
}

void resetGame() {
  if (currentLevel == 2) {
    groundY = 83; // 10 less than 93
  } else {
    groundY = 30;
  }

  charX = 100;
  charY = groundY;
  charHeight = 125;
  charWidth = 125;
  lives = 5;
  isInvincible = false;
  invincibilityTimer = 0;
  initObstacles();
  initApples();

  gameRunTimeSeconds = 0;
  isCaveActive = false;
  caveX = -1000;
  levelOneComplete = false;

  heartsSpawnedCount = 0;
  heart1SpawnTime = 0;
  heart2SpawnTime = 0;
  heart1Queued = false;
  heart2Queued = false;
  for (int i = 0; i < 2; i++)
    heartItems[i].active = false;

  gameState = GAME;
  isFallingSequence = false;
  isCharFrozen = false;
  fallY = (float)groundY;
  fallVel = 0;
  hitBridgeIndex = -1;
  jumpPressTimer = 0;
  bendPressTimer = 0;
  prevUpPressed = false;
  prevDownPressed = false;

  if (currentLevel == 1) {
    playMusicTrack(TRACK_LEVEL1);
  } else if (currentLevel == 2) {
    playMusicTrack(TRACK_LEVEL2);
  }
}

void updateCavePhysics() {
  if (!isGamePaused && gameState == GAME && isCaveActive) {
    if (caveX > 650) {
      caveX -= 3;
    } else {
      // Cave is "on sight" on the right side - stop BG and move character
      if (charX < caveX + 50) {
        isCharFrozen = true; // Prevent player input
        isLeftArrowPressed = false;
        specialKeyPressed[GLUT_KEY_LEFT] = 0;
        isRightArrowPressed = true;
        charX += 5;
        // Basic animation while moving automatically
        charAnimCounter++;
        if (charAnimCounter >= charAnimSpeed) {
          charAnimCounter = 0;
          charFrameIndex++;
          if (charFrameIndex >= 9) // Loop 1-8 (Images 2-9)
            charFrameIndex = 1;
        }
      } else {
        isCharFrozen = false;
        levelOneComplete = true;
        updateHighScore(currentLevel, applesCollected);
        gameState = LEVEL_COMPLETE;
      }
    }
  }
}

// Fire pending action after animation delay
static void firePendingAction() {
  switch (pendingAction) {
  // ---- Main Menu ----
  case 1: // Start -> Initial Story
    gameState = INITIAL_STORY;
    initialStoryIndex = 0;
    storyTimerCount = 0;
    playMusicTrack(TRACK_STORY);
    break;
  case 2: // Settings
    previousState = MENU;
    gameState = SETTINGS;
    break;
  case 3: // Customization
    previousState = MENU;
    gameState = CUSTOMIZATION;
    break;
  case 4: // Exit
    exit(0);
    break;
  case 5: // Highscores
    previousState = MENU;
    gameState = HIGHSCORES;
    break;
  // ---- Custom Menu ----
  case 10: // Char customization sub-menu
    customSubState = 1;
    break;
  case 11: // Weather customization sub-menu
    customSubState = 2;
    break;
  // ---- Story navigation ----
  case 20: // Next (handled per-context via pendingState)
    if (gameState == INITIAL_STORY) {
      initialStoryIndex++;
      storyTimerCount = 0;
      if (initialStoryIndex >= 12) {
        gameState = LEVEL_SELECTION;
        storyTimerCount = 0;
      }
    } else if (gameState == LEVEL1_INTRO) {
      gameState = LEVEL1_STORY;
      storyTimerCount = 0;
      level1StoryIndex = 0;
      playMusicTrack(TRACK_LEVEL1);
    } else if (gameState == LEVEL1_STORY) {
      level1StoryIndex++;
      storyTimerCount = 0;
      if (level1StoryIndex >= 3)
        resetGame();
    } else if (gameState == LEVEL2_INTRO) {
      gameState = LEVEL2_STORY;
      storyTimerCount = 0;
      level2StoryIndex = 0;
      playMusicTrack(TRACK_LEVEL2);
    } else if (gameState == LEVEL2_STORY) {
      level2StoryIndex++;
      storyTimerCount = 0;
      if (level2StoryIndex >= 3) {
        currentLevel = 2;
        resetGame();
      }
    } else if (gameState == LEVEL2_CAVE_STORY) {
      level2CaveStoryIndex++;
      storyTimerCount = 0;
      if (level2CaveStoryIndex >= 3) {
        gameState = NEXT_LEVEL_IQ;
        initCaveState();
        initIQ();
        playMusicTrack(TRACK_CAVE);
        storyTimerCount = 0;
      }
    } else if (gameState == LEVEL3_INTRO || gameState == LEVEL4_INTRO) {
      gameState = LEVEL_NOT_READY;
    }
    break;
  case 21: // Skip
    if (gameState == INITIAL_STORY) {
      gameState = LEVEL_SELECTION;
      storyTimerCount = 0;
    } else if (gameState == LEVEL1_INTRO || gameState == LEVEL1_STORY) {
      currentLevel = 1;
      resetGame();
    } else if (gameState == LEVEL2_INTRO || gameState == LEVEL2_STORY ||
               gameState == LEVEL2_CAVE_STORY) {
      if (gameState == LEVEL2_CAVE_STORY) {
        gameState = NEXT_LEVEL_IQ;
        initCaveState();
        initIQ();
        playMusicTrack(TRACK_CAVE);
      } else {
        currentLevel = 2;
        resetGame();
      }
    } else if (gameState == LEVEL3_INTRO || gameState == LEVEL4_INTRO) {
      gameState = LEVEL_NOT_READY;
    }
    break;
  case 22: // Previous
    if (gameState == INITIAL_STORY) {
      if (initialStoryIndex > 0) {
        initialStoryIndex--;
        storyTimerCount = 0;
      }
    } else if (gameState == LEVEL1_INTRO) {
      gameState = LEVEL_SELECTION;
      storyTimerCount = 0;
      playMusicTrack(TRACK_STORY);
    } else if (gameState == LEVEL2_INTRO || gameState == LEVEL3_INTRO ||
               gameState == LEVEL4_INTRO) {
      gameState = LEVEL_SELECTION;
      storyTimerCount = 0;
      playMusicTrack(TRACK_STORY);
    } else if (gameState == LEVEL1_STORY) {
      if (level1StoryIndex > 0) {
        level1StoryIndex--;
        storyTimerCount = 0;
      } else {
        gameState = LEVEL1_INTRO;
        storyTimerCount = 0;
      }
    } else if (gameState == LEVEL2_STORY) {
      if (level2StoryIndex > 0) {
        level2StoryIndex--;
        storyTimerCount = 0;
      } else {
        gameState = LEVEL2_INTRO;
        storyTimerCount = 0;
      }
    } else if (gameState == LEVEL2_CAVE_STORY) {
      if (level2CaveStoryIndex > 0) {
        level2CaveStoryIndex--;
        storyTimerCount = 0;
      } else {
        gameState = NEXT_LEVEL_IQ; // Go back to cave
        storyTimerCount = 0;
      }
    }
    break;
  // ---- Level Selection ----
  case 30:
    playMusicTrack(TRACK_NONE);
    currentLevel = 1;
    gameState = LEVEL1_INTRO;
    storyTimerCount = 0;
    break;
  case 31:
    playMusicTrack(TRACK_NONE);
    currentLevel = 2;
    gameState = LEVEL2_INTRO;
    storyTimerCount = 0;
    break;
  case 32:
    playMusicTrack(TRACK_NONE);
    gameState = LEVEL3_INTRO;
    storyTimerCount = 0;
    break;
  case 33:
    playMusicTrack(TRACK_NONE);
    gameState = LEVEL4_INTRO;
    break;
  case 34:
    previousState = LEVEL_SELECTION;
    gameState = CUSTOMIZATION;
    break;
  // ---- Home button ----
  case 23:
    gameState = MENU;
    playMusicTrack(TRACK_INTRO);
    break;
  default:
    break;
  }
  pendingAction = -1;
  btnAnimActive = false;
  btnAnimCode = -1;
  btnAnimContext = -1;
}

// Play click sound (respects isSoundOn toggle)
static void playClickSound() {
  if (isSoundOn) {
    mciSendStringA("close click", NULL, 0, NULL);
    mciSendStringA("open \"sound\\click.wav\" type waveaudio alias click", NULL,
                   0, NULL);
    mciSendStringA("play click", NULL, 0, NULL);
  }
}

// Helper: start grow animation for a button, register the pending action
static void startBtnAnim(int context, int code, int action) {
  if (btnAnimActive)
    return; // ignore if already animating
  playClickSound();
  btnAnimActive = true;
  btnAnimContext = context;
  btnAnimCode = code;
  btnAnimTimer = 1; // fire after 1 buttonAnimationTimer tick (0.5 second)
  pendingAction = action;
}

// Animation timer logic (runs every 0.5 seconds)
void buttonAnimationTimer() {
  if (btnAnimActive) {
    btnAnimTimer--;
    if (btnAnimTimer <= 0) {
      firePendingAction();
    }
  }
}

void globalTimerLogic() {
  // Animation logic moved to buttonAnimationTimer() for 0.5s precision

  if (gameState == INTRO) {
    introTimer++;
    if (introTimer >= 3) {
      gameState = MENU;
    }
  }

  if (gameState == INITIAL_STORY && !btnAnimActive) {
    storyTimerCount++;
    if (storyTimerCount >= 5) {
      storyTimerCount = 0;
      initialStoryIndex++;
      if (initialStoryIndex >= 12) {
        gameState = LEVEL_SELECTION;
        storyTimerCount = 0;
      }
    }
  } else if (gameState == LEVEL_SELECTION) {
    // No timer progression for level selection screen itself
  } else if (gameState == LEVEL1_INTRO && !btnAnimActive) {
    storyTimerCount++;
    if (storyTimerCount >= 3) {
      gameState = LEVEL1_STORY;
      storyTimerCount = 0;
      level1StoryIndex = 0;
      playMusicTrack(TRACK_LEVEL1);
    }
  } else if (gameState == LEVEL2_INTRO && !btnAnimActive) {
    // No automatic transition for intro per previous user requests
  } else if (gameState == LEVEL2_STORY && !btnAnimActive) {
    storyTimerCount++;
    if (storyTimerCount >= 5) {
      storyTimerCount = 0;
      level2StoryIndex++;
      if (level2StoryIndex >= 3) {
        currentLevel = 2;
        resetGame();
      }
    }
  } else if (gameState == LEVEL2_CAVE_STORY && !btnAnimActive) {
    storyTimerCount++;
    if (storyTimerCount >= 5) {
      storyTimerCount = 0;
      level2CaveStoryIndex++;
      if (level2CaveStoryIndex >= 3) {
        gameState = NEXT_LEVEL_IQ;
        initCaveState();
        initIQ();
        playMusicTrack(TRACK_CAVE);
        storyTimerCount = 0;
      }
    }
  } else if (gameState == LEVEL3_INTRO || gameState == LEVEL4_INTRO) {
    // Disable automatic transition to gameplay for levels 2, 3, and 4
  } else if (gameState == LEVEL1_STORY && !btnAnimActive) {
    storyTimerCount++;
    if (storyTimerCount >= 5) {
      storyTimerCount = 0;
      level1StoryIndex++;
      if (level1StoryIndex >= 3) {
        resetGame();
      }
    }
  }

  if (gameState == GAME && !isGamePaused && !levelOneComplete) {
    gameRunTimeSeconds++;

    int transitionTime = (currentLevel == 1) ? 60 : 90;

    if (gameRunTimeSeconds >= transitionTime && !isCaveActive) {
      isCaveActive = true;
      caveX = 1000;
      for (int i = 0; i < 3; i++) {
        obstacles[i].active = false;
        obstacles[i].x = -200;
      }
    }
  }
}

void checkCollision() {
  if (gameState != GAME || isGamePaused || isInvincible)
    return;
  if (levelOneComplete && currentLevel != 2)
    return;

  if (currentLevel == 2) {
    // ---- SHARK LOGIC for Level 2 ----
    for (int i = 0; i < 2; i++) {
      if (sharks[i].active && !sharks[i].isDying && !sharks[i].isRetreating) {
        int sW = (sharks[i].type == 1) ? 70 : 90;
        int sH = (sharks[i].type == 1) ? 70 : 90;

        // Check proximity for damage
        if (charX + charWidth - 30 > sharks[i].x &&
            charX + 10 < sharks[i].x + sW && charY + charHeight > sharks[i].y &&
            charY < sharks[i].y + sH) {
          // Avoid damage if currently "attacking" (recently pressed space)
          if (isDoubleShiftTriggered || isShiftPressed) {
            continue;
          }
          // Normal Damage
          int damage = (sharks[i].type == 1) ? 1 : 2;
          lives -= damage;
          startScreenShake(15, 8); // Medium shake on shark bite
          if (lives <= 0) {
            updateHighScore(currentLevel, applesCollected);
            gameState = GAME_OVER;
          } else {
            isInvincible = true;
            invincibilityTimer = 60;
          }
        }
      }
    }

    // ---- DRAGON KILLING WITH SHIFT ----
    for (int i = 0; i < 5; i++) {
      if (dragons[i].active && !dragons[i].isVanishing) {
        // Dragon center is x+150, y+150 (300x300 size)
        float targetX = (float)(charX + (float)charWidth / 2.0f);
        float targetY = (float)(charY + (float)charHeight / 2.0f);
        float dx = targetX - (dragons[i].x + 150.0f);
        float dy = targetY - (dragons[i].y + 150.0f);
        float dist = (float)sqrt(dx * dx + dy * dy);

        if (dist < 150.0f) {
          if (keyPressed[' ']) { // Space allows killing
            dragons[i].isVanishing = true;
            dragons[i].vanishingTimer = 30;
          } else if (!isInvincible) {
            // Contact damage from dragon if not attacking/shifting
            lives--;
            if (lives <= 0) {
              updateHighScore(currentLevel, applesCollected);
              gameState = GAME_OVER;
            } else {
              isInvincible = true;
              invincibilityTimer = 60;
            }
          }
        }
      }
    }

    // ---- BOMB COLLISION ----
    for (int i = 0; i < 3; i++) {
      if (bombs[i].active) {
        if (charX + charWidth - 30 > bombs[i].x &&
            charX + 30 < bombs[i].x + 70 &&
            charY + charHeight - 30 > bombs[i].y &&
            charY + 10 < bombs[i].y + 70) {
          bombs[i].active = false;
          isCharFrozen = true;
          freezeTimer = 100; // 3 seconds @ 30ms (approx 100 ticks)
          lives--;
          startScreenShake(20, 15); // Large shake on bomb explosion

          if (lives <= 0) {
            updateHighScore(currentLevel, applesCollected);
            gameState = GAME_OVER;
          }
          for (int j = 0; j < 3; j++) {
            if (!explosions[j].active) {
              explosions[j].active = true;
              // Center the explosion on the character's body
              explosions[j].x = (float)(charX + charWidth / 2);
              explosions[j].y = (float)(charY + charHeight / 2);
              explosions[j].frame = 0;
              explosions[j].timer = 0;
              break;
            }
          }
        }
      }
    }
    return;
  }

  // Level 1 logic
  for (int i = 0; i < 3; i++) {
    if (obstacles[i].active) {
      int obsHitW = 100;
      int obsHitH = (obstacles[i].type == 2) ? 70 : 100;
      if (charX + charWidth - 85 > obstacles[i].x + 20 &&
          charX + 85 < obstacles[i].x + obsHitW &&
          charY + charHeight - 50 > obstacles[i].y &&
          charY < obstacles[i].y + obsHitH) {
        lives--;
        startScreenShake(15, 8); // Medium shake on obstacle clash
        if (lives <= 0) {
          updateHighScore(currentLevel, applesCollected);
          gameState = GAME_OVER;
        } else {
          isInvincible = true;
          invincibilityTimer = 60;
        }
      }
    }
  }
}

/* -------------------- WRAPPERS -------------------- */
void autoScrollRecursiveWrapper() {
  if (!isGamePaused && gameState == GAME &&
      (currentLevel == 2 || !levelOneComplete) &&
      (!isCaveActive || caveX > 650)) {
    autoScrollRecursive();
  }
}

void updateObstaclePhysicsWrapper() {
  if (!isGamePaused && gameState == GAME &&
      (currentLevel == 2 || !levelOneComplete)) {
    updateObstaclePhysics();
  }
}

void updateCharacterMovementWrapper() {
  if (!isGamePaused && gameState == GAME) {
    if (!isCaveActive || caveX > 650) {
      updateCharacterMovement();
    }
    if (isInvincible) {
      invincibilityTimer--;
      if (invincibilityTimer <= 0) {
        isInvincible = false;
      }
    }
  }
}

void updateApplesWrapper() {
  if (!isGamePaused && gameState == GAME &&
      (currentLevel == 2 || !levelOneComplete)) {
    updateApplePhysics();
    checkAppleCollision();
  }
}

void updateWeatherWrapper() {
  if (!isGamePaused && gameState == GAME &&
      (currentLevel == 2 || !levelOneComplete)) {
    updateWeatherPhysics();
  }
}

void updateHeartWrapper() {
  if (!isGamePaused && gameState == GAME &&
      (currentLevel == 2 || !levelOneComplete)) {
    updateHeartLogic();
  }
}

void updateWizardWrapper() {
  if (gameState == NEXT_LEVEL_IQ) {
    updateWizardLogic();
  }
}

// MASTER LOGIC UPDATE
void updateAttackAnimation() {
  if (!isAttacking)
    return;

  int maxFrames = (selectedCharacter == 0) ? 7 : 6;

  attackAnimCounter++;
  if (attackAnimCounter >= attackAnimSpeed) {
    attackAnimCounter = 0;
    attackFrameIndex++;
    if (attackFrameIndex >= maxFrames) {
      attackFrameIndex = 0;
      isAttacking = false;
    }
  }
}

void updateFallSequence() {
  if (!isFallingSequence)
    return;

  fallY += fallVel;
  fallVel -= 0.8f; // Stronger gravity for fall effect

  if (fallY < -200) {
    // Reached bottom - Respawn Logic
    if (hitBridgeIndex != -1 && bridges[hitBridgeIndex].active) {
      charX = (int)bridges[hitBridgeIndex].x + 450 + 10;
      if (charX > 880)
        charX = 880;
    }

    // Trigger Jump Effect
    isFallingSequence = false;
    isCharFrozen = false;
    isJumping = true;
    charY = groundY + 2;   // Start just above ground to avoid immediate cancel
    verticalVelocity = 25; // Moderate jump up
    currentJumpDirection = 0; // Jump straight up at landing spot
    hitBridgeIndex = -1;
  }
}

// Polling for Space key state and double-space detection
void updateShiftInput() {
  static bool prevSpace = false;
  static int spaceDoubleTimer = 0;
  static int doubleSpaceActiveTimer = 0;
  static int singleSpaceActiveTimer = 0;

  bool currentSpace = keyPressed[' '] != 0;

  if (doubleSpaceActiveTimer > 0)
    doubleSpaceActiveTimer--;
  if (singleSpaceActiveTimer > 0)
    singleSpaceActiveTimer--;

  // CRITICAL: If Space is held, force clear Arrow keys to prevent accidental
  // jumps
  if (currentSpace) {
    specialKeyPressed[GLUT_KEY_UP] = 0;
    specialKeyPressed[GLUT_KEY_DOWN] = 0;
  }

  if (currentSpace && !prevSpace) {
    if (spaceDoubleTimer > 0) {
      // ** DOUBLE TAP detected ** -> KILL any on-screen shark
      doubleSpaceActiveTimer = 15;
      spaceDoubleTimer = 0;
      singleSpaceActiveTimer = 0; // Cancel single tap effect if it just started

      if (currentLevel == 2 && gameState == GAME) {
        for (int i = 0; i < 2; i++) {
          // If shark is on screen and not already dying, kill it
          if (sharks[i].active && !sharks[i].isDying &&
              sharks[i].x < screenWidth + 200) {
            sharks[i].isDying = true;
            sharks[i].dyingVel = 5.0f; // Pop up

            // Re-center on screen if it was just warped away by a single tap
            if (sharks[i].x > (float)screenWidth) {
              sharks[i].x = (float)(screenWidth - 150);
            }

            // AWARD SCORE
            int scoreGain = (sharks[i].type == 1) ? 10 : 20;
            applesCollected += scoreGain;
          }
        }
      }
    } else {
      // ** SINGLE TAP detected ** -> RETREAT any on-screen shark
      spaceDoubleTimer = 15;
      singleSpaceActiveTimer = 15;

      // Wait a tiny bit or just do it? We do it immediately but double tap can
      // override.
      if (currentLevel == 2 && gameState == GAME) {
        for (int i = 0; i < 2; i++) {
          if (sharks[i].active && !sharks[i].isDying &&
              sharks[i].x < screenWidth) {
            // RETREAT: Send back to the right
            sharks[i].isRetreating = true;
          }
        }
      }
    }
  }

  if (spaceDoubleTimer > 0)
    spaceDoubleTimer--;
  prevSpace = currentSpace;

  // Let the triggers stay alive for the duration of the timers
  // Re-using the same variables (isDoubleShiftTriggered/isShiftPressed) to
  // avoid global re-declarations
  isDoubleShiftTriggered = (doubleSpaceActiveTimer > 0);
  isShiftPressed = (singleSpaceActiveTimer > 0 || currentSpace);
}

// MASTER LOGIC UPDATE
// Master Game Loop (runs every 30ms) - Consolidates individual timers to
// avoid
//  iGraphics limit
void masterGameLoop() {
  updateShiftInput();
  autoScrollRecursiveWrapper();
  updateObstaclePhysicsWrapper();
  checkCollision();
  updateCharacterMovementWrapper();
  updateFallSequence();
  updateScreenShake();
  updateScorePopups();

  // Magnet Spawn & Check
  if (gameState == GAME && !isGamePaused && !isMagnetVisible &&
      !isMagnetBuffActive) {
    magnetSpawnTimer++;
    if (magnetSpawnTimer > magnetNextSpawnThreshold) {
      magnetSpawnTimer = 0;
      magnetNextSpawnThreshold = 233 + rand() % 201;
      if (rand() % 2 == 0) { // Still keeping 50% chance but can make it 100% if
                             // user wants precise delivery
        isMagnetVisible = true;
        magnetX = (float)(charX + 200 + rand() % 400);
        if (magnetX > screenWidth - 50)
          magnetX = screenWidth - 50;
        magnetY = screenHeight + 50;
      }
    }
  }

  updateApplesWrapper();
  checkMagnetCollision(); // From AppleHandler.h
  updateWeatherWrapper();
  updateHeartWrapper();
  updateCavePhysics();
  updateWizardWrapper();
  updateRunAnimation();
  updateDragonPhysics();
  updateShardLogic();
  updateAttackAnimation();

  // Handle freeze timer
  if (isCharFrozen) {
    freezeTimer--;
    if (freezeTimer <= 0) {
      isCharFrozen = false;
    }
  }
}

/* -------------------- DRAW FUNCTION -------------------- */
void iDraw() {
  iClear();

  if (gameState == INTRO) {
    iShowImage(0, 0, screenWidth, screenHeight, introImage);
  } else if (gameState == MENU) {
    drawMainMenu();
  } else if (gameState == CUSTOMIZATION) {
    drawCustomizationMenu();
  } else if (gameState == SETTINGS) {
    drawSettingsMenu();
  } else if (gameState == INITIAL_STORY) {
    drawInitialStory();
  } else if (gameState == LEVEL_SELECTION) {
    drawLevelSelection();
  } else if (gameState == LEVEL1_INTRO) {
    drawLevel1Intro();
  } else if (gameState == LEVEL2_INTRO) {
    drawLevel2Intro();
  } else if (gameState == LEVEL2_CAVE_STORY) {
    drawLevel2CaveStory();
  } else if (gameState == LEVEL3_INTRO) {
    drawLevel3Intro();
  } else if (gameState == LEVEL4_INTRO) {
    drawLevel4Intro();
  } else if (gameState == LEVEL1_STORY) {
    drawLevel1Story();
  } else if (gameState == LEVEL2_STORY) {
    drawLevelTwoStory();
  } else if (gameState == LEVEL3_INTRO) {
    drawLevel3Intro();
  } else if (gameState == LEVEL4_INTRO) {
    drawLevel4Intro();
  } else if (gameState == LEVEL1_STORY) {
    drawLevel1Story();
  } else if (gameState == GAME || gameState == LEVEL_COMPLETE ||
             gameState == GAME_OVER) {
    drawBackground();
    drawApples();
    drawObstacles();
    drawDragons();

    if (isCaveActive) {
      if (currentLevel == 2) {
        iShowImage(caveX, 90, 300, 300, caveImgL2);
      } else {
        iShowImage(caveX, 0, 400, 400, caveImg);
      }
    }

    drawCharacter();
    drawMagnet(); // From AppleHandler.h
    drawExplosions();
    drawWeatherEffects();
    drawLivesUI();
    drawAppleScoreUI();
    drawTimerUI();
    drawShardElements();

    // Draw Score Pop-ups
    iSetColor(255, 255, 0); // Yellow for popups
    for (int i = 0; i < 10; i++) {
      if (popups[i].active) {
        char popupText[10];
        sprintf_s(popupText, "+1");
        iText(popups[i].x, popups[i].y, popupText, GLUT_BITMAP_HELVETICA_18);
      }
    }
    iSetColor(255, 255, 255); // Reset color

    if (gameState == GAME) {
      drawPauseMenu();
    } else if (gameState == GAME_OVER) {
      iSetColor(255, 0, 0);
      iText(screenWidth / 2.0 - 50, screenHeight / 2.0, (char *)"GAME OVER",
            GLUT_BITMAP_TIMES_ROMAN_24);
      iSetColor(255, 255, 255);
      iText(screenWidth / 2.0 - 80, screenHeight / 2.0 - 30,
            (char *)"Press 'R' to Retry", GLUT_BITMAP_HELVETICA_18);
    } else if (gameState == LEVEL_COMPLETE) {
      drawLevelComplete();
    }
  } else if (gameState == NEXT_LEVEL_IQ) {
    drawCave();
    if (isLevelTransitioning)
      return;
    if (!isWizardMoving) {
      drawIQ();
    }

    // Draw buttons only after answering
    if (iqAnswered) {
      // Home Button Removed per user request
      iShowImage(btnNextIQ_Level2.x, btnNextIQ_Level2.y, btnNextIQ_Level2.w,
                 btnNextIQ_Level2.h, imgNextLevel);
    }
  } else if (gameState == HIGHSCORES) {
    iShowImage(0, 0, screenWidth, screenHeight, mainMenuBG);
    iSetColor(255, 255, 255);
    iText(screenWidth / 2.0 - 100, screenHeight / 2.0 + 100,
          (char *)"HIGH SCORES", GLUT_BITMAP_TIMES_ROMAN_24);

    char scoreBuf[50];
    for (int i = 0; i < 4; i++) {
      sprintf_s(scoreBuf, "Level %d: %d Food", i + 1, highScores[i]);
      iText(screenWidth / 2.0 - 80, screenHeight / 2.0 + 50 - (i * 40),
            scoreBuf, GLUT_BITMAP_HELVETICA_18);
    }

    iSetColor(200, 200, 200);
    iText(screenWidth / 2.0 - 130, 50, (char *)"Press 'B' to Return to Menu",
          GLUT_BITMAP_HELVETICA_18);
  } else if (gameState == LEVEL_NOT_READY) {
    iShowImage(0, 0, screenWidth, screenHeight, levelSelectionBG);
    iSetColor(255, 255, 255);
    iText(screenWidth / 2.0 - 180, screenHeight / 2.0,
          (char *)"LEVEL GAMEPLAY NOT READY YET", GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(200, 200, 200);
    iText(screenWidth / 2.0 - 150, screenHeight / 2.0 - 50,
          (char *)"Press 'B' to Return to Selection", GLUT_BITMAP_HELVETICA_18);
  }
}

/* -------------------- INPUT FUNCTIONS -------------------- */
void iMouseMove(int mx, int my) {}
void iPassiveMouseMove(int mx, int my) {}

void iMouse(int button, int state, int mx, int my) {
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    // Block all menu clicks while animation is running
    if (btnAnimActive)
      return;

    if (gameState == GAME) {
      int pauseAction = handlePauseMenuMouse(mx, my);
      if (pauseAction != 0) {
        playClickSound();
      }
      if (pauseAction == 2) {
        resetGame();
      } else if (pauseAction == 3) {
        previousState = GAME;
        gameState = SETTINGS;
      } else if (pauseAction == 4) {
        gameState = LEVEL_SELECTION;
      }
      return;
    }

    if (gameState == MENU) {
      int result = handleMainMenuMouse(mx, my);
      if (result == 1) {
        startBtnAnim(0, 1, 1); // Start -> Initial Story
      } else if (result == 3) {
        startBtnAnim(0, 3, 3); // Customization
      } else if (result == 2) {
        startBtnAnim(0, 2, 2); // Settings
      } else if (result == 5) {
        startBtnAnim(0, 5, 5);  // Highscores
      } else if (result == 4) { // Exit (via handleMainMenuMouse -> calls
                                // exit(0) but we catch code 4 below)
        startBtnAnim(0, 4, 4);  // Exit animation then exit(0)
      }
    } else if (gameState == SETTINGS) {
      // Settings buttons: no grow animation requested, back/close are excluded
      // anyway
      int result = handleSettingsMenuMouse(mx, my);
      playClickSound(); // always play on any settings button click
      if (result != -1) {
        if (result == MENU)
          gameState = MENU;
        else if (result == GAME)
          gameState = GAME;
      }
    } else if (gameState == CUSTOMIZATION) {
      int initialState = customSubState;
      int result = handleCustomizationMouse(mx, my);
      // Back button (result == 0): instant -- excluded from animation
      if (result == 0) {
        playClickSound();
        if (previousState != -1)
          gameState = (GameState)previousState;
        else
          gameState = MENU;
      }
      if (result == 2) {
        playClickSound();
      }
      // Sub-menu entry buttons get animation
      // ONLY if we were in the main customization menu (initialState == 0)
      if (initialState == 0) {
        if (customSubState == 1) {
          customSubState = 0; // undo immediate switch
          startBtnAnim(1, 10, 10);
        } else if (customSubState == 2) {
          customSubState = 0; // undo immediate switch
          startBtnAnim(1, 11, 11);
        }
      }
    } else if (gameState == INITIAL_STORY) {
      if (handleHomeButtonMouse(mx, my)) {
        startBtnAnim(2, 23, 23); // Home
        return;
      }
      int action = handleStoryMouse(mx, my);
      if (action == 1) {
        startBtnAnim(2, 20, 20); // Next
      } else if (action == 2) {
        startBtnAnim(2, 21, 21); // Skip
      } else if (action == 3) {
        startBtnAnim(2, 22, 22); // Previous
      }
    } else if (gameState == LEVEL_SELECTION) {
      if (handleHomeButtonMouse(mx, my)) {
        startBtnAnim(2, 23, 23); // Home
        return;
      }
      int action = handleLevelSelectionMouse(mx, my);
      if (action == 1) {
        startBtnAnim(2, 30, 30);
      } else if (action == 2) {
        startBtnAnim(2, 31, 31);
      } else if (action == 3) {
        startBtnAnim(2, 32, 32);
      } else if (action == 4) {
        startBtnAnim(2, 33, 33);
      } else if (action == 5) {
        startBtnAnim(2, 34, 34);
      }
    } else if (gameState == LEVEL1_INTRO) {
      if (handleHomeButtonMouse(mx, my)) {
        startBtnAnim(2, 23, 23); // Home
        return;
      }
      int action = handleStoryMouse(mx, my);
      if (action == 1) { // Next
        startBtnAnim(2, 20, 20);
      } else if (action == 2) { // Skip
        startBtnAnim(2, 21, 21);
      } else if (action == 3) { // Previous
        startBtnAnim(2, 22, 22);
      }
    } else if (gameState == LEVEL2_INTRO) {
      if (handleHomeButtonMouse(mx, my)) {
        startBtnAnim(2, 23, 23); // Home
        return;
      }
      int action = handleStoryMouse(mx, my);
      if (action == 1) { // Next
        startBtnAnim(2, 20, 20);
      } else if (action == 2) { // Skip
        startBtnAnim(2, 21, 21);
      } else if (action == 3) { // Previous
        startBtnAnim(2, 22, 22);
      }
    } else if (gameState == LEVEL2_CAVE_STORY) {
      if (handleHomeButtonMouse(mx, my)) {
        startBtnAnim(2, 23, 23); // Home
        return;
      }
      int action = handleStoryMouse(mx, my);
      if (action == 1) { // Next
        startBtnAnim(2, 20, 20);
      } else if (action == 2) { // Skip
        startBtnAnim(2, 21, 21);
      } else if (action == 3) { // Previous
        startBtnAnim(2, 22, 22);
      }
    } else if (gameState == LEVEL3_INTRO || gameState == LEVEL4_INTRO) {
      if (handleHomeButtonMouse(mx, my)) {
        startBtnAnim(2, 23, 23); // Home
        return;
      }
      int action = handleStoryMouse(mx, my);
      if (action == 3) { // Previous
        startBtnAnim(2, 22, 22);
      } else if (action == 1) { // Next (no-op but animate anyway)
        startBtnAnim(2, 20, 20);
      } else if (action == 2) { // Skip
        startBtnAnim(2, 21, 21);
      }
    } else if (gameState == LEVEL1_STORY || gameState == LEVEL2_STORY) {
      int action = handleStoryMouse(mx, my);
      if (action == 1) { // Next
        startBtnAnim(2, 20, 20);
      } else if (action == 2) { // Skip
        startBtnAnim(2, 21, 21);
      } else if (action == 3) { // Previous
        startBtnAnim(2, 22, 22);
      }
    } else if (gameState == LEVEL_COMPLETE) {
      int action = handleLevelCompleteMouse(mx, my);
      if (action != 0)
        playClickSound();
      if (action == 1) {
        playMusicTrack(TRACK_NONE);
        if (currentLevel == 2) {
          // Go directly to cave 2 story after level 2 completion
          gameState = LEVEL2_CAVE_STORY;
          level2CaveStoryIndex = 0;
          storyTimerCount = 0;
        } else {
          gameState = NEXT_LEVEL_IQ;
          initCaveState();
          initIQ();
        }
      } else if (action == 2)
        resetGame();
      else if (action == 3) {
        gameState = MENU;
        playMusicTrack(TRACK_INTRO);
      }
    } else if (gameState == NEXT_LEVEL_IQ) {
      if (isLevelTransitioning) {
        int result = handleCaveTransitionClick(mx, my);
        if (result == 1) {
          playClickSound();
          playMusicTrack(TRACK_NONE);
          if (currentLevel == 2) {
            gameState = LEVEL3_INTRO;
          } else {
            gameState = LEVEL2_INTRO;
          }
          storyTimerCount = 0;
        } else if (result == 2) {
          playClickSound(); // Next button on phase 1 transition
          playMusicTrack(TRACK_CAVE);
        }
        return;
      }
      if (!isWizardMoving && !isCharacterEntering) { // Wait for char too
        bool wasAnswered = iqAnswered;
        handleIQClick(mx, my);
        if (!wasAnswered && iqAnswered) {
          playClickSound(); // IQ answer button clicked
        }
      }
      int action = handleNextLevelIQMouse(mx, my);
      // Action 3 (Home) removed (ignored)
      if (iqAnswered && action == 4) {
        playClickSound();
        if (iqCorrect) {
          // Correct answer: Show Shard Explanation Splash
          hasClaimedShard = true;
          isLevelTransitioning = true;
          transitionPhase = 2; // Show shardexplain.png
        } else {
          // Incorrect answer: Skip Explanation, go directly to next stage
          playMusicTrack(TRACK_NONE);
          if (currentLevel == 2) {
            gameState = LEVEL3_INTRO;
          } else {
            gameState = LEVEL2_INTRO;
          }
          storyTimerCount = 0;
        }
        // Do NOT switch to LEVEL2_INTRO yet for correct answer. Wait for
        // transitions.
      }
    }
  }
}

void iKeyboard(unsigned char key) {
  if (gameState == INTRO && key == '\r') {
    loadMainMenuAssets();
    gameState = MENU;
  }
  if (gameState == SETTINGS && (key == 'm' || key == 'M')) {
    if (previousState != -1)
      gameState = (GameState)previousState;
    else
      gameState = MENU;
  }
  if (gameState == GAME_OVER && (key == 'r' || key == 'R')) {
    resetGame();
  }
  if (gameState == HIGHSCORES && (key == 'b' || key == 'B')) {
    gameState = MENU;
  }
  if (gameState == LEVEL_NOT_READY && (key == 'b' || key == 'B')) {
    gameState = LEVEL_SELECTION;
    playMusicTrack(TRACK_STORY);
  }

  // Storytelling Shortcuts: Space for Skip
  if (!btnAnimActive && key == ' ') {
    if (gameState == INITIAL_STORY || gameState == LEVEL1_INTRO ||
        gameState == LEVEL1_STORY || gameState == LEVEL2_INTRO ||
        gameState == LEVEL2_STORY || gameState == LEVEL2_CAVE_STORY ||
        gameState == LEVEL3_INTRO || gameState == LEVEL4_INTRO) {
      startBtnAnim(2, 21, 21); // Skip
    }
  }

  if (gameState == GAME && key == ' ') {
    if (!isAttacking) {
      isAttacking = true;
      attackFrameIndex = 0;
      attackAnimCounter = 0;
    }
  }
}

void iSpecialKeyboard(unsigned char key) {
  if (btnAnimActive)
    return;

  // Storytelling Shortcuts: Right Arrow for Next, Left Arrow for Previous
  if (gameState == INITIAL_STORY || gameState == LEVEL1_INTRO ||
      gameState == LEVEL1_STORY || gameState == LEVEL2_INTRO ||
      gameState == LEVEL2_STORY || gameState == LEVEL2_CAVE_STORY ||
      gameState == LEVEL3_INTRO || gameState == LEVEL4_INTRO) {
    if (key == GLUT_KEY_RIGHT) {
      startBtnAnim(2, 20, 20); // Next
    } else if (key == GLUT_KEY_LEFT) {
      // Return to level selection but play the music too
      gameState = LEVEL_SELECTION;
      storyTimerCount = 0;
      playMusicTrack(TRACK_STORY);
      // startBtnAnim(2, 22, 22); // Previous
    }
  } else if (gameState == LEVEL_COMPLETE) {
    if (key == GLUT_KEY_RIGHT) {
      // Simulate Next button click (Action 1)
      playClickSound();
      playMusicTrack(TRACK_NONE);
      if (currentLevel == 2) {
        gameState = LEVEL2_CAVE_STORY;
        level2CaveStoryIndex = 0;
        storyTimerCount = 0;
      } else {
        gameState = NEXT_LEVEL_IQ;
        initCaveState();
        initIQ();
      }
    }
  } else if (gameState == NEXT_LEVEL_IQ) {
    if (key == GLUT_KEY_RIGHT) {
      if (isLevelTransitioning) {
        // Next on transition Phase 1 -> Phase 2 (or Cave Story)
        int result = handleCaveTransitionClick(760, 60); // Fake coords for Next
        if (result == 1) {
          playClickSound();
          playMusicTrack(TRACK_NONE);
          if (currentLevel == 2) {
            gameState = LEVEL2_CAVE_STORY;
            level2CaveStoryIndex = 0;
          } else {
            gameState = LEVEL2_INTRO;
          }
          storyTimerCount = 0;
        } else if (result == 2) {
          playClickSound();
          playMusicTrack(TRACK_CAVE);
        }
      } else if (iqAnswered) {
        // Next button after answering IQ
        playClickSound();
        if (iqCorrect) {
          hasClaimedShard = true;
          isLevelTransitioning = true;
          transitionPhase = 2; // Show shardexplain.png
        } else {
          playMusicTrack(TRACK_NONE);
          if (currentLevel == 2) {
            gameState = LEVEL3_INTRO;
          } else {
            gameState = LEVEL2_INTRO;
          }
          storyTimerCount = 0;
        }
      }
    }
  }
}
void fixedUpdate() {}

/* -------------------- MAIN FUNCTION -------------------- */
int main() {
  iInitialize(screenWidth, screenHeight, (char *)"Fragment of Divinity");
  srand((unsigned int)time(NULL));

  loadHighScores();

  initObstacles();
  introImage = iLoadImage((char *)"images fod\\menu\\intro.png");
  loadBackgroundAssets();
  loadObstacleAssets();
  loadCharacterCustomizationAssets();
  loadMainMenuAssets();
  loadCustomizationAssets();
  loadPauseMenuAssets();
  loadSettingsAssets();
  loadAppleAssets();
  loadLifeAssets();
  loadStoryAssets();
  loadWeatherAssets();
  caveImg = iLoadImage((char *)"background img\\cave.bmp");
  caveImgL2 = iLoadImage((char *)"background img\\cave2.png");
  loadLevelCompleteAssets();
  loadCaveAssets();
  caveSceneBgL2 = iLoadImage((char *)"Wizard\\cave22.png");
  wizardL2Img = iLoadImage((char *)"Wizard\\wizard2.0.png");
  loadCharacter2Assets();
  loadDragonAssets();
  initDragons();
  initShard();
  haloImg = iLoadImage((char *)"scores and items\\halo.png");

  iSetTimer(30, masterGameLoop);
  iSetTimer(500, buttonAnimationTimer);
  iSetTimer(1000, globalTimerLogic);

  playMusicTrack(TRACK_INTRO);

  iStart();
  return 0;
}
//text change