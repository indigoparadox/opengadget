
#ifndef ISOMAP_H
#define ISOMAP_H

#include "defines.h"
#include "dep/arckogako.h"
#include "units.h"

#define ISOMAP_UNIT_DATA_SIZE 32

typedef enum {
   ISOMAP_TERRAIN_PLAINS =  0x00,
   ISOMAP_TERRAIN_ROAD =    0x01,
   ISOMAP_TERRAIN_SWAMP =   0x03,
   ISOMAP_TERRAIN_TREES =   0x04,
   ISOMAP_TERRAIN_BEACH =   0x05,
   ISOMAP_TERRAIN_SEA =     0x06,
   ISOMAP_TERRAIN_RIVER =   0x07,
   ISOMAP_TERRAIN_WASTE =   0x08,
   ISOMAP_TERRAIN_HILL =    0x0A,
   ISOMAP_TERRAIN_BRIDGE =  0x0C,
   ISOMAP_TERRAIN_HQ =      0x14,
   ISOMAP_TERRAIN_CITY =    0x15
} ISOMAP_TERRAIN;

typedef enum {
   ISOMAP_WEATHER_CLEAR =   0x00,
   ISOMAP_WEATHER_RAIN =    0x09,
   ISOMAP_WEATHER_SNOW =    0x12
} ISOMAP_WEATHER;

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

struct isomap {
   uint32_t width;
   uint32_t height;
   uint8_t weather;
   struct units_unit* units;
   int units_count;
   uint8_t* tiles;
};

#define isomap_get_tile( x, y, map_height ) ((x) * (map_height)) + (y)

struct isomap* isomap_load_map( uint8_t* map_data, uint32_t map_data_len );

#endif /* ISOMAP_H */