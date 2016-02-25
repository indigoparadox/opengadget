
#include "isomap.h"

struct isomap* isomap_load_map( uint8_t* map_data, uint32_t map_data_len ) {
   int32_t i;
   int32_t cursor = -1;
   int32_t units_cursor = -1;
   uint32_t map_portion_len;
   char* data_iter = NULL;
   struct isomap* isomap_out = NULL;
   uint8_t* isomap_tiles_x_cursor = NULL;

   isomap_out = calloc( 1, sizeof( struct isomap ) );

   /* This seems to have the same offset in all files I examined. */
   isomap_out->weather = map_data[64];

   /* Parse the map dimensions. */
   isomap_find_section( "MAPF", map_data, map_data_len );
   cursor += 5;

   /* TODO: Handle unfound cursor. */
   isomap_rendian_copy( isomap_out->height, cursor );
   cursor += 4;
   isomap_rendian_copy( isomap_out->width, cursor );

   /* Parse the map terrain tiles. */
   map_portion_len = isomap_out->width * isomap_out->height;
   isomap_find_section( "MAPD", map_data, map_data_len );
   cursor += 8;

   /*
   if( 0 > cursor ) {
      LOG_ERROR( "Unable to find map start." );
      goto cleanup;
   }
   */

   isomap_out->tiles = calloc( map_portion_len, sizeof( uint8_t ) );

   memcpy( isomap_out->tiles, &(map_data[cursor]), map_portion_len );

   /* Parse the units. */
   isomap_find_section( "MAPU", map_data, map_data_len );
   cursor += 4;

   memcpy( &map_portion_len, &(map_data[cursor]), sizeof( uint32_t ) );
   cursor += sizeof( uint32_t );

   isomap_out->units_count = map_portion_len / ISOMAP_UNIT_DATA_SIZE;

   isomap_out->units = calloc( isomap_out->units_count, sizeof( struct units_unit ) );
   for( units_cursor = 0 ; units_cursor < isomap_out->units_count; units_cursor ++ ) {
      memcpy( &(isomap_out->units[units_cursor].type), &(map_data[cursor]), sizeof( uint32_t ) );
      cursor += sizeof( uint32_t );

      // ???
      cursor += sizeof( uint32_t );

      memcpy( &(isomap_out->units[units_cursor].x), &(map_data[cursor]), sizeof( uint32_t ) );
      cursor += sizeof( uint32_t );

      memcpy( &(isomap_out->units[units_cursor].y), &(map_data[cursor]), sizeof( uint32_t ) );
      cursor += sizeof( uint32_t );

      cursor += (4 * sizeof( uint32_t ));
   }

cleanup:
   
   return isomap_out;
}
