#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfx.h>

#include <string>
#include <math.h>
using namespace std;
SDL_Renderer *init(SDL_Window **sajad);
int main(){
    SDL_Window *sajad;
    SDL_Renderer *fathian = init(&sajad);
    return 0;
}