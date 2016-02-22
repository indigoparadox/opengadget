
#include "isomap.h"

struct isomap* isomap_load_map( uint8_t* map_data, uint32_t map_data_len ) {
   uint32_t i;
   int32_t map_start = -1;
   uint16_t map_portion_len;
   char* data_iter = NULL;
   struct isomap* isomap_out = NULL;

   for( i = 0 ; i < map_data_len ;  i++ ) {
      data_iter = &(map_data[i]);
      if( 0 == strncmp( data_iter, "MAPU", 4 ) ) {
         /* We found the start of the map section. */
         map_start = i + 6;
         break;
      }
   }

   if( 0 > map_start ) {
      LOG_ERROR( "Unable to find map start." );
      goto cleanup;
   }

   /* TODO: There might be endian issues, here. */
   memcpy( &map_portion_len, &(map_data[map_start]), sizeof( uint16_t ) );
   map_start += 2;
   isomap_out = calloc( 1, sizeof( struct isomap ) );
   isomap_out->tiles = calloc( map_portion_len, sizeof( uint8_t ) );
   memcpy( isomap_out->tiles, &(map_data[map_start]), map_portion_len );

   FILE* file = fopen( mapout, "wb" );
   fwrite( isomap_out->tiles, sizeof( uint8_t ), map_portion_len, file );
   fclose( file );
   

cleanup:
   
   return isomap_out;
}
