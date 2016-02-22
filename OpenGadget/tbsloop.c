
#include "tbsloop.h"

extern SDL_Renderer* opengadget_renderer;

RETVAL tbsloop_loop( struct tbsloop_config* config ) {
   RETVAL retval = 0;
   int i = 0;

   for( i = 0 ; 3 > i ; i++ ) {
      SDL_RenderClear( opengadget_renderer );
      //SDL_RenderCopy()
   }

   return retval;
}
