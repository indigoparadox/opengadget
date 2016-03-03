
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

#ifndef UNITS_H
#define UNITS_H

#include <stdint.h>

#include "defines.h"
#include "isomap.h"

typedef enum {
   UNITS_TYPE_INFANTRY,
   UNITS_TYPE_H_INFANTRY,
   UNITS_TYPE_MEDIC,
   UNITS_TYPE_TANK,
   UNITS_TYPE_SCOUT,
   UNITS_TYPE_APC,
   UNITS_TYPE_SPA,
   UNITS_TYPE_AAA,
   UNITS_TYPE_SPAR,
   UNITS_TYPE_FIGHTER,
   UNITS_TYPE_BOMBER,
   UNITS_TYPE_TRANS_HELI,
   UNITS_TYPE_BATTLESHIP,
   UNITS_TYPE_SUBMARINE,
   UNITS_TYPE_LANDING_CRAFT,
   UNITS_TYPE_TURRET_ANTI_LAND,
   UNITS_TYPE_TURRET_ANTI_AIR,
   UNITS_TYPE_TURRET_ANTI_SEA,
   UNITS_TYPE_OBSTACLE,
   UNITS_TYPE_MAX
} UNITS_TYPE;

typedef enum {
   UNITS_FACING_UP,
   UNITS_FACING_DOWN,
   UNITS_FACING_RIGHT,
   UNITS_FACING_LEFT,
} UNITS_FACING;

typedef enum {
   UNITS_TEAM_WHITE,
   UNITS_TEAM_BLACK,
   UNITS_TEAM_ARMY,
   UNITS_TEAM_SILVER,
   UNITS_TEAM_CONTINENTAL,
   UNITS_TEAM_MAX
} UNITS_TEAM;

struct units_unit {
   uint32_t type;
   uint32_t team;
   int range;
   struct isomap_tile* tile;
   UNITS_FACING facing;
   OG_BOOL selected;
   struct isomap_tile** path_list;
   int path_list_count;
   int path_next_percent_x;
   int path_next_percent_y;
};

void units_walk_range( struct isomap_tile* tile, struct unit* unit, int mobility );
OG_BOOL units_move( struct units_unit* unit, struct isomap_tile* tile_current, struct isomap_tile** tiles_previous, int tiles_previous_count, int distance, struct isomap_tile* tile_dst );
int units_get_mobility_range( struct units_unit* unit );
int units_get_mobility_cost( struct units_unit* unit, struct isomap_tile* tile );

#endif /* UNITS_H */
