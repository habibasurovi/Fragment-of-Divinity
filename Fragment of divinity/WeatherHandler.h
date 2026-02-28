#ifndef WEATHERHANDLER_H
#define WEATHERHANDLER_H

#include "BackgroundHandler.h" // For selectedWeather
#include "iGraphics.h"
#include <stdio.h>
#include <stdlib.h>

extern int selectedWeather; // Defined in CustomizationMenu.h
extern int shakeOffsetX;
extern int shakeOffsetY;

// Images
extern int imgIce, imgWater;

// Particles
struct WeatherParticle {
  int x, y;
  int speed;
  bool active;
};

#define MAX_PARTICLES 50
extern struct WeatherParticle weatherParticles[MAX_PARTICLES];

inline void loadWeatherAssets() {
  // Creating weather effect logic
  // Task 2: use ice.bmp for winter, water.bmp for rainy
  imgIce = iLoadImage("background img\\ice.bmp");
  imgWater = iLoadImage("background img\\water.bmp");

  for (int i = 0; i < MAX_PARTICLES; i++) {
    weatherParticles[i].active = true;
    weatherParticles[i].x = rand() % 1000;
    weatherParticles[i].y = rand() % 600;
    weatherParticles[i].speed = 5 + rand() % 10;
  }
}

inline void updateWeatherPhysics() {
  // Only update if weather is active
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

inline void drawWeatherEffects() {
  if (selectedWeather == 1) { // Rainy
    // Use water.bmp
    for (int i = 0; i < MAX_PARTICLES; i++) {
      // Significantly reduced size for rain
      iShowImage(weatherParticles[i].x + shakeOffsetX, weatherParticles[i].y + shakeOffsetY, 5, 8, imgWater);
    }
  } else if (selectedWeather == 2) { // Winter
    // Use ice.bmp
    for (int i = 0; i < MAX_PARTICLES; i++) {
      // Significantly reduced size for snow
      iShowImage(weatherParticles[i].x + shakeOffsetX, weatherParticles[i].y + shakeOffsetY, 8, 8, imgIce);
    }
  }
}

#endif
