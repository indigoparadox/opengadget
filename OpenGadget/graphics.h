
/* This file is part of OpenGadget.
*
* OpenGadget is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the Free
* Software Foundation, either version 3 of the License, or (at your option)
* any later version.
*
* OpenGadget is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with OpenGadget.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef GRAPHICS_H
#define GRAPHICS_H

#ifdef USE_SDL
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_log.h>

typedef SDL_Texture OG_Texture;
typedef SDL_Rect OG_Rect;
#elif defined( USE_ALLEGRO )
#include <allegro.h>
typedef BITMAP OG_Texture;
typedef struct {
    int x;
    int y;
    int w;
    int h;
} OG_Rect;
#else
typedef void OG_Texture;
typedef struct {
    int x;
    int y;
    int w;
    int h;
} OG_Rect;
#endif /* USE_SDL */

#include "dep/bstrlib.h"
#include "dep/arckogako.h"
#include "defines.h"

#define GRAPHICS_SCREEN_WIDTH 640
#define GRAPHICS_SCREEN_HEIGHT 480

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

typedef enum {
   GRAPHICS_FONT_SANS,

   GRAPHICS_FONT_MAX
} GRAPHICS_FONT;

OG_RETVAL graphics_init( void );
void graphics_cleanup( void );
void graphics_set_title( const bstring title );
void graphics_clear( void );
void graphics_end_draw( void );
void graphics_sleep( const int milliseconds );
OG_Texture* graphics_image_load( const bstring image_name, const struct pak_file* pak );
OG_Texture* graphics_text_create( bstring text, GRAPHICS_FONT font_index, int size );
void graphics_draw( OG_Texture* texture, OG_Rect* src_rect, OG_Rect* dst_rect );
void graphics_draw_tile( const OG_Texture* texture, const int src_x, const int src_y, const int dest_x, const int dest_y );
void graphics_draw_bg( OG_Texture* background );
void graphics_transform_isometric_reverse(
   int* tile_x,
   int* tile_y,
   int screen_x,
   int screen_y,
   int map_width,
   int map_height,
   const OG_Rect* viewport,
   int rotation
);
#ifdef USE_INLINE_HELPERS
#ifdef _MSC_VER
__inline
#else
inline
#endif /* _MSC_VER */
#endif /* USE_INLINE_HELPERS */
void graphics_isometric_tile_rotate(
   int* x, int* y, int width, int height, GRAPHICS_ROTATE rotation
);
#ifdef USE_INLINE_HELPERS
#ifdef _MSC_VER
__inline
#else
inline
#endif /* _MSC_VER */
#endif /* USE_INLINE_HELPERS */
void graphics_transform_isometric(
   float tile_x, float tile_y, int* screen_x, int* screen_y, const OG_Rect* viewport
);

#endif /* GRAPHICS_H */

