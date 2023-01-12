/*  Jumping into paintings is still cool!
    TO DO:
        Adjust player position offset.
            in setupMap:
                    calculate window x,y coordinates
                    for each cell. store in data array.
        Collision detection between player and ground.
        Get map size dinamically. */
#include <SDL2/SDL.h>
#include <stdio.h>

#define SCREEN_W 1024
#define SCREEN_H 512
#define CELL_SIZE 64

typedef struct{
    SDL_Rect rect;
    int size;
    int speed;
    int gravity;
}Player; Player p;

typedef struct{
    int empty;
    int ground;
    int water;
    int startPos;
    int endPos;
}Tile; Tile tile;

SDL_Window * win = NULL;
SDL_Renderer * rend = NULL;

const int mapX = 16, mapY = 8;
int spawnPointX, spawnPointY;

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
void updatePlayer();
void drawPlayer();

/*  Level */
void setupMap();
void drawMap();

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

    setupMap();
    p.rect.x = spawnPointX; p.rect.y = spawnPointY;
    p.size = CELL_SIZE / 2;
    p.rect.w = p.size; p.rect.h = p.size;
    p.speed = 5;
    p.gravity = 3;
}

void update(){
    updatePlayer();
}

void display(){
    SDL_SetRenderDrawColor(rend, 0, 0, 0, 255);
    SDL_RenderClear(rend);

    drawMap();
    drawPlayer();

    SDL_RenderPresent(rend);
}

void updatePlayer(){
    p.rect.y += p.gravity;
    // if player falls off screen reset pos
    if(p.rect.y > SCREEN_H){p.rect.x = spawnPointX; p.rect.y = spawnPointX;}
}

void drawPlayer(){
    SDL_SetRenderDrawColor(rend, 200, 125, 0, 255);
    SDL_RenderFillRect(rend, &p.rect);
}

void setupMap(){
    tile.empty = 0;
    tile.ground = 1;
    tile.water = 2;
    tile.startPos = 3;
    tile.endPos = 4;

    int cell = 0;
    for(int i = 0; i < mapY; i++){
        for(int j = 0; j < mapX; j++){
            if(map[cell] == tile.startPos){
                spawnPointX = j * CELL_SIZE + CELL_SIZE / 2;
                spawnPointY = i * CELL_SIZE + CELL_SIZE / 2;
                break;
            }
            cell++;
        }
    }
}

void drawMap(){
    SDL_Rect rect = {0, 0, CELL_SIZE, CELL_SIZE};
    int cell = 0;
    for(int i = 0; i < mapY; i++){
        for(int j = 0; j < mapX; j++){
            if(map[cell] == tile.empty){
                SDL_SetRenderDrawColor(rend, 255, 255, 255, 255);
            }else if(map[cell] == tile.ground){
                SDL_SetRenderDrawColor(rend, 100, 75, 13, 255);
            }else if(map[cell] == tile.water){
                SDL_SetRenderDrawColor(rend, 75, 0, 175, 255);
            }else if(map[cell] == tile.startPos){
                SDL_SetRenderDrawColor(rend, 0, 255, 0, 255);
            }
            rect.x = j * CELL_SIZE; rect.y = i * CELL_SIZE;
            SDL_RenderFillRect(rend, &rect);
            cell++;
        }
    }
}