
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>
#include <SDL_image.h>

#include "dep\bstrlib.h"
#include "dep\arckogako.h"
#include "defines.h"

#define GRAPHICS_TILE_WIDTH 48
#define GRAPHICS_TILE_HEIGHT 48
#define GRAPHICS_TILE_OFFSET_X 24
#define GRAPHICS_TILE_OFFSET_Y 24
#define GRAPHICS_TILES_X_COUNT 4
#define GRAPHICS_TILES_Y_COUNT 4

typedef enum {
   GRAPHICS_ROTATE_0,
   GRAPHICS_ROTATE_90,
   GRAPHICS_ROTATE_180,
   GRAPHICS_ROTATE_270
} GRAPHICS_ROTATE;

OG_RETVAL graphics_init( void );
void graphics_cleanup( void );
void graphics_set_title( const bstring title );
void graphics_clear( void );
void graphics_end_draw( void );
void graphics_sleep( const int milliseconds );
SDL_Texture* graphics_image_load( const bstring image_name, const struct pak_file* pak );
void graphics_draw_tile( const SDL_Texture* texture, const int src_x, const int src_y, const int dest_x, const int dest_y );
void graphics_transform_isometric(
   int tile_x,
   int tile_y,
   int* screen_x,
   int* screen_y,
   int map_width,
   int map_height,
   const SDL_Rect* viewport,
   int rotation
);

#endif /* GRAPHICS_H */

