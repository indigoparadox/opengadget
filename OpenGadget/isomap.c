
#include "isomap.h"

struct isomap* isomap_load_map( uint8_t* map_data, uint32_t map_data_len ) {
   int32_t i;
   int32_t cursor = -1;
   uint32_t map_portion_len;
   char* data_iter = NULL;
   struct isomap* isomap_out = NULL;

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

cleanup:
   
   return isomap_out;
}
