
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

   while( 1 ) {

      while( SDL_PollEvent( &event ) ) {
         switch( event.type ) {
            case SDL_QUIT:
               goto cleanup;

            case SDL_KEYUP:
               if( SDLK_r == event.key.keysym.sym ) {
                  rotation++;
                  if( 3 < rotation ) {
                     rotation = 0;
                  }
               }
               break;
         }

      }

      graphics_clear();

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
