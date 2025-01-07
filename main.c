#include <stdio.h>
#include <stdbool.h>
#include <windows.h>
#include "inc/SDL.h"
#include "inc/SDL_ttf.h"
#undef main

typedef struct RGB {
    Uint8 r, g, b;
} RGB;

RGB getColorAt(int x, int y){
    RGB color;
    color.r = 0;
    color.g = 0;
    color.b = 0;

    HDC dc = GetDC(NULL);
    if(dc == NULL) {
        printf("Error getting device context\n");
        return color;
    }

    COLORREF pixelColor = GetPixel(dc, x, y);
    color.r = GetRValue(pixelColor);
    color.g = GetGValue(pixelColor);
    color.b = GetBValue(pixelColor);

    ReleaseDC(NULL, dc);
    return color;
}
int main(int argc, char* argv[]) {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;
    int mouseX;
    int mouseY;
    SDL_Rect Message_rectRGB;
    Message_rectRGB.x = 0;
    Message_rectRGB.y = 0;
    Message_rectRGB.w = 300;
    Message_rectRGB.h = 60;
    SDL_Rect Message_rectHEX;
    Message_rectHEX.x = 0;
    Message_rectHEX.y = 50;
    Message_rectHEX.w = 300;
    Message_rectHEX.h = 60;
    char formatted_textRGB[100];
    char formatted_textHEX[100];

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

    SDL_Color textColor = {255, 255, 255, 255};

    SDL_bool quit = SDL_FALSE;
    SDL_Event e;
    
    while (!quit) {
    while (SDL_PollEvent(&e) != 0) {
        if (e.type == SDL_QUIT) {
            quit = SDL_TRUE;
        }
    }

    POINT cursorPos;
    GetCursorPos(&cursorPos);
    mouseX = cursorPos.x;
    mouseY = cursorPos.y;

    RGB color = getColorAt(mouseX, mouseY);

    snprintf(formatted_textRGB, sizeof(formatted_textRGB), 
        "RGB: %d %d %d",
        color.r, color.g, color.b);
    snprintf(formatted_textHEX, sizeof(formatted_textHEX), 
        "HEX: #%02x%02x%02x",
        color.r, color.g, color.b);

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, 255);
    SDL_RenderClear(renderer);

    SDL_Surface* surfaceMessageRGB = TTF_RenderText_Solid(font, formatted_textRGB, textColor);
    SDL_Surface* surfaceMessageHEX = TTF_RenderText_Solid(font, formatted_textHEX, textColor);
    if (surfaceMessageRGB == NULL) {
        printf("Surface creation failed: %s\n", TTF_GetError());
        quit = SDL_TRUE;
        break;
    }
    if (surfaceMessageHEX == NULL) {
        printf("Surface creation failed: %s\n", TTF_GetError());
        quit = SDL_TRUE;
        break;
    }

    SDL_Texture* MessageRGB = SDL_CreateTextureFromSurface(renderer, surfaceMessageRGB);
    SDL_Texture* MessageHEX = SDL_CreateTextureFromSurface(renderer, surfaceMessageHEX);
    if (MessageRGB == NULL) {
        printf("Texture creation failed: %s\n", SDL_GetError());
        SDL_FreeSurface(surfaceMessageRGB);
        quit = SDL_TRUE;
        break;
    }
    if (MessageHEX == NULL) {
        printf("Texture creation failed: %s\n", SDL_GetError());
        SDL_FreeSurface(surfaceMessageHEX);
        quit = SDL_TRUE;
        break;
    }

    SDL_RenderCopy(renderer, MessageRGB, NULL, &Message_rectRGB);
    SDL_RenderCopy(renderer, MessageHEX, NULL, &Message_rectHEX);
    SDL_RenderPresent(renderer);

    SDL_FreeSurface(surfaceMessageRGB);
    SDL_FreeSurface(surfaceMessageHEX);
    SDL_DestroyTexture(MessageRGB);
    SDL_DestroyTexture(MessageHEX);
    }

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
    return 0;
}