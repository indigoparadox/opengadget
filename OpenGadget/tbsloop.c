
#include "tbsloop.h"

extern bstring data_path;

OG_RETVAL tbsloop_loop( struct tbsloop_config* config ) {
   OG_RETVAL retval = 0;
   int i = 0;
   int j, k, j_max, k_max, x_iter, y_iter;
   SDL_Rect viewport;
   ISOMAP_RENDER_ROTATE rotation = ISOMAP_RENDER_ROTATE_0;
   SDL_Event event;

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

   while( 1 ) {

      while( SDL_PollEvent( &event ) ) {
         switch( event.type ) {
            case SDL_QUIT:
               goto cleanup;
         }

      }

      graphics_clear();
      //SDL_RenderCopy()

#if 0
      for( x_iter = 0 ; config->map->width > x_iter ; x_iter++ ) {
         for( y_iter = 0 ; config->map->height > y_iter ; y_iter++ ) {
            /*
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
            */
                  //x_iter = j;
                  //y_iter = k;

            isomap_render_draw_tile(
               &(config->map->tiles[isomap_get_tile( x_iter, y_iter, config->map )]),
               &viewport,
               rotation
            );

            //if(  )
         }
      }
      for( i = 0 ; config->map->units_count > i ; i++ ) {
         isomap_render_draw_unit(
            &(config->map->units[i]),
            0,
            &viewport,
            rotation
         );
      }
#endif

      if( ISOMAP_RENDER_ROTATE_0 == rotation || ISOMAP_RENDER_ROTATE_270 == rotation ) {
         for( i = config->map->tiles_count - 1 ; 0 <= i ; i-- ) {
            isomap_render_draw_tile(
               &(config->map->tiles[i]),
               &viewport,
               rotation
               );
         }
      } else {
         for( i = 0 ; config->map->tiles_count > i ; i++ ) {
            isomap_render_draw_tile(
               &(config->map->tiles[i]),
               &viewport,
               rotation
               );
         }
      }

      graphics_end_draw();

      graphics_sleep( 1000 );
   }

cleanup:

   isomap_render_cleanup();

   return retval;
}
