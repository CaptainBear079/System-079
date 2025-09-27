#include <stdbool.h>

typedef struct _RGBA_ {
    #ifdef __32BIT__
    // 32 Bit RGBA Color
    unsigned long int red, green, blue, alpha;
    #else
    // 64 Bit RGBA Color
    unsigned long long red, green, blue, alpha;
    #endif
} RGBA;
typedef struct _Image_ {
    #ifdef __32BIT__
    // 32 Bit Image
    unsigned long int size_x, size_y;
    #else
    // 64 Bit Image
    unsigned long long size_x, size_y;
    #endif
    RGBA* pixels;
} Image;
typedef struct _Sprite_ {
    #ifdef __32BIT__
    // 32 Bit Sprite
    unsigned long int pos_x, pos_y;
    unsigned long int layer;
    #else
    // 64 Bit Sprite
    unsigned long long pos_x, pos_y;
    unsigned long long layer;
    #endif
    int size;
    Image* sprite;
} Sprite;

int main() {
    // Init game
    bool quit = false;
    Sprite* sprites;
    // Main loop
    while(quit == false) {
        // Draw sprites
        while(*sprites) {}
    }
    return 0;
}