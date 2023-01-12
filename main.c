/*  Jumping into paintings is still cool!
    TO DO:
        Calculate map cells window position and area limits.
        Adjust player position offset.
        Collision detection between player and ground.
        Get map size dinamically. */

#include <SDL2/SDL.h>
#include <stdio.h>

#define SCREEN_W 1024
#define SCREEN_H 512
#define CELL_SIZE 64

SDL_Window * win = NULL;
SDL_Renderer * rend = NULL;

typedef struct{
    int x;
    int y;
}Pos;

typedef struct{
    int empty;
    int ground;
    int water;
    int startPos;
    int endPos;
}Tile;

typedef struct{
    Pos mapSize;
    int mapLength;
    int spawnX;
    int spawnY;
    int *map;
    Tile tile;
}Level; Level level;

typedef struct{
    SDL_Rect rect;
    int size;
    int speed;
    int gravity;
}Player; Player p;

int map[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 1, 2, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0
};

/*  Main Loop */
void init();
void update();
void display();

/*  Player */
void playerInit();
void playerUpdate();
void playerDraw();

/*  Level */
void levelInit();
void mapSetup();
void mapDraw();

int main(void){

    init();

    int run = 1;
    while (run)
    {
        SDL_Event ev;
        while(SDL_PollEvent(&ev)){
            switch(ev.type){
                case SDL_QUIT:
                    run = 0;
                    break;
                case SDL_KEYDOWN:
                    switch(ev.key.keysym.scancode){
                        case SDL_SCANCODE_ESCAPE:
                            run = 0;
                            break;
                        case SDL_SCANCODE_W:
                            p.rect.y -= p.speed;
                            break;
                        case SDL_SCANCODE_S:
                            p.rect.y += p.speed;
                            break;
                        case SDL_SCANCODE_A:
                            p.rect.x -= p.speed;
                            break;
                        case SDL_SCANCODE_D:
                            p.rect.x += p.speed;
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        update();
        display();
    }
    
    SDL_DestroyWindow(win);
    SDL_Quit();

    return 0;
}

void init(){
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_CreateWindowAndRenderer(SCREEN_W, SCREEN_H, 0, &win, &rend);
    SDL_SetWindowTitle(win, "Raycaster");

    levelInit();
    mapSetup();
    p.rect.x = level.spawnX; p.rect.y = level.spawnY;
    p.size = CELL_SIZE / 2;
    p.rect.w = p.size; p.rect.h = p.size;
    p.speed = 5;
    p.gravity = 3;
}

void update(){
    playerUpdate();
}

void display(){
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);

    mapDraw();
    playerDraw();

    SDL_RenderPresent(rend);
}

void playerUpdate(){
    p.rect.y += p.gravity;
    // if player falls off screen reset pos
    if(p.rect.y > SCREEN_H){p.rect.x = level.spawnX; p.rect.y = level.spawnX;}
}

void playerDraw(){
    SDL_SetRenderDrawColor(rend, 200, 125, 0, 255);
    SDL_RenderFillRect(rend, &p.rect);
}

void levelInit(){
    level.tile.empty = 0;
    level.tile.ground = 1;
    level.tile.water = 2;
    level.tile.startPos = 3;
    level.tile.endPos = 4;
    level.mapSize.x = 16; level.mapSize.y = 8;
    /*  load map from file
        get mapX, mapY, mapLength
        set spawnX, spawnY
        calculate cells real pos,
        edge limits and center pos */
}

void mapSetup(){
    int cell = 0;
    for(int i = 0; i < level.mapSize.y; i++){
        for(int j = 0; j < level.mapSize.x; j++){
            if(map[cell] == level.tile.startPos){
                level.spawnX = j * CELL_SIZE + CELL_SIZE / 2;
                level.spawnY = i * CELL_SIZE + CELL_SIZE / 2;
                break;
            }
            cell++;
        }
    }
}

void mapDraw(){
    SDL_Rect rect = {0, 0, CELL_SIZE, CELL_SIZE};
    int cell = 0;
    for(int i = 0; i < level.mapSize.y; i++){
        for(int j = 0; j < level.mapSize.x; j++){
            if(map[cell] == level.tile.empty){
                SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
            }else if(map[cell] == level.tile.ground){
                SDL_SetRenderDrawColor(rend, 100, 75, 13, 255);
            }else if(map[cell] == level.tile.water){
                SDL_SetRenderDrawColor(rend, 75, 0, 175, 255);
            }else if(map[cell] == level.tile.startPos){
                SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
            }
            rect.x = j * CELL_SIZE; rect.y = i * CELL_SIZE;
            SDL_RenderFillRect(rend, &rect);
            cell++;
        }
    }
}