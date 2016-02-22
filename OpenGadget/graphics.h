
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <SDL.h>
#include <SDL_image.h>

#include "dep\bstrlib.h"
#include "dep\arckogako.h"
#include "defines.h"

RETVAL graphics_init( void );
void graphics_begin_draw( void );
void graphics_end_draw( void );
void graphics_sleep( int milliseconds );
SDL_Texture* graphics_image_load( bstring image_name, struct pak_file* pak );
void graphics_draw( SDL_Texture* texture, const SDL_Rect* src_rect, const SDL_Rect* dest_rect );

#endif /* GRAPHICS_H */

