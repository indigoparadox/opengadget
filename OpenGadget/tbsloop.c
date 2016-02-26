
#include "tbsloop.h"

extern bstring data_path;

OG_RETVAL tbsloop_loop( struct tbsloop_config* config ) {
   OG_RETVAL retval = 0;
   int i = 0;
   int j, k, j_max, k_max, x_iter, y_iter;
   SDL_Rect viewport;
   GRAPHICS_ROTATE rotation = GRAPHICS_ROTATE_0;
   SDL_Event event;
   int animation_frame = 0;
   bstring new_title = NULL;

   new_title = bfromcstr( "" );
   bassignformat( new_title, "%s - %d", bdata( config->map_name ), rotation );

   graphics_set_title( new_title );

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
               switch( event.key.keysym.sym ) {
                  case SDLK_r:
                     rotation++;
                     if( 3 < rotation ) {
                        rotation = 0;
                     }
                     bassignformat( new_title, "%s - %d", bdata( config->map_name ), rotation );
                     graphics_set_title( new_title );
                     break;

                  case SDLK_RIGHT:
                     viewport.x -= 100;
                     break;

                  case SDLK_LEFT:
                     viewport.x += 100;
                     break;

                  case SDLK_UP:
                     viewport.y += 100;
                     break;

                  case SDLK_DOWN:
                     viewport.y -= 100;
                     break;

                  case SDLK_ESCAPE:
                     goto cleanup;
               }
               break;
         }

      }

      graphics_clear();

      if( GRAPHICS_ROTATE_0 == rotation || GRAPHICS_ROTATE_270 == rotation ) {
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

      for( i = 0 ; config->map->tiles_count > i ; i++ ) {
         if( NULL != config->map->tiles[i].unit ) {
            isomap_render_draw_unit(
               config->map->tiles[i].unit,
               animation_frame,
               &viewport,
               rotation
               );
         }
      }

      graphics_end_draw();

      animation_frame++;
      if( 3 < animation_frame ) {
         animation_frame = 0;
      }

      graphics_sleep( 1000 );
   }

cleanup:

   isomap_render_cleanup();

   return retval;
}
