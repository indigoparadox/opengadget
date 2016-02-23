
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

RETVAL graphics_init( void );
void graphics_cleanup( void );
void graphics_set_title( bstring title );
void graphics_clear( void );
void graphics_end_draw( void );
void graphics_sleep( int milliseconds );
SDL_Texture* graphics_image_load( bstring image_name, struct pak_file* pak );
void graphics_draw_tile( SDL_Texture* texture, int src_x, int src_y, int dest_x, int dest_y );

#endif /* GRAPHICS_H */

