
#include "isomap.h"

struct isomap* isomap_load_map( uint8_t* map_data, uint32_t map_data_len ) {
   int32_t i, j;
   int32_t cursor = -1;
   int32_t units_cursor = -1;
   uint32_t units_portion_len;
   uint32_t x_tmp, y_tmp;
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
   isomap_rendian_copy( isomap_out->width, cursor );
   cursor += 4;
   isomap_rendian_copy( isomap_out->height, cursor );

   /* Parse the map terrain tiles. */
   isomap_out->tiles_count = isomap_out->width * isomap_out->height;
   isomap_find_section( "MAPD", map_data, map_data_len );
   cursor += 8;

   /*
   if( 0 > cursor ) {
      LOG_ERROR( "Unable to find map start." );
      goto cleanup;
   }
   */

   isomap_out->tiles = calloc( isomap_out->tiles_count, sizeof( struct isomap_tile ) );

   /* Load the tiles backwards so that they don't overlap when drawn by index. */
   //memcpy( isomap_out->tiles, &(map_data[cursor]), map_portion_len );
   int index = 0;
   for( i = 0 ; isomap_out->height > i ; i ++ ) {
      for( j = isomap_out->width - 1 ; 0 <= j ; j-- ) {
         isomap_out->tiles[index].terrain = map_data[cursor++];
         isomap_out->tiles[index].x = i; //(j / isomap_out->width);
         isomap_out->tiles[index].y = j; //(j % isomap_out->width);
         isomap_out->tiles[index].map = isomap_out;
         isomap_out->tiles[index].index = index;
         index++;
      }
   }

   /* Parse the units. */
   isomap_find_section( "MAPU", map_data, map_data_len );
   cursor += 4;

   memcpy( &units_portion_len, &(map_data[cursor]), sizeof( uint32_t ) );
   cursor += sizeof( uint32_t );

   isomap_out->units_count = units_portion_len / ISOMAP_UNIT_DATA_SIZE;

   isomap_out->units = calloc( isomap_out->units_count, sizeof( struct units_unit ) );
   for( units_cursor = 0 ; units_cursor < isomap_out->units_count; units_cursor ++ ) {
      memcpy( &(isomap_out->units[units_cursor].type), &(map_data[cursor]), sizeof( uint32_t ) );
      cursor += sizeof( uint32_t );

      // ???
      cursor += sizeof( uint32_t );

      memcpy( &y_tmp, &(map_data[cursor]), sizeof( uint32_t ) );
      cursor += sizeof( uint32_t );

      memcpy( &x_tmp, &(map_data[cursor]), sizeof( uint32_t ) );
      cursor += sizeof( uint32_t );

      isomap_out->tiles[isomap_get_tile( x_tmp, y_tmp, isomap_out )].unit =
         &(isomap_out->units[units_cursor]);
      isomap_out->units[units_cursor].tile = 
         &(isomap_out->tiles[isomap_get_tile( x_tmp, y_tmp, isomap_out )]);

      cursor += (4 * sizeof( uint32_t ));
   }

cleanup:
   
   return isomap_out;
}
