#include "MainGame.h"
#include "Sprite.h"
#include "Vertex.h"

#include <cstdlib> // Para generar números aleatorios
#include <ctime>   // Para obtener el tiempo actual

MainGame::MainGame()
{
    window = nullptr;
    width = 800;
    height = 600;
    gameState = GameState::PLAY;

    srand(time(NULL)); // Inicializar la semilla para los números aleatorios

    // Inicializar los temporizadores para cada cuadrante
    for (int i = 0; i < 4; ++i) {
        lastSpawnTime[i] = 0.0f;
        rectangleIDs[i] = 0;
    }
}

MainGame::~MainGame()
{
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void MainGame::init()
{
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Rectángulos en Cuadrantes", SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);

    // Validar si hubo un error al crear la ventana
    if (window == nullptr) {
        std::cerr << "Error al crear la ventana SDL: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return;
    }

    // Validar si hubo un error al crear el contexto OpenGL
    SDL_GLContext glcontext = SDL_GL_CreateContext(window);
    GLenum error = glewInit();
    if (error != GLEW_OK) {
        std::cerr << "Error al inicializar GLEW: " << glewGetErrorString(error) << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return;
    }

    // Configurar el doble búfer
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Inicializar el tiempo de última aparición de los rectángulos en todos los cuadrantes
    for (int i = 0; i < 4; ++i) {
        lastSpawnTime[i] = SDL_GetTicks(); // Inicializar con el tiempo actual
    }
}


void MainGame::draw()
{
    glClearDepth(1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    sprite.draw();

    // Dibujar los rectángulos en cada cuadrante
    for (int i = 0; i < 4; ++i) {
        if (rectangleIDs[i] != 0) {
            glBindBuffer(GL_ARRAY_BUFFER, rectangleIDs[i]);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glDisableVertexAttribArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }

    SDL_GL_SwapWindow(window);
}

void MainGame::run()
{
    init();
    sprite.init(-1, -1, 1, 1);
    update();
}

void MainGame::update()
{
    while (gameState != GameState::EXIT) {
        processInput();
        spawnRectangles(); // Llamar al método spawnRectangles() en cada iteración de update()
        draw();
    }
}

void MainGame::processInput()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            gameState = GameState::EXIT;
            break;
        case SDL_MOUSEMOTION:
            /*cout << "Posicion en X: " << event.motion.x <<
                " Posicion Y" << event.motion.y << endl;*/
            break;
        }
    }
}

void MainGame::spawnRectangles()
{
    Uint32 currentTime = SDL_GetTicks();

    // Verificar y actualizar la aparición de rectángulos en cada cuadrante
    for (int i = 0; i < 4; ++i) {
        if (currentTime - lastSpawnTime[i] >= spawnInterval) {
            lastSpawnTime[i] = currentTime;

            // Generar un número aleatorio para determinar el cuadrante
            int quadrant = rand() % 4;

            // Determinar las coordenadas del rectángulo en el cuadrante aleatorio
            float rectX, rectY;
            switch (quadrant) {
            case 0: // Cuadrante superior izquierdo
                rectX = -1.0f;
                rectY = 1.0f;
                break;
            case 1: // Cuadrante superior derecho
                rectX = 0.0f;
                rectY = 1.0f;
                break;
            case 2: // Cuadrante inferior izquierdo
                rectX = -1.0f;
                rectY = -1.0f;
                break;
            case 3: // Cuadrante inferior derecho
                rectX = 0.0f;
                rectY = -1.0f;
                break;
            }

            // Imprimir las coordenadas del rectángulo
            std::cout << "Rectángulo en cuadrante " << i << ": X = " << rectX << ", Y = " << rectY << std::endl;

            // Configurar el color del rectángulo
            setColorByIndex(quadrant);

            // Dibujar el rectángulo en el cuadrante correspondiente
            sprite.init(rectX, rectY, 1, 1); // Ajustar el tamaño del rectángulo aquí
            sprite.draw();
        }
    }
}


void MainGame::setColorByIndex(int index)
{
    switch (index) {
    case 0: // Rojo
        glColor3f(1.0f, 0.0f, 0.0f);
        break;
    case 1: // Verde
        glColor3f(0.0f, 1.0f, 0.0f);
        break;
    case 2: // Azul
        glColor3f(0.0f, 0.0f, 1.0f);
        break;
    case 3: // Amarillo
        glColor3f(1.0f, 1.0f, 0.0f);
        break;
    default:
        glColor3f(1.0f, 1.0f, 1.0f); // Color blanco por defecto
        break;
    }
}






