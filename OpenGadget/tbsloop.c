
#include "tbsloop.h"

extern bstring data_path;

OG_RETVAL tbsloop_loop( struct tbsloop_config* config ) {
   OG_RETVAL retval = 0;
   int i = 0;
   int j, k, j_max, k_max, x_iter, y_iter;
   SDL_Rect viewport;
   ISOMAP_RENDER_ROTATE rotation = ISOMAP_RENDER_ROTATE_90;

   graphics_set_title( config->map_name );

   memset( &viewport, '\0', sizeof( SDL_Rect ) );

   retval = isomap_render_load_textures( data_path );
   if( retval ) {
      goto cleanup;
   }

   viewport.w = 640;
   viewport.h = 480;
   viewport.x = 0;
   viewport.y = 200;

   switch( rotation ) {
      case ISOMAP_RENDER_ROTATE_0:
         j_max = config->map->width;
         k_max = config->map->height;
         break;

      case ISOMAP_RENDER_ROTATE_90:
         k_max = config->map->width;
         j_max = config->map->height;
         break;

   }

   graphics_clear();
   //SDL_RenderCopy()

   for( j = 0 ; j_max > j ; j++ ) {
      for( k = 0 ; k_max > k ; k++ ) {
         switch( rotation ) {
            case ISOMAP_RENDER_ROTATE_0:
               x_iter = j;
               y_iter = k;
               break;

            case ISOMAP_RENDER_ROTATE_90:
               x_iter = k;
               y_iter = j;
               break;

         }

         isomap_render_draw_tile(
            x_iter, y_iter,
            config->map->tiles,
            config->map->width,
            config->map->height,
            &viewport,
            rotation
         );
      }
   }

   graphics_end_draw();

   for( i = 0 ; 10000 > i ; i++ ) {

      graphics_end_draw();

      graphics_sleep( 1000 );
   }

cleanup:

   isomap_render_cleanup();

   return retval;
}
