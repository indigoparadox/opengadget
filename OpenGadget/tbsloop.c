
#include "tbsloop.h"

extern SDL_Renderer* opengadget_renderer;
extern bstring data_path;

RETVAL tbsloop_loop( struct tbsloop_config* config ) {
   RETVAL retval = 0;
   int i = 0;
   bstring gfx_data_path = NULL;
   FILE* gfx_data_file = NULL;
   struct pak_file* gfx_data_pak = NULL;
   SDL_Surface* terrain_images[0xFF];
   const char* gfx_tile_name_format = "MP%d_%02d_%d";
   bstring gfx_tile_name = NULL;

   gfx_data_path = bformat( "%s\\graphic.pak", bdata( data_path ) );
   gfx_tile_name = bfromcstr( "" );

   memset( terrain_images, '\0', 0xFF * sizeof( SDL_Surface* ) );

   gfx_data_file = fopen( bdata( gfx_data_path ), "rb" );
   if( NULL == gfx_data_file ) {
      goto cleanup;
   }
   gfx_data_pak = pakopener_try_open( gfx_data_file );
   if( NULL == gfx_data_pak ) {
      goto cleanup;
   }

   //isomap_terr

   bassignformat( gfx_tile_name, gfx_tile_name_format, 1, ISOMAP_TERRAIN_PLAINS, 0 );
   terrain_images[ISOMAP_TERRAIN_PLAINS] = graphics_image_load( gfx_tile_name, gfx_data_pak );
   bassignformat( gfx_tile_name, gfx_tile_name_format, 1, ISOMAP_TERRAIN_ROAD, 0 );
   terrain_images[ISOMAP_TERRAIN_ROAD] = graphics_image_load( gfx_tile_name, gfx_data_pak );
   bassignformat( gfx_tile_name, gfx_tile_name_format, 1, ISOMAP_TERRAIN_SWAMP, 0 );
   terrain_images[ISOMAP_TERRAIN_SWAMP] = graphics_image_load( gfx_tile_name, gfx_data_pak );
   bassignformat( gfx_tile_name, gfx_tile_name_format, 1, ISOMAP_TERRAIN_TREES, 0 );
   terrain_images[ISOMAP_TERRAIN_TREES] = graphics_image_load( gfx_tile_name, gfx_data_pak );

   for( i = 0 ; 3 > i ; i++ ) {
      SDL_RenderClear( opengadget_renderer );
      //SDL_RenderCopy()
   }

cleanup:
   
   for( i = 0; 0xFF > i; i++ ) {
      if( NULL != terrain_images[i] ) {
         SDL_FreeSurface( terrain_images[i] );
      }
   }

   return retval;
}
