#include <SDL.h>
#include <cstdio>

int main(int argc, char* argv[]){
    SDL_version v;
    SDL_GetVersion(&v);
    std::printf("SDL %d.%d.%d\n", v.major, v.minor, v.patch);
    return 0;
}