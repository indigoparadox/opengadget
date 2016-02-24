
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

typedef enum {
   ISOMAP_RENDER_BITWISE_UP = 1,
   ISOMAP_RENDER_BITWISE_RIGHT = 2,
   ISOMAP_RENDER_BITWISE_DOWN = 4,
   ISOMAP_RENDER_BITWISE_LEFT = 8
} ISOMAP_RENDER_BITWISE;

struct isomap_render_texture {
   int texture_index;
   SDL_Rect sprite_rect;
};

#define isomap_render_load_texture( terrain_id ) \
   bassignformat( gfx_tile_name, gfx_tile_name_format, 1, terrain_id, 0 ); \
   isomap_render_textures[terrain_id] = graphics_image_load( gfx_tile_name, gfx_data_pak );

#define isomap_render_adjacent( tile_1, tile_2, tile_index, tiles_count ) \
      ( \
         tile_1 == tile_2 || \
         ( tiles_count <= tile_index || 0 > tile_index ) || \
         ( \
            ISOMAP_TERRAIN_SEA == tile_1 && ( \
               ISOMAP_TERRAIN_RIVER == tile_2 || \
               ISOMAP_TERRAIN_BRIDGE == tile_2 || \
               ISOMAP_TERRAIN_BEACH == tile_2 \
            ) \
         ) || ( \
            ISOMAP_TERRAIN_RIVER == tile_1 && ( \
               ISOMAP_TERRAIN_SEA == tile_2 || \
               ISOMAP_TERRAIN_BRIDGE == tile_2 \
            ) \
         )  || ( \
            ISOMAP_TERRAIN_BEACH == tile_1 && ( \
               ISOMAP_TERRAIN_SEA == tile_2 \
            ) \
         ) || ( \
            ISOMAP_TERRAIN_BRIDGE == tile_1 && ( \
               ISOMAP_TERRAIN_PLAINS == tile_2 || \
               ISOMAP_TERRAIN_WASTE == tile_2 || \
               ISOMAP_TERRAIN_TREES == tile_2 || \
               ISOMAP_TERRAIN_ROAD == tile_2 \
            ) \
         ) \
      )

/* TODO: Maybe make this smarter with XOR at some point, but don't            *
 * overcomplicate right now.                                                  */
#define isomap_render_tile_rotate( x, y, width, height, temp, rotation ) \
   switch( rotation ) { \
      case ISOMAP_RENDER_ROTATE_90: \
         temp = y; \
         y = height - x - 1; \
         x = temp; \
         break; \
      case ISOMAP_RENDER_ROTATE_180: \
         x = width - x - 1; \
         y = height - y - 1; \
         break; \
      case ISOMAP_RENDER_ROTATE_270: \
         temp = x; \
         x = width - y - 1; \
         y = temp; \
         break; \
   }

OG_RETVAL isomap_render_load_textures( const bstring data_path );
void isomap_render_cleanup( void );
void isomap_render_draw_tile(
   int x,
   int y,
   const uint8_t* tiles,
   int map_width,
   int map_height,
   const SDL_Rect* viewport,
   const ISOMAP_RENDER_ROTATE rotation
);

#endif /* ISOMAP_RENDER_H */
