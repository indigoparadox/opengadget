
#ifndef TBSLOOP_H
#define TBSLOOP_H

#include <SDL.h>

#include "defines.h"
#include "graphics.h"
#include "isomap.h"
#include "isomap_terrain.h"
#include "isomap_render.h"

struct tbsloop_config {
   struct isomap* map;
   bstring map_name;
};

RETVAL tbsloop_loop( struct tbsloop_config* config );

#endif /* TBSLOOP_H */
