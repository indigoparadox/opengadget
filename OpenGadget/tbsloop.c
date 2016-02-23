
#include "tbsloop.h"

extern bstring data_path;

RETVAL tbsloop_loop( struct tbsloop_config* config ) {
   RETVAL retval = 0;
   int i = 0, x, y, tile_index;
   bstring gfx_data_path = NULL;
   FILE* gfx_data_file = NULL;
   struct pak_file* gfx_data_pak = NULL;
   SDL_Texture* terrain_images[0xFF];
   const char* gfx_tile_name_format = "MP%d_%02d_%d";
   bstring gfx_tile_name = NULL;
   SDL_Texture* sprite_texture = NULL;
   SDL_Rect viewport;

   graphics_set_title( config->map_name );

   gfx_data_path = bformat( "%s\\graphic.pak", bdata( data_path ) );
   gfx_tile_name = bfromcstr( "" );

   memset( terrain_images, '\0', 0xFF * sizeof( SDL_Texture* ) );
   memset( &viewport, '\0', sizeof( SDL_Rect ) );

   gfx_data_file = fopen( bdata( gfx_data_path ), "rb" );
   if( NULL == gfx_data_file ) {
      goto cleanup;
   }
   gfx_data_pak = pakopener_try_open( gfx_data_file );
   if( NULL == gfx_data_pak ) {
      goto cleanup;
   }

   //isomap_terr

   /*
   bassignformat( gfx_tile_name, gfx_tile_name_format, 1, ISOMAP_TERRAIN_PLAINS, 0 );
   terrain_images[ISOMAP_TERRAIN_PLAINS] = graphics_image_load( gfx_tile_name, gfx_data_pak );
   bassignformat( gfx_tile_name, gfx_tile_name_format, 1, ISOMAP_TERRAIN_ROAD, 0 );
   terrain_images[ISOMAP_TERRAIN_ROAD] = graphics_image_load( gfx_tile_name, gfx_data_pak );
   bassignformat( gfx_tile_name, gfx_tile_name_format, 1, ISOMAP_TERRAIN_SWAMP, 0 );
   terrain_images[ISOMAP_TERRAIN_SWAMP] = graphics_image_load( gfx_tile_name, gfx_data_pak );
   bassignformat( gfx_tile_name, gfx_tile_name_format, 1, ISOMAP_TERRAIN_TREES, 0 );
   terrain_images[ISOMAP_TERRAIN_TREES] = graphics_image_load( gfx_tile_name, gfx_data_pak );
   */
   //tbsloop_load_texture( ISOMAP_TERRAIN_PLAINS );
   for( i = 0 ; ISOMAP_TERRAIN_HQ > i ; i++ ) {
      tbsloop_load_texture( i );
   }

   viewport.w = 640;
   viewport.h = 480;
   viewport.y = 200;

   for( i = 0 ; 10 > i ; i++ ) {

      graphics_begin_draw();
      //SDL_RenderCopy()

      for( x = 0 ; config->map->width > x ; x++ ) {
         for( y = config->map->height ; 0 <= y ; y-- ) {

            tile_index = config->map->tiles[(y*x) + x];
            sprite_texture = terrain_images[tile_index];
            if( NULL == sprite_texture ) {
               continue;
            }
            
            graphics_draw_tile(
               sprite_texture,
               0, 0,
               (y * GRAPHICS_TILE_WIDTH / 2) + (x * GRAPHICS_TILE_WIDTH / 2),
               viewport.y + ((x * GRAPHICS_TILE_OFFSET_X / 2) - (y * GRAPHICS_TILE_OFFSET_X / 2))
            );
         }
      }

      graphics_end_draw();

      graphics_sleep( 1000 );
   }

cleanup:
   
   for( i = 0; 0xFF > i; i++ ) {
      if( NULL != terrain_images[i] ) {
         SDL_DestroyTexture( terrain_images[i] );
      }
   }

   return retval;
}
