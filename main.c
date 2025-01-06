#include <stdio.h>
#include <stdbool.h>
#include "inc/SDL.h"
#include "inc/SDL_ttf.h"
#undef main

typedef struct RGB {
    int r, g, b;
} RGB;
int main(int argc, char* argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    bool isMouseDownLeft = 0;
    SDL_Rect Message_rect2;
    Message_rect2.x = 0;
    Message_rect2.y = 0;
    Message_rect2.w = 350;
    Message_rect2.h = 40;
    RGB backgroundColor;
    backgroundColor.r = 0;
    backgroundColor.g = 0;
    backgroundColor.b = 0;
    RGB background;
    background.r = 0;
    background.g = 0;
    background.b = 0;
    RGB holdLeftColor;
    holdLeftColor.r = 0;
    holdLeftColor.g = 0;
    holdLeftColor.b = 255;
    char formatted_text[100];

    printf("Enter backgrond hex values: #");
    scanf("%02x%02x%02x", &backgroundColor.r, &backgroundColor.g, &backgroundColor.b);
    
    printf("Enter hold left hex values: #");
    scanf("%02x%02x%02x", &holdLeftColor.r, &holdLeftColor.g, &holdLeftColor.b);

    background.r = backgroundColor.r;
    background.g = backgroundColor.g;
    background.b = backgroundColor.b;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }
    if (TTF_Init() != 0) {
        printf("SDL_ttf could not initialize! SDL_Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    window = SDL_CreateWindow("SDL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALWAYS_ON_TOP);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    TTF_Font *font = TTF_OpenFont("assets\\Inter_28pt-Light.ttf", 15);
    if (font == NULL) {
        printf("Font could not be loaded! SDL_Error: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, 255);
    SDL_RenderClear(renderer);

    SDL_Color textColor = {255, 255, 255, 255};

    snprintf(formatted_text, sizeof(formatted_text), 
        "RGB: %d %d %d   Hex: #%02X%02X%02X",
        background.r, background.g, background.b, 
        background.r, background.g, background.b);
    SDL_Surface* surfaceMessage2 = TTF_RenderText_Solid(font, formatted_text, textColor);

    if (surfaceMessage2 == NULL) {
        printf("Message could not be rendered! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    SDL_Texture* Message2 = SDL_CreateTextureFromSurface(renderer, surfaceMessage2);
    SDL_RenderCopy(renderer, Message2, NULL, &Message_rect2);
    SDL_RenderPresent(renderer);

    SDL_bool quit = SDL_FALSE;
    SDL_Event e;
    
    while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = SDL_TRUE;
        } else if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_LEFT) {
            isMouseDownLeft = 1;
        } else if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_LEFT) {
            isMouseDownLeft = 0;
        }
    }

    if (isMouseDownLeft == 1){
        background.r = holdLeftColor.r;
        background.g = holdLeftColor.g;
        background.b = holdLeftColor.b;

    } else {
        background.r = backgroundColor.r;
        background.g = backgroundColor.g;
        background.b = backgroundColor.b;

    }

    SDL_SetRenderDrawColor(renderer, background.r, background.g, background.b, 255);
    SDL_RenderClear(renderer);

    snprintf(formatted_text, sizeof(formatted_text), 
        "RGB: %d %d %d   Hex: #%02X%02X%02X", 
        background.r, background.g, background.b, 
        background.r, background.g, background.b);

    SDL_Surface* surfaceMessage2 = TTF_RenderText_Solid(font, formatted_text, textColor);
    if (surfaceMessage2 == NULL) {
        printf("Surface creation failed: %s\n", TTF_GetError());
        quit = SDL_TRUE;
        break;
    }

    SDL_Texture* Message2 = SDL_CreateTextureFromSurface(renderer, surfaceMessage2);
    if (Message2 == NULL) {
        printf("Texture creation failed: %s\n", SDL_GetError());
        SDL_FreeSurface(surfaceMessage2);
        quit = SDL_TRUE;
        break;
    }

    SDL_RenderCopy(renderer, Message2, NULL, &Message_rect2);
    SDL_RenderPresent(renderer);

    SDL_FreeSurface(surfaceMessage2);
    SDL_DestroyTexture(Message2);
    }

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_FreeSurface(surfaceMessage2);
    SDL_DestroyTexture(Message2);
    SDL_Quit();
    return 0;
}