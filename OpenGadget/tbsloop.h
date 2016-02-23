
#ifndef TBSLOOP_H
#define TBSLOOP_H

#include <SDL.h>

#include "defines.h"
#include "isomap.h"
#include "isomap_terrain.h"
#include "graphics.h"


typedef enum {
   ISORENDER_ROTATE_0,
   ISORENDER_ROTATE_90,
   ISORENDER_ROTATE_180,
   ISORENDER_ROTATE_270
} ISORENDER_ROTATE;

#define tbsloop_load_texture( terrain_id ) \
   bassignformat( gfx_tile_name, gfx_tile_name_format, 1, terrain_id, 0 ); \
   terrain_images[terrain_id] = graphics_image_load( gfx_tile_name, gfx_data_pak );

struct tbsloop_config {
   struct isomap* map;
   bstring map_name;
};

RETVAL tbsloop_loop( struct tbsloop_config* config );

#endif /* TBSLOOP_H */
