
#ifndef ISOMAP_RENDER_H
#define ISOMAP_RENDER_H

#include "defines.h"
#include "graphics.h"
#include "isomap.h"

typedef enum {
   ISOMAP_RENDER_ROTATE_0,
   ISOMAP_RENDER_ROTATE_90,
   ISOMAP_RENDER_ROTATE_180,
   ISOMAP_RENDER_ROTATE_270
} ISOMAP_RENDER_ROTATE;

#define isomap_render_load_texture( terrain_id ) \
   bassignformat( gfx_tile_name, gfx_tile_name_format, 1, terrain_id, 0 ); \
   isomap_render_textures[terrain_id] = graphics_image_load( gfx_tile_name, gfx_data_pak );

void isomap_render_load_textures( bstring data_path );
void isomap_render_cleanup( void );
void isomap_render_draw_tile( uint8_t* tiles, int map_width, int map_height, SDL_Rect* viewport, ISOMAP_RENDER_ROTATE rotation );

#endif /* ISOMAP_RENDER_H */
