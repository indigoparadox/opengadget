
#ifndef TBSLOOP_H
#define TBSLOOP_H

#include <SDL.h>

#include "defines.h"
#include "isomap.h"
#include "isomap_terrain.h"
#include "graphics.h"

struct tbsloop_config {
   struct isomap* map;
};

RETVAL tbsloop_loop( struct tbsloop_config* config );

#endif /* TBSLOOP_H */
