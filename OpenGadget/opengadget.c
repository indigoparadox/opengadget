
#include <SDL.h>
#include <stdio.h>

#include "dep/arckogako.h"
#include "isomap.h"
#include "tbsloop.h"

bstring data_path = NULL;

int main( int argc, char* argv[] ) {
   OG_RETVAL retval = 0;
   FILE* pak_file = NULL;
   FILE* gfx_file = NULL;
   struct pak_file* map_pak = NULL;
   struct isomap* map = NULL;
   struct tbsloop_config map_config;
   uint8_t* map_data = NULL;
   bstring map_data_path = NULL;

   data_path = bfromcstr( argv[1] );
   map_data_path = bformat( "%s\\map.pak", argv[1] );

   retval = graphics_init();
   if( 0 != retval ) {
      goto cleanup;
   }

   pak_file = fopen( bdata( map_data_path ), "rb" );

   map_pak = pakopener_try_open( pak_file );
   if( NULL == map_pak ) {
      SDL_LogCritical( SDL_LOG_CATEGORY_APPLICATION, "Unable to open map. Aborting." );
      retval = 8;
      goto cleanup;
   }

   map_data = pakopener_open_entry( map_pak, &(map_pak->entries[0]) );
   if( NULL == map_data ) {
      SDL_LogCritical( SDL_LOG_CATEGORY_APPLICATION, "Unable to open map. Aborting." );
      retval = 8;
      goto cleanup;
   }

   map_config.map_name = bfromcstr( map_pak->entries[0].name );
   map_config.map = isomap_load_map( map_data, map_pak->entries[0].unpacked_size );

   /* Free this outside of cleanup since it's been loaded and we can save     *
    * some memory.                                                            */
   free( map_data );
   map_data = NULL;
   pakopener_free( map_pak );
   map_pak = NULL;

   retval = tbsloop_loop( &map_config );

cleanup:
   
   if( NULL != pak_file ) {
      fclose( pak_file );
   }

   if( NULL != map_config.map_name ) {
      bdestroy( map_config.map_name );
   }

   if( NULL != map_pak ) {
      /* This frees pak_file above, too. */
      pakopener_free( map_pak );
   }

   if( NULL != map_data ) {
      free( map_data );
   }

   graphics_cleanup();

   SDL_Quit();

   return retval;
}
