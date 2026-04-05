#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <math.h>
#include <mmsystem.h>
#include <stdio.h>
#include <time.h>


// iGraphics.h variables
unsigned int keyPressed[512];
unsigned int specialKeyPressed[512];

#include "GameState.h"

// ---- Global Game Variables ----
int currentLevel = 1;
GameState gameState = INTRO;
GameState prevGameStateForFade = INTRO;
int prevStoryIndexForFade = -1;
float currentFadeAlpha = 1.0f;
int previousState = MENU; // Changed to int to match header
bool isGamePaused = false;
int lives = 5;
int charX = 100, charY = 30;
int charWidth = 165, charHeight = 165;
bool isCaveStopped = false; // New flag for transition stop
int applesCollected = 0;
int gameRunTimeSeconds = 0;
int storyTimerCount = 0;
int level4HeartTimer = 0; // Timer for Level 4 heart drops (every 60s)
// bossLifeFrameImg is defined in Boss.h with SELECTANY - DO NOT REDEFINE HERE
// -------------------------------

// ---- Button Animation Variables ----
bool btnAnimActive = false;
int btnAnimTimer = 0;
int btnAnimCode = -1;
int btnAnimContext = -1;
int pendingAction = -1;
GameState pendingState = MENU;
// ---- Screen Shake Variables ----
float globalScreenShakeX = 0.0f;
float globalScreenShakeY = 0.0f;
// ------------------------------------

// ---- Damage Popups ----
struct DamagePopup {
  float x, y;
  int amount;
  int timer;
  bool active;
  bool isCrit;
};
DamagePopup damagePopups[20];

inline void spawnDamagePopup(int amount, float x, float y,
                             bool isCrit = false) {
  for (int i = 0; i < 20; i++) {
    if (!damagePopups[i].active) {
      damagePopups[i].active = true;
      damagePopups[i].amount = amount;
      damagePopups[i].x =
          x + (rand() % 30) - 15; // Random little horizontal jiggle
      damagePopups[i].y = y;
      damagePopups[i].timer = 45; // 1.5 seconds at 30ups
      damagePopups[i].isCrit = isCrit;
      break;
    }
  }
}
// -----------------------

#include "AppleHandler.h"
#include "BackgroundHandler.h"
#include "Boss.h" // Move Boss.h here so it can see Worms and other sprites
#include "Character.h"
#include "CharacterCustomization.h"
#include "CustomizationMenu.h"
#include "GameOverHandler.h"
#include "GunBarHandler.h"
#include "HighScoreHandler.h"
#include "IQ.h"
#include "LevelCompleteHandler.h"
#include "LifeHandler.h"
#include "MainMenu.h"
#include "ObstacleHandler.h"
#include "PauseMenu.h"
#include "SettingsMenu.h"
#include "ShardHandler.h"
#include "SoundHandler.h"
#include "StoryHandler.h"
#include "WeatherHandler.h"
#include "attack.h"
#include "cave.h"
#include "character 2.h"
#include "cinder.h"
#include "cloneattack.h"
#include "dragon.h"
#include "gunattack.h"
#include "iGraphics.h"
#include "owl.h"

#pragma comment(lib, "winmm.lib")

// --- Redundant re-declarations removed below... ---

/* -------------------- CONSTANTS -------------------- */
#define screenWidth 1000
#define screenHeight 600

// Forward Declarations
void drawLevel1Story();
void drawLevelTwoStory();
void drawLevelThreeStory();
void drawCaveTwoStory();
void drawTimer();

/* -------------------- GLOBAL VARIABLES -------------------- */
// Game States (Moved to GameState.h)

int iScreenHeight, iScreenWidth;
int iMouseX, iMouseY;
int ifft = 0;
void (*iAnimFunction[10])(void) = {0};
int iAnimCount = 0;
int iAnimDelays[10];
int iAnimPause[10];

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
int level2StoryIndex = 0;
int level3StoryImages[3];
int level3StoryIndex = 0;
int level4StoryImages[3];
int level4StoryIndex = 0;
int cave2StoryImages[3];
int cave2StoryIndex = 0;
int finalVictoryImages[2];
int finalVictoryIndex = 0;
int finalStoryImages[11];
int finalStoryIndex = 0;
int gunImg;
int gunExplainImg;

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
int imgGhost1, imgGhost2;
WeatherParticle weatherParticles[MAX_PARTICLES];
struct Ghost ghosts[MAX_GHOSTS];
int ghostSpawnTimer = 0;

// Game Timer (Task 1 & 3)

// Cave (Task 3)
int caveImg;
int caveX = -1000;
bool levelOneComplete = false;

// Level Complete & IQ Screen (Task Updated)
int levelCompleteBG;                             // lcf.bmp
int caveIQBG;                                    // cavebg.bmp
int imgNextLevel, imgRestartLevel, imgHomeLevel; // New button images

ButtonRect btnNextLevel, btnRestartLevel, btnHomeLevel;
ButtonRect btnNextIQ;        // For the button on the IQ screen
ButtonRect btnNextIQ_Level2; // Next level button for IQ screen

// Game Over Variables
int imgGameOverBG;
int imgSettingsBtn;

ButtonRect btnRestartGO;
ButtonRect btnHomeGO;
ButtonRect btnSettingsGO;

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

// Magnet Item globals
Magnet magnetItem;
int magnetImg;
int magnetSpawnTimer = 0;
int magnetEffectTimer = 0;
int magnetEffectActive = 0;

// Cave Transition Variables
int levelChange1, levelChange2;
int dialogueBoxImg; // Added for Task 5
bool isLevelTransitioning = false;
int transitionTimer = 0;
int transitionPhase = 0;

// Level 2 & 3 Cave & Wizard Assets
int caveImgL2, caveSceneBgL2, wizardL2Img;
int caveImgL3, wizardL3Img;

// Level 3 background assets
int l3bG1, l3bG2, l3bG2_1, l3bG2_2;
int charAttackDamagelessTimer = 0; // 1-second stun guard after attack
bool isL3Changed = false;

// Level 4 Boss variables moved to Boss.h

// IQ State Variables (Declared extern, defined in iMain.cpp)
bool iqAnswered = false;
bool iqCorrect = false;

// Level 3 Mini Game Variables
int miniGameTargetX = 400;
int miniGameTargetY = 300;
int miniGameTargetSpeedX = 6;
int miniGameTargetSpeedY = 6;
int miniGameClicks = 0;
Riddle *currentRiddles = NULL;
int currentRiddlePoolSize = 6;
int currentRiddleIndex = -1;
int previousRiddleIndex = -1;
int postAnswerTimer = 0;
int postAnswerIndex = 0; // 0: None, 1: First Line, 2: Second Line
IQButton iqOptions[3] = {
    {150, 480, 250, 60, ""}, {150, 410, 250, 60, ""}, {150, 340, 250, 60, ""}};

int charFrameIndex = 1; // Start at frame 2 (Index 1)
bool isCharMoving = false;
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
int originalHeight = 165; // Increased for Level 1
int bendHeight = 120;     // Slightly more bigger as requested
int doubleBendHeight = 80;
int jumpImageIndex = 0;
int currentJumpDirection = 0;
int jumpHorizontalSpeed = 4; // Level 1: 4, Other Levels: 7
int doubleJumpHorizontalSpeed = 7;
bool isShiftPressed = false;
bool isDoubleShiftTriggered = false;

// Gun Attack Globals
Fireball fireballs[MAX_FIREBALLS];
bool isGunAttacking = false;
int gunAttackFrameIndex = 0;
int gunAttackAnimCounter = 0;
int gunAttackShotsRemaining = 0;
int arynGun[3];
int kaeroGun[3];
int leoraGun[3];
int blueFireball[3];
int blueFireballBack[3];
int redFireball[3];
int redFireballBack[3];

// AppleHandler.h variables
Apple apples[20];
// int applesCollected = 0;
int appleImg;
int appleSpawnTimer = 0;

// HighScore (persisted to highscores.txt)
int highScores[4] = {0, 0, 0, 0};

// ObstacleHandler.h variables
int obs1, obs2, obs3;
int obs1Imgs[7];
int obs2Imgs[10];
int obs3Imgs[9];

int shark11, shark12, shark13;
int shark21, shark22, shark23;
int obsBrokenBridge;

int obs221, obs222, obs223;
int imgBroken;
int imgRLL, imgGLL;
int imgRBL, imgGBL;

int hitBridgeIndex = -1;

bool isCaveActive = false;

// BackgroundHandler.h variables
int bG1, bG2, bG3, bG4;
int sbG1, sbG2, sbG3, sbG4;
int wbG1, wbG2, wbG3, wbG4;
int s2bG1, s2bG2, s2bG3, s2bG4;
int w2bG1, w2bG2, w2bG3, w2bG4;
int r2bG1, r2bG2, r2bG3, r2bG4;
int bGX = 0;

// PauseMenu.h variables
int pauseBtnImage;
int pauseFrameImage;
int btnContinue, btnRestart, btnSettingsPause, btnExitPause;
PauseButton bPause, bContinue, bRestart, bSettingsP, bExitP;
int frameX, frameY, frameW, frameH;

int obstacleSpawnTimer = 0;
int screenShakeTimer = 0;

