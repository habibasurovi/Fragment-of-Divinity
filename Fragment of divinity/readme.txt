========================================================================
    FRAGMENT OF DIVINITY  --  Game README
========================================================================

A 2D side-scrolling action RPG built with iGraphics (OpenGL/GLUT) in C++.
Defeat enemies, solve riddles, collect shards, and uncover the truth
behind the divine fragment shattered across four cursed realms.

------------------------------------------------------------------------
  HOW TO BUILD & RUN
------------------------------------------------------------------------

1. Open "Fragment of divinity.sln" in Visual Studio 2013 (or later).
2. Set configuration to Debug or Release (x86).
3. Build the solution (Ctrl+Shift+B).
4. Run the executable from within Visual Studio (F5), or navigate to
   the Debug\ folder and run "Fragment of divinity.exe" directly.
   (The working directory MUST be "Fragment of divinity\" so that
    all asset folders are reachable.)

------------------------------------------------------------------------
  GAME OVERVIEW & FLOW
------------------------------------------------------------------------

  INTRO  →  MAIN MENU
           |
           ├─ Start  →  Initial Story (12 pages)  →  Level Selection
           |               ↓ (after selecting a level)
           |            Level Intro  →  Level Story  →  GAMEPLAY
           |                                               ↓ (timer ends)
           |                                          Level Complete Menu
           |                                               ↓ (Next)
           |                                    Cave / IQ Riddle Screen
           |                                               ↓ (correct/next)
           |                                          Next Level Intro ...
           |
           |  (After clearing Level 4 and defeating the Final Boss)
           |
           └─  FINAL VICTORY frames (2 pages, cave-style next button)
                        ↓ (Next after frame 2)
               FINAL STORY (11-page epilogue)
                        ↓ (Next on last page)
               MAIN MENU

------------------------------------------------------------------------
  PLAYER CONTROLS
------------------------------------------------------------------------

  MOVEMENT
    ← / →         Walk left / right
    ↑  (tap)      Jump
    ↑  (double)   Double jump
    ↓  (tap)      Bend / duck
    ↓  (double)   Super-bend (lower hitbox)

  COMBAT
    SPACE          Melee attack / kill dragon (Level 2)
    SHIFT          Trigger 5-shot gun burst (Levels 3 & 4, requires gun)
    C              Clone Attack  (Level 4 only)
    D              Jump Attack   (Level 4 only)
    B              Teleport Attack (Level 4 only)
    E              Meteroid Attack (Level 4 only)
    Left-click     Activate Gun Bar  |  Use Owl power-up icon (Level 4)

  MENUS & STORY NAVIGATION
    → (Right Arrow)   Next page / Next victory frame
    ← (Left Arrow)    Previous page
    SPACE             Skip story sequence
    ESC               Pause / Unpause game
    M                 Close Settings menu

  HIGH SCORES
    B                 Return to Main Menu from High Scores screen

------------------------------------------------------------------------
  LEVELS
------------------------------------------------------------------------

  LEVEL 1 – Mistveil Forest
    Classic platformer run. Avoid obstacles, collect apples.
    Cave IQ: Solve a riddle to claim the Divine Shard.

  LEVEL 2 – Cinder WindPath
    Introduces the Gun mechanic. Shoot dragons and bombs.
    Cave IQ: Correct answer unlocks the Elemental Gun.

  LEVEL 3 – Nightshade Manor
    Companion mechanic (Aizen the Owl). Cinder enemy type added.
    Cave IQ: Correct answer grants the Companion for Level 4.

  LEVEL 4 – The Doomspire
    Final boss arena. 9-skill rotating boss with:
      • Vanish, Jump, Pull, Push, Enemy Spawn, Top Obstacles,
        Fireball, Rest Phase, Hitground (Hole Birds).
    Owl companion can be activated from the power-up icon (bottom-left).
    Hearts drop every 60 seconds to help survive.

  FINAL BOSS DEATH SEQUENCE
    On defeat, the boss plays its 21-frame death animation.
    After 30 ticks, the game transitions to:

  FINAL VICTORY (2 frames)
    Two story frames appear centered over the frozen Level 4 background.
    Navigate with the animated Next button (bottom-right) or → key.
    Music: sound\final victory.wav (looping).

  FINAL STORY (11 pages)
    Epilogue story sequence with standard story navigation buttons.
    Next on the last page returns to the Main Menu.
    Music: sound\game won story.wav (looping).

