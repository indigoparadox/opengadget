
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

#ifndef ISOMAP_H
#define ISOMAP_H

#include "defines.h"
#include "dep/arckogako.h"
#include "units.h"

#if DEBUG
#include "graphics.h"
#endif /* DEBUG */

#define ISOMAP_UNIT_DATA_SIZE 32

#define ISOMAP_BUILDINGS_TILES_OFFSET 0x14

typedef enum {
   ISOMAP_TERRAIN_PLAINS =  0x00,
   ISOMAP_TERRAIN_ROAD =    0x01,

   ISOMAP_TERRAIN_SWAMP =   0x03,
   ISOMAP_TERRAIN_TREES =   0x04,
   ISOMAP_TERRAIN_BEACH =   0x05,
   ISOMAP_TERRAIN_SEA =     0x06,
   ISOMAP_TERRAIN_RIVER =   0x07,
   ISOMAP_TERRAIN_WASTE =   0x08,
   ISOMAP_TERRAIN_HOLE =    0x09,
   ISOMAP_TERRAIN_HILL =    0x0A,
   ISOMAP_TERRAIN_MNTN =    0x0B,
   ISOMAP_TERRAIN_BRIDGE =  0x0C,

   ISOMAP_TERRAIN_HQ =      0x14,
   ISOMAP_TERRAIN_CITY =    0x15,
   ISOMAP_TERRAIN_FACTORY = 0x16,
   ISOMAP_TERRAIN_AIRPORT = 0x17,
   ISOMAP_TERRAIN_HARBOR =  0x18,
   ISOMAP_TERRAIN_DETSAT =  0x19,
   ISOMAP_TERRAIN_ATTSAT =  0x1A,
   ISOMAP_TERRAIN_MAX
} ISOMAP_TERRAIN;

typedef enum {
   ISOMAP_WEATHER_CLEAR =   0x00,
   ISOMAP_WEATHER_RAIN =    0x09,
   ISOMAP_WEATHER_SNOW =    0x12
} ISOMAP_WEATHER;

struct isomap_tile {
   int index;
   uint8_t terrain;
   uint32_t x;
   uint32_t y;
   struct units_unit* unit;
   struct isomap* map;
   OG_BOOL movable;
#if DEBUG
   SDL_Surface* coords;
#endif /* DEBUG */
};

struct isomap {
   uint32_t width;
   uint32_t height;
   uint8_t weather;
   struct units_unit* units;
   int units_count;
   struct isomap_tile* tiles;
   uint32_t tiles_count;
};

#define isomap_rendian_copy( target, cursor ) \
   for( i = 3 ; 0 <= i ; i-- ) { \
      target += map_data[cursor + i]; \
      target << 1; \
   }

#define isomap_find_section( section_name, map_bytes, map_bytes_len ) \
   for( i = 0 ; i < map_bytes_len ; i++ ) { \
      if( 0 == strncmp( &(map_bytes[i]), section_name, 4 ) ) { \
         /* We found the start of the section. */ \
         cursor = i; \
         break; \
      } \
   }

#define isomap_get_tile( x, y, map ) ((y) * (map->width)) + (x)

struct isomap* isomap_load_map( uint8_t* map_data, uint32_t map_data_len );
#if 0
int isomap_get_tile( int x, int y, struct isomap* map );
#endif

#endif /* ISOMAP_H */