// Level 3 Assets
// Worm assets moved to Boss.h
int l3HoleImg;
int handImgs[4];
int shortPillerImg, longPillerImg;
int flameImgs[4];
int fireImgs[4];
int l34obsImg;
int l34HoleImgsAnimated[6];

bool isL3HoleRemoved = false;
bool flamesHitGround[2];
bool isFlameFalling[2];
float flameYPos[2];
float flameVel[2];
int flameFrame = 0;
int fireFrame = 0;
int handFrame = 0;
float handOffset = 0;
float handDir = 1;
int wormFrame = 0;

Obstacle obstacles[3];
Shark sharks[2];
BrokenBridge bridges[2];
L34Obstacle l34Obstacles[2];
int sharksSpawnedInLevel = 0;

int skullImgs[4];
Skull skulls[3];
int skullSpawnTimer = 0;
int skullsKilled = 0;

// Bomb Mechanic Globals
int bombImgs[4];
int bombAnimCounter = 0;
int bombAnimFrame = 0;
int explosionImgs[7];
Bomb bombs[3];
Explosion explosions[3];
bool isCharFrozen = false;
int freezeTimer = 0;
bool isFallingSequence = false;
float fallY = 0;
float fallVel = 0;
int obsGapMin = 400;
int obsGapMax = 700;
int obsHighY = 110;
int obsLowY = 20;
int level2SpawnDist = 0;
int bombSpawnTimer = 0;
int distSinceLast = 0;
int nextSpawnGap = 300;

// Shard & Halo Globals
float fallingShardX = 0, fallingShardY = 0;
bool isFallingShardActive = false;
int shardSpawnTimer = 0;
bool isHaloActive = false;
int haloTimer = 0;
int haloFrames[5];
bool hasClaimedShard = false;
bool hasClaimedGun = false;
bool hasCompanion = false; // Set when Level 3 cave IQ is won correctly

// Level 4 Owl Power-up
int l3CompanionImg;       // Companion splash image (Wizard\l3companion.png)
int owlImg;               // Owl power-up icon (level4\owl\owl power up.png)
int owlBulletImg;         // Owl bullet image (level4\owl\bullet.png)
int owlFrames[9];         // Owl animation frames
int owlStandingFrames[4]; // Owl standing frames
bool owlReady = true;     // Whether owl is available to use
int owlCooldownTimer = 0; // Seconds remaining (10 -> 0)

// MainMenu.h variables
int mainMenuBG;
int btnStart, btnSettings, btnCustomization, btnExit, btnHighscore;
Button bStart, bSettings, bCustomization, bExit, bHighscore;

// Button Click Animation globals (isMusicOn/SoundOn moved to SoundHandler.h)
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

int arynAttack[4];
int kaeroAttack[6];
int leoraAttack[3];
int arynAttackBack[4];
int kaeroAttackBack[6];
int leoraAttackBack[3];

int arynFall;
int kaeroFall;
int leoraFall;

int arynJumpAttack[2];
int kaeroJumpAttack[2];
int leoraJumpAttack[2];

// Static (idle replacement) images for Level 4
int arynStatic[3];
int arynStaticBack[3];
int kaeroStatic[3];
int kaeroStaticBack[3];
int leoraStatic[3];
int leoraStaticBack[3];

// Resting motion images
int arynResting[3];
int kaeroResting[3];
int leoraResting[3];

// Level 4 Special Attacks
bool isTeleportAttacking = false;
int teleportFrameIndex = 0;
int teleportAnimCounter = 0;
bool isMeteroidAttacking = false;
int meteroidFrameIndex = 0;
int meteroidAnimCounter = 0;

// Clone Attack State
ClonePhase clonePhase = CLONE_IDLE;
int cloneSummonFrame = 0;
int cloneSummonCounter = 0;
int cloneRunFrame = 0;
int cloneRunCounter = 0;
int cloneAtkFrame = 0;
int cloneAtkCounter = 0;
int cloneDoneTimer = 0;

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
bool isJumpAttacking = false;
int jumpAttackFrameIndex = 0;
int jumpAttackAnimCounter = 0;
bool isRightArrowPressed = false;
bool isLeftArrowPressed = false;
bool charFacingLeft = false;
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
int dragonFlyImgs[3][3];
int dragonAttackImgs[3][2];
int dragonBackFlyImgs[3][3];
int dragonBackAttackImgs[3][2];
Dragon dragons[5];
int dragonSpawnTimeCounter = 0;
int scoreGain = 0;

// Level 3 NPCs
int npc1Walk[9], npc1Attack[9];
int npc2Walk[9], greenFireImgs[9];
WalkingNPC npcList[MAX_NPCS];
GreenFire fireList[MAX_FIRES];
int npcSpawnTimer = 0;
bool npcHitRegistered[MAX_NPCS] = {false};
bool npcSlashDone = false; // Ensures only 1 hit per slash

/* -------------------- POPUP TEXT LOGIC ------------------- */
const char *killPhrases[5] = {"The enemy is vanquished! Your power rises.",
                              "Victory is yours! Your legend grows.",
                              "Another foe falls! Your strength endures.",
                              "Well fought, warrior! Your power prevails.",
                              "The foe has fallen! Your courage shines."};

#define MAX_POPUPS 5
struct FloatingText {
  float x, y;
  int textIndex;
  int timer;
  bool active;
};
FloatingText popups[MAX_POPUPS];

void spawnPopupText(float x, float y) {
  // Only allow one popup at a time for cleanliness
  for (int i = 0; i < MAX_POPUPS; i++) {
    if (popups[i].active)
      return; // already showing one
  }
  for (int i = 0; i < MAX_POPUPS; i++) {
    if (!popups[i].active) {
      popups[i].active = true;
      popups[i].x = 500; // center X (will be offset in draw)
      popups[i].y = 440; // Fixed Y coordinate
      popups[i].textIndex = rand() % 5;
      popups[i].timer = 90; // ~3s at 30fps
      break;
    }
  }
}

/* -------------------- APPLE SCORE POPUP LOGIC ------------------- */
struct AppleScorePopup {
  float x, y;
  int timer;
  bool active;
};
AppleScorePopup appleScorePopups[15];

void spawnAppleScorePopup(float x, float y) {
  for (int i = 0; i < 15; i++) {
    if (!appleScorePopups[i].active) {
      appleScorePopups[i].active = true;
      appleScorePopups[i].x = x + 10;
      appleScorePopups[i].y = y + 20;
      appleScorePopups[i].timer = 30; // 1 second
      break;
    }
  }
}

/* -------------------- COMBO & SCORE LOGIC ------------------- */
int currentCombo = 0;
int comboDisplayTimer = 0;

struct ScorePopup {
  float x, y;
  int amount;
  int timer;
  bool active;
};
ScorePopup scorePopups[5];

void spawnScorePopup(int amount) {
  for (int i = 0; i < 5; i++) {
    if (!scorePopups[i].active) {
      scorePopups[i].active = true;
      scorePopups[i].amount = amount;
      scorePopups[i].x = SCREEN_W - 130; // Beneath Apple score
      scorePopups[i].y = SCREEN_H - 120; // Lower than apple UI
      scorePopups[i].timer = 45;         // 1.5 sec
      break;
    }
  }
}

/* -------------------- LOGIC FUNCTIONS -------------------- */

void resetGame() {
  if (currentLevel == 2 || currentLevel == 3 || currentLevel == 4) {
    groundY = 83; // Standard ground for levels 2, 3, and 4
  } else {
    groundY = 30;
  }

  isJumpAttacking = false;
  jumpAttackFrameIndex = 0;
  jumpAttackAnimCounter = 0;

  charX = 100;
  charY = groundY;
  if (currentLevel == 1) {
    charHeight = 165;
    charWidth = 165;
    originalHeight = 165;
    bendHeight = 120;
    doubleBendHeight = 80;
    jumpHorizontalSpeed = 4;
  } else {
    charHeight = 125;
    charWidth = 125;
    originalHeight = 125;
    bendHeight = 70;
    doubleBendHeight = 40;
    jumpHorizontalSpeed = 7; // covers 5/7th of original 10 (approx 7)
  }
  isCaveActive = false;
  isCaveStopped = false;
  lives = 5;
  isInvincible = false;
  invincibilityTimer = 0;
  initObstacles();
  // Ghost reset
  for (int i = 0; i < MAX_GHOSTS; i++)
    ghosts[i].active = false;
  ghostSpawnTimer = 0;
  isL3Changed = false;
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
  // Play Level specific music
  if (currentLevel == 1)
    playMusicTrack(TRACK_LEVEL1);
  else if (currentLevel == 2)
    playMusicTrack(TRACK_LEVEL2);
  else if (currentLevel == 3)
    playMusicTrack(TRACK_LEVEL3);
  else if (currentLevel == 4)
    playMusicTrack(TRACK_LEVEL4);

  applesCollected = 0; // Fix: Reset score per-level for correct high scores
  level3StoryIndex = 0;
  cave2StoryIndex = 0;
  if (currentLevel == 3) {
    initGunBar(); // Reset gun bar for Level 3
    initCinder();
  }
  if (currentLevel == 4) {
    loadBossAssets(); // Deferred loading to prevent slow startup
    level4Phase = 1;
    level4ScrollCount = 0;
    bGX = 0;
    initFinalBoss();

    // Reset Level 4 Special Attacks & Clones
    isTeleportAttacking = false;
    isMeteroidAttacking = false;
    clonePhase = CLONE_IDLE;

    // Reset owl companion
    owlCompanion.active = false;

    // Start owl with 10s countdown automatically at Level 4 launch
    owlReady = false;
    owlCooldownTimer = 10;
  }
}

