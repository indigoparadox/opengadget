
#ifndef ISOMAP_RENDER_H
#define ISOMAP_RENDER_H

#include <math.h>

#include "defines.h"
#include "graphics.h"
#include "isomap.h"

typedef enum {
   ISOMAP_RENDER_ROTATE_0,
   ISOMAP_RENDER_ROTATE_90,
   ISOMAP_RENDER_ROTATE_180,
   ISOMAP_RENDER_ROTATE_270
} ISOMAP_RENDER_ROTATE;

struct isomap_render_texture {
   int texture_index;
   SDL_Rect sprite_rect;
};

#define isomap_render_load_texture( terrain_id ) \
   bassignformat( gfx_tile_name, gfx_tile_name_format, 1, terrain_id, 0 ); \
   isomap_render_textures[terrain_id] = graphics_image_load( gfx_tile_name, gfx_data_pak );

OG_RETVAL isomap_render_load_textures( const bstring data_path );
void isomap_render_cleanup( void );
void isomap_render_draw_tile(
   const int x,
   const int y,
   const uint8_t* tiles,
   const int map_width,
   const int map_height,
   const SDL_Rect* viewport,
   const ISOMAP_RENDER_ROTATE rotation
);

#endif /* ISOMAP_RENDER_H */
