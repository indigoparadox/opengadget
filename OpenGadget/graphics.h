
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

OG_RETVAL graphics_init( void );
void graphics_cleanup( void );
void graphics_set_title( const bstring title );
void graphics_clear( void );
void graphics_end_draw( void );
void graphics_sleep( const int milliseconds );
SDL_Texture* graphics_image_load( const bstring image_name, const struct pak_file* pak );
void graphics_draw_tile( const SDL_Texture* texture, const int src_x, const int src_y, const int dest_x, const int dest_y );

#endif /* GRAPHICS_H */

