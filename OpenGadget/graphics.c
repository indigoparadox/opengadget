
#include "graphics.h"

SDL_Surface* graphics_image_load( bstring image_name, struct pak_file* pak ) {
   int i;
   struct pak_entry* entry = NULL;
   SDL_Surface* image_out = NULL;
   uint8_t* image_data = NULL;
   SDL_RWops* image_rw = NULL;

   for( i = 0 ; pak->count > i ; i++ ) {
      /* TODO: Pick the shortest length possible so we don't overflow. */
      if( 0 == strncmp( bdata( image_name ), pak->entries[i].name, blength( image_name ) ) ) {
         entry = &(pak->entries[i]);
         break;
      }
   }
   if( NULL == entry ) {
      /* TODO: Error message. */
      goto cleanup;
   }

   image_data = pakopener_open_entry( pak, entry );
   if( NULL == image_data ) {
      /* TODO: Error message. */
      goto cleanup;
   }

   image_rw = SDL_RWFromConstMem( image_data, entry->unpacked_size );

   image_out = IMG_Load_RW( image_rw, 0 );

cleanup:

   if( NULL != image_rw ) {
      SDL_RWclose( image_rw );
   }

   if( NULL != image_data ) {
      free( image_data );
   }

   return image_out;
}
