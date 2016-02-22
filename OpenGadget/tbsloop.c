
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
   SDL_Rect sprite_rect;
   SDL_Rect screen_rect;
   SDL_Texture* sprite_texture = NULL;

   gfx_data_path = bformat( "%s\\graphic.pak", bdata( data_path ) );
   gfx_tile_name = bfromcstr( "" );

   memset( terrain_images, '\0', 0xFF * sizeof( SDL_Texture* ) );
   memset( &sprite_rect, '\0', sizeof( SDL_Rect ) );
   memset( &screen_rect, '\0', sizeof( SDL_Rect ) );

   screen_rect.w = ISOMAP_TILE_WIDTH;
   screen_rect.h = ISOMAP_TILE_HEIGHT;
   sprite_rect.w = ISOMAP_TILE_WIDTH;
   sprite_rect.h = ISOMAP_TILE_HEIGHT;

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

   for( i = 0 ; 10 > i ; i++ ) {

      graphics_begin_draw();
      //SDL_RenderCopy()

      for( y = 0 ; config->map->height > y ; y++ ) {
         for( x = 0 ; config->map->width > x ; x++ ) {

            tile_index = config->map->tiles[(y*x) + x];
            sprite_texture = terrain_images[tile_index];
            if( NULL == sprite_texture ) {
               continue;
            }

            screen_rect.x = x * screen_rect.w;
            screen_rect.y = y * screen_rect.h;
            graphics_draw( sprite_texture, &sprite_rect, &screen_rect );
         }
      }

      graphics_end_draw();

      graphics_sleep( 1000 );
   }

cleanup:
   
   for( i = 0; 0xFF > i; i++ ) {
      if( NULL != terrain_images[i] ) {
         SDL_FreeSurface( terrain_images[i] );
      }
   }

   return retval;
}
