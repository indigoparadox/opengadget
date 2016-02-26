
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

/* TODO: Maybe make this smarter with XOR at some point, but don't            *
* overcomplicate right now.                                                  */
#define graphics_isometric_tile_rotate( x, y, width, height, rotation ) \
   switch( rotation ) { \
      case GRAPHICS_ROTATE_90: \
         x = x ^ y; \
         y = x ^ y; \
         x = x ^ y; \
         y = height - y; \
         break; \
      case GRAPHICS_ROTATE_180: \
         x = width - x; \
         y = height - y; \
         break; \
      case GRAPHICS_ROTATE_270: \
         x = x ^ y; \
         y = x ^ y; \
         x = x ^ y; \
         x = width - x; \
         break; \
   }

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
void graphics_transform_isometric_reverse(
   int* tile_x,
   int* tile_y,
   int screen_x,
   int screen_y,
   int map_width,
   int map_height,
   const SDL_Rect* viewport,
   int rotation
);

#endif /* GRAPHICS_H */