void updateCavePhysics() {
  if (!isGamePaused && gameState == GAME && isCaveActive) {
    if (!isCaveStopped) {
      // Cave moves naturally along with the scrolling background
      caveX -= SCROLL_SPD;
      // When it reaches its specific stopping coordinate
      if (caveX <= 600) {
        caveX = 600;
        isCaveStopped = true;
      }
      return; // Character continues exactly the same way before stopping
    }

    // After cave reaches stopping coordinate, background scroll stops (handled
    // in autoScrollRecursiveWrapper) and character walks forward to the middle
    // of the cave.

    // Stop user input for autoplay sequence
    isRightArrowPressed = false;
    isLeftArrowPressed = false;
    isJumping = false;
    isBending = false;
    isAttacking = false;
    verticalVelocity = 0;
    if (charY > groundY) {
      charY -= 10;
      if (charY < groundY)
        charY = groundY;
    } else if (charY < groundY) {
      charY = groundY;
    }

    int caveMid = (currentLevel == 3 || currentLevel == 2) ? (caveX + 150)
                                                           : (caveX + 200);
    if (charX + charWidth / 2 < caveMid) {
      charX += 5;
      // Animation
      charAnimCounter++;
      if (charAnimCounter >= charAnimSpeed) {
        charAnimCounter = 0;
        charFrameIndex++;
        if (charFrameIndex >= 10)
          charFrameIndex = 1;
      }
    } else {
      // Reached middle of cave, transition to level complete/IQ
      gameState = LEVEL_COMPLETE;
    }
  }
}

