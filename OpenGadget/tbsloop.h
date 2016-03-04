
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

#ifndef TBSLOOP_H
#define TBSLOOP_H

#include <SDL.h>

#include "defines.h"
#include "graphics.h"
#include "isomap.h"
#include "isomap_terrain.h"
#include "isomap_render.h"

#define TBSLOOP_VIEWPORT_STEP 20

typedef enum {
   TBSLOOP_STATE_FREE,
   TBSLOOP_STATE_MOVING,
   TBSLOOP_STATE_SCROLLING_X_RIGHT,
   TBSLOOP_STATE_SCROLLING_X_LEFT,
   TBSLOOP_STATE_SCROLLING_Y_UP,
   TBSLOOP_STATE_SCROLLING_Y_DOWN,
} TBSLOOP_STATE;

struct tbsloop_config {
   struct isomap* map;
   bstring map_name;
   GRAPHICS_ROTATE rotation;
   TBSLOOP_STATE current_state;
   TBSLOOP_STATE previous_state;
   struct units_unit* moving_unit;
   SDL_Rect viewport;
};

OG_RETVAL tbsloop_loop( struct tbsloop_config* config );

#endif /* TBSLOOP_H */
