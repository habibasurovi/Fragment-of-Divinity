#ifndef WEATHERHANDLER_H
#define WEATHERHANDLER_H

#include "BackgroundHandler.h" // For selectedWeather
#include "iGraphics.h"
#include <stdio.h>
#include <stdlib.h>

extern int selectedWeather; // Defined in CustomizationMenu.h

// Images
extern int imgIce, imgWater;
extern int imgGhost1, imgGhost2;
extern int currentLevel;

// Particles
struct WeatherParticle {
  int x, y;
  int speed;
  bool active;
};

#define MAX_PARTICLES 50
extern struct WeatherParticle weatherParticles[MAX_PARTICLES];

struct Ghost {
  float x, y;
  int type;
  float speed;
  bool active;
};

#define MAX_GHOSTS 2
extern struct Ghost ghosts[MAX_GHOSTS];
extern int ghostSpawnTimer;

inline void loadWeatherAssets() {
  // Creating weather effect logic
  // Task 2: use ice.bmp for winter, water.bmp for rainy
  imgIce = iLoadImage("background img\\ice.bmp");
  imgWater = iLoadImage("background img\\water.bmp");
  imgGhost1 = iLoadImage("level3\\ghost1.png");
  imgGhost2 = iLoadImage("level3\\ghost2.png");

  for (int i = 0; i < MAX_PARTICLES; i++) {
    weatherParticles[i].active = true;
    weatherParticles[i].x = rand() % 1000;
    weatherParticles[i].y = rand() % 600;
    weatherParticles[i].speed = 5 + rand() % 10;
  }

  for (int i = 0; i < MAX_GHOSTS; i++) {
    ghosts[i].active = false;
  }
}

inline void updateWeatherPhysics() {
  // Only update if weather is active and not level 3
  if (currentLevel != 3) {
    if (selectedWeather == 1 || selectedWeather == 2) {
      for (int i = 0; i < MAX_PARTICLES; i++) {
        weatherParticles[i].y -= weatherParticles[i].speed;
        if (weatherParticles[i].y < 0) {
          weatherParticles[i].y = 600;
          weatherParticles[i].x = rand() % 1000;
        }
      }
    }
  }

  // Level 3 Ghost Logic
  if (currentLevel == 3) {
    ghostSpawnTimer++;
    if (ghostSpawnTimer > 300) { // Randomly spawn ghost
      if (rand() % 100 < 2) {    // Low chance to spawn
        for (int i = 0; i < MAX_GHOSTS; i++) {
          if (!ghosts[i].active) {
            ghosts[i].active = true;
            ghosts[i].x = 1100;
            ghosts[i].y = (float)(100 + rand() % 400);
            ghosts[i].type = (rand() % 2) + 1;
            ghosts[i].speed = (float)(SCROLL_SPD + 3 + rand() % 4);
            ghostSpawnTimer = 0; // Reset timer to prevent close ghosts
            break;
          }
        }
      }
    }

    for (int i = 0; i < MAX_GHOSTS; i++) {
      if (ghosts[i].active) {
        ghosts[i].x -= ghosts[i].speed;
        if (ghosts[i].x < -200) {
          ghosts[i].active = false;
        }
      }
    }
  }
}

inline void drawWeatherEffects() {
  if (currentLevel != 3) {
    if (selectedWeather == 1) { // Rainy
      // Use water.bmp
      for (int i = 0; i < MAX_PARTICLES; i++) {
        // Significantly reduced size for rain
        iShowImage(weatherParticles[i].x, weatherParticles[i].y, 5, 8, imgWater);
      }
    } else if (selectedWeather == 2) { // Winter
      // Use ice.bmp
      for (int i = 0; i < MAX_PARTICLES; i++) {
        // Significantly reduced size for snow
        iShowImage(weatherParticles[i].x, weatherParticles[i].y, 8, 8, imgIce);
      }
    }
  }

  if (currentLevel == 3) {
    for (int i = 0; i < MAX_GHOSTS; i++) {
      if (ghosts[i].active) {
        int img = (ghosts[i].type == 1) ? imgGhost1 : imgGhost2;
        iShowImage((int)ghosts[i].x, (int)ghosts[i].y, 100, 100, img);
      }
    }
  }
}

#endif