// Fire pending action after animation delay
static void firePendingAction() {
  btnAnimActive = false; // Reset flag to prevent repeated execution
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
    } else if (gameState == LEVEL3_INTRO) {
      gameState = LEVEL3_STORY;
      storyTimerCount = 0;
      level3StoryIndex = 0;
      playMusicTrack(TRACK_LEVEL3);
    } else if (gameState == LEVEL3_STORY) {
      level3StoryIndex++;
      storyTimerCount = 0;
      if (level3StoryIndex >= 3) {
        currentLevel = 3;
        resetGame();
      }
    } else if (gameState == LEVEL4_INTRO) {
      gameState = LEVEL4_STORY;
      storyTimerCount = 0;
      level4StoryIndex = 0;
      playMusicTrack(TRACK_LEVEL4);
    } else if (gameState == LEVEL4_STORY) {
      level4StoryIndex++;
      storyTimerCount = 0;
      if (level4StoryIndex >= 3) {
        currentLevel = 4;
        resetGame();
      }
    } else if (gameState == CAVE2_STORY) {
      cave2StoryIndex++;
      storyTimerCount = 0;
      if (cave2StoryIndex >= 3) {
        gameState = NEXT_LEVEL_IQ;
        initCaveState();
        initIQ();
        playMusicTrack(TRACK_CAVE);
      }
    } else if (gameState == FINAL_VICTORY) {
      finalVictoryIndex++;
      if (finalVictoryIndex >= 2) {
        finalStoryIndex = 0;
        gameState = FINAL_STORY;
        playMusicTrack(TRACK_GAME_WON_STORY);
      }
    } else if (gameState == FINAL_STORY) {
      finalStoryIndex++;
      storyTimerCount = 0;
      if (finalStoryIndex >= 11) {
        gameState = MENU;
        playMusicTrack(TRACK_INTRO);
      }
    }
    break;
  case 21: // Skip
    if (gameState == INITIAL_STORY) {
      gameState = LEVEL_SELECTION;
      storyTimerCount = 0;
    } else if (gameState == LEVEL1_INTRO || gameState == LEVEL1_STORY) {
      currentLevel = 1;
      resetGame();
    } else if (gameState == LEVEL2_INTRO || gameState == LEVEL2_STORY) {
      currentLevel = 2;
      resetGame();
    } else if (gameState == LEVEL3_INTRO || gameState == LEVEL3_STORY) {
      currentLevel = 3;
      resetGame();
      playMusicTrack(TRACK_LEVEL3);
    } else if (gameState == CAVE2_STORY) {
      gameState = NEXT_LEVEL_IQ;
      initCaveState();
      initIQ();
      playMusicTrack(TRACK_CAVE);
    } else if (gameState == LEVEL4_INTRO || gameState == LEVEL4_STORY) {
      currentLevel = 4;
      resetGame();
      level4Phase = 1;
      bGX = 0;
      level4ScrollCount = 0;
    } else if (gameState == FINAL_STORY) {
      gameState = MENU;
      playMusicTrack(TRACK_INTRO);
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
    } else if (gameState == LEVEL3_STORY) {
      if (level3StoryIndex > 0) {
        level3StoryIndex--;
        storyTimerCount = 0;
      } else {
        gameState = LEVEL3_INTRO;
        storyTimerCount = 0;
      }
    } else if (gameState == LEVEL4_STORY) {
      if (level4StoryIndex > 0) {
        level4StoryIndex--;
        storyTimerCount = 0;
      } else {
        gameState = LEVEL4_INTRO;
        storyTimerCount = 0;
      }
    } else if (gameState == CAVE2_STORY) {
      if (cave2StoryIndex > 0) {
        cave2StoryIndex--;
        storyTimerCount = 0;
      } else {
        gameState = LEVEL_COMPLETE;
        storyTimerCount = 0;
      }
    } else if (gameState == FINAL_STORY) {
      if (finalStoryIndex > 0) {
        finalStoryIndex--;
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
    currentLevel = 3;
    gameState = LEVEL3_INTRO;
    storyTimerCount = 0;
    break;
  case 33:
    playMusicTrack(TRACK_NONE);
    currentLevel = 4;
    gameState = LEVEL4_INTRO;
    storyTimerCount = 0;
    level4Phase = 1;
    bGX = 0;
    level4ScrollCount = 0;
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
// playClickSound() -> see SoundHandler.h

// Helper: start grow animation for a button, register the pending action
static void startBtnAnim(int context, int code, int action) {
  if (btnAnimActive)
    return; // ignore if already animating
  playClickSound();
  btnAnimActive = true;
  btnAnimContext = context;
  btnAnimCode = code;
  btnAnimTimer = 10; // fire after 10 masterGameLoop ticks (300ms)
  pendingAction = action;
}

void globalTimerLogic() {
  // Animation logic moved to masterGameLoop() for 30ms precision

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
        playMusicTrack(TRACK_STORY);
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
  } else if (gameState == LEVEL3_STORY && !btnAnimActive) {
    storyTimerCount++;
    if (storyTimerCount >= 5) {
      storyTimerCount = 0;
      level3StoryIndex++;
      if (level3StoryIndex >= 3) {
        currentLevel = 3;
        resetGame();
      }
    }
  } else if (gameState == LEVEL4_STORY && !btnAnimActive) {
    storyTimerCount++;
    if (storyTimerCount >= 5) {
      storyTimerCount = 0;
      level4StoryIndex++;
      if (level4StoryIndex >= 3) {
        currentLevel = 4;
        resetGame();
      }
    }
  } else if (gameState == FINAL_STORY && !btnAnimActive) {
    storyTimerCount++;
    if (storyTimerCount >= 5) { // Same speed as initial/other stories
      storyTimerCount = 0;
      finalStoryIndex++;
      if (finalStoryIndex >= 11) {
        gameState = MENU;
        playMusicTrack(TRACK_INTRO);
      }
    }
  }

  if (currentLevel == 4 && gameState == GAME && !isGamePaused) {
    level4HeartTimer++;
    // Spawn heart every 60 seconds
    if (level4HeartTimer >= 60) {
      level4HeartTimer = 0;
      spawnHeartLevel4();
    }

    // Owl cooldown countdown (1 tick per globalTimerLogic call = 1 second)
    if (!owlReady && owlCooldownTimer > 0) {
      owlCooldownTimer--;
      if (owlCooldownTimer <= 0) {
        owlCooldownTimer = 0;
        owlReady = true;
      }
    }
  }
  if (gameState == GAME && !isGamePaused && !levelOneComplete) {
    int transitionTime =
        (currentLevel == 3) ? 60 : 45; // 45s for L1/2, 60s for L3
    gameRunTimeSeconds++;

    if (currentLevel != 4 && gameRunTimeSeconds >= transitionTime &&
        !isCaveActive) {
      isCaveActive = true;
      caveX = 1000;
      isCaveStopped = false;

      for (int i = 0; i < 3; i++) {
        // Stop obstacles spawning
        bombs[i].active = false;
      }
      for (int i = 0; i < 2; i++) {
        bridges[i].active = false;
      }
      for (int i = 0; i < MAX_NPCS; i++) {
        // No longer needed, as they walk naturally left
      }
      for (int i = 0; i < MAX_FIRES; i++) {
        // No additional fire retreat here, handled by 26s cutoff
      }
      for (int i = 0; i < 5; i++) {
        if (dragons[i].active) {
          dragons[i].isVanishing = true;
          dragons[i].vanishingTimer = 30;
        }
      }
      for (int i = 0; i < MAX_GHOSTS; i++) {
        ghosts[i].active = false;
      }
      for (int i = 0; i < 3; i++) {
        skulls[i].active = false;
      }
    }
  }
}

void checkCollision() {
  int obsHitW = 0, obsHitH = 0; // Declarations at top for VS2013 compatibility

  if (gameState != GAME || isGamePaused)
    return;
  if (levelOneComplete && currentLevel == 1)
    return;

  if (currentLevel == 2 || currentLevel == 3) {
    // ---- SHARK/WORM LOGIC ----
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
          if (isInvincible)
            continue; // FIX: Prevent instant death by respecting invincibility
                      // frames

          // Normal Damage
          int damage =
              (currentLevel == 3) ? 2 : ((sharks[i].type == 1) ? 1 : 2);
          lives -= damage;
          if (lives <= 0) {
            gameState = GAME_OVER;
          } else {
            isInvincible = true;
            invincibilityTimer = 60;
          }
        }
      }
    }

    // ---- DRAGON KILLING WITH SHIFT (Level 2 Only) ----
    if (currentLevel == 2) {
      for (int i = 0; i < 5; i++) {
        if (dragons[i].active && !dragons[i].isVanishing) {
          // Dragon center is x+150, y+150 (300x300 size)
          float targetX = (float)(charX + (float)charWidth / 2.0f);
          float targetY = (float)(charY + (float)charHeight / 2.0f);
          float dx = targetX - (dragons[i].x + 150.0f);
          float dy = targetY - (dragons[i].y + 150.0f);
          float dist = (float)sqrt((double)(dx * dx + dy * dy));

          if (dist < 150.0f) {
            if (keyPressed[' ']) { // Space allows killing
              dragons[i].isVanishing = true;
              dragons[i].vanishingTimer = 30;
            } else if (!isInvincible) {
              // Contact damage from dragon if not attacking/shifting
              lives--;
              if (lives <= 0)
                gameState = GAME_OVER;
              else {
                isInvincible = true;
                invincibilityTimer = 60;
              }
            }
          }
        }
      }
    }

    // ---- BOMB COLLISION (Level 2 Only) ----
    if (currentLevel == 2) {
      for (int i = 0; i < 3; i++) {
        if (bombs[i].active) {
          // Generous hitbox: bomb is 50x60
          if (charX + charWidth - 10 > bombs[i].x &&
              charX + 10 < bombs[i].x + 50 &&
              charY + charHeight - 10 > bombs[i].y && charY < bombs[i].y + 60) {
            bombs[i].active = false;
            // Spawn explosion centered on character
            for (int j = 0; j < 3; j++) {
              if (!explosions[j].active) {
                explosions[j].active = true;
                explosions[j].x =
                    (float)(charX + charWidth / 2.0); // center X of char
                explosions[j].y =
                    (float)(charY + charHeight / 2.0); // center Y of char body
                explosions[j].frame = 0;
                explosions[j].timer = 0;
                break;
              }
            }
            if (!isInvincible) {
              isCharFrozen = true;
              freezeTimer = 90;
              lives--;
              if (lives <= 0)
                gameState = GAME_OVER;
              else {
                isInvincible = true;
                invincibilityTimer = 90;
              }
            }
          }
        }
      }
    }

    // ---- LEVEL 3 NPC & FIRE COLLISION ----
    if (currentLevel == 3) {
      // 1. NPC Damage to Player (NPC1 and NPC2 physical contact)
      // (NPC1 melee attack is already handled in updateObstaclePhysics via
      // hasAttacked) We check for basic contact here too just in case
      for (int i = 0; i < MAX_NPCS; i++) {
        if (npcList[i].active) {
          // Player attacking NPC: one damage per slash (fires at mid-swing
          // frame 3)
          if (isAttacking && attackFrameIndex == 3 && !npcSlashDone) {
            // Generous hitbox: NPC can be left or right of player within reach
            float npcCenterX = npcList[i].x + 50.0f;
            float playerReachRight = (float)(charX + charWidth + 160);
            float playerReachLeft = (float)(charX - 60);
            float npcBottom = npcList[i].y;
            float npcTop = npcList[i].y + 120;
            if (npcCenterX > playerReachLeft && npcCenterX < playerReachRight &&
                charY < npcTop && charY + charHeight > npcBottom) {
              npcList[i].life--;
              npcList[i].stunTimer = 30; // 1-second stun
              npcSlashDone = true;       // prevent double-hit in same frame
              if (npcList[i].life <= 0) {
                npcList[i].active = false;

                currentCombo++;
                comboDisplayTimer = 60;
                int bonusPts = (currentCombo - 1) * 10;
                int totalGained = 20 + bonusPts;
                applesCollected += totalGained;
                spawnScorePopup(totalGained);
                playBonusScoreSound();

                spawnPopupText(npcList[i].x, npcList[i].y + 50);
              } else {
                npcList[i].hitFlashTimer = 10;
              }
            }
          }

          // NPC contact damage
          if (!isInvincible && !isFallingSequence &&
              npcList[i].stunTimer <= 0 && charAttackDamagelessTimer <= 0) {
            if (charX + charWidth - 40 > npcList[i].x + 20 &&
                charX + 40 < npcList[i].x + 130 &&
                charY + charHeight > npcList[i].y &&
                charY < npcList[i].y + 150) {
              lives--;
              isInvincible = true;
              invincibilityTimer = 60;
              if (lives <= 0)
                gameState = GAME_OVER;
            }
          }
        }
      }

      // Reset slash state globally outside the NPC active loop
      if (!isAttacking)
        npcSlashDone = false;

      // 2. Green Fire vs Player
      for (int i = 0; i < MAX_FIRES; i++) {
        if (fireList[i].active) {
          if (!isInvincible && !isFallingSequence &&
              charAttackDamagelessTimer <= 0) {
            // Generous fire collision box for reliable hits
            if (charX + charWidth - 15 > fireList[i].x &&
                charX + 15 < fireList[i].x + 100 &&
                charY + charHeight > fireList[i].y &&
                charY < fireList[i].y + 100) {
              fireList[i].active = false;
              lives--;
              isInvincible = true;
              invincibilityTimer = 60;
              if (lives <= 0)
                gameState = GAME_OVER;
            }
          }
        }
      }

      // 3. Skull Collision
      for (int i = 0; i < 3; i++) {
        if (skulls[i].active && !skulls[i].isVanishing) {
          // Player hitting skull
          if (isAttacking && attackFrameIndex >= 1 && attackFrameIndex <= 6 &&
              !npcSlashDone) {
            float skullCx = skulls[i].x + 30.0f;
            float playerReachRight = (float)(charX + charWidth + 160);
            float playerReachLeft = (float)(charX - 60);
            float skullTop = skulls[i].y + 60.0f;
            float skullBottom = skulls[i].y;

            if (skullCx > playerReachLeft && skullCx < playerReachRight &&
                charY < skullTop && charY + charHeight > skullBottom) {
              skulls[i].isVanishing = true;
              skulls[i].vanishingTimer = 10;
              npcSlashDone = true;

              skullsKilled++;
              if (skullsKilled >= 5) {
                lives++;
                skullsKilled -= 5;
                spawnPopupText(charX, charY + 100);
              }
              currentCombo++;
              comboDisplayTimer = 60;
              applesCollected += 10;
              spawnScorePopup(10);
              playBonusScoreSound();
              continue;
            }
          }

          // Skull touching player
          if (!isInvincible && !isFallingSequence &&
              charAttackDamagelessTimer <= 0) {
            float charCx = charX + charWidth / 2.0f;
            float charCy = charY + charHeight / 2.0f;
            float sCx = skulls[i].x + 30.0f;
            float sCy = skulls[i].y + 30.0f;
            float dx = charCx - sCx;
            float dy = charCy - sCy;
            float dist = (float)sqrt((double)(dx * dx + dy * dy));

            if (dist < 35.0f) {
              skulls[i].isVanishing = true;
              skulls[i].vanishingTimer = 10;

              lives--;
              if (lives <= 0) {
                gameState = GAME_OVER;
              } else {
                isFallingSequence = true;
                isCharFrozen = true;
                fallY = (float)charY;
                fallVel = -2;
                isInvincible = true;
                invincibilityTimer = 120;
              }
            }
          }
        }
      }
    }
  }

  // Standard Obstacle Collision (Levels 1, 2, 3)
  if (currentLevel == 4) {
    handleBossHit();
    // Fix: Reset slash state for Level 4 Boss to allow subsequent attacks
    if (!isAttacking)
      npcSlashDone = false;
    return;
  }
  for (int i = 0; i < 3; i++) {
    if (obstacles[i].active) {
      if (obstacles[i].type == 1) {
        obsHitW = 150; // Visual width
        obsHitH = 150; // Visual height
      } else if (obstacles[i].type == 2) {
        obsHitW = 150; // Visual width
        obsHitH = 100; // Visual height
      } else {
        obsHitW = 120; // Visual width
        obsHitH = 120; // Visual height
      }

      if (charX + charWidth - 85 > obstacles[i].x + 20 &&
          charX + 85 < obstacles[i].x + obsHitW &&
          charY + charHeight - 50 > obstacles[i].y &&
          charY < obstacles[i].y + obsHitH) {
        if (!isInvincible && charAttackDamagelessTimer <= 0) {
          lives--;
          if (lives <= 0) {
            gameState = GAME_OVER;
          } else {
            isInvincible = true;
            invincibilityTimer = 60;
          }
        }
      }
    }
  }
}

