
#include "tbsloop.h"

extern bstring data_path;

RETVAL tbsloop_loop( struct tbsloop_config* config ) {
   RETVAL retval = 0;
   int i = 0, x, y, projected_x, projected_y, tile_index;
   bstring gfx_data_path = NULL;
   FILE* gfx_data_file = NULL;
   struct pak_file* gfx_data_pak = NULL;
   SDL_Texture* terrain_images[0xFF];
   const char* gfx_tile_name_format = "MP%d_%02d_%d";
   bstring gfx_tile_name = NULL;
   SDL_Texture* sprite_texture = NULL;
   SDL_Rect viewport;
   int x_y_index;
   ISORENDER_ROTATE rotation = ISORENDER_ROTATE_0;

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
   viewport.x = 200;
   viewport.y = 200;

   for( i = 0 ; 10000 > i ; i++ ) {

      graphics_begin_draw();
      //SDL_RenderCopy()

      for( x = config->map->width - 1 ; 0 <= x ; x-- ) {
         for( y = 0 ; config->map->height > y ; y++ ) {

            switch( rotation ) {
               case ISORENDER_ROTATE_0:
                  projected_x = y;
                  projected_y = config->map->height - x - 1;
                  break;

               case ISORENDER_ROTATE_90:
                  projected_x = config->map->width - x - 1;
                  projected_y = config->map->height - y - 1;
                  break;

               case ISORENDER_ROTATE_180:
                  projected_x = config->map->width - y - 1;
                  projected_y = x;
                  break;

               case ISORENDER_ROTATE_270:
                  projected_x = x;
                  projected_y = y;
                  break;
            }

            x_y_index = (x * config->map->height) + y;
            tile_index = config->map->tiles[x_y_index];
            sprite_texture = terrain_images[tile_index];
            if( NULL == sprite_texture ) {
               continue;
            }
            
            graphics_draw_tile(
               sprite_texture,
               0, 0,
               viewport.x + (projected_y * GRAPHICS_TILE_WIDTH / 2) + (projected_x * GRAPHICS_TILE_WIDTH / 2),
               viewport.y + ((projected_x * GRAPHICS_TILE_OFFSET_X / 2) - (projected_y * GRAPHICS_TILE_OFFSET_X / 2))
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
