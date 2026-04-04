#ifndef SOUND_HANDLER_H_
#define SOUND_HANDLER_H_

// =====================================================================
// SoundHandler.h  --  All music and sound effect logic
// Requires: <windows.h>, <mmsystem.h> (via iGraphics.h)
// =====================================================================

#include "iGraphics.h"
#include <mmsystem.h>

// ---- Settings flags ----
#ifdef _MSC_VER
#define SND_SELECTANY __declspec(selectany)
#else
#define SND_SELECTANY __attribute__((weak))
#endif

SND_SELECTANY bool isMusicOn = true;
SND_SELECTANY bool isSoundOn = true;

// ---- Music Tracks ----
enum MusicTrack {
  TRACK_NONE,
  TRACK_INTRO,
  TRACK_STORY,
  TRACK_LEVEL1,
  TRACK_LEVEL2,
  TRACK_LEVEL3,
  TRACK_LEVEL4,
  TRACK_CAVE,
  TRACK_GAMEOVER,
  TRACK_FINAL_VICTORY,
  TRACK_GAME_WON_STORY
};

SND_SELECTANY MusicTrack currentTrack = TRACK_NONE;

// ---- Play a looping music track ----
inline void playMusicTrack(MusicTrack track) {
  if (!isMusicOn) {
    PlaySound(NULL, 0, 0);
    currentTrack = track; // Preserve requested track state
    return;
  }

  if (currentTrack == track && currentTrack != TRACK_NONE)
    return; // Already playing this track

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
  case TRACK_LEVEL3:
    PlaySound((char *)"sound\\level3.wav", NULL,
              SND_FILENAME | SND_ASYNC | SND_LOOP);
    break;
  case TRACK_LEVEL4:
    PlaySound((char *)"sound\\level4.wav", NULL,
              SND_FILENAME | SND_ASYNC | SND_LOOP);
    break;
  case TRACK_CAVE:
    PlaySound((char *)"sound\\cave.wav", NULL,
              SND_FILENAME | SND_ASYNC | SND_LOOP);
    break;
  case TRACK_GAMEOVER:
    PlaySound((char *)"sound\\gameover.wav", NULL,
              SND_FILENAME | SND_ASYNC); // Play ONCE for game over
    break;
  case TRACK_FINAL_VICTORY:
    PlaySound((char *)"sound\\final victory.wav", NULL,
              SND_FILENAME | SND_ASYNC | SND_LOOP);
    break;
  case TRACK_GAME_WON_STORY:
    PlaySound((char *)"sound\\game won story.wav", NULL,
              SND_FILENAME | SND_ASYNC | SND_LOOP);
    break;
  }
}

// ---- Play a one-shot UI click sound ----
inline void playClickSound() {
  if (isSoundOn) {
    mciSendStringA("close click", NULL, 0, NULL);
    mciSendStringA("open \"sound\\click.wav\" type waveaudio alias click", NULL,
                   0, NULL);
    mciSendStringA("play click", NULL, 0, NULL);
  }
}

// ---- Play a one-shot bonus score sound (NPC kill reward) ----
inline void playBonusScoreSound() {
  if (isSoundOn) {
    mciSendStringA("close bonusscore", NULL, 0, NULL);
    mciSendStringA(
        "open \"sound\\bonus scores.wav\" type waveaudio alias bonusscore",
        NULL, 0, NULL);
    mciSendStringA("play bonusscore", NULL, 0, NULL);
  }
}

#endif // SOUND_HANDLER_H_
