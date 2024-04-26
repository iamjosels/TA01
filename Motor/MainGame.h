#pragma once

#include <SDL/SDL.h>
#include <GL/glew.h>
#include <iostream>
#include "Sprite.h"

enum class GameState {
    PLAY,
    EXIT
};

class MainGame {
private:
    Sprite sprite;
    int width;
    int height;
    SDL_Window* window;
    GameState gameState;
    float lastSpawnTime[4];
    GLuint rectangleIDs[4];
    const float spawnInterval = 2000.0f; // Intervalo de aparición en milisegundos

    void init();
    void processInput();
    void setColorByIndex(int index);
    void spawnRectangles();


public:
    MainGame();
    ~MainGame();
    void run();
    void draw();
    void update();
};