------------------------------------------------------------------------
  AUDIO
------------------------------------------------------------------------

  Music tracks (sound\ folder):
    intro.wav           Main menu loop
    story.wav           Story / level-selection loop
    level1.wav          Level 1 gameplay loop
    level2.wav          Level 2 gameplay loop
    level3.wav          Level 3 gameplay loop
    level4.wav          Level 4 gameplay / boss fight loop
    cave.wav            Cave & IQ riddle screen loop
    gameover.wav        Game-over sting (plays once)
    final victory.wav   Final victory frames loop  ← NEW
    game won story.wav  Final story epilogue loop  ← NEW

  Sound effects (sound\ folder):
    click.wav           UI button click
    bonus scores.wav    Bonus score earned (NPC kill reward)

------------------------------------------------------------------------
  CUSTOMIZATION
------------------------------------------------------------------------

  Characters (via Customization menu):
    1. Aryn    2. Kaero    3. Leora

  Weather themes (via Customization menu):
    Summer / Rainy / Winter  (affects Level 1 & 2 backgrounds)

------------------------------------------------------------------------
  PROJECT STRUCTURE (key files)
------------------------------------------------------------------------

  iMain.cpp               Master entry point, all game loop & input logic
  GameState.h             GameState enum (all screen/state identifiers)
  StoryHandler.h          Story page assets, draw & click functions
  SoundHandler.h          Music track enum & playback helpers
  Boss.h                  Final boss data, skills, animations (2800+ lines)
  BackgroundHandler.h     Background scrolling & asset loading
  Character.h             Player character physics & animation
  cave.h                  Cave / wizard / IQ transition screen
  IQ.h                    Riddle question system
  LevelCompleteHandler.h  Level complete UI & buttons
  GameOverHandler.h       Game-over menu UI & buttons
  ObstacleHandler.h       All obstacle types (sharks, bombs, worms, etc.)
  owl.h                   Owl companion logic (Level 4)
  attack.h                Melee attack animation
  gunattack.h             Gun burst & fireball logic
  cloneattack.h           Clone attack (Level 4)

  story\                  All story page images
    initial story\        12-page opening story
    level 1\ .. level 4\  Per-level intro & story pages
    cave2story\           Cave 2 transition story
    final story\          11-page ending epilogue  ← NEW
  level4\final victory\   2-frame victory popup images  ← NEW
  sound\                  All music & SFX wav files
  Boss\                   All boss animation frames
  images fod\             Menu & UI images

------------------------------------------------------------------------
  GAME STATES REFERENCE (GameState.h)
------------------------------------------------------------------------

  INTRO              Splash / loading screen
  MENU               Main menu
  CUSTOMIZATION      Character & weather selection
  SETTINGS           Music & sound toggles
  INITIAL_STORY      12-page opening story
  LEVEL_SELECTION    Level picker
  LEVEL1_INTRO …     Per-level intro & story pages
  GAME               Active gameplay
  GAME_OVER          Game-over screen
  LEVEL_COMPLETE     Level complete screen
  NEXT_LEVEL_IQ      Cave + riddle transition
  CAVE2_STORY        Cave 2 story pages
  HIGHSCORES         High score table
  LEVEL_NOT_READY    Placeholder for unimplemented levels
  FINAL_VICTORY      Post-boss 2-frame victory popup  ← NEW
  FINAL_STORY        11-page game-ending epilogue     ← NEW

========================================================================
