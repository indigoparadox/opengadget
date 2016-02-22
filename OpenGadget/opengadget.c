
#include <SDL.h>
#include <stdio.h>

#include "dep/arckogako.h"
#include "isomap.h"
#include "tbsloop.h"

SDL_Renderer* opengadget_renderer = NULL;

int main( int argc, char* argv[] ) {
   RETVAL retval = 0;
   SDL_Window* win = NULL;
   FILE* pak_file = NULL;
   struct pak_file* map_pak = NULL;
   struct isomap* map = NULL;
   struct tbsloop_config map_config;
   uint8_t* map_data = NULL;

   retval = SDL_Init( SDL_INIT_VIDEO );
   if(  0 != retval ) {
      SDL_LogCritical( SDL_LOG_CATEGORY_VIDEO, "SDL_Init: %s", SDL_GetError() );
      goto cleanup;
   }
   
   win = SDL_CreateWindow( "OpenGadget", 100, 100, 640, 480, SDL_WINDOW_SHOWN );
   if( NULL == win ) {
      SDL_LogCritical( SDL_LOG_CATEGORY_VIDEO, "SDL_CreateWindow: %s", SDL_GetError() );
      retval = 2;
      goto cleanup;
   }

   opengadget_renderer = SDL_CreateRenderer( win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
   if( NULL == opengadget_renderer ) {
      SDL_LogCritical( SDL_LOG_CATEGORY_VIDEO, "SDL_CreateRenderer: %s", SDL_GetError() );
      retval = 4;
      goto cleanup;
   }

   pak_file = fopen( argv[1], "rb" );

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

   map_config.map = isomap_load_map( map_data, map_pak->entries[0].unpacked_size );

   tbsloop_loop( &map_config );

cleanup:

   if( NULL != win ) {
      SDL_DestroyWindow( win );
   }

   if( NULL != map_pak ) {
      /* This frees pak_file above, too. */
      pakopener_free( map_pak );
   }

   if( NULL != map_data ) {
      free( map_data );
   }

   SDL_Quit();

   return retval;
}
