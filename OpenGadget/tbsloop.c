
#include "tbsloop.h"

extern bstring data_path;

OG_RETVAL tbsloop_loop( struct tbsloop_config* config ) {
   OG_RETVAL retval = 0;
   int i = 0;
   uint32_t x, y;
   SDL_Rect viewport;

   graphics_set_title( config->map_name );

   memset( &viewport, '\0', sizeof( SDL_Rect ) );

   retval = isomap_render_load_textures( data_path );
   if( retval ) {
      goto cleanup;
   }

   viewport.w = 640;
   viewport.h = 480;
   viewport.x = -10;
   viewport.y = 200;

   graphics_clear();
   //SDL_RenderCopy()

   for( x = 0 ; config->map->width > x ; x++ ) {
      for( y = 0 ; config->map->height > y ; y++ ) {
         isomap_render_draw_tile(
            x, y,
            config->map->tiles,
            config->map->width,
            config->map->height,
            &viewport,
            ISOMAP_RENDER_ROTATE_0
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
