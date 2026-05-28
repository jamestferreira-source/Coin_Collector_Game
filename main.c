#include <stdio.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <SDL2/SDL_ttf.h>


// FPS
#define FPS 120
#define FRAME_DELAY (1000 / FPS)

// Coin info
#define MAX_COINS 15
#define COINSPAWN (FPS * 2500 / 1000)  // spawn a coin every 2500ms

// Sizes and speeds
#define PLAYERSIZE 20
#define COINSIZE 15
#define FONTSIZE 40
#define PLAYERSPEED 5
#define SCORE_PER_COIN 5


typedef struct {
    int x;
    int y;
} Coin;


void draw_rect(SDL_Renderer* renderer, int x, int y, int w, int h) {
    SDL_Rect r = {x, y, w, h};
    SDL_RenderFillRect(renderer, &r);
}

void draw_text(SDL_Renderer* renderer, TTF_Font* font, const char* text, int x, int y, SDL_Color color) {
    if (!font) return;
    SDL_Surface* surface = TTF_RenderText_Blended(font, text, color);
    if (!surface) return;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        SDL_FreeSurface(surface);
        return;
    }
    SDL_Rect destination = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &destination);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}


int main(int argc, char *argv[]) {
    (void)argc;
    (void)argv;
    srand(time(NULL));

    int w;
    int h;
    int running = 1;

    int coin_timer = 0;
    int coin_count = 0;
    int score = 0;
    Coin coins[MAX_COINS];
    char buffer[32];

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Coin Collector", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    if (TTF_Init() != 0) {
        fprintf(stderr, "TTF_Init: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    TTF_Font* font = TTF_OpenFont("fonts/Tiny5-Regular.ttf", FONTSIZE);
    if (!font) {
        fprintf(stderr, "TTF_OpenFont: %s\n", TTF_GetError());
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Event event;
    SDL_Color white = {255, 255, 255, 255};

    if (SDL_GetRendererOutputSize(renderer, &w, &h) != 0) {
        fprintf(stderr, "SDL_GetRendererOutputSize: %s\n", SDL_GetError());
        TTF_CloseFont(font);
        TTF_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    int player_x = w / 2;
    int player_y = h / 2;


    while (running) {
        coin_timer++;
        Uint32 start_time = SDL_GetTicks();

        // Event handling
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
            }
        }

        // Movement
        const Uint8* key = SDL_GetKeyboardState(NULL);
        if (key[SDL_SCANCODE_W]) player_y -= PLAYERSPEED;
        if (key[SDL_SCANCODE_S]) player_y += PLAYERSPEED;
        if (key[SDL_SCANCODE_D]) player_x += PLAYERSPEED;
        if (key[SDL_SCANCODE_A]) player_x -= PLAYERSPEED;
        if (player_x < 0) player_x = 0;
        if (player_y < 0) player_y = 0;
        if (player_x > w - PLAYERSIZE) player_x = w - PLAYERSIZE;
        if (player_y > h - PLAYERSIZE) player_y = h - PLAYERSIZE;

        // Background
        SDL_SetRenderDrawColor(renderer, 34, 139, 34, 255);
        SDL_RenderClear(renderer);

        // Player
        SDL_SetRenderDrawColor(renderer, 144, 238, 144, 255);
        draw_rect(renderer, player_x, player_y, PLAYERSIZE, PLAYERSIZE);
        SDL_Rect player = {player_x, player_y, PLAYERSIZE, PLAYERSIZE};

        // Coin spawning
        if (coin_timer >= COINSPAWN) {
            if (coin_count < MAX_COINS) {
                coins[coin_count].x = (rand() % 61 + 20) * w / 100;
                coins[coin_count].y = (rand() % 61 + 20) * h / 100;
                coin_count++;
            }
            coin_timer = 0;
        }

        // Coin rendering and collection
        for (int i = 0; i < coin_count; i++) {
            SDL_Rect coin = {coins[i].x, coins[i].y, COINSIZE, COINSIZE};
            SDL_SetRenderDrawColor(renderer, 255, 215, 0, 255);
            draw_rect(renderer, coins[i].x, coins[i].y, COINSIZE, COINSIZE);
            if (SDL_HasIntersection(&coin, &player)) {
                coins[i] = coins[coin_count - 1];
                coin_count--;
                score += SCORE_PER_COIN;
                i--;
            }
        }

        // Score display
        snprintf(buffer, sizeof(buffer), "Score: %d", score);
        int text_w, text_h;
        TTF_SizeText(font, buffer, &text_w, &text_h);
        draw_text(renderer, font, buffer, w - text_w - 20, 20, white);

        SDL_RenderPresent(renderer);  // swap back buffer to screen

        // Frame cap
        Uint32 frame_time = SDL_GetTicks() - start_time;
        if (frame_time < FRAME_DELAY) {
            SDL_Delay(FRAME_DELAY - frame_time);
        }
    }

    TTF_CloseFont(font);
    TTF_Quit();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