/* -------------------- WRAPPERS -------------------- */
void autoScrollRecursiveWrapper() {
  if (!isGamePaused && gameState == GAME && (!isCaveActive || !isCaveStopped)) {
    bool shouldScroll = false;
    if (currentLevel == 1 && !levelOneComplete)
      shouldScroll = true;
    else if (currentLevel == 2 || currentLevel == 3)
      shouldScroll = true;
    else if (currentLevel == 4 && level4Phase == 1)
      shouldScroll = true;

    if (shouldScroll) {
      if (currentLevel == 4) {
        bGX -= SCROLL_SPD;
        if (bGX <= -SCREEN_W) {
          bGX = -SCREEN_W;        // Stop background
          if (level4Phase == 1) { // Only initialize once
            level4Phase = 2;      // Transition to boss appearance
            initFinalBoss();
            boss4Obj.active = true;
            boss4Obj.x = SCREEN_W + 50;
          }
        }
      } else {
        autoScrollRecursive();
      }
    }
  }
}

void updateObstaclePhysicsWrapper() {
  if (!isGamePaused && gameState == GAME && currentLevel != 4 &&
      (currentLevel == 2 || currentLevel == 3 || !levelOneComplete)) {
    updateObstaclePhysics();
  }
}

void updateCharacterMovementWrapper() {
  if (!isGamePaused && gameState == GAME) {
    updateCharacterMovement();
    if (isInvincible) {
      invincibilityTimer--;
      if (invincibilityTimer <= 0 && !isHaloActive) {
        isInvincible = false;
      }
    }
  }
}

void updateApplesWrapper() {
  if (!isGamePaused && gameState == GAME && currentLevel != 4 &&
      (currentLevel == 2 || currentLevel == 3 || !levelOneComplete)) {
    updateApplePhysics();
    checkAppleCollision();
    updateMagnetPhysics();
  }
}

void updateWeatherWrapper() {
  if (!isGamePaused && gameState == GAME && currentLevel != 4 &&
      (currentLevel == 2 || currentLevel == 3 || !levelOneComplete)) {
    updateWeatherPhysics();
  }
}

void updateHeartWrapper() {
  if (!isGamePaused && gameState == GAME) {
    updateHeartLogic();
  }
}

