#ifndef HIGHSCOREHANDLER_H
#define HIGHSCOREHANDLER_H

#include <stdio.h>
#include <string.h>

extern int highScores[4];

// Call this at the start of the game
inline void loadHighScores() {
    FILE *fp;
    // Attempt to open the file to read
    if (fopen_s(&fp, "highscores.txt", "r") == 0 && fp != NULL) {
        // Read 4 integers for 4 levels
        if (fscanf_s(fp, "%d %d %d %d", &highScores[0], &highScores[1], &highScores[2], &highScores[3]) != 4) {
             // If parsing fails, reset
             highScores[0] = 0; highScores[1] = 0; highScores[2] = 0; highScores[3] = 0;
        }
        fclose(fp);
    } else {
        // If file doesn't exist, start with 0s
        highScores[0] = 0;
        highScores[1] = 0;
        highScores[2] = 0;
        highScores[3] = 0;
    }
}

// Internal call to save the array array back to the file
inline void saveHighScores() {
    FILE *fp;
    if (fopen_s(&fp, "highscores.txt", "w") == 0 && fp != NULL) {
        fprintf(fp, "%d %d %d %d\n", highScores[0], highScores[1], highScores[2], highScores[3]);
        fclose(fp);
    }
}

// Call this when level ends (GameOver or LevelComplete)
// level should be 1, 2, 3, or 4
// score is applesCollected
inline void updateHighScore(int level, int score) {
    if (level >= 1 && level <= 4) {
        int index = level - 1;
        if (score > highScores[index]) {
            highScores[index] = score;
            saveHighScores();
        }
    }
}

#endif // HIGHSCOREHANDLER_H
