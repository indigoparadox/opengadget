
#include "tbsloop.h"

extern bstring data_path;

RETVAL tbsloop_loop( struct tbsloop_config* config ) {
   RETVAL retval = 0;
   int i = 0;
   SDL_Rect viewport;
   ISOMAP_RENDER_ROTATE rotation = ISOMAP_RENDER_ROTATE_0;

   graphics_set_title( config->map_name );

   memset( &viewport, '\0', sizeof( SDL_Rect ) );


   isomap_render_load_textures( data_path );

   viewport.w = 640;
   viewport.h = 480;
   viewport.x = -10;
   viewport.y = 200;

   for( i = 0 ; 10000 > i ; i++ ) {

      graphics_begin_draw();
      //SDL_RenderCopy()

      isomap_render_draw_tile(
         config->map->tiles,
         config->map->width,
         config->map->height,
         &viewport,
         rotation
      );

      graphics_end_draw();

      graphics_sleep( 1000 );
   }

cleanup:

   isomap_render_cleanup();

   return retval;
}