void updateLevel4Logic() {
  if (currentLevel != 4 || gameState != GAME || isGamePaused)
    return;

  if (level4Phase == 1) {
    // Background scroll handles shifting to phase 2
  } else if (level4Phase == 2) {
    // Boss comes from right side to x=750
    if (boss4Obj.x > 750) {
      boss4Obj.x -= 4; // Faster Walking speed (was 2)
      updateFinalBossLogic();
    } else {
      boss4Obj.x = 750;
      level4Phase = 3;         // resting phase
      boss4Obj.frameIndex = 0; // Reset animation frame for standing
      boss4Obj.animCounter = 0;
    }
  } else if (level4Phase == 3 || level4Phase == 4) {
    updateFinalBossLogic();
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

  int maxFrames = 6;
  if (selectedCharacter == 0)
    maxFrames = 4;
  else if (selectedCharacter == 2)
    maxFrames = 3;

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
    if (currentLevel == 3) {
      isL3Changed = !isL3Changed; // Toggle background on fall
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

  bool currentSpace = (keyPressed[' '] != 0);

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

      if ((currentLevel == 2 || currentLevel == 3) && gameState == GAME) {
        for (int i = 0; i < 2; i++) {
          // If shark is on screen and not already dying, kill it
          float sx = sharks[i].x;
          float cx = (float)charX;
          float dist = (sx > cx) ? (sx - cx) : (cx - sx);
          float attackDist = 250.0f;
          if (sharks[i].active && !sharks[i].isDying && dist < attackDist) {
            sharks[i].isDying = true;
            sharks[i].dyingVel = 5.0f; // Pop up
            spawnPopupText(sharks[i].x, sharks[i].y + 50);

            // AWARD SCORE
            int scoreGain = 0;
            if (currentLevel == 3) {
              scoreGain = 100; // extra points!
            } else {
              scoreGain = (sharks[i].type == 1) ? 10 : 20;
            }

            currentCombo++;
            comboDisplayTimer = 60;
            int bonusPts = (currentCombo - 1) * 10;
            int totalGained = scoreGain + bonusPts;
            applesCollected += totalGained;
            spawnScorePopup(totalGained);
            playBonusScoreSound();
          }
        }
      }
    } else {
      // ** SINGLE TAP detected ** -> RETREAT any on-screen shark
      spaceDoubleTimer = 15;
      singleSpaceActiveTimer = 15;

      // Wait a tiny bit or just do it? We do it immediately but double tap can
      // override.
      if ((currentLevel == 2 || currentLevel == 3) && gameState == GAME) {
        for (int i = 0; i < 2; i++) {
          float sx = sharks[i].x;
          float cx = (float)charX;
          float dist = (sx > cx) ? (sx - cx) : (cx - sx);
          float attackDist = 250.0f;
          if (sharks[i].active && !sharks[i].isDying && dist < attackDist) {
            // RETREAT: Send back to bottom right
            sharks[i].isRetreating = true;
            sharks[i].jumpState = 0;
            sharks[i].velocityY = 0;
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

  static bool prevShiftKey = false;
  bool currentShiftKey = (GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
  if (currentShiftKey && !prevShiftKey) {
    if (gameState == GAME && (currentLevel == 3 || currentLevel == 4) &&
        hasClaimedGun) {
      if (gunBarAvailable && !gunBarEquipped && !isGunAttacking) {
        gunBarEquipped = true;
        gunBarAnimTimer = 15;
        gunBarAvailable = false;

        isGunAttacking = true;
        gunAttackFrameIndex = 0;
        gunAttackAnimCounter = 0;
        gunAttackShotsRemaining = 2;
        charAttackDamagelessTimer = 30; // 1-second stun guard
      }
    }
  }
  prevShiftKey = currentShiftKey;


  autoScrollRecursiveWrapper();
  updateLevel4Logic();
  // Trigger FINAL_VICTORY when boss death sequence completes
  if (currentLevel == 4 && gameState == GAME && level4Phase == 5) {
    finalVictoryIndex = 0;
    gameState = FINAL_VICTORY;
    playMusicTrack(TRACK_FINAL_VICTORY);
  }
  updateObstaclePhysicsWrapper();
  checkCollision();
  updateCharacterMovementWrapper();
  updateFallSequence();
  updateApplesWrapper();
  updateWeatherWrapper();
  updateHeartWrapper();
  updateCavePhysics();
  updateWizardWrapper();
  updateRunAnimation();
  if (charAttackDamagelessTimer > 0)
    charAttackDamagelessTimer--;
  updateDragonPhysics();
  if (currentLevel == 3 && gameState == GAME && !isGamePaused) {
    updateCinderPhysics();
    checkCinderHit();
  }
  updateShardLogic();
  updateAttackAnimation();
  updateGunAttackAnimation();
  updateFireballs();
  updateCloneAttackAnimation();
  updateTeleportAttackAnimation();
  updateMeteroidAttackAnimation();
  updateOwl();
  updateOwlCollisions();

  // Save high score when game ends (GAME_OVER)
  static bool gameOverScoreSaved = false;
  if (gameState == GAME_OVER && !gameOverScoreSaved) {
    playMusicTrack(TRACK_GAMEOVER);
    updateHighScore(currentLevel, applesCollected);
    gameOverScoreSaved = true;
  }
  if (gameState == GAME) {
    gameOverScoreSaved = false; // Reset flag when back in game
  }
  if (isCharFrozen) {
    freezeTimer--;
    if (freezeTimer <= 0) {
      isCharFrozen = false;
    }
  }

  if (btnAnimActive) {
    btnAnimTimer--;
    if (btnAnimTimer <= 0) {
      firePendingAction();
    }
  }

  // Combo Reset logic: If you become invincible this frame (meaning you took
  // damage), lose combo!
  static bool wasInvincibleThisFrame = false;
  if (isInvincible && !wasInvincibleThisFrame) {
    currentCombo = 0;
  }
  wasInvincibleThisFrame = isInvincible;

  if (comboDisplayTimer > 0)
    comboDisplayTimer--;

  // Update Score Popups
  for (int i = 0; i < 5; i++) {
    if (scorePopups[i].active) {
      scorePopups[i].y -= 1.0f; // Drift downwards like dropped loot
      scorePopups[i].timer--;
      if (scorePopups[i].timer <= 0)
        scorePopups[i].active = false;
    }
  }

  // Update Apple Popups
  for (int i = 0; i < 15; i++) {
    if (appleScorePopups[i].active) {
      appleScorePopups[i].y += 1.5f; // Drift upwards
      appleScorePopups[i].timer--;
      if (appleScorePopups[i].timer <= 0)
        appleScorePopups[i].active = false;
    }
  }

  // Update Popups
  for (int i = 0; i < MAX_POPUPS; i++) {
    if (popups[i].active) {
      popups[i].timer--;
      if (popups[i].timer <= 0) {
        popups[i].active = false;
      }
    }
  }

  // Update Damage Popups (Combat)
  for (int i = 0; i < 20; i++) {
    if (damagePopups[i].active) {
      damagePopups[i].y += 1.0f; // Drift upwards
      damagePopups[i].timer--;
      if (damagePopups[i].timer <= 0) {
        damagePopups[i].active = false;
      }
    }
  }

  updateGunBar(); // Gun bar cooldown/equip logic
}

/* -------------------- DRAW FUNCTION -------------------- */
void iDraw() {
  iClear();
  iSetColor(255, 255, 255); // Global color reset to prevent "orange box" ghosts

  bool shaking = (globalScreenShakeX != 0.0f || globalScreenShakeY != 0.0f);
  if (shaking) {
    glPushMatrix();
    glTranslatef(globalScreenShakeX, globalScreenShakeY, 0.0f);
  }

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
  } else if (gameState == LEVEL2_STORY) {
    drawLevelTwoStory();
  } else if (gameState == LEVEL3_INTRO) {
    drawLevel3Intro();
  } else if (gameState == LEVEL3_STORY) {
    drawLevelThreeStory();
  } else if (gameState == LEVEL4_INTRO) {
    drawLevel4Intro();
  } else if (gameState == LEVEL4_STORY) {
    drawLevelFourStory();
  } else if (gameState == LEVEL1_STORY) {
    drawLevel1Story();
  } else if (gameState == CAVE2_STORY) {
    drawCaveTwoStory();
  } else if (gameState == FINAL_VICTORY) {
    drawFinalVictory();
  } else if (gameState == FINAL_STORY) {
    drawFinalStory();
  } else if (gameState == GAME || gameState == LEVEL_COMPLETE ||
             gameState == GAME_OVER) {
    drawBackground();
    if (currentLevel != 4) {
      drawApples();
      drawMagnet();
      drawObstacles();
      drawDragons();
      if (currentLevel == 3) {
        drawCinder();
      }
    } else {
      drawBoss();
    }

    if (isCaveActive) {
      if (currentLevel == 3) {
        iShowImage(caveX, 90, 300, 300, caveImgL3);
      } else if (currentLevel == 2) {
        iShowImage(caveX, 90, 300, 300, caveImgL2);
      } else {
        iShowImage(caveX, 0, 400, 400, caveImg);
      }
    }

    if (!(currentLevel == 4 && level4Phase == 5)) {
      if (!isTeleportAttacking && !isMeteroidAttacking) {
        drawCharacter();
      }
      drawTeleportAttack(); // Teleport animation handles its own state check
      drawMeteroidAttack(); // Meteroid animation handles its own state check
      drawCloneAttack(); // Clone animation drawn over character (Level 4 only)
      drawCharFireFlame(); // Fireball on-fire flame: drawn AFTER char so it is
                           // in front
      drawOwl();
    }
    drawFireballs();
    drawExplosions();
    if (currentLevel != 4) {
      drawWeatherEffects();
    }
    drawLivesUI();
    if (currentLevel != 4) {
      drawAppleScoreUI();
    }
    drawShardElements();
    drawTimer();

    // --- Level 4 Owl Power-up UI (bottom-left) ---
    if (currentLevel == 4 && hasCompanion && gameState == GAME) {
      // Draw owl icon (visible always, no grey box)
      iSetColor(255, 255, 255);
      iShowImage(10, 10, 120, 120, owlImg);

      if (!owlReady) {
        // Only draw the timer on top of the power up image
        char cdStr[8];
        sprintf_s(cdStr, sizeof(cdStr), "%d", owlCooldownTimer);
        iSetColor(255, 220, 0);
        iText(64, 62, (char *)cdStr, (void *)GLUT_BITMAP_TIMES_ROMAN_24);
      }
    }

    // Draw Combo Text
    if (currentCombo > 1 && comboDisplayTimer > 0) {
      char comboText[50];
      sprintf_s(comboText, sizeof(comboText), "COMBO x%d!", currentCombo);

      // Pulse effect
      int r = (comboDisplayTimer % 20 < 10) ? 255 : 255;
      int g = (comboDisplayTimer % 20 < 10) ? 140 : 215;
      int b = (comboDisplayTimer % 20 < 10) ? 0 : 0;

      iSetColor(255, 255, 255);
      iText(52, 348, (char *)comboText, (void *)GLUT_BITMAP_TIMES_ROMAN_24);
      iSetColor(r, g, b);
      iText(50, 350, (char *)comboText, (void *)GLUT_BITMAP_TIMES_ROMAN_24);
    }

    // Draw Score Popups
    for (int i = 0; i < 5; i++) {
      if (scorePopups[i].active) {
        char scoreStr[20];
        sprintf_s(scoreStr, sizeof(scoreStr), "+%d", scorePopups[i].amount);

        iText(scorePopups[i].x + 1, scorePopups[i].y - 1, (char *)scoreStr,
              (void *)GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(50, 255, 50); // Neon Green
        iText(scorePopups[i].x, scorePopups[i].y, (char *)scoreStr,
              (void *)GLUT_BITMAP_TIMES_ROMAN_24);
      }
    }

    // Draw Apple Popups
    for (int i = 0; i < 15; i++) {
      if (appleScorePopups[i].active) {
        iSetColor(255, 255, 0); // Yellow
        iText(appleScorePopups[i].x, appleScorePopups[i].y, (char *)"+1",
              (void *)GLUT_BITMAP_HELVETICA_18);
      }
    }

    // Draw Damage Popups
    for (int i = 0; i < 20; i++) {
      if (damagePopups[i].active) {
        char dmgStr[20];
        if (damagePopups[i].isCrit) {
          sprintf_s(dmgStr, sizeof(dmgStr), "CRIT! -%d",
                    damagePopups[i].amount);
        } else {
          sprintf_s(dmgStr, sizeof(dmgStr), "-%d", damagePopups[i].amount);
        }

        // Shadow outline for readability
        iSetColor(0, 0, 0);
        iText(damagePopups[i].x + 1, damagePopups[i].y - 1, (char *)dmgStr,
              (void *)GLUT_BITMAP_TIMES_ROMAN_24);
        iText(damagePopups[i].x - 1, damagePopups[i].y + 1, (char *)dmgStr,
              (void *)GLUT_BITMAP_TIMES_ROMAN_24);

        // Main color: Red or Bright Orange for crits
        if (damagePopups[i].isCrit) {
          iSetColor(255, 140, 0); // Orange
        } else {
          iSetColor(255, 50, 50); // Red
        }
        iText(damagePopups[i].x, damagePopups[i].y, (char *)dmgStr,
              (void *)GLUT_BITMAP_TIMES_ROMAN_24);
      }
    }

    // Draw Popups
    for (int i = 0; i < MAX_POPUPS; i++) {
      if (popups[i].active) {
        const char *phrase = killPhrases[popups[i].textIndex];
        // Calculate approximate text width for centering
        int textLen = 0;
        while (phrase[textLen])
          textLen++;
        float textX = 500.0f - (textLen * 6.0f); // Center text on screen

        // Shadow layer for depth
        iSetColor(80, 50, 20);
        iText(textX + 1, popups[i].y - 1, (char *)phrase,
              (void *)GLUT_BITMAP_TIMES_ROMAN_24);

        // Main text - muted amber tone
        iSetColor(180, 140, 60);
        iText(textX, popups[i].y, (char *)phrase,
              (void *)GLUT_BITMAP_TIMES_ROMAN_24);
      }
    }

    if (gameState == GAME) {
      drawPauseMenu();
      drawGunBar(); // Gun bar (Level 3 only, if gun was earned)
    } else if (gameState == GAME_OVER) {
      drawGameOverMenu();
    } else if (gameState == LEVEL_COMPLETE) {
      drawLevelComplete();
    }
  } else if (gameState == NEXT_LEVEL_IQ) {
    drawCave();
    drawPauseMenu();
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
    // Draw background
    iShowImage(0, 0, screenWidth, screenHeight, mainMenuBG);

    // Panel Background
    iSetColor(0, 0, 0);
    iFilledRectangle(screenWidth / 2.0 - 320.0, 100.0, 640.0, 420.0);
    iSetColor(180, 140, 60);
    iRectangle(screenWidth / 2.0 - 320.0, 100.0, 640.0, 420.0);
    iRectangle(screenWidth / 2.0 - 318.0, 102.0, 636.0, 416.0);

    // Title
    iSetColor(255, 220, 80);
    iText(screenWidth / 2.0 - 110.0, 480.0, (char *)"HIGH SCORES",
          (void *)GLUT_BITMAP_TIMES_ROMAN_24);

    // Subtitle
    iSetColor(200, 200, 200);
    iText(screenWidth / 2.0 - 160.0, 450.0,
          (char *)"Best Food Collected per Level",
          (void *)GLUT_BITMAP_HELVETICA_18);

    // Separator line
    iSetColor(180, 140, 60);
    iLine(screenWidth / 2.0 - 280.0, 440.0, screenWidth / 2.0 + 280.0, 440.0);

    // Level entries
    const char *levelNames[3] = {"Level 1  -  Mistveil Forest",
                                 "Level 2  -  Cinder WindPath",
                                 "Level 3  -  Nightshade Manor"};
    int entryY[3] = {360, 300, 240}; // Shifted down slightly to center 3 rows
    char scoreText[64];
    for (int i = 0; i < 3; i++) {
      iSetColor(255, 220, 150);
      iText(screenWidth / 2.0 - 240.0, (double)entryY[i], (char *)levelNames[i],
            (void *)GLUT_BITMAP_HELVETICA_18);
      if (highScores[i] > 0) {
        sprintf_s(scoreText, sizeof(scoreText), "%d food", highScores[i]);
        iSetColor(100, 255, 180);
      } else {
        sprintf_s(scoreText, sizeof(scoreText), "Not played yet");
        iSetColor(140, 140, 140);
      }
      iText(screenWidth / 2.0 + 60.0, (double)entryY[i], (char *)scoreText,
            (void *)GLUT_BITMAP_HELVETICA_18);
    }

    // Footer
    iSetColor(180, 140, 60);
    iLine(screenWidth / 2.0 - 280.0, 165.0, screenWidth / 2.0 + 280.0, 165.0);
    iSetColor(200, 200, 200);
    iText(screenWidth / 2.0 - 120.0, 130.0,
          (char *)"Press 'B' to Return to Menu",
          (void *)GLUT_BITMAP_HELVETICA_18);

  } else if (gameState == LEVEL_NOT_READY) {
    iShowImage(0, 0, screenWidth, screenHeight, levelSelectionBG);
    iSetColor(255, 255, 255);
    iText(screenWidth / 2.0 - 180.0, screenHeight / 2.0,
          (char *)"LEVEL GAMEPLAY NOT READY YET",
          (void *)GLUT_BITMAP_TIMES_ROMAN_24);
    iSetColor(200, 200, 200);
    iText(screenWidth / 2.0 - 150.0, screenHeight / 2.0 - 50.0,
          (char *)"Press 'B' to Return to Selection",
          (void *)GLUT_BITMAP_HELVETICA_18);
  }

  if (shaking) {
    glPopMatrix();
  }

  // --- Screen Fade Logic ---
  int currentStoryIndexForFade = -1;
  if (gameState == INITIAL_STORY)
    currentStoryIndexForFade = initialStoryIndex;
  else if (gameState == LEVEL1_STORY)
    currentStoryIndexForFade = level1StoryIndex;
  else if (gameState == LEVEL2_STORY)
    currentStoryIndexForFade = level2StoryIndex;
  else if (gameState == LEVEL3_STORY)
    currentStoryIndexForFade = level3StoryIndex;
  else if (gameState == LEVEL4_STORY)
    currentStoryIndexForFade = level4StoryIndex;
  else if (gameState == CAVE2_STORY)
    currentStoryIndexForFade = cave2StoryIndex;
  else if (gameState == FINAL_VICTORY)
    currentStoryIndexForFade = finalVictoryIndex;
  else if (gameState == FINAL_STORY)
    currentStoryIndexForFade = finalStoryIndex;

  if (gameState != prevGameStateForFade ||
      (currentStoryIndexForFade != -1 &&
       currentStoryIndexForFade != prevStoryIndexForFade)) {
    currentFadeAlpha = 1.0f; // Start fully black on transition
    prevGameStateForFade = gameState;
    prevStoryIndexForFade = currentStoryIndexForFade;
  }

  if (currentFadeAlpha > 0.0f) {
    currentFadeAlpha -= 0.035f; // Fade in over approx 1-second
    if (currentFadeAlpha < 0.0f) {
      currentFadeAlpha = 0.0f;
    }
  }

  iDrawFadeOverlay(currentFadeAlpha);
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
      // Owl trigger in Level 4
      if (currentLevel == 4 && hasCompanion && owlReady &&
          !owlCompanion.active) {
        if (mx >= 10 && mx <= 130 && my >= 10 && my <= 130) {
          initOwl();
          playClickSound();
          return;
        }
      }
      if (handleGunBarClick(mx, my)) {
        if (!isGunAttacking) {
          isGunAttacking = true;
          gunAttackFrameIndex = 0;
          gunAttackAnimCounter = 0;
          gunAttackShotsRemaining = 2;
          if (currentLevel == 3 || currentLevel == 4)
            charAttackDamagelessTimer = 30; // 1-second stun guard
        }
      }
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
        playMusicTrack(TRACK_STORY);
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
      bool wasMusicOn = isMusicOn;
      int result = handleSettingsMenuMouse(mx, my);

      if (wasMusicOn != isMusicOn) {
        MusicTrack toPlay = currentTrack;
        currentTrack = TRACK_NONE; // Force restart of the track logic
        playMusicTrack(toPlay);
      }

      playClickSound(); // always play on any settings button click
      if (result != -1) {
        gameState = (GameState)result;
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
    } else if (gameState == LEVEL1_STORY || gameState == LEVEL2_STORY ||
               gameState == LEVEL3_STORY || gameState == LEVEL4_STORY ||
               gameState == CAVE2_STORY) {
      int action = handleStoryMouse(mx, my);
      if (action == 1) { // Next
        startBtnAnim(2, 20, 20);
      } else if (action == 2) { // Skip
        startBtnAnim(2, 21, 21);
      } else if (action == 3) { // Previous
      } else if (action == 3) { // Previous
        startBtnAnim(2, 22, 22);
      }
    } else if (gameState == GAME_OVER) {
      int action = handleGameOverMouse(mx, my);
      if (action != 0)
        playClickSound();
      if (action == 1) { // Restart
        resetGame();
      } else if (action == 2) { // Home
        gameState = MENU;
        playMusicTrack(TRACK_INTRO);
      } else if (action == 3) { // Settings
        previousState = (int)gameState;
        gameState = SETTINGS;
      }
    } else if (gameState == LEVEL_COMPLETE) {
      int action = handleLevelCompleteMouse(mx, my);
      if (action != 0)
        playClickSound();
      if (action == 1) {
        playMusicTrack(TRACK_NONE);
        if (currentLevel == 2) {
          gameState = CAVE2_STORY;
          cave2StoryIndex = 0;
          storyTimerCount = 0;
          playMusicTrack(
              TRACK_CAVE); // Start cave music during its story frames
        } else if (currentLevel == 3) {
          gameState = NEXT_LEVEL_IQ;
          initCaveState();
          initIQ();
          playMusicTrack(TRACK_CAVE);
          storyTimerCount = 0;
        } else {
          gameState = NEXT_LEVEL_IQ;
          initCaveState();
          initIQ();
          playMusicTrack(TRACK_CAVE);
        }
      } else if (action == 2)
        resetGame();
      else if (action == 3) {
        gameState = MENU;
        playMusicTrack(TRACK_INTRO);
      }
    } else if (gameState == NEXT_LEVEL_IQ) {
      int pauseAction = handlePauseMenuMouse(mx, my);
      if (pauseAction != 0) {
        playClickSound();
        if (pauseAction == 2) {
          resetGame();
        } else if (pauseAction == 3) {
          previousState = (int)gameState;
          gameState = SETTINGS;
        } else if (pauseAction == 4) {
          gameState = LEVEL_SELECTION;
          playMusicTrack(TRACK_STORY);
        }
        return;
      }
      if (isGamePaused)
        return;

      if (isLevelTransitioning) {
        int result = handleCaveTransitionClick(mx, my);
        if (result == 1) {
          playClickSound();
          playMusicTrack(TRACK_STORY);
          if (currentLevel == 1) {
            gameState = LEVEL_SELECTION;
            currentLevel = 2; // Move to Next
          } else if (currentLevel == 2) {
            gameState = LEVEL_SELECTION;
            currentLevel = 3; // Move to Next
          } else if (currentLevel == 3) {
            gameState = LEVEL_SELECTION;
            currentLevel = 4; // Move to Next
            level4Phase = 1;
            bGX = 0;
            level4ScrollCount = 0;
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
          // Correct answer: Show item/companion splash screen
          if (currentLevel == 1)
            hasClaimedShard = true;
          else if (currentLevel == 2)
            hasClaimedGun = true;
          else if (currentLevel == 3)
            hasCompanion = true; // Companion (Aizen) claimed!
          isLevelTransitioning = true;
          transitionPhase = 2; // Show shard/gun/companion explain screen
        } else {
          // Incorrect answer: Skip Explanation, proceed to next level intro
          playMusicTrack(TRACK_STORY);
          if (currentLevel == 1) {
            gameState = LEVEL_SELECTION;
            currentLevel = 2;
          } else if (currentLevel == 2) {
            gameState = LEVEL_SELECTION;
            currentLevel = 3;
          } else if (currentLevel == 3) {
            gameState = LEVEL_SELECTION;
            currentLevel = 4;
            level4Phase = 1;
            bGX = 0;
            level4ScrollCount = 0;
          }
          storyTimerCount = 0;
        }
        // Do NOT switch to LEVEL2_INTRO yet for correct answer. Wait for
        // transitions.
      }
    } else if (gameState == FINAL_VICTORY) {
      // Animated Next button — uses same grow anim as story buttons (code 20)
      if (handleFinalVictoryClick(mx, my)) {
        startBtnAnim(2, 20, 20);
      }
    } else if (gameState == FINAL_STORY) {
      int action = handleStoryMouse(mx, my);
      if (action == 1) {
        startBtnAnim(2, 20, 20); // Next
      } else if (action == 2) {
        startBtnAnim(2, 21, 21); // Skip
      } else if (action == 3) {
        startBtnAnim(2, 22, 22); // Previous
      }
    }
  }
}

void iKeyboard(unsigned char key) {
  if (key == 27 && (gameState == GAME || gameState == NEXT_LEVEL_IQ)) {
    isGamePaused = !isGamePaused;
  }
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
  if (gameState == HIGHSCORES && (key == 'b' || key == 'B')) {
    gameState = MENU;
  }
  if (gameState == LEVEL_NOT_READY && (key == 'b' || key == 'B')) {
    gameState = LEVEL_SELECTION;
  }

  // Storytelling Shortcuts: Space for Skip
  if (!btnAnimActive && key == ' ') {
    if (gameState == INITIAL_STORY || gameState == LEVEL1_INTRO ||
        gameState == LEVEL1_STORY || gameState == LEVEL2_INTRO ||
        gameState == LEVEL2_STORY || gameState == LEVEL3_INTRO ||
        gameState == LEVEL3_STORY || gameState == LEVEL4_STORY ||
        gameState == CAVE2_STORY || gameState == LEVEL4_INTRO ||
        gameState == FINAL_STORY) {
      startBtnAnim(2, 21, 21); // Skip
    }
  }

  if (gameState == GAME && key == ' ') {
    if (!isAttacking) {
      isAttacking = true;
      attackFrameIndex = 0;
      attackAnimCounter = 0;
      if (currentLevel == 3 || currentLevel == 4)
        charAttackDamagelessTimer = 30; // 1-second stun guard
    }
  }

  // Clone Attack: 'C' in Level 4
  if (gameState == GAME && currentLevel == 4 && (key == 'c' || key == 'C')) {
    triggerCloneAttack();
  }

  // Jump Attack: 'D' in Level 4
  if (gameState == GAME && currentLevel == 4 && (key == 'd' || key == 'D')) {
    if (!isJumping && !isAttacking && !isGunAttacking) {
      isJumping = true;
      isJumpAttacking = true;
      jumpAttackFrameIndex = 0;
      jumpAttackAnimCounter = 0;
      verticalVelocity = 32; // Standard jump speed
      jumpHorizontalSpeed = 10;
      currentJumpDirection =
          (isSpecialKeyPressed(GLUT_KEY_LEFT) || isLeftArrowPressed) ? -1 : 1;
    }
  }

  // Teleport Attack: 'B' in Level 4
  if (gameState == GAME && currentLevel == 4 && (key == 'b' || key == 'B')) {
    triggerTeleportAttack();
  }

  // Meteoroid Attack: 'E' in Level 4
  if (gameState == GAME && currentLevel == 4 && (key == 'e' || key == 'E')) {
    triggerMeteroidAttack();
  }

  // Populate keyPressed array
  keyPressed[key] = 1;
}

void iKeyboardUp(unsigned char key) { keyPressed[key] = 0; }

void iSpecialKeyboard(unsigned char key) {
  if (btnAnimActive)
    return;

  // Storytelling Shortcuts: Right Arrow for Next, Left Arrow for Previous
  if (gameState == INITIAL_STORY || gameState == LEVEL1_INTRO ||
      gameState == LEVEL1_STORY || gameState == LEVEL2_INTRO ||
      gameState == LEVEL2_STORY || gameState == LEVEL3_INTRO ||
      gameState == LEVEL3_STORY || gameState == LEVEL4_STORY ||
      gameState == CAVE2_STORY || gameState == LEVEL4_INTRO ||
      gameState == FINAL_STORY) {
    if (isGamePaused)
      return;
    if (key == GLUT_KEY_RIGHT) {
      startBtnAnim(2, 20, 20); // Next
    } else if (key == GLUT_KEY_LEFT) {
      startBtnAnim(2, 22, 22); // Previous
    }
  }

  // FINAL_VICTORY: right arrow advances to next frame (animated)
  if (gameState == FINAL_VICTORY && !btnAnimActive && key == GLUT_KEY_RIGHT) {
    startBtnAnim(2, 20, 20);
  }

  // Level Complete and Cave next button shortcuts
  if (gameState == LEVEL_COMPLETE ||
      (gameState == NEXT_LEVEL_IQ && isLevelTransitioning)) {
    if (key == GLUT_KEY_RIGHT) {
      if (gameState == LEVEL_COMPLETE) {
        playClickSound();
        playMusicTrack(TRACK_NONE);
        if (currentLevel == 2) {
          gameState = CAVE2_STORY;
          cave2StoryIndex = 0;
          storyTimerCount = 0;
          playMusicTrack(TRACK_CAVE);
        } else if (currentLevel == 3) {
          gameState = NEXT_LEVEL_IQ;
          initCaveState();
          initIQ();
          playMusicTrack(TRACK_CAVE);
          storyTimerCount = 0;
        } else {
          gameState = NEXT_LEVEL_IQ;
          initCaveState();
          initIQ();
          playMusicTrack(TRACK_CAVE);
        }
      } else if (gameState == NEXT_LEVEL_IQ && isLevelTransitioning) {
        playClickSound();
        if (transitionPhase == 1) {
          // Entry cave image: go into cave scene (same as mouse click on phase
          // 1)
          isLevelTransitioning = false;
          transitionPhase = 0;
          playMusicTrack(TRACK_CAVE);
        } else if (transitionPhase == 2) {
          // Explain screen: proceed to next level intro (mirrors mouse handler
          // exactly)
          playMusicTrack(TRACK_NONE);
          if (currentLevel == 1) {
            gameState = LEVEL2_INTRO;
            currentLevel = 2;
          } else if (currentLevel == 2) {
            gameState = LEVEL3_INTRO;
            currentLevel = 3;
          } else if (currentLevel == 3) {
            gameState = LEVEL4_INTRO;
            currentLevel = 4;
            level4Phase = 1; // RESET Level 4 Phase
            bGX = 0;         // RESET Background
          }
          storyTimerCount = 0;
        }
      }
    }
  }

  // Populate specialKeyPressed array
  specialKeyPressed[key] = 1;
}

void iSpecialKeyboardUp(unsigned char key) { specialKeyPressed[key] = 0; }
void drawTimer() {
  if (gameState == GAME && !levelOneComplete && currentLevel != 4) {
    char timeStr[32];
    int transitionTime = (currentLevel == 3) ? 60 : 45;
    int remaining = transitionTime - gameRunTimeSeconds;
    if (remaining < 0)
      remaining = 0;
    sprintf_s(timeStr, sizeof(timeStr), "Time: %02d:%02d", remaining / 60,
              remaining % 60);
    iSetColor(255, 255, 255);
    iText((int)(iScreenWidth / 2.0 - 50.0), (int)(iScreenHeight - 60),
          (char *)timeStr, (void *)GLUT_BITMAP_TIMES_ROMAN_24);
  }
}
void fixedUpdate() {}

/* -------------------- MAIN FUNCTION -------------------- */
int main() {
  iInitialize(screenWidth, screenHeight, (char *)"Fragment of Divinity");
  srand((unsigned int)time(NULL));
  loadHighScores(); // Load persisted high scores from file

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
  loadGameOverAssets();
  loadCaveAssets();
  caveSceneBgL2 = iLoadImage((char *)"Wizard\\cave22.png");
  wizardL2Img = iLoadImage((char *)"Wizard\\wizard2.0.png");
  caveImgL3 = iLoadImage((char *)"level3\\cave3.png");
  wizardL3Img = iLoadImage((char *)"level3\\wizard3.png");
  l3CompanionImg = iLoadImage((char *)"Wizard\\l3companion.png");
  owlImg = iLoadImage((char *)"level4\\owl\\owl power up.png");
  owlBulletImg = iLoadImage((char *)"level4\\owl\\bullet.png");
  char owlFramePath[50];
  for (int i = 0; i < 9; i++) {
    sprintf_s(owlFramePath, sizeof(owlFramePath), "level4\\owl\\frame_%03d.png",
              i);
    owlFrames[i] = iLoadImage(owlFramePath);
  }
  for (int i = 0; i < 4; i++) {
    sprintf_s(owlFramePath, sizeof(owlFramePath),
              "level4\\owl\\standing\\frame_%03d.png", i);
    owlStandingFrames[i] = iLoadImage(owlFramePath);
  }
  loadCharacter2Assets();
  loadDragonAssets();
  initDragons();
  initShard();
  for (int i = 0; i < 5; i++) {
    char hFramePath[100];
    sprintf_s(hFramePath, sizeof(hFramePath), "scores and items\\halo\\frame_%03d.png", i * 2);
    haloFrames[i] = iLoadImage(hFramePath);
  }
  loadGunBarAssets();    // Gun bar image
  loadGunAttackAssets(); // Load Gun Attack and Fireball assets
  loadCinderAssets();
  loadBossAssets(); // Preload boss assets at startup to eliminate skip-button
                    // delay
  loadCloneAttackAssets();    // Preload clone images
  loadTeleportAttackAssets(); // Preload teleport attack images
  // loadBossAssets(); // Deferred to resetGame() to speed up startup

  // Register UP functions if supported by the library
  // glutKeyboardUpFunc(iKeyboardUp);
  // glutSpecialUpFunc(iSpecialKeyboardUp);

  iSetTimer(30, masterGameLoop);
  iSetTimer(1000, globalTimerLogic);

  playMusicTrack(TRACK_INTRO);

  iStart();
  return 0;
}
