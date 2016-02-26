
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

#ifndef ISOMAP_RENDER_H
#define ISOMAP_RENDER_H

#include <math.h>

#include "defines.h"
#include "graphics.h"
#include "isomap.h"
#include "units.h"

#define ISOMAP_RENDER_UNIT_TEXTURES_MAX 20

#define ISOMAP_RENDER_BGTILE_WIDTH 580
#define ISOMAP_RENDER_BGTILE_HEIGHT 186

typedef enum {
   ISOMAP_RENDER_TEXTURE_TYPE_TERRAIN,
   ISOMAP_RENDER_TEXTURE_TYPE_UNIT,
   ISOMAP_RENDER_TEXTURE_TYPE_UI
} ISOMAP_RENDER_TEXTURE_TYPE;

typedef enum {
   ISOMAP_RENDER_UI_MAPCURSOR,
   ISOMAP_RENDER_UI_MAPMARKER,
   ISOMAP_RENDER_UI_BGTILE
} ISOMAP_RENDER_UI;

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

#define isomap_render_adjacent( tile_1, tile_2 ) \
      ( \
         tile_1->terrain == tile_2->terrain || \
         ( \
            (tile_1->map->tiles + tile_1->map->tiles_count) <= tile_2 || \
               tile_1->map->tiles > tile_2 ) || \
         ( \
            ISOMAP_TERRAIN_SEA == tile_1->terrain && ( \
               ISOMAP_TERRAIN_RIVER == tile_2->terrain || \
               ISOMAP_TERRAIN_BRIDGE == tile_2->terrain || \
               ISOMAP_TERRAIN_BEACH == tile_2->terrain \
            ) \
         ) || ( \
            ISOMAP_TERRAIN_RIVER == tile_1->terrain && ( \
               ISOMAP_TERRAIN_SEA == tile_2->terrain || \
               ISOMAP_TERRAIN_BRIDGE == tile_2->terrain \
            ) \
         )  || ( \
            ISOMAP_TERRAIN_BEACH == tile_1->terrain && ( \
               ISOMAP_TERRAIN_SEA == tile_2->terrain \
            ) \
         ) || ( \
            ISOMAP_TERRAIN_BRIDGE == tile_1->terrain && ( \
               ISOMAP_TERRAIN_PLAINS == tile_2->terrain || \
               ISOMAP_TERRAIN_WASTE == tile_2->terrain || \
               ISOMAP_TERRAIN_TREES == tile_2->terrain || \
               ISOMAP_TERRAIN_ROAD == tile_2->terrain \
            ) \
         ) \
      )

OG_RETVAL isomap_render_load_textures( const bstring data_path );
void isomap_render_cleanup( void );
void isomap_render_draw_tile(
   const struct isomap_tile* tile,
   const SDL_Rect* viewport,
   const GRAPHICS_ROTATE rotation
);
void isomap_render_draw_unit(
   const struct units_unit* unit,
   const uint8_t ani_frame,
   const SDL_Rect* viewport,
   const GRAPHICS_ROTATE rotation
);
void isomap_render_loop( const struct isomap* map, const SDL_Rect* viewport, GRAPHICS_ROTATE rotation );
SDL_Texture* isomap_render_get_texture( ISOMAP_RENDER_TEXTURE_TYPE type, int index );

#endif /* ISOMAP_RENDER_H */